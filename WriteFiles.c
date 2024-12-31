#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "WriteFiles.h"

#define ARE_SIZE 3 /* Number of bits in the ARE field in a word */
#define DATA_SIZE 15 /* Number of bits in the data word */
#define OPCODE_SIZE 4 /* Number of bits in the opcode field in a word */
#define OPERAND_SIZE 12 /* Number of bits in the operand field in a word */
#define SRC 3 /* Number of bits in the source method field on a word */
#define DEST 3 /* Number of bits in the destination method field in a word */


void write_to_files(char * name)
{
	char ob_extension[] = ".ob", ent_extension[] = ".ent", ext_extension[] = ".ext";
	char * file_name;
	FILE * fptr;


	/* Creating the object file
	 * If the instruction list is empty and the data list is empty
	 * no need to create the file. */
	if(head_instruction != NULL || head_data != NULL)
	{
		instruction_table * temp_instruct = head_instruction;
		data_table * temp_data = head_data;

		file_name = (char *)malloc((strlen(name) + 1) * sizeof(char));
		if(file_name == NULL) /* Failed at allocation */
		{
			fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
			exit(0);
		}

		strcpy(file_name, name);
		strcat(file_name, ob_extension);

		fptr = fopen(file_name, "w");
		if(fptr == NULL) /* Failed opening file */
		{
			fprintf(stderr, "%sFailed opening file '%s'\n", PRINT_ERROR, file_name);
			return;
		}

		/* Writing the headline of the object file */
		fprintf(fptr, "  %d %d\n", IC, DC);

		/* Writing the instuction part to the object file */
		while(temp_instruct != NULL)
		{
			if(temp_instruct->is_first == YES)
			{
				fprintf(fptr, "%04d %05o\n", temp_instruct->address, code_word_to_int(temp_instruct->code, 0).print);
			}
			if(temp_instruct->is_second_0_1 == YES && temp_instruct->is_label == NO)
			{
				fprintf(fptr, "%04d %05o\n", temp_instruct->address, code_word_to_int(temp_instruct->code, 1).print);
			}
			if(temp_instruct->is_second_0_1 == YES && temp_instruct->is_label == YES)
			{
				fprintf(fptr, "%04d %05o\n", temp_instruct->address, code_word_to_int(temp_instruct->code, 2).print);
			}
			if(temp_instruct->is_second_2_3 == YES)
			{
				fprintf(fptr, "%04d %05o\n", temp_instruct->address, code_word_to_int(temp_instruct->code, 3).print);
			}

			temp_instruct = temp_instruct->next;
		}


		/* Writing the data part to the object file */
	       while(temp_data != NULL)
	       {
		       fprintf(fptr, "%04d %05o\n", temp_data->address, code_word_to_int(temp_data->code, 4).print);

		       temp_data = temp_data->next;
	       }

	       free(file_name);
	}



	/* Creting the entry file.
	 * If the entry list is empty - Meaning there is no entry line,
	 * no need to create the file. */
	if(head_entry != NULL)
	{
		entry_table * temp_entry = head_entry;

		file_name = (char *)malloc((strlen(name) + 1) * sizeof(char));
		if(file_name == NULL) /* Failed at allocation */
		{
			fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
			exit(0);
		}

		strcpy(file_name, name);
		strcat(file_name, ent_extension);

		fptr = fopen(file_name, "w");
		if(fptr == NULL) /* Failed opening file */
		{
			fprintf(stderr, "%sFailed opening file '%s'\n", PRINT_ERROR, file_name);
			return;
		}

		/* Going through the entry list, and entring to the file */
		while(temp_entry != NULL)
		{
			fprintf(fptr, "%s\t%04d\n", temp_entry->entry_name, temp_entry->address);
			temp_entry = temp_entry->next;
		}

		free(file_name);
	}



	/* Creting the extern file.
	 * If the extern list is empty - Meaning there is no extern line,
	 * no need to create the file. */
	if(head_extern != NULL)
	{
		extern_table * temp_extern = head_extern;

		file_name = (char *)malloc((strlen(name) + 1) * sizeof(char));
		if(file_name == NULL) /* Failed at allocation */
		{
			fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
			exit(0);
		}

		strcpy(file_name, name);
		strcat(file_name, ext_extension);

		fptr = fopen(file_name, "w");
		if(fptr == NULL) /* Failed opening file */
		{
			fprintf(stderr, "%sFailed opening file '%s'\n", PRINT_ERROR, file_name);
			return;
		}

		/* Going through the entry list, and entring to the file */
		while(temp_extern != NULL)
		{
			fprintf(fptr, "%s\t%04d\n", temp_extern->extern_name, temp_extern->address);
			temp_extern = temp_extern->next;
		}

		free(file_name);
	}
}


