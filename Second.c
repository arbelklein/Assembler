#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "Second.h"
#include "Types.h"
#include "LabelTable.h"
#include "DataTable.h"
#include "InstructionTable.h"
#include "EntryTable.h"
#include "ExternTable.h"

void SecondRound(FILE *fptr)
{
	/* In the second round,  we are looking for entry lines.
	 * If we found one, we are checking in the label table, that they exist, and if so we entring it to the entry table.
	 * Also, we are going through the instruction list, and adding to the extern list everytime that we came across extern label. */

	int i, LC = 0; /* LC= Line Counter */
	char line[LINE], line_copy[LINE];
	char *strptr;
	entry_table * temp_entry;
	extern_table * temp_extern;
	instruction_table * temp_instruct;
	int is_errors = 0;

	update_label_table(); /* Updating the label table */
	update_instruction_table(); /* Updating the instruction table */
	update_data_table(); /* Updating the data table */

	while(fgets(line, LINE, fptr) != NULL)
	{
		i = 0;
		LC++;

		strcpy(line_copy, line);

		/* Skipping white spaces */
		while(line_copy[i] == ' ' || line_copy[i] == '\t')
			i++;

		/* Empty line or comment line */
		if(line_copy[i] == '\n' || line_copy[i] == ';')
			continue; /* Move on to the next line */

		/* We checked syntax errors in the first round, so we know that the line is in the correct syntax.
		 * So we can run on it freely */

		/* Getting the first word in the line */
		strptr = strtok((line_copy + i), " \t");

		/* It is a label */
		if(strptr[strlen(strptr) - 1] == ':')
		{
			strptr = strtok(NULL, " \t");

			/* The first char of guidance is '.' */
			strptr++;

			/* Label at guidance line */
			if(is_guidance(strptr))
			{
				/* It is no an entry line, so we already dealt with that in the first round */
				if(!is_entry(strptr))
				{
					continue; /* Move on to the next line */
				}

				/* Getting the label name */
				strptr = strtok(NULL, " \t\n");

				temp_entry = add_to_entry_table(strptr, LC);
				if(temp_entry == NULL) /* There are errors */
				{
					is_errors = 1;
					continue; /* Move on to the next line */
				}
				head_entry = temp_entry;
			}

			/* Label at Instruction line */
			else
			{
				continue; /* Move on to the next line */
			}
		}

		/* Guidance line.
		 * The first char of guidance is '.' */
		else if(is_guidance(++strptr))
		{
			/* It is no an entry line, so we already dealt with that in the first round */
			if(!is_entry(strptr))
			{
				continue; /* Move on to the next line */
			}

			/* Getting the label name */
			strptr = strtok(NULL, " \t\n");

			temp_entry = add_to_entry_table(strptr, LC);
			if(temp_entry == NULL) /* There are errors */
			{
				is_errors = 1;
				continue; /* Move on to the next line */
			}
			head_entry = temp_entry;
		}

		/* Instruction line */
		else
		{
			continue; /* Move on to the next line */
		}
	}

	if(!is_errors) /* There arent errors */
	{
		/* Going through the instruction list and adding to the extern list everytime we get to extern list */
		temp_instruct = head_instruction;
		while(temp_instruct != NULL)
		{
			/* This is a label instruct */
			if(temp_instruct->is_label == YES)
			{
				/* The label is extern */
				if(temp_instruct->code.second_word_1.ARE == E)
				{
					temp_extern = add_to_extern_table(temp_instruct->label_name, temp_instruct->address);
					if(temp_extern == NULL) /* There are errors */
					{
						return;
					}
					head_extern = temp_extern;
				}
			}

			temp_instruct = temp_instruct->next;
		}
	}
}


/* This function check if the guidance line is entry.
 * Return 1 if true, and 0 otherwise. */
int is_entry(char * guide)
{
	if(strcmp(guide, "entry") == 0)
		return 1;
	return 0;
}
