#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "InstructionTable.h"
#include "LabelTable.h"

/* This function insert the first word to the instruction table (to the end of the table).
 * Return the head of the new list. */
instruction_table * add_to_instruction_table_first(int address, int opcode, int source_method, int dest_method, int ARE, int LC)
{
	instruction_table * head = head_instruction;
	instruction_table * temp = head;
	instruction_table * new = (instruction_table *)malloc(sizeof(instruction_table));

	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->address = address;

	new->code.first_word.opcode = opcode;

	if(source_method == -1) /* There is no source method */
	{
		new->code.first_word.source_method_0 = 0;
                new->code.first_word.source_method_1 = 0;
                new->code.first_word.source_method_2 = 0;
                new->code.first_word.source_method_3 = 0;
	}
	if(source_method == 0)
	{
		new->code.first_word.source_method_0 = 1;
		new->code.first_word.source_method_1 = 0;
		new->code.first_word.source_method_2 = 0;
		new->code.first_word.source_method_3 = 0;
	}	
	if(source_method == 1)
	{
		new->code.first_word.source_method_0 = 0;
		new->code.first_word.source_method_1 = 1;
		new->code.first_word.source_method_2 = 0;
		new->code.first_word.source_method_3 = 0;
	}	
	if(source_method == 2)
	{
		new->code.first_word.source_method_0 = 0;
		new->code.first_word.source_method_1 = 0;
		new->code.first_word.source_method_2 = 1;
		new->code.first_word.source_method_3 = 0;
	}
	if(source_method == 3)
	{
		new->code.first_word.source_method_0 = 0;
		new->code.first_word.source_method_1 = 0;
		new->code.first_word.source_method_2 = 0;
		new->code.first_word.source_method_3 = 1;
	}


	if(dest_method == -1) /* There is no dest method */
	{
		new->code.first_word.destination_method_0 = 0;
                new->code.first_word.destination_method_1 = 0;
                new->code.first_word.destination_method_2 = 0;
                new->code.first_word.destination_method_3 = 0;
	}
	if(dest_method == 0)
	{
		new->code.first_word.destination_method_0 = 1;
		new->code.first_word.destination_method_1 = 0;
		new->code.first_word.destination_method_2 = 0;
		new->code.first_word.destination_method_3 = 0;
	}	
	if(dest_method == 1)
	{
		new->code.first_word.destination_method_0 = 0;
		new->code.first_word.destination_method_1 = 1;
		new->code.first_word.destination_method_2 = 0;
		new->code.first_word.destination_method_3 = 0;
	}
	if(dest_method == 2)
	{
		new->code.first_word.destination_method_0 = 0;
		new->code.first_word.destination_method_1 = 0;
		new->code.first_word.destination_method_2 = 1;
		new->code.first_word.destination_method_3 = 0;
	}
	if(dest_method == 3)
	{
		new->code.first_word.destination_method_0 = 0;
		new->code.first_word.destination_method_1 = 0;
		new->code.first_word.destination_method_2 = 0;
		new->code.first_word.destination_method_3 = 1;	
	}

	new->code.first_word.ARE = ARE;
	new->is_first = YES;
	new->is_second_0_1 = NO;
	new->is_label = NO;
	new->label_name = NULL;
	new->is_second_2_3 = NO;
	new->line_in_file = LC;

	new->next = NULL;

	if(head == NULL) /* The list is empty */
	{
		head = new;
		return head;
	}

	/* Going to the end of the list */
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = new;

	return head;
}


/* This function insert the second word for method 0 to the instruction table (to the end of the table).
 * Return the head of the new list. */
instruction_table * add_to_instruction_table_0(int address, int operand, int ARE, int LC)
{
	instruction_table * head = head_instruction;
	instruction_table * temp = head;
	instruction_table * new = (instruction_table *)malloc(sizeof(instruction_table));

	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->address = address;
	new->code.second_word_0.operand = operand;
	new->code.second_word_0.ARE = ARE;
	new->is_first = NO;
	new->is_second_0_1 = YES;
	new->is_label = NO;
	new->label_name = NULL;
	new->is_second_2_3 = NO;
	new->line_in_file = LC;
	new->next = NULL;

	if(head == NULL) /* The list is empty */
	{
		return new;
	}

	/* Going to the end of the list */
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = new;

	return head;

}


/* This function insert the second word for method 1 to the instruction table (to the end of the table).
 * Return the head of the new list. */
instruction_table * add_to_instruction_table_1(int address, char * label_name, int LC)
{
	instruction_table * head = head_instruction;
	instruction_table * temp = head;
	instruction_table * new = (instruction_table *)malloc(sizeof(instruction_table));

	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->address = address;
	new->code.second_word_1.operand = 0;
	new->code.second_word_1.ARE = 0;
	new->is_first = NO;
	new->is_second_0_1 = YES;
	new->is_label = YES;

	new->label_name = (char *)malloc((strlen(label_name) + 1) * sizeof(char));
	if(new->label_name == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocation memory\n", PRINT_FATAL);
		exit(0);
	}
	strcpy(new->label_name, label_name);
	
	new->is_second_2_3 = NO;
	new->line_in_file = LC;

	new->next = NULL;

	if(head == NULL) /* The list is empty */
	{
		return new;
	}

	/* Going to the end of the list */
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = new;

	return head;

}


/* This function insert the second word for method 2 and 3 to the instruction table (to the end of the table).
 * Return the head of the new list. */
