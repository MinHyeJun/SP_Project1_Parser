/* 
 * my_assembler 함수를 위한 변수 선언 및 매크로를 담고 있는 헤더 파일이다. 
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
 * instruction 목록 파일로 부터 정보를 받아와서 생성하는 구조체 변수이다.
 * 구조는 각자의 instruction set의 양식에 맞춰 직접 구현하되
 * 라인 별로 하나의 instruction을 저장한다.
 * instruction의 이름을 기준으로 형식, OPCODE, 사용하는 operand 개수 및 종류를 구분할 필요
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
 * 어셈블리 할 소스코드를 입력받는 테이블이다. 라인 단위로 관리할 수 있다.
 */
char *input_data[MAX_LINES];
static int line_num;

int label_num; 

/* 
 * 어셈블리 할 소스코드를 토큰단위로 관리하기 위한 구조체 변수이다.
 * operator는 renaming을 허용한다.
 * nixbpe는 8bit 중 하위 6개의 bit를 이용하여 n,i,x,b,p,e를 표시한다.
 */
struct token_unit {
	char *label;  // 명령어 라인 중 lable
	char *operator;  // 명령어 라인 중 operator
	char *operand[MAX_OPERAND];  // 명령어 라인 중 operand
	char *comment;  // 명령어 라인 중 comment
	char nixbpe; // 하위 6bit 사용: __nixbpe

};

// 어셈블리할 소스코드를 5000라인까지 관리하는 테이블 생성
typedef struct token_unit token; 
token *token_table[MAX_LINES]; 
static int token_line;

/*
 * 심볼을 관리하는 구조체이다.
 * 심볼 테이블은 심볼 이름, 심볼의 위치로 구성된다.
 */
struct symbol_unit {
	char symbol[10];
	int addr;
};

typedef struct symbol_unit symbol;
symbol sym_table[MAX_LINES];  // 심볼 이름과 주소를 저장할 테이블
static int sym_num;  // 저장된 심볼 수를 저장하는 변수

// 리터럴을 관리하는 구조체
// 이름과 위치로 구성
struct literal_unit {
	char literal[20];
	int addr;
};

typedef struct literal_unit literal;
literal lit_table[MAX_LINES]; // 리터럴 값과 주소를 저장할 테이블
static int lit_num;  // 저장된 리터럴 수를 저장하는 변수
static int lit_index;  // 사용한 리터럴 수를 저장하는 변수

char object_codes[MAX_LINES][9];  // 오브젝트 코드를 담는 배열
char extref[3][20];  // 참조 선언한 심볼을 관리하는 배열
int end_addr[SUBPROG_NUM];  // 각 섹션의 마지막 명령어 주소를 저장하는 배열

// 참조하는 심볼의 정보를 modification record를 위해 저장하는 구조체
// 해당 참조 심볼 이름, 참조하는 주소, 명령어 형태, 참조하는 서브 프로그램 구분 저장
struct modif_unit {
	symbol ref_symbol;
	int modif_size;
	int program_num;
};

typedef struct modif_unit modif;
modif modif_table[20];
static int modif_cnt;

static int locctr;  // 현재 명령어의 주소
static int program_cnt;  // PC레지스터 역할의 변수
static int sub_prog_num;  // 현재 프로그램 구분을 위한 변수

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
void make_symtab_output(char *file_name);  // 심볼 테이블을 만드는 함수
void insert_symbol(token *inputToken);  // 심볼 테이블에 심볼을 넣는 함수
int operate_address(char * input_operand); // 심볼 테이블에 넣을 주소를 계산하는 함수
int search_symbol_address(char * symbol);  // 심볼을 통해 해당 심볼의 주소를 구하는 함수
int search_lit_address(char * literal);  // 리터럴을 통해 해당 리터럴의 주소를 구하는 함수
void insert_literal_littab(char * input_literal);  // 리터럴 테이블에 리터럴을 넣는 함수
void increase_locctr(token * inputToken);  // location counter를 증가시키는 함수
void increase_program_cnt(token * inputToken); // pc 값을 증가시키는 함수
void insert_addr_littab(void);  // 리터럴 테이블에 주소를 넣는 함수
void address_to_array(short address, char * arr, int arr_num);  // short형의 주소를 배열로 바꾸는 함수

/* 추후 프로젝트에서 사용하게 되는 함수*/
static int assem_pass2(void);
void make_objectcode_output(char *file_name);