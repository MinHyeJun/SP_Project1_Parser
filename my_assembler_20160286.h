/* 
 * my_assembler �Լ��� ���� ���� ���� �� ��ũ�θ� ��� �ִ� ��� �����̴�. 
 * 
 */
#define MAX_INST 256
#define MAX_LINES 5000
#define MAX_OPERAND 3
#define SUBPROG_NUM 3

#define A_REGISTER 0
#define X_REGISTER 1
#define L_REGISTER 2
#define B_REGISTER 3
#define S_REGISTER 4
#define T_REGISTER 5

#define N_NIXBPE 32
#define I_NIXBPE 16
#define X_NIXBPE 8
#define B_NIXBPE 4
#define P_NIXBPE 2
#define E_NIXBPE 1

/* 
 * instruction ��� ���Ϸ� ���� ������ �޾ƿͼ� �����ϴ� ����ü �����̴�.
 * ������ ������ instruction set�� ��Ŀ� ���� ���� �����ϵ�
 * ���� ���� �ϳ��� instruction�� �����Ѵ�.
 * instruction�� �̸��� �������� ����, OPCODE, ����ϴ� operand ���� �� ������ ������ �ʿ�
 */
struct inst_unit {
	/* add your code here */
	char inst[10];
	int form;
	char opcode[3];
	int oprnd_num;
};
typedef struct inst_unit inst;
inst *inst_table[MAX_INST];
int inst_index;

/*
 * ����� �� �ҽ��ڵ带 �Է¹޴� ���̺��̴�. ���� ������ ������ �� �ִ�.
 */
char *input_data[MAX_LINES];
static int line_num;

int label_num; 

/* 
 * ����� �� �ҽ��ڵ带 ��ū������ �����ϱ� ���� ����ü �����̴�.
 * operator�� renaming�� ����Ѵ�.
 * nixbpe�� 8bit �� ���� 6���� bit�� �̿��Ͽ� n,i,x,b,p,e�� ǥ���Ѵ�.
 */
struct token_unit {
	char *label;  // ��ɾ� ���� �� lable
	char *operator;  // ��ɾ� ���� �� operator
	char *operand[MAX_OPERAND];  // ��ɾ� ���� �� operand
	char *comment;  // ��ɾ� ���� �� comment
	char nixbpe; // ���� 6bit ���: __nixbpe

};

// ������� �ҽ��ڵ带 5000���α��� �����ϴ� ���̺� ����
typedef struct token_unit token; 
token *token_table[MAX_LINES]; 
static int token_line;

/*
 * �ɺ��� �����ϴ� ����ü�̴�.
 * �ɺ� ���̺��� �ɺ� �̸�, �ɺ��� ��ġ�� �����ȴ�.
 */
struct symbol_unit {
	char symbol[10];
	int addr;
};

typedef struct symbol_unit symbol;
symbol sym_table[MAX_LINES];

static int sym_num; ////////

struct literal_unit {
	char literal[20];
	int addr;
};

typedef struct literal_unit literal;
literal lit_table[MAX_LINES];
static int lit_num;
static int lit_index;

char object_codes[MAX_LINES][9];
char extref[3][20];
int end_addr[SUBPROG_NUM];

struct modif_unit {
	symbol ref_symbol;
	int op_or_dif;
};

typedef struct modif_unit modif;
modif modif_table[20];
static modif_cnt;

static int locctr;
static int program_cnt;
static int sub_prog_num;

//--------------

static char *input_file;
static char *output_file;
int init_my_assembler(void);
int init_inst_file(char *inst_file);
int init_input_file(char *input_file);
int token_parsing(char *str);
int search_opcode(char *str);
static int assem_pass1(void);
void make_opcode_output(char *file_name);

void freeAll();

////////////////////////
void make_symtab_output(char *file_name);
void insert_symbol(token *inputToken);
int operate_address(char * input_operand);
int search_symbol_address(char * symbol);
int search_lit_address(char * literal);
void insert_literal_littab(char * input_literal);
void increase_locctr(token * inputToken);
void insert_addr_littab(void);
void address_to_array(short address, char * arr, int arr_num);

/* ���� ������Ʈ���� ����ϰ� �Ǵ� �Լ�*/
static int assem_pass2(void);
void make_objectcode_output(char *file_name);