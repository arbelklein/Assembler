extern char guidance_list[GUIDANCES][GUIDE_NAME];
extern char register_list[REGISTERS][REGISTER_SIZE]; 
extern char instruction_list[CODES][CODE_SIZE];

/*-------------------------------------------------*/

extern int IC;
extern int DC;
extern int EC;

/*-------------------------------------------------*/

extern label_table * head_label;
extern data_table * head_data;
extern instruction_table * head_instruction;
extern entry_table * head_entry;
extern extern_table * head_extern;

/*------------------------------------------------*/

entry_table * add_to_entry_table(char *, int);

entry_table * check_in_entry_table(char *);

void print_entry_table();

void free_entry_table();
