extern char guidance_list[GUIDANCES][GUIDE_NAME];
extern char register_list[REGISTERS][REGISTER_SIZE];
extern char instruction_list[CODES][CODE_SIZE];

/*-------------------------------------------------*/

extern int IC;
extern int DC;
extern int EC;

/*------------------------------------------------*/

extern label_table * head_label;
extern data_table * head_data;
extern instruction_table * head_instruction;
extern entry_table * head_entry;
extern extern_table * head_extern;

/*------------------------------------------------*/

int is_fully_line(char *);

int check_label_syntax(char *, int);

int check_guidance_syntax(char *, int, int, int);

int check_commas(char *, int, int);

data_table * code_guidance(char *, int, int);

int check_instruction_syntax(char *, int, int);

instruction_table * code_instruction(char *, int, int);