instruction_table * add_to_instruction_table_2_3(int address, int source, int dest, int ARE, int LC)
{
	instruction_table * head = head_instruction;
	instruction_table * temp = head;
	instruction_table * new = (instruction_table *)malloc(sizeof(instruction_table));

	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->address = address;
	new->code.second_word_2_3.source_method = source;
	new->code.second_word_2_3.destination_method = dest;
	new->code.second_word_2_3.ARE = ARE;
	new->is_first = NO;
	new->is_second_0_1 = NO;
	new->is_label = NO;
	new->label_name = NULL;
	new->is_second_2_3 = YES;
	new->line_in_file = LC;
	new->next = NULL;

	if(head == NULL) /* The list is empty */
	{
		return new;
	}

	/* Going to the end of the list */
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = new;

	return head;

}


/* This function prints the instruction table */
void print_instruction_table()
{
	instruction_table * temp = head_instruction;

	printf("Instruction Table:\n");

	while(temp != NULL)
	{
		if(temp->is_first == YES)
		{
			printf("\nNode:\taddress= %d,\t", temp->address);

			printf("opcode= %d,\t", temp->code.first_word.opcode);

			if(temp->code.first_word.source_method_0)
				printf("source_method= %d,\t", 0);
			if(temp->code.first_word.source_method_1)
				printf("source_method= %d,\t", 1);
			if(temp->code.first_word.source_method_2)
				printf("source_method= %d,\t", 2);
			if(temp->code.first_word.source_method_3)
				printf("source_method= %d,\t", 3);

			if(temp->code.first_word.destination_method_0)
				printf("dest_method= %d,\t", 0);
			if(temp->code.first_word.destination_method_1)
				printf("dest_method= %d,\t", 1);
			if(temp->code.first_word.destination_method_2)
				printf("dest_method= %d,\t", 2);
			if(temp->code.first_word.destination_method_3)
				printf("dest_method= %d,\t", 3);

			if(temp->code.first_word.ARE == A)
				printf("ARE= %c,\t", 'A');
			if(temp->code.first_word.ARE == R)
				printf("ARE= %c,\t", 'R');
			if(temp->code.first_word.ARE == E)
				printf("ARE= %c,\t", 'E');
			if(temp->code.first_word.ARE == 0)
				printf("ARE= %d,\t", 0);

			printf("line= %d\n", temp->line_in_file);
		}
		if(temp->is_second_0_1 == YES && temp->is_label == NO)
		{
			printf("\nNode:\taddress= %d,\t", temp->address);

			printf("operand= %d,\t", temp->code.second_word_0.operand);

			if(temp->code.second_word_0.ARE == A)
				printf("ARE= %c,\t", 'A');
			if(temp->code.second_word_0.ARE == R)
				printf("ARE= %c,\t", 'R');
			if(temp->code.second_word_0.ARE == E)
				printf("ARE= %c,\t", 'E');
			if(temp->code.second_word_0.ARE == 0)
				printf("ARE= %d,\t", 0);

			printf("line= %d\n", temp->line_in_file);
		}
		if(temp->is_second_0_1 == YES && temp->is_label == YES)
		{
			printf("\nNode:\taddress= %d,\t", temp->address);

			printf("operand= %d,\t", temp->code.second_word_1.operand);

			printf("label= '%s',\t", temp->label_name);

			if(temp->code.second_word_1.ARE == A)
				printf("ARE= %c,\t", 'A');
			if(temp->code.second_word_1.ARE == R)
				printf("ARE= %c,\t", 'R');
			if(temp->code.second_word_1.ARE == E)
				printf("ARE= %c,\t", 'E');
			if(temp->code.second_word_1.ARE == 0)
				printf("ARE= %d,\t", 0);

			printf("line= %d\n", temp->line_in_file);
		}
		if(temp->is_second_2_3 == YES)
		{
			printf("\nNode:\taddress= %d,\t", temp->address);

			printf("source= %d,\t", temp->code.second_word_2_3.source_method);

			printf("destination= %d,\t", temp->code.second_word_2_3.destination_method);

			if(temp->code.second_word_2_3.ARE == A)
				printf("ARE= %c,\t", 'A');
			if(temp->code.second_word_2_3.ARE == R)
				printf("ARE= %c,\t", 'R');
			if(temp->code.second_word_2_3.ARE == E)
				printf("ARE= %c,\t", 'E');
			if(temp->code.second_word_2_3.ARE == 0)
				printf("ARE= %d,\t", 0);

			printf("line= %d\n", temp->line_in_file);
		}
		temp = temp->next;
	}
}


/* This function free the data table */
void free_instruction_table()
{
	instruction_table * curr;
	instruction_table * temp;

	if(head_instruction != NULL)
	{
		curr = head_instruction->next;
		head_instruction = NULL;

		while(curr != NULL)
		{
			temp = curr->next;
			if(curr->is_label == YES)
			{
				free(curr->label_name);
			}
			free(curr);
			curr = temp;
		}
	}
}


/* This function updates the instructions that there are labels in, to the label address. */
void update_instruction_table()
{
	instruction_table * temp = head_instruction;
	label_table * temp_label;

	while(temp != NULL)
	{
		if(temp->is_label == YES)
		{
			temp_label = check_in_label_table(temp->label_name);
			if(temp_label == NULL)
			{
				fprintf(stderr, "%sLabel '%s' is not defined in file (in line %d)\n", PRINT_ERROR, temp->label_name, temp->line_in_file);
				EC++;
				temp = temp->next;
				continue; /* Move on to the next node */
			}

			temp->code.second_word_1.operand = temp_label->address;

			/* Updating the ARE field */
			if(temp_label->is_extern == YES)
			{
				temp->code.second_word_1.ARE = E;
			}
			else
			{
				temp->code.second_word_1.ARE = R;
			}
		}
		temp = temp->next;
	}
}
