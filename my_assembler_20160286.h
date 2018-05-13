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
symbol sym_table[MAX_LINES];  // �ɺ� �̸��� �ּҸ� ������ ���̺�
static int sym_num;  // ����� �ɺ� ���� �����ϴ� ����

// ���ͷ��� �����ϴ� ����ü
// �̸��� ��ġ�� ����
struct literal_unit {
	char literal[20];
	int addr;
};

typedef struct literal_unit literal;
literal lit_table[MAX_LINES]; // ���ͷ� ���� �ּҸ� ������ ���̺�
static int lit_num;  // ����� ���ͷ� ���� �����ϴ� ����
static int lit_index;  // ����� ���ͷ� ���� �����ϴ� ����

char object_codes[MAX_LINES][9];  // ������Ʈ �ڵ带 ��� �迭
char extref[3][20];  // ���� ������ �ɺ��� �����ϴ� �迭
int end_addr[SUBPROG_NUM];  // �� ������ ������ ��ɾ� �ּҸ� �����ϴ� �迭

// �����ϴ� �ɺ��� ������ modification record�� ���� �����ϴ� ����ü
// �ش� ���� �ɺ� �̸�, �����ϴ� �ּ�, ��ɾ� ����, �����ϴ� ���� ���α׷� ���� ����
struct modif_unit {
	symbol ref_symbol;
	int modif_size;
	int program_num;
};

typedef struct modif_unit modif;
modif modif_table[20];
static int modif_cnt;

static int locctr;  // ���� ��ɾ��� �ּ�
static int program_cnt;  // PC�������� ������ ����
static int sub_prog_num;  // ���� ���α׷� ������ ���� ����

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
void make_symtab_output(char *file_name);  // �ɺ� ���̺��� ����� �Լ�
void insert_symbol(token *inputToken);  // �ɺ� ���̺� �ɺ��� �ִ� �Լ�
int operate_address(char * input_operand); // �ɺ� ���̺� ���� �ּҸ� ����ϴ� �Լ�
int search_symbol_address(char * symbol);  // �ɺ��� ���� �ش� �ɺ��� �ּҸ� ���ϴ� �Լ�
int search_lit_address(char * literal);  // ���ͷ��� ���� �ش� ���ͷ��� �ּҸ� ���ϴ� �Լ�
void insert_literal_littab(char * input_literal);  // ���ͷ� ���̺� ���ͷ��� �ִ� �Լ�
void increase_locctr(token * inputToken);  // location counter�� ������Ű�� �Լ�
void increase_program_cnt(token * inputToken); // pc ���� ������Ű�� �Լ�
void insert_addr_littab(void);  // ���ͷ� ���̺� �ּҸ� �ִ� �Լ�
void address_to_array(short address, char * arr, int arr_num);  // short���� �ּҸ� �迭�� �ٲٴ� �Լ�

/* ���� ������Ʈ���� ����ϰ� �Ǵ� �Լ�*/
static int assem_pass2(void);
void make_objectcode_output(char *file_name);