/* This function codes the word into an int.
 * It get a number to signal which word it is:
 * 0 - It is a first word of instruction,
 * 1 - It is a second word of instruction of method 0,
 * 2 - It is a second word of instruction of method 1,
 * 3 - It is a second word of instruction of method 2 and 3,
 * 4 - It is a data word.
 * Return the number. */
struct print_word code_word_to_int(union word num, int what)
{
	struct print_word bit_op, value;

	bit_op.print = 1;
	value.print = 0;

	switch(what)
	{
		int i, j;
		case 0: /* First word of instruction */

			/* Coding the ARE field. */
			for(i = 0; i < ARE_SIZE; i++)
			{
				value.print |= (bit_op.print & num.first_word.ARE);
				bit_op.print = bit_op.print << 1;
			}

			/* Coding the dest method */

			/* Method 0 */
			value.print |= (bit_op.print & (num.first_word.destination_method_0 << i));
			bit_op.print = bit_op.print << 1;
			i++;

			/* Method 1 */
			value.print |= (bit_op.print & (num.first_word.destination_method_1 << i));
			bit_op.print = bit_op.print << 1;
			i++;

			/* Method 2 */
			value.print |= (bit_op.print & (num.first_word.destination_method_2 << i));
			bit_op.print = bit_op.print << 1;
			i++;

			/* Method 3 */
			value.print |= (bit_op.print & (num.first_word.destination_method_3 << i));
			bit_op.print = bit_op.print << 1;
			i++;

			/* Codind the source method */

			/* Method 0 */
			value.print |= (bit_op.print & (num.first_word.source_method_0 << i));
			bit_op.print = bit_op.print << 1;
			i++;

			/* Method 1 */
			value.print |= (bit_op.print & (num.first_word.source_method_1 << i));
			bit_op.print = bit_op.print << 1;
			i++;

			/* Method 2 */
			value.print |= (bit_op.print & (num.first_word.source_method_2 << i));
			bit_op.print = bit_op.print << 1;
			i++;

			/* Method 3 */
			value.print |= (bit_op.print & (num.first_word.source_method_3 << i));
			bit_op.print = bit_op.print << 1;
			i++;


			/* Coding the opcode */
			for(j = 0; j < OPCODE_SIZE; j++)
			{
				value.print |= (bit_op.print & (num.first_word.opcode << i));
				bit_op.print = bit_op.print << 1;
			}

			break;


		case 1: /* Second word of instrucion of method 0 */

			/* Coding the ARE field. */
			for(i = 0; i < ARE_SIZE; i++)
			{
				value.print |= (bit_op.print & num.second_word_0.ARE);
				bit_op.print = bit_op.print << 1;
			}

			/* Coding the operand */
			for(j = 0; j < OPERAND_SIZE; j++)
			{
				value.print |= (bit_op.print & (num.second_word_0.operand << i));
				bit_op.print = bit_op.print << 1;
			}

			break;

		case 2: /* Second word of instruction of method 1 */

			/* Coding the ARE field. */
			for(i = 0; i < ARE_SIZE; i++)
			{
				value.print |= (bit_op.print & num.second_word_1.ARE);
				bit_op.print = bit_op.print << 1;
			}

			/* Coding the operand */
			for(j = 0; j < OPERAND_SIZE; j++)
			{
				value.print |= (bit_op.print & (num.second_word_1.operand << i));
				bit_op.print = bit_op.print << 1;
			}

			break;

		case 3: /* Second word of instrucion of method 2 and 3 */

			/* Coding the ARE field. */
			for(i = 0; i < ARE_SIZE; i++)
			{
				value.print |= (bit_op.print & num.second_word_2_3.ARE);
				bit_op.print = bit_op.print << 1;
			}

			/* Coding the destination method field */
			for(j = 0; j < DEST; j++)
			{
				value.print |= (bit_op.print & (num.second_word_2_3.destination_method << i));
				bit_op.print = bit_op.print << 1;
			}

			i += j;

			/* Coding the source method field */
			for(j = 0; j < SRC; j++)
			{
				value.print |= (bit_op.print & (num.second_word_2_3.source_method << i));
				bit_op.print = bit_op.print << 1;
			}

			break;

		case 4: /* Data word */

			/* Coding the data field */
			for(i = 0; i < DATA_SIZE; i++)
			{
				value.print |= (bit_op.print & num.data_word.data);
				bit_op.print = bit_op.print << 1;
			}

	}


	return value;
}
