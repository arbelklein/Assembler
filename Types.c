#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "Types.h"

/* This function check if the string is in the register list.
 * Return 1 if true, and 0 otherwise. */
int is_register(char * str)
{
	int j;
	
	for(j = 0; j < REGISTERS; j++)
	{
		/* Found a register that the str is equal to. */
		if(strcmp(str, register_list[j]) == 0)
		{
			return 1;
		}
	}

	return 0;
}


/* This function check if the string is in the instruction list.
 * Return 1 if true, and 0 otherwise. */
int is_instruction(char * str)
{
	int j;

	for(j = 0; j < CODES; j++)
	{
		/* Found a code that the str is equal to. */
		if(strcmp(str, instruction_list[j]) == 0)
		{
			return 1;
		}
	}
	
	return 0;
}


/* This function check if the string is in the guidance list.
 * Return 1 if true, and 0 otherwise. */
int is_guidance(char * str)
{
	int j;

	for(j = 0; j < GUIDANCES; j++)
	{
		/* Found a guidance that the str is equal to. */
		if(strcmp(str, guidance_list[j]) == 0)
		{		
			return 1;
		}
	}

	return 0;
}
