#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"


#include "Methods.h"
#include "Types.h"

int check_label_syntax(char *, int);

/* This function checks the method syntax-wise.
 *Inserting to the method variable the number of the method.
 * Return 0 if there are errors, and 1 otherwise. */
int check_method(char * method, int * method_index, int LC)
{
	int i = 0;

	/* Method 0 */
	if(method[i] == '#')
	{
		i++;

		/* Checking the method */
		if(method_0(method + i, LC) == 0)
		{
			return 0;
		}

		(*method_index) = 0;

		return 1;
	}

	/* Method 2 */
	if(method[i] == '*')
	{
		i++;

		/* Checking the method */
		if(method_2(method + i, LC) == 0)
		{
			return 0;
		}

		(*method_index) = 2;

		return 1;
	}

	/* Method 3 */
	if(is_register(method) == 1)
	{
		(*method_index) = 3;

		return 1;
	}

	/* Method 1 */
	else
	{
		if(method_1(method, LC) == 0)
		{
			return 0;
		}

		(*method_index) = 1;

		return 1;
	}
}

/* This function checks the method 0 syntax.
 * Return 0 if there are errors, and 1 otherwise. */
int method_0(char * line, int LC)
{
	/* Right after the # there must be a number */

	int index = 0;

	/* The next char is not digit or - or + */
	if(line[index] != '-' && line[index] != '+' && !isdigit(line[index]))
	{
		fprintf(stderr, "%sIllegal number (in line %d)\n", PRINT_ERROR, LC);
		EC++;
		return 0;
	}

	/* The next char is - or +, meaning that right after it there must be a digit */
	if(line[index] == '-' || line[index] == '+')
	{
		if(!isdigit(line[index + 1]))
		{
			fprintf(stderr, "%sIllegal number (in line %d)\n",PRINT_ERROR, LC);
			EC++;
			return 0;
		}

		index++;
	}

	/* Going through the string */
	while(line[index] != '\0')
	{
		if(!isdigit(line[index]))
		{
			fprintf(stderr, "%sIllegal number (in line %d)\n", PRINT_ERROR, LC);
			EC++;
			return 0;
		}

		index++;
	}

	return 1;
}


/* This function checks the method 1 syntax.
 * Return 0 if there are errors, and 1 otherwise. */
int method_1(char * line, int LC)
{
	/* It is a label. Checking if its in the correct syntax */

	if(check_label_syntax(line, LC) == 0)
	{
		return 0;
	}

	return 1;
}


/* This function checks the method 2 syntax.
 * Return 0 if there are errors, and 1 otherwise. */
int method_2(char * line, int LC)
{
	/* Right after the * there must be a register */

	if(is_register(line) == 0)
	{
		fprintf(stderr, "%sIllegal register (in line %d)\n", PRINT_ERROR, LC);
		EC++;
		return 0;
	}

	return 1;
}
