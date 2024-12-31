extern char guidance_list[GUIDANCES][GUIDE_NAME];
extern char register_list[REGISTERS][REGISTER_SIZE]; 
extern char instruction_list[CODES][CODE_SIZE];

/*------------------------------------------------*/

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

instruction_table * add_to_instruction_table_first(int, int, int, int, int, int);

instruction_table * add_to_instruction_table_0(int, int, int, int);

instruction_table * add_to_instruction_table_1(int, char *, int);

instruction_table * add_to_instruction_table_2_3(int, int, int, int, int);

void print_instruction_table();

void free_instruction_table();

void update_instruction_table();
