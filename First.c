#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "First.h"
#include "LabelTable.h"
#include "DataTable.h"
#include "InstructionTable.h"
#include "Methods.h"
#include "Types.h"

void FirstRound(FILE *fptr)
{
	int i, j, LC = 0; /* LC= Line Counter */
	char line[LINE], curr_str[LABEL_NAME + 1], label_name[LABEL_NAME];
	enum state is_error_in_line;

	while(fgets(line, LINE, fptr) != NULL)
	{
		int h;

		i = 0;
		LC++;
		is_error_in_line = NO;

		/* Check if the line is full */
		if(is_fully_line(line) == 0)
		{
			fprintf(stderr, "%sLine too long (in line %d)\n", PRINT_ERROR, LC);
			fgets(line, LINE, fptr); /* Reading the extra data in the line */
			continue; /* Move on to the next line */
		}

		/* Skipping white spaces */
		while(line[i] == ' ' || line[i] == '\t')
			i++;

		/* Empty line or a comment line */
		if(line[i] == '\n' || line[i] == ';')
			continue; /* Move on to the next line */

		/* There is a comma at the beginning of the line */
		if(line[i] == ',')
		{
			fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
			EC++;
			is_error_in_line = YES;
			continue; /* Move on to the next line */
		}

		h = i;

		/* Going through the line until we reach a white space */
		for(j = 0; j < LABEL_NAME && (line[h] != ' ' && line[h] != '\t' && line[h] != '\n'); j++, h++)
		{
			curr_str[j] = line[h];
		}

		if(j == LABEL_NAME) /* Reach the end of the label name */
		{
			/* It doesnt ends with :, so the label name isnt good */
			if(curr_str[j-1] != ':')
			{
				fprintf(stderr, "%sLabel name is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				is_error_in_line = YES;
				continue; /* Move on to the next line */
			}
		}
	
		curr_str[j] = '\0';

		if(curr_str[strlen(curr_str) - 1] == ':') /* This is a label */
		{
			enum state is_data; /* Is the line is guidance */
			int instruction_ok;
			label_table * temp_label;
			data_table * temp_data;
			instruction_table * temp_instruction;


			i = h;

			strncpy(label_name, curr_str, (strlen(curr_str) - 1));
			label_name[strlen(curr_str)-1] = '\0';

			is_error_in_line = check_label_syntax(label_name, LC);

			if(is_error_in_line == YES) /* There is errors in the line */
			{
				continue; /* Move on to the next line */
			}

			/* Skipping white spaces */
			while(line[i] == ' ' || line[i] == '\t')
				i++;

			/* It is label for guidance line */
			if(line[i] == '.')
			{
				int address = DC;
				int guide_ok;

				i++;
				is_data = YES;

				guide_ok = check_guidance_syntax(line, LC, i, YES);
					
				if(guide_ok == 2) /* There is a label at the begining oof a entry or extern line */
				{
					fprintf(stderr, "%sThere is a label defined at an entry or extern line (in line %d)\n", PRINT_WARN, LC);
					continue; /* Move on to the next line */
				}

				else if(guide_ok == 1) /* There are no errors */
				{

					/* Adding the label to the table */
					temp_label = add_to_label_table(label_name, address, is_data, NO, LC);
					if(temp_label == NULL) /* There is error in adding the label */
					{
						continue; /* Move on to the next line */
					}
					head_label = temp_label;

					/* Coding the guidance in the data table */
					temp_data = code_guidance(line, i, LC);
					if(temp_data == NULL) /* There are errors */
					{
						continue; /* Move on to the next line */
					}
					head_data = temp_data;

					continue; /* Move on to the next line */

				}
				else /* There are errors in the line */
				{
					continue; /* Move on to the next line */
				}
					
			}

			is_data = NO;

			/* There is a label, and empty line */
			if(line[i] == '\n')
			{
				fprintf(stderr, "%sEmpty label (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				is_error_in_line = YES;
			}

			if(is_error_in_line == YES) /* There are errors in the line */
			{
				continue; /* Move on to the next line */
			}

			/* The line isnt empty, and it is not a guidance line, so it must be
			 * an instruction line. */

			instruction_ok = check_instruction_syntax(line, LC, i);

			if(instruction_ok == 0) /* There are errors in the line */
			{
				continue; /* Move on to the next line */
			}

			/* Adding the label to the table */
			temp_label = add_to_label_table(label_name, (IC + BUFFER), is_data, NO, LC);
			if(temp_label == NULL) /* There is error in adding the label */
			{
				continue; /* Move on to the next line */
			}
			head_label = temp_label;


			/* Coding the instruction in the instruction table */
			temp_instruction = code_instruction(line, i, LC);
			if(temp_instruction == NULL) /* There are errors */
			{
				continue; /* Move on to the next line */
			}
			head_instruction = temp_instruction;

		}


		else if(curr_str[0] == '.') /* Meaning it is a guidance line */
		{
			int guide_ok;
			data_table * temp;


			i++;

			guide_ok = check_guidance_syntax(line, LC, i, YES);
					

			if(guide_ok == 0) /* There are errors in the line */
			{
				continue; /* Move on to the next line */
			}
		
			/* Coding the guidance in the data table */
			temp = code_guidance(line, i, LC);
			if(temp == NULL) /* There are errors */
			{
				continue; /* Move on to the next line */
			}
			head_data = temp;
		}

		else if(is_instruction(curr_str)) /* Meaning it is an instruction line */
		{
			int instruction_ok;
			instruction_table * temp;
			
			instruction_ok = check_instruction_syntax(line, LC, i);	
			if(instruction_ok == 0) /* There are errors in the line */
			{
				continue; /* Move on to the next line */
			}

			/* Coding the instruction in the instruction table */
			temp = code_instruction(line, i, LC);
			if(temp == NULL) /* There are errors */
			{
				continue; /* Move on to the next line */
			}
			head_instruction = temp;
		}

		/* The string is not a guidance or a code, so it must be a label,
		 * But the label name is illegal, need to check why. */
		else
		{
			for(j = 0; j < strlen(curr_str); j++)
			{
				if(curr_str[j] == ':')
				{
					if(j == 0) /* The : is at the begining of the string */
					{
						fprintf(stderr, "%sMissing label name (in line %d)\n", PRINT_ERROR, LC);
						EC++;
						is_error_in_line = YES;
						break;
					}
					else
					{
						/* Empty label */
						if(curr_str[j+1] == '\n')
						{
							fprintf(stderr, "%sEmpty label (in line %d)\n", PRINT_ERROR, LC);
							EC++;
							is_error_in_line = YES;
							break;
						}

						/* Meaning there is missing a white space */
						fprintf(stderr, "%sMissing white space after ':' (in line %d)\n", PRINT_ERROR, LC);
						EC++;
						is_error_in_line = YES;
						break;
					}
				}
			}

			if(is_error_in_line == YES) /* Meaning we already found the error in the label name */
			{
				continue; /* Move on to the next line */
			}

			/* Meaning there isnt : in the string */
			if(j == strlen(curr_str))
			{
				fprintf(stderr, "%sMissing ':' (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				is_error_in_line = YES;
			}

			else if(strlen(curr_str) == LABEL_NAME)
			{
				 /* The label name is too long. */
				fprintf(stderr, "%sLabel name is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				continue; /* Move on to the next line */
			}
		}
	}
}


/* This function checks if there is a '\n' in the line.
 * If no, then the line is no full, so returnin 0, and returning 1 otherwise. */
int is_fully_line(char * line)
{
	return (strchr(line, '\n') != NULL);
}


/* This function checks if the label is in the correct syntax.
 * Return 1 if true, and 0 otherwise. */
int check_label_syntax(char * label_name, int LC)
{
	int j;

	/* Check if it is a part of saved words of registers */
	if(is_register(label_name))
	{
		fprintf(stderr, "%sLabel name '%s' is a saved word (in line %d)\n", PRINT_ERROR, label_name, LC);
		EC++;
		return 0;
	}

	/* Check if it a part of saved words of codes */
	if(is_instruction(label_name))
	{
		fprintf(stderr, "%sLabel name '%s' is a saved word (in line %d)\n", PRINT_ERROR, label_name, LC);
		EC++;
		return 0;
	}

	/* Check if it a part of saved words of guidances */
	if(is_guidance(label_name))
	{
		fprintf(stderr, "%sLabel name '%s' is a saved word (in line %d)\n", PRINT_ERROR, label_name, LC);
		EC++;
		return 0;
	}	

	/* We didnt find any errors so far, meaning the name isnt a saved word.
	 * So, we are going through the name, and checking it containing only letter and digits. */
	for(j = 0; j < strlen(label_name); j++)
	{
		if(j == 0)
		{
			if(!isalpha(label_name[j])) /* Label name must start with a letter */
			{
				fprintf(stderr, "%sLabel name '%s' must start with a letter (in line %d)\n", PRINT_ERROR, label_name, LC);
				EC++;
				return 0;
			}
		}
					
		if(!(isalpha(label_name[j]) || isdigit(label_name[j]))) /* There is illegal char in the name */
		{
			fprintf(stderr, "%sLabel name '%s' must contain only letters and digits (in line %d)\n", PRINT_ERROR, label_name, LC);
			EC++;
			return 0;
		}
	}

	return 1;
}


/* This function checks the guidance line.
 * Return 2 if there is a label at the begining of the line and it is an entry or extern guidance.
 * Return 0 if there are errors in the line, and 1 otherwise. */
int check_guidance_syntax(char * line, int LC, int index, int is_label)
{
	int j = 0, guidance_index = -1;
	char guide_name[LABEL_NAME];

	while(line[index] != ' ' && line[index] != '\t' && line[index] != '\n' && j != GUIDE_NAME)
	{
		guide_name[j] = line[index];
		index++;
		j++;
	}
	if(j == GUIDE_NAME) /* Illegal guidance */
	{
		fprintf(stderr, "%sGuidance is illegal (in line %d)\n", PRINT_ERROR, LC);
		EC++;
		return 0;
	}
	guide_name[j] = '\0';

	/* Searching for the guidance type */
	for(j=0; j < GUIDANCES; j++)
	{
		/* Found the type */
		if(strcmp(guidance_list[j], guide_name) == 0)
			guidance_index = j;
	}

	switch(guidance_index)
	{
		char label_name[LABEL_NAME];

		case 0: /* Data */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing data (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the guidance */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Check if there illegal commas */
			if(check_commas(line, index, LC) == 0)
			{
				return 0;
			}

			while(line[index] != '\n')
			{
				/* Skipping white spaces */
				while(line[index] == ' ' || line[index] == '\t')
					index++;
				
				/* Isnt a digit */
				if(!isdigit(line[index]))
				{
					/* The char is a comma, and it is ok because we alreay check all commas */
					if(line[index] == ',')
					{
						index++;
						continue; /* Move on to the next char */
					}

					/* + and - are llegal char, as long there is a number after them */
					if(line[index] == '+' || line[index] == '-')
					{
						/* The char afterwards is not a digit */
						if(!isdigit(line[index+1]))
						{
							fprintf(stderr, "%sIllegal data (in line %d)\n", PRINT_ERROR, LC);
							EC++;
							return 0;
						}
						else
						{
							index++;
							continue; /* Move on to the next char */
						}
					}
					else
					{
						fprintf(stderr, "%sIllegal data (in line %d)\n", PRINT_ERROR, LC);
						EC++;
						return 0;
					}
				}
				else
				{
					index++;
				}

			}

			/* Reach the new line, and everything went ok */
			return 1;

		case 1: /* String */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing string (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Checking if it start with " */
			if(line[index] != '"')
			{
				fprintf(stderr, "%sMissing opening quotes (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			index++;

			/* Running on the string until we reach the \" or end line */
			while(line[index] != '"' && index != LINE)
			{
				if(line[index] == '\n') /* Reach new line before closing quote */
				{
					fprintf(stderr, "%sMissing closing quotes (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}

				if(!isprint(line[index])) /* The char isnt printable */
				{
					if(line[index] != '\t') /* Tab is llegal char */
					{
						fprintf(stderr, "%sIllegal character in the string (in line %d)\n", PRINT_ERROR, LC);
						EC++;
						return 0;
					}
				}

				index++;
			}

			if(index == LINE) /* The line is longer that what is acceptable */
			{
				fprintf(stderr, "%sLine too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* The char is \", so we check if there is text afterwards */
			index++;

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			if(line[index] != '\n') /* There are char after end of string */
			{
				fprintf(stderr, "%sExtraneous text after end of string (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			return 1;

		case 2: /* Entry */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* There is no label name, it is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing label name (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Getting label name */
			j = 0;
			while(line[index] != ' ' && line[index] != '\t' && line[index] != '\n' && j != LABEL_NAME)
			{
				label_name[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Meaning the label name is too long */
			{
				fprintf(stderr, "%sLabel name is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}
			label_name[j] = '\0';

			/* Checking if the label name is in correct syntax */
			if(check_label_syntax(label_name, LC))
			{
				/* Skipping white spaces */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				/* There are char that are not white spaces after the label name */
				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after end of label (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}

				/* In the first round we ignore the entry */
				if(is_label == YES)
				{
					/* There is a label defined an entry line */
					return 2;
				}
				return 1;
			}

			else
			{
				return 0;
			}
			
			
		case 3: /* Extern */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* There is no label name, it is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing label name (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Getting label name */
			j = 0;
			while(line[index] != ' ' && line[index] != '\t' && line[index] != '\n' && j != LABEL_NAME)
			{
				label_name[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Meaning the label name is too long */
			{
				fprintf(stderr, "%sLabel name is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}
			label_name[j] = '\0';

			/* Checking if the label name is in correct syntax */
			if(check_label_syntax(label_name, LC))
			{
				/* The label name is ok.
				 * Need to insert it to the label table,
				 * But first need to check if it is already in there,
				 * and if so, need to check if it extern or not. */
				label_table * temp;

				/* Skipping white spaces */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				/* There are char that are not white spaces after the label name */
				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after end (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}

				/* The label table is not empty */
				if(head_label != NULL)
				{
					/* Checking if the label is already on the list */
					temp = check_in_label_table(label_name);

					/* There is alreay label with that name in the table */
					if(temp != NULL)
					{
						if(temp->is_extern == NO)
						{
							/* The label is not extern, meaning it is defined in the file */
							fprintf(stderr, "%sMultiple definitions to label '%s' (in line %d)\n", PRINT_ERROR, label_name, LC);
							EC++;
							return 0;
						}

						else
						{
							/* The label is alredy in the table, and it is extern, so no need to renter it */
							fprintf(stderr, "%sLabel '%s' was already decalred extern in the file (in line %d)\n", PRINT_WARN, label_name, LC);
							if(is_label == YES)
							{
								return 2;
							}
							return 1;
						}
					}
				}

				/* There is no label with that name in the table, so entring it to the table */
				temp = add_to_label_table(label_name, 0, UNKNOWN, YES, LC);
				if(temp == NULL) /* There are errors */
				{
					return 0;
				}
				head_label = temp;
				
				if(is_label == YES)
				{
					/* Meaning there is a label at the begining of the line */
					return 2;
				}
				return 1;
			}

			else /* The label name is not correct */
			{
				return 0;
			}



		default: /* It is unknown guidance */
			fprintf(stderr, "%sUnknown guidance (in line %d)\n", PRINT_ERROR, LC);
			EC++;
			return 0;
	}
}


/* This function check if there are illegal commas
 * Return 1 if everything is ok, and 0 otherwise */
int check_commas(char * line, int index, int LC)
{
	int j = index;

	/* Running on the line to see if there is illegal comma */
	for(; index < strlen(line); index++)
	{
		if(line[index] == ',')
		{
			if(index == (LINE - 1)) /* Reached the end of the line, and there is no more input */
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Check if there are two commas in a row */
			else
			{
				int h = index + 1;
						
				/* Skipping white spaces */
				while(line[h] == ' ' || line[h] == '\t')
					h++;

				/* Two commas in a row */
				if(line[h] == ',')
				{
					fprintf(stderr, "%sMultiple consecutive commas (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}

				/* There is comma and right after a new line */
				if(line[h] == '\n')
				{
					fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}

			}
		}
	}

	/* Running on the line to see if there is missing comma */
	for(; j < strlen(line); j++)
	{
		/* Reached a nonn digit char */
		if(!isdigit(line[j]))
		{
		 	/* Handle with - or + happens in the check_guidance_syntax function */

			/* Reached a white space, checking if there is a comma */
			if(line[j] == ' ' || line[j] == '\t')
			{
				/* Skipping white spaces */
				while(line[j] == ' ' || line[j] == '\t')
					j++;

				/* Reached a char which is not a white space.
				 * Can be: 1) comma. 2) new line */
				if(line[j] != ',' && line[j] != '\n')
				{
					fprintf(stderr, "%sMissing a comma (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}
			
			/* Skipping the white spaces after the comma */
			if(line[j] == ',')
			{
				j++;
				while(line[j] == ' ' || line[j] == '\t')
					j++;
			}
		}
	}

	/* Didnt find mistakes */
	return 1;
}


/* This function codes the guidance line into the data tabel.
 * Return the head of the new list. */
data_table * code_guidance(char * line, int index, int LC)
{
	/* Since the guidance line was already checked syntax-wise,
	 * we know that the line is ok, so we can find the guide easily */

	int j = 0, guidance_index = -1;
	char guide_name[LABEL_NAME], line_copy[LINE];
	char * strptr;
	data_table * head = head_data;

	strcpy(line_copy, line);


	/* Getting the guide name */
	strptr = strtok((line_copy + index), " \t");

	strcpy(guide_name, strptr);

	/* Searching for the guidance type */
	for(j=0; j < GUIDANCES; j++)
	{
		/* Found the type */
		if(strcmp(guidance_list[j], guide_name) == 0)
			guidance_index = j;
	}

	switch(guidance_index)
	{
		int num, j;
		char * resultptr;

		case 0: /* .data */
			strptr = strtok(NULL, " \t,\n");
			while(strptr != NULL)
			{
				num = strtol(strptr, &resultptr, 10);

				if(num > DATA_HIGH_BOUND || num < DATA_LOW_BOUD)
				{
					fprintf(stderr, "%sNumber out of range (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return NULL;
				}

				head = add_to_data_table_num(DC, num, LC);
				if(head == NULL) /* There are errors in adding the data */
				{
					return NULL;
				}
				head_data = head;

				DC++;
				strptr = strtok(NULL, " \t,\n");
			}

			break;

		case 1: /* .string */
			strptr = strtok(NULL, "\"");

			for(j = 0; j < strlen(strptr); j++)
			{
				head = add_to_data_table_str(DC, strptr[j], LC);
				if(head == NULL) /* There are errors in adding the data */
				{
					return NULL;
				}
				head_data = head;

				DC++;
			}

			/* Coding the \0 */
			head = add_to_data_table_str(DC, '\0', LC);
			if(head == NULL) /* There are errors in adding the data */
			{
				return NULL;
			}
			head_data = head;

			DC++;

			break;
	}

	return head;
}

/* This function checks the instruction line.
 * Return 0 if there are error in the line, and 1 otherwise. */
int check_instruction_syntax(char * line, int LC, int index)
{
	int j = 0, instruction_index = -1;
	char instruction_name[CODE_SIZE];

	while(line[index] != ' ' && line[index] != '\t' && line[index] != '\n' && j != CODE_SIZE)
	{
		instruction_name[j] = line[index];
		index++;
		j++;
	}

	if(j == CODE_SIZE) /* Illegal instruction */
	{
		fprintf(stderr, "%sInstruction is illegal (in line %d)\n", PRINT_ERROR, LC);
		EC++;
		return 0;
	}
	instruction_name[j] = '\0';

	/* Searching for the instruction */
	for(j = 0; j < CODES; j++)
	{
		/* Found the instruction */
		if(strcmp(instruction_list[j], instruction_name) == 0)
			instruction_index = j;
	}

	switch(instruction_index)
	{
		int source_method, dest_method;
		char operand[LABEL_NAME];

		case 1: /* cmp */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the instruction */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Next there must be a source operand.
			 * Legal source operand methods for cmp are: 0,1,2,3 */

			j = 0;

			/* Getting the source operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sSource operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == '\n') /* End of line, with no second operand */
			{
				fprintf(stderr, "%sMissing destination operand (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			operand[j] = '\0';

			/* Checking source method */
			if(check_method(operand, &source_method, LC) == 0)
			{
				return 0;
			}

			/* Since any type of method is acceptable as a source method here, no need to check it */

			/* Next there must be a comma */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			if(line[index] != ',')
			{
				fprintf(stderr, "%sMissing comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Checking there is no two commas in a row */

			index++;

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			if(line[index] == ',')
			{
				fprintf(stderr, "%sMultiple consecutive commas (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Next there must be a destination operand.
			 * Legal destination operand methods for cmp are: 0,1,2,3 */

			j = 0;

			/* Getting the destination operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sDestination operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == ' ' || line[index] == '\t' || line[index] == ',')
			{
				/* Reach white space, so need to check there is no more text afterwards */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}

			operand[j] = '\0';

			/* Checking the destination operand */
			if(check_method(operand, &dest_method, LC) == 0)
			{
				return 0;
			}

			/* Since any type of method is acceptable as a destination method here, no need to check it */


			/* Everything is ok */
			return 1;


		case 0: /* mov */
		case 2: /* add */
		case 3: /* sub */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the instruction */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Next there must be a source operand.
			 * Legal source operand methods for mov, add, sub are: 0,1,2,3 */

			j = 0;

			/* Getting the source operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sSource operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == '\n') /* End of line, with no second operand */
			{
				fprintf(stderr, "%sMissing destination operand (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			operand[j] = '\0';

			/* Checking source method */
			if(check_method(operand, &source_method, LC) == 0)
			{
				return 0;
			}

			/* Since any type of method is acceptable as a source method here, no need to check it */

			/* Next there must be a comma */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			if(line[index] != ',')
			{
				fprintf(stderr, "%sMissing comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Checking there is no two commas in a row */

			index++;

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			if(line[index] == ',')
			{
				fprintf(stderr, "%sMultiple consecutive commas (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Next there must be a destination operand.
			 * Legal destination operand methods for mov, add, sub are: 1,2,3 */

			j = 0;

			/* Getting the destination operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sDestination operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == ' ' || line[index] == '\t' || line[index] == ',')
			{
				/* Reach white space, so need to check there is no more text afterwards */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}

			operand[j] = '\0';

			/* Checking the destination operand */
			if(check_method(operand, &dest_method, LC) == 0)
			{
				return 0;
			}

			/* Need to the check that destination method is acceptable */
			if(dest_method == 0)
			{
				fprintf(stderr, "%sDestination method is illegal (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Everything is ok */
			return 1;


		case 4: /* lea */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the instruction */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Next there must be a source operand.
			 * Legal source operand methods for lea are: 1 */

			j = 0;

			/* Getting the source operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sSource operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == '\n') /* End of line, with no second operand */
			{
				fprintf(stderr, "%sMissing destination operand (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			operand[j] = '\0';

			/* Checking source method */
			if(check_method(operand, &source_method, LC) == 0)
			{
				return 0;
			}

			/* Need to the check that source method is acceptable */
			if(source_method != 1)
			{
				fprintf(stderr, "%sSource method is illegal (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Next there must be a comma */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			if(line[index] != ',')
			{
				fprintf(stderr, "%sMissing comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Checking there is no two commas in a row */

			index++;

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			if(line[index] == ',')
			{
				fprintf(stderr, "%sMultiple consecutive commas (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Next there must be a destination operand.
			 * Legal destination operand methods for lea are: 1,2,3 */

			j = 0;

			/* Getting the destination operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sDestination operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == ' ' || line[index] == '\t' || line[index] == ',')
			{
				/* Reach white space, so need to check there is no more text afterwards */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}

			operand[j] = '\0';

			/* Checking the destination operand */
			if(check_method(operand, &dest_method, LC) == 0)
			{
				return 0;
			}

			/* Need to the check that destination method is acceptable */
			if(dest_method == 0)
			{
				fprintf(stderr, "%sDestination method is illegal (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Everything is ok */
			return 1;


		case 5: /* clr */
		case 6: /* not */
		case 7: /* inc */
		case 8: /* dec */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the instruction */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}


			/* Next there must be a destination operand.
			 * Legal destination operand methods for clr, not, inc, lea are: 1,2,3 */

			j = 0;

			/* Getting the destination operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sDestination operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == ' ' || line[index] == '\t')
			{
				/* Reach white space, so need to check there is no more text afterwards */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}

			operand[j] = '\0';

			/* Checking the destination operand */
			if(check_method(operand, &dest_method, LC) == 0)
			{
				return 0;
			}

			/* Need to the check that destination method is acceptable */
			if(dest_method == 0)
			{
				fprintf(stderr, "%sDestination method is illegal (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Everything is ok */
			return 1;


		case 9: /* jmp */
		case 10: /* bne */
		case 13: /* jsr */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the instruction */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}


			/* Next there must be a destination operand.
			 * Legal destination operand methods for jmp, bne, jsr are: 1,2 */

			j = 0;

			/* Getting the destination operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sDestination operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == ' ' || line[index] == '\t')
			{
				/* Reach white space, so need to check there is no more text afterwards */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}

			operand[j] = '\0';

			/* Checking the destination operand */
			if(check_method(operand, &dest_method, LC) == 0)
			{
				return 0;
			}

			/* Need to the check that destination method is acceptable */
			if(dest_method == 0 || dest_method == 3)
			{
				fprintf(stderr, "%sDestination method is illegal (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Everything is ok */
			return 1;


		case 11: /* red */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the instruction */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}


			/* Next there must be a destination operand.
			 * Legal destination operand methods for red are: 1,2,3 */

			j = 0;

			/* Getting the destination operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sDestination operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == ' ' || line[index] == '\t')
			{
				/* Reach white space, so need to check there is no more text afterwards */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}

			operand[j] = '\0';

			/* Checking the destination operand */
			if(check_method(operand, &dest_method, LC) == 0)
			{
				return 0;
			}

			/* Need to the check that destination method is acceptable */
			if(dest_method == 0)
			{
				fprintf(stderr, "%sDestination method is illegal (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Everything is ok */
			return 1;


		case 12: /* prn */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* It is an empty line */
			if(line[index] == '\n')
			{
				fprintf(stderr, "%sMissing operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Illegal comma right after the instruction */
			if(line[index] == ',')
			{
				fprintf(stderr, "%sIllegal comma (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}


			/* Next there must be a destination operand.
			 * Legal destination operand methods for prn are: 0,1,2,3 */

			j = 0;

			/* Getting the destination operand.
			 * Going until we reach a white space or comma.
			 * If we reach new line or we getting to the limit of the string its a mistake. */
			while(line[index] != ' ' && line[index] != '\t' && line[index] != ',' && line[index] != '\n' && j != LABEL_NAME)
			{
				operand[j] = line[index];
				index++;
				j++;
			}

			if(j == LABEL_NAME) /* Too long */
			{
				fprintf(stderr, "%sDestination operand is too long (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			if(line[index] == ' ' || line[index] == '\t')
			{
				/* Reach white space, so need to check there is no more text afterwards */
				while(line[index] == ' ' || line[index] == '\t')
					index++;

				if(line[index] != '\n')
				{
					fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
					EC++;
					return 0;
				}
			}

			operand[j] = '\0';

			/* Checking the destination operand */
			if(check_method(operand, &dest_method, LC) == 0)
			{
				return 0;
			}

			/* Since any type of method is acceptable as a destination method here, no need to check it */


			/* Everything is ok */
			return 1;


		case 14: /* rts */
		case 15: /* stop */

			/* Skipping white spaces */
			while(line[index] == ' ' || line[index] == '\t')
				index++;

			/* Next there must be a new line */

			if(line[index] != '\n')
			{
				fprintf(stderr, "%sExtraneous text after operands (in line %d)\n", PRINT_ERROR, LC);
				EC++;
				return 0;
			}

			/* Everything is ok */
			return 1;


		default: /* Unknon instruction */
			fprintf(stderr, "%sUnknown instruction (in line %d)\n", PRINT_ERROR, LC);
			EC++;
			return 0;
	}


	return 1;
}


/* This function codes the instruction line into the instruction tabel.
 * Return the head of the new list. */
instruction_table * code_instruction(char * line, int index, int LC)
{
	/* Since the instruction line was already checked syntax-wise,
	 * we know that the line is ok, so we can find the instruction easily */

	int j = 0, instruction_index = -1;
	char instruction_name[LABEL_NAME], line_copy[LINE];
	char * strptr;
	instruction_table * head;

	strcpy(line_copy, line);


	/* Getting the instruction name */
	strptr = strtok((line_copy + index), " \t\n");

	strcpy(instruction_name, strptr);

	/* Searching for the guidance type */
	for(j=0; j < CODES; j++)
	{
		/* Found the type */
		if(strcmp(instruction_list[j], instruction_name) == 0)
			instruction_index = j;
	}

	switch(instruction_index)
	{
		char source_operand[LABEL_NAME], dest_operand[LABEL_NAME];
		int source_method, dest_method;
		char * resultptr;

		/* The ARE field can be 4 (if the A is on), 2 (if the R is on) and 1 (if the E is on). */

		/* Two operands instructions */
		case 0: /* mov */
		case 1: /* cmp */
		case 2: /* add */
		case 3: /* sub */
		case 4: /* lea */

			/* Getting the source operand */
			strptr = strtok(NULL, " \t,");

			strcpy(source_operand, strptr);

			/* Getting the source method type */
			check_method(source_operand, &source_method, 0);

			/* Getting the destination operand */
			strptr = strtok(NULL, " \t\n,");

			strcpy(dest_operand, strptr);

			/* Getting the destination method type */
			check_method(dest_operand, &dest_method, 0);

			/* Adding the first word to the table */
			head = add_to_instruction_table_first(IC + BUFFER, instruction_index, source_method, dest_method, A, LC);
			if(head == NULL) /* There are errors in adding the instruction */
			{
				return NULL;
			}
			head_instruction = head;
			

			IC++;

			/* The source method is type 2 or 3,
			 * and the destination method is type 2 or 3,
			 * Meaning we need to add only one more word */
			if((source_method == 2 || source_method == 3) && (dest_method == 2 || dest_method == 3))
			{
				/* Because the source and dest methods are 2 or 3, the operands are registers, 
				 * Meaning that the last char is the register number. */
				head = add_to_instruction_table_2_3(IC + BUFFER, source_operand[strlen(source_operand) - 1], dest_operand[strlen(dest_operand) - 1], A, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
				break; /* Finished codeing this line */
			}

			/* Coding the source */
			if(source_method == 2 || source_method == 3)
			{
				/* Because the source method is 2 or 3, the operands are registers, 
				 * Meaning that the last char is the register number. */

				head = add_to_instruction_table_2_3(IC + BUFFER, source_operand[strlen(source_operand) - 1], 0, A, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;
			

				IC++;
			}
			else if(source_method == 0)
			{
				/* The operand is a number, which come right after the # sign */
				head = add_to_instruction_table_0(IC + BUFFER, strtol(source_operand + 1, &resultptr, 10), A, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}
			else
			{
				/* The operand is a label, which we will fill in the second round */
				head = add_to_instruction_table_1(IC + BUFFER, source_operand, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}


			/* Coding the destination */
			if(dest_method == 2 || dest_method == 3)
			{
				/* Because the dest method is 2 or 3, the operands are registers, 
				 * Meaning that the last char is the register number. */
				head = add_to_instruction_table_2_3(IC + BUFFER, 0, dest_operand[strlen(dest_operand) - 1], A, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}
			else if(dest_method == 0)
			{
				/* The operand is a number, which come right after the # sign */
				head = add_to_instruction_table_0(IC + BUFFER, strtol(dest_operand + 1, &resultptr, 10), A, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}
			else
			{
				/* The operand is a label, which we will fill in the second round */
				head = add_to_instruction_table_1(IC + BUFFER, dest_operand, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}

			break;

		/* One operand instructions */
		case 5: /* clr */
		case 6: /* not */
		case 7: /* inc */
		case 8: /* dec */
		case 9: /* jmp */
		case 10: /* bne */
		case 11: /* red */
		case 12: /* prn */
		case 13: /* jsr */

			/* There is no source method, only destination */
			source_method = -1;

			/* Getting the destination operand */
			strptr = strtok(NULL, " \t\n");

			strcpy(dest_operand, strptr);

			/* Getting the destination method type */
			check_method(dest_operand, &dest_method, 0);

			/* Adding the first word to the table */
			head = add_to_instruction_table_first(IC + BUFFER, instruction_index, source_method, dest_method, A, LC);
			if(head == NULL) /* There are errors in adding the instruction */
			{
				return NULL;
			}
			head_instruction = head;

			IC++;


			/* Coding the destination */
			if(dest_method == 2 || dest_method == 3)
			{
				/* Because the dest method is 2 or 3, the operands are registers, 
				 * Meaning that the last char is the register number. */
				head = add_to_instruction_table_2_3(IC + BUFFER, 0, dest_operand[strlen(dest_operand) - 1], A, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}
			else if(dest_method == 0)
			{
				/* The operand is a number, which come right after the # sign */
				head = add_to_instruction_table_0(IC + BUFFER, strtol(dest_operand + 1, &resultptr, 10), A, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}
			else
			{
				/* The operand is a label, which we will fill in the second round */
				head = add_to_instruction_table_1(IC + BUFFER, dest_operand, LC);
				if(head == NULL) /* There are errors in adding the instruction */
				{
					return NULL;
				}
				head_instruction = head;

				IC++;
			}

			break;

		/* Zero operands instructions */
		case 14: /* rts */
		case 15: /* stop */

			/* There is no source method */
			source_method = -1;

			/* There is no destination method */
			dest_method = -1;
			
			/* Adding the only word to the table */
			head = add_to_instruction_table_first(IC + BUFFER, instruction_index, source_method, dest_method, A, LC);
			if(head == NULL) /* There are errors in adding the instruction */
			{
				return NULL;
			}
			head_instruction = head;

			IC++;

			break;

	}

	return head;
}
