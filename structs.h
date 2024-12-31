
enum state {YES, NO, UNKNOWN};

union word {
	struct {
		unsigned int opcode : 4;
		unsigned int source_method_0 : 1;
		unsigned int source_method_1 : 1;
		unsigned int source_method_2 : 1;
		unsigned int source_method_3 : 1;
		unsigned int destination_method_0 : 1;
		unsigned int destination_method_1 : 1;
		unsigned int destination_method_2 : 1;
		unsigned int destination_method_3 : 1;
		unsigned int ARE : 3;
	}first_word; /* The struct for the first word */

	struct {
		int operand : 12;
		unsigned int ARE : 3;
	}second_word_0; /* The struct for the second word of method 0 */

	struct {
                unsigned int operand : 12;
                unsigned int ARE : 3;
        }second_word_1; /* The struct for the second word of method 1 */

	struct {
                unsigned int source_method : 3;
                unsigned int destination_method : 3;
                unsigned int ARE : 3;
	}second_word_2_3; /* The struct for the second word of methods 2 and 3 */

	struct {
		int data : 15;
	}data_word; /* The struct for the data word */
};

struct print_word{
	int print : 16;
};


struct instruction_node {
	int address;
	union word code;
	enum state is_first;
	enum state is_second_0_1;
	enum state is_label;
	char * label_name;
	enum state is_second_2_3;
	int line_in_file;
	struct instruction_node* next;
};
typedef struct instruction_node instruction_table;
/* Link list to represent the instruction table */


struct data_node {
	int address;
	union word code;
	int line_in_file;
	struct data_node* next;
};
typedef struct data_node data_table;
/* Link list to represent the data table */


struct label_node {
	char * label_name;
	unsigned int address;
	enum state is_data;
	enum state is_extern;
	int line_in_file;
	struct label_node* next;
};
typedef struct label_node label_table;
/* Link list to represent the label table */


struct entry_node {
	char * entry_name;
	int address;
	struct entry_node* next;
};
typedef struct entry_node entry_table; 
/* Link list to represent the entry table, that will be written in the .ent file.
 * It's in this way, because if there are errors, we dont want to create the .ent file */


struct extern_node {
	char * extern_name;
	int address;
	struct extern_node* next;
};
typedef struct extern_node extern_table;
/* Link list to represent the extern table, that will be written in the .ext file
 * It's in this way, because if there are errors, we dont want to create the .ext file */
