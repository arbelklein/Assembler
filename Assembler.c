#include "includes.h"
#include "definitions.h"
#include "structs.h"
#include "colors.h"

#include "Assembler.h"
#include "LabelTable.h"
#include "DataTable.h"
#include "InstructionTable.h"
#include "EntryTable.h"
#include "ExternTable.h"
#include "WriteFiles.h"


int main(int argc, char *argv[])
{
	char file_extension[] = ".as", *file_name;
	int i;
	FILE *fptr;

	if(argc == 1) /* No arguments entered */
	{
		fprintf(stderr, "%sno input files\n", PRINT_FATAL);
		return 1;
	}


	for(i = 1; i < argc; i++) /* Going through the files given */
	{
		file_name = (char *)malloc((strlen(argv[i]) + 1) * sizeof(char));
		if(file_name == NULL) /* Failed at allocation */
		{
			fprintf(stderr, "%sFailed allocating memory\n", PRINT_FATAL);
			exit(0);
		}

		strcpy(file_name, argv[i]);
		strcat(file_name, file_extension);

		fptr = fopen(file_name, "r");
		if(fptr == NULL) /* Failed opening file */
		{
			fprintf(stderr, "%sFailed opening file '%s'\n", PRINT_ERROR, file_name);
			continue; /* Move on to the next file */
		}

		IC = 0;
		DC = 0;
		EC = 0;

		FirstRound(fptr); /*First round on the file */

		if(EC == 0)
		{
			rewind(fptr);
			
			SecondRound(fptr); /* Second round on the file */

			if(EC == 0) /* There are no errors in the second round */
			{
				write_to_files(argv[i]);
			}
		}

		free_label_table();
		free_data_table();
		free_instruction_table();
		free_entry_table();
		free_extern_table();


		fclose(fptr);
		free(file_name);
	}

	return 0;
}
