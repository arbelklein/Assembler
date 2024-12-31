#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "ExternTable.h"


/* This function insert entry to the extern table (to the end of the table).
 * It goes over the instruction list, and everytime it meets an extern label.
 * Return NULL if there are errors, and the head of the new list otherwise. */
extern_table * add_to_extern_table(char * label_name, int address)
{
	extern_table * head = head_extern;
	extern_table * temp;
	extern_table * new = (extern_table *)malloc(sizeof(extern_table));

	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFaile allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->extern_name = (char *)malloc((strlen(label_name) + 1) * sizeof(char));
	if(new->extern_name == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}
	strcpy(new->extern_name, label_name);

	new->address = address;
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


/* This function prints the extern table */
void print_extern_table()
{
	extern_table * temp = head_extern;

	printf("Extern Table:\n");

	while(temp != NULL)
	{
		printf("\nNode:\tname= '%s',\taddress= %d\n", temp->extern_name, temp->address);

		temp = temp->next;
	}
}


/* This function free the extern table */
void free_extern_table()
{
	extern_table * curr;
	extern_table * temp;

	if(head_extern != NULL)
	{
		curr = head_extern->next;
		head_extern = NULL;

		while(curr != NULL)
		{
			temp = curr->next;
			free(curr->extern_name);
			free(curr);
			curr = temp;
		}
	}
}
