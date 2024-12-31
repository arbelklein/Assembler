#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "DataTable.h"

/* This function insert numbers (at .data guide) to the data table (to the end of the table).
 * Return the head of the new list. */
data_table * add_to_data_table_num(int address, int num, int LC)
{
	data_table * head = head_data;
	data_table * temp = head;
	data_table * new = (data_table *)malloc(sizeof(data_table));

	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->address = address;
	new->code.data_word.data = num;
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

/* This function insert chars (at .string guide) to the data table (to the end of the table).
 * Return the head of the new list. */
data_table * add_to_data_table_str(int address, char ch, int LC)
{
	data_table * head = head_data;
	data_table * temp = head;
	data_table * new = (data_table *)malloc(sizeof(data_table));

	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->address = address;
	new->code.data_word.data = ch;
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


/* This function prints the data table */
void print_data_table()
{
	data_table * temp = head_data;

	printf("Data Table:\n");

	while(temp != NULL)
	{
		printf("\nNode:\taddress= %d,\tcode= %d\n", temp->address, temp->code.data_word.data);
		temp = temp->next;
	}
}


/* This function free the data table */
void free_data_table()
{
	data_table * curr;
	data_table * temp;

	if(head_data != NULL)
	{
		curr = head_data->next;
		head_data = NULL;

		while(curr != NULL)
		{
			temp = curr->next;
			free(curr);
			curr = temp;
		}
	}
}

/* This function updates the address of every node */
void update_data_table()
{
	data_table * temp = head_data;

	while(temp != NULL)
	{
		temp->address += (IC + BUFFER);

		temp = temp->next;
	}
}
