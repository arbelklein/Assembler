char guidance_list[GUIDANCES][GUIDE_NAME] = {"data", "string", "entry", "extern"};
char register_list[REGISTERS][REGISTER_SIZE] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char instruction_list[CODES][CODE_SIZE] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

/*------------------------------------------------------*/

int IC = 0; /* IC = Instruction Counter */
int DC = 0; /* DC = Data Counter */
int EC = 0; /* EC = Error Counter */

/*------------------------------------------------------*/

label_table * head_label = NULL;
data_table * head_data = NULL;
instruction_table * head_instruction = NULL;
entry_table * head_entry = NULL;
extern_table * head_extern = NULL;

/*------------------------------------------------------*/

void FirstRound(FILE *);

void update_label_table();

void update_instruction_table();

void SecondRound(FILE *);
