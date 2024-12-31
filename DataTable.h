extern char guidance_list[GUIDANCES][GUIDE_NAME];
extern char register_list[REGISTERS][REGISTER_SIZE]; 
extern char instruction_list[CODES][CODE_SIZE];

/*------------------------------------------------*/

extern int IC;
extern int DC;
extern int EC;

/*-----------------------------------------------*/

extern label_table * head_label;
extern data_table * head_data;
extern instruction_table * head_instruction;
extern entry_table * head_entry;
extern extern_table * head_extern;

/*------------------------------------------------*/

data_table * add_to_data_table_num(int, int, int);

data_table * add_to_data_table_str(int, char, int);

void print_data_table();

void free_data_table();

void update_data_table();
