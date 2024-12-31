DEPS = includes.h definitions.h structs.h colors.h


assembler: assembler.o first.o second.o writefiles.o \
	datatable.o labletable.o instructiontable.o entrytable.o externtable.o \
	   types.o methods.o
	gcc -g assembler.o first.o second.o writefiles.o \
		datatable.o labletable.o instructiontable.o entrytable.o externtable.o \
		types.o methods.o \
		-o assembler -Wall -ansi -pedantic


assembler.o: Assembler.c $(DEPS) Assembler.h LabelTable.h DataTable.h InstructionTable.h EntryTable.h ExternTable.h WriteFiles.h
	gcc -c Assembler.c -o assembler.o -Wall -ansi -pedantic

first.o: First.c $(DEPS) First.h LabelTable.h DataTable.h InstructionTable.h Methods.h Types.h
	gcc -c First.c -o first.o -Wall -ansi -pedantic

second.o: Second.c $(DEPS) Second.h LabelTable.h DataTable.h InstructionTable.h EntryTable.h ExternTable.h
	gcc -c Second.c -o second.o -Wall -ansi -pedantic

writefiles.o: WriteFiles.c $(DEPS) WriteFiles.h
	gcc -c WriteFiles.c -o writefiles.o -Wall -ansi -pedantic


datatable.o: DataTable.c $(DEPS) DataTable.h
	gcc -c DataTable.c -o datatable.o -Wall -ansi -pedantic

labletable.o: LabelTable.c $(DEPS) LabelTable.h
	gcc -c LabelTable.c -o labletable.o -Wall -ansi -pedantic

instructiontable.o: InstructionTable.c $(DEPS) InstructionTable.h
	gcc -c InstructionTable.c -o instructiontable.o -Wall -ansi -pedantic

entrytable.o: EntryTable.c $(DEPS) EntryTable.h LabelTable.h
	gcc -c EntryTable.c -o entrytable.o -Wall -ansi -pedantic

externtable.o: ExternTable.c $(DEPS) ExternTable.h
	gcc -c ExternTable.c -o externtable.o -Wall -ansi -pedantic

types.o: Types.c $(DEPS) Types.h
	gcc -c Types.c -o types.o -Wall -ansi -pedantic

methods.o: Methods.c $(DEPS) Methods.h Types.h
	gcc -c Methods.c -o methods.o -Wall -ansi -pedantic


clean:
	rm *.o
