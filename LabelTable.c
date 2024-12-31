#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "LabelTable.h"


/* This function insert label to the label table (to the end of the table).
 * Return NULL if there are errors, and the head of the new list otherwise. */
label_table * add_to_label_table(char * name, int address, int is_data, int is_extern, int LC)
{
	label_table * head = head_label;
	label_table * temp;
	label_table * new;

	/* The list is not empty */
	if(head != NULL)
	{
		/* Checking if the label already exist in the list */
		temp = check_in_label_table(name);

		/* There is a node with that label name. */
		if(temp != NULL)
		{
			fprintf(stderr, "%sLabel already exist (in line %d)\n", PRINT_ERROR, LC);
			EC++;
			return NULL;
		}

		/* There isnt a node with that label name */
	}

       
	new = (label_table *)malloc(sizeof(label_table));
	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->label_name = (char *)malloc((strlen(name) + 1) * sizeof(char));
	if(new->label_name == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocation memory\n", PRINT_FATAL);
		exit(0);
	}
	strcpy(new->label_name, name);

	new->address = address;
	new->is_data = is_data;
	new->is_extern = is_extern;
	new->line_in_file = LC;
	new->next = NULL;

	if(head == NULL) /* The list is empty */
	{
		return new;
	}

	/* Going to the end of the list */
	temp = head;
	while(temp->next != NULL)
		temp = temp->next;

	temp->next = new;

	return head;
}


/* This function search in the label table for a label name.
 * Return the node if it found, and NULL if not. */
label_table * check_in_label_table(char * name)
{
	label_table * temp;

	temp = head_label;
	while(temp != NULL) /* Going through the table */
	{
		/* Found the name in the table */
		if(strcmp(name, temp->label_name) == 0)
		{
			return temp;
		}
		temp = temp->next;
	}

	/* Couldnt find the name in the list, so returning NULL */
	return NULL;
}


/* This function prints the label table */
void print_label_table()
{
	label_table * temp = head_label;

	printf("Label Table:\n");

	while(temp != NULL)
	{
		printf("\nNode:\tname= '%s',\taddress= %d,\t", temp->label_name, temp->address);
		
		if(temp->is_data == YES)
			printf("is_data= %s,\t", "YES");
		if(temp->is_data == NO)
			printf("is_data= %s,\t", "NO");
		if(temp->is_data == UNKNOWN)
			printf("is_data= %s,\t", "UNKNOWN");

		printf("is_extern= %s\n", temp->is_extern ? "NO" : "YES");

		temp = temp->next;
	}
}


/* This function free the label table */
void free_label_table()
{
	label_table * curr;
	label_table * temp;

	if(head_label != NULL)
	{
		curr = head_label->next;
		head_label = NULL;

		while(curr != NULL)
		{
			temp = curr->next;
			free(curr->label_name);
			free(curr);
			curr = temp;
		}
	}
}


/* This function updates the labels that are data, to their correct address */
void update_label_table()
{
	label_table * temp = head_label;

	while(temp != NULL)
	{
		if(temp->is_data == YES)
		{
			temp->address += (IC + BUFFER);
		}
		temp = temp->next;
	}

}
