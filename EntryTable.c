#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "EntryTable.h"
#include "LabelTable.h"


/* This function insert entry to the entry table.
 * The entry list is sorted by the address.
 * Return NULL if there are errors, and the head of the new list otherwise. */
entry_table * add_to_entry_table(char * label_name, int LC)
{
	entry_table * head = head_entry;
	entry_table * temp, * curr;
	entry_table * new;
	label_table * label;

	/* The list is not empty */
	if(head != NULL)
	{
		/* Checking if the label already exist in the list */
		temp = check_in_entry_table(label_name);

		/* There is a node with that label name, so no need to renter it */
		if(temp != NULL)
		{
			return head;
		}

		/* There isnt a node with that label name. */
	}

	/* Checking if the label is in the label list */
	label = check_in_label_table(label_name);
	if(label == NULL)
	{
		fprintf(stderr, "%sCan't find label '%s' in the list (in line %d)\n", PRINT_ERROR, label_name, LC);
		EC++;
		return NULL;
	}

	/* There is a label with that name in the list, 
	 * but it defines extern */
	if(label != NULL && label->is_extern == YES)
	{
		fprintf(stderr, "%sMulitiple definitions to label '%s' (in line %d)\n", PRINT_ERROR, label_name, LC);
		EC++;
		return NULL;
	}

	/* Found the label in the label list */

	new = (entry_table *)malloc(sizeof(entry_table));
	if(new == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFaile allocating memory\n", PRINT_FATAL);
		exit(0);
	}

	new->entry_name = (char *)malloc((strlen(label_name) + 1) * sizeof(char));
	if(new->entry_name == NULL) /* Failed at allocation */
	{
		fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
		exit(0);
	}
	strcpy(new->entry_name, label_name);

	new->address = label->address;
	new->next = NULL;

	if(head == NULL) /* The list is empty */
	{
		return new;
	}


	/* Going to the end of the list */
	curr = head;
	while(curr != NULL)
	{
		temp = curr->next;

		if(temp == NULL) /* Reached the end of the list */
		{
			if(curr->address > new->address) /* The new node needs to be before the curr node */
			{
				new->next = curr;
				return new;
			}

			/* The new node need to be after the curr */
			break;
		}

		if((curr->address < new->address) && (new->address < temp->address)) /* Found the place to insert the new node */
		{
			curr->next = new;
			new->next = temp;
			return head;
		}
		
		curr = temp; /* Moving to the next node */
	}

	curr->next = new;

	return head;
}


/* This function search in the entry list for a label name
 * Return the node if it found, and NULL if not. */
entry_table * check_in_entry_table(char * name)
{
	entry_table * temp = head_entry;

	/* Going through the table */
	while(temp != NULL)
	{
		/* Found the name in the table */
		if(strcmp(name, temp->entry_name) == 0)
		{
			return temp;
		}
		temp = temp->next;
	}

	/* Couldnt find the name in the list, so returning NULL */
	return NULL;
}


/* This function prints the entry table */
void print_entry_table()
{
	entry_table * temp = head_entry;

	printf("Entry Table:\n");

	while(temp != NULL)
	{
		printf("\nNode:\tname= '%s',\taddress= %d\n", temp->entry_name, temp->address);

		temp = temp->next;
	}
}


/* This function free the entry table */
void free_entry_table()
{
	entry_table * curr;
	entry_table * temp;

	if(head_entry != NULL)
	{
		curr = head_entry->next;
		head_entry = NULL;

		while(curr != NULL)
		{
			temp = curr->next;
			free(curr->entry_name);
			free(curr);
			curr = temp;
		}
	}
}
