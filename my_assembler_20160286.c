/*
 * 화일명 : my_assembler_00000000.c 
 * 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
 * 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
 * 파일 내에서 사용되는 문자열 "00000000"에는 자신의 학번을 기입한다.
 */

/*
 *
 * 프로그램의 헤더를 정의한다. 
 *
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// 파일명의 "00000000"은 자신의 학번으로 변경할 것.
#include "my_assembler_20160286.h"

/* ----------------------------------------------------------------------------------
 * 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
 * 매계 : 실행 파일, 어셈블리 파일 
 * 반환 : 성공 = 0, 실패 = < 0 
 * 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다. 
 *		   또한 중간파일을 생성하지 않는다. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[]) 
{
	// 프로그램 초기화를 위한 자료구조 생성 및 파일 읽어옴
	if(init_my_assembler()< 0)
	{
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n"); 
		return -1 ; 
	}

	// 어셈블리 코드의 Pass1 수행
	if(assem_pass1() < 0 ){
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n") ; 
		return -1 ; 
	}

	// 어셈블리 코드의 명령어 Opcode와 함께 파일에 출력
	//make_opcode_output("output_20160286.txt");
	//make_opcode_output(NULL);

	make_symtab_output("symtab_20160286");
	assem_pass2();
	make_objectcode_output("output_20160286");
	//make_objectcode_output(NULL);
	

	/*
	* 추후 프로젝트에서 사용되는 부분
	*
	if(assem_pass2() < 0 ){
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n") ; 
		 return -1 ; 
	}

	make_objectcode_output("output") ; 
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0 , 에러 발생 = -1
 * 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기 
 *		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
 *		   구현하였다. 
 * ----------------------------------------------------------------------------------
 */
int init_my_assembler(void)
{
	int result ; 

	if((result = init_inst_file("inst.data")) < 0 )
		return -1 ;
	if((result = init_input_file("input.txt")) < 0 )
		return -1 ; 
	return result ; 
}

/* ----------------------------------------------------------------------------------
 * 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을 
 *        생성하는 함수이다. 
 * 매계 : 기계어 목록 파일
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : 기계어 목록파일 형식은 자유롭게 구현한다. 예시는 다음과 같다.
 *	
 *	===============================================================================
 *		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
 *	===============================================================================	   
 *		
 * ----------------------------------------------------------------------------------
 */                                                      
int init_inst_file(char *inst_file)
{
	FILE * file;  // 인자로 받은 파일명으로 파일을 열기 위한 파일 포인터
	int errno;  // 에러 체크 

	/* add your code here */

	char str[1024];  // 한 라인을 읽어오기 위한 문자열
	char *ptr;  // 읽어온 문자열을 공백 단위로 분리하여 읽어오기 위한 포인터

	fopen_s(&file, inst_file, "r");  // 인자로 받은 파일명으로 파일을 읽기용으로 엶

	if (file == NULL)  // 파일을 열 수 없다면 표준 출력 후, 에러상태(-1)로 표시
	{
		printf("파일을 읽을 수 없습니다.");
		errno =  -1;
	}
	else  // 파일을 열었다면 한 라인씩 읽어 이름, 형식, 기계어 코드, 오퍼랜드의 개수 순으로 나누어 구조체에 저장
	{

		while (0 == feof(file))  // 파일의 끝(EOF)에 도달하기 전까지 반복
		{
			fgets(str, 1024, file);  // 파일을 한 라인씩 str에 읽어들임

			ptr = strtok(str, " ");  // 공백을 기준으로 문자열 str을 분리
			
			// 명령어셋을 관리하는 테이블의 한 원소에 구조체 inst 하나만큼의 크기 동적 할당
			inst_table[inst_index] = malloc(sizeof(inst));  

			// 명령어 이름, 형식, 기계어 코드, 오퍼랜드 개수 순으로 저장하고 나누기를 반복
			strcpy(inst_table[inst_index]->inst, ptr);  // 명령어 이름 저장
			ptr = strtok(NULL, " ");
			inst_table[inst_index]->form = atoi(ptr);  // 형식 저장
			ptr = strtok(NULL, " ");
			strcpy(inst_table[inst_index]->opcode, ptr);  // 기계어 코드 저장
			ptr = strtok(NULL, " ");
			inst_table[inst_index]->oprnd_num = atoi(ptr);  // 오퍼랜드 개수 저장

			// 하나를 무사히 저장했다면 inst 개수를 저장하는 inst_index의 값을 하나 올림
			inst_index++;  
		}

		errno = 0;  // 오류없이 파일에서 읽어오기를 완료했으므로 에러(0)가 없음을 표시
	}
	fclose(file);  // 연 파일을 닫음

	return errno;  // 에러 상태 반환
}

/* ----------------------------------------------------------------------------------
 * 설명 : 어셈블리 할 소스코드를 읽어 소스코드 테이블(input_data)를 생성하는 함수이다. 
 * 매계 : 어셈블리할 소스파일명
 * 반환 : 정상종료 = 0 , 에러 < 0  
 * 주의 : 라인단위로 저장한다.
 *		
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{
	FILE * file;  // 인자로 받은 파일명으로 파일을 열기 위한 파일 포인터
	int errno;  // 에러 체크 

	/* add your code here */

	fopen_s(&file, input_file, "r");  // 인자로 받은 파일명으로 파일을 읽기용으로 엶

	if (file == NULL)  // 파일을 열 수 없다면 표준 출력 후, 에러상태(-1)로 표시
	{
		printf("파일을 읽을 수 없습니다.");
		errno = -1;
	}
	else  // 파일을 열었다면 한 라인씩 읽어 저장
	{
		while (0 == feof(file))  // 파일의 끝(EOF)에 도달하기 전까지 반복 
		{
			// input_data 배열의 각 원소에 문자열 동적 할당 후 어셈블리할 소스코드 저장
			input_data[line_num] = malloc(sizeof(char) * 1024);  
			fgets(input_data[line_num], 1024, file); 

			line_num++;  // 하나를 무사히 저장했다면 소스코드 라인 수를 저장하는 line_num의 값을 하나 올림
		}

		errno = 0;  // 오류없이 파일에서 읽어오기를 완료했으므로 에러(0)가 없음을 표시
	}
	fclose(file);  // 연 파일을 닫음
	
	return errno;  // 에러 상태 반환
}

/* ----------------------------------------------------------------------------------
 * 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다. 
 *        패스 1로 부터 호출된다. 
 * 매계 : 파싱을 원하는 문자열  
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다. 
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
	// op_index: 토큰 테이블을 작성할 소스 코드의 명령어가 inst_table 상으로 어디에 있는지 저장
	// input: 인자로 받은 문자열 str을 임시로 저장하기 위한 포인터
	// line: 탭과 개행 기준으로 문자열 input을 분리할 때, 분리된 문자열을 임시로 담기 위한 포인터 
	// temp: 오퍼랜드를 저장하기 위한 임시 문자열
	int op_index = -1;
	char  * input, *line, temp[256];

	if (str[0] == '.')  // 소스코드의 시작이 "."이라면 토큰을 분리하지 않고 함수를 끝냄
		return 0;

	// 문자열 str의 내용을 복사하기 위해 input에 동적 할당
	input = malloc(sizeof(char) * 1024);

	strcpy(input, str);  // 문자열 str의 내용을 input으로 복사

	// 토큰을 나누기 전에 토큰 테이블의 한 원소에 구조체 token 하나만큼 동적 할당
	// 각 구조체 내부의 변수들은 NULL로 초기화함
	token_table[token_line] = malloc(sizeof(token));
	token_table[token_line]->label = NULL;
	token_table[token_line]->operator = NULL;
	for (int i = 0; i < MAX_OPERAND; i++)
		token_table[token_line]->operand[i] = NULL;
	token_table[token_line]->comment = NULL;
	token_table[token_line]->nixbpe = 0;

	line = strtok(input, "\t\n");  // 탭과 개행을 기준으로 문자열을 분리하여 차례로 가져옴

	if (input[0] != '\t')  // 소스코드의 첫 문자가 탭이 아닌 경우, 레이블이 존재하여 line에 레이블이 반환됨
	{
		// 구조체 내부에 레이블 저장하는 변수 동적 할당 후, line에 담긴 레이블 정보를 구조체 내부에 저장
		token_table[token_line]->label = malloc(sizeof(char) * 30);
		strcpy(token_table[token_line]->label, line);
		line = strtok(NULL, "\t\n");  // 레이블을 저장했으므로 다음 저장할 문자열을 가져옴
	}

	if (line != NULL)  // 다음 저장할 문자열이 있는 경우 line에 명령어 혹은 지시어가 반환됨
	{
		// line의 문자열이 명령어인 경우, 해당 명령어의 inst_table 상의 index 반환
		op_index = search_opcode(line);

		// 구조체 내부에 레이블 저장하는 변수 동적 할당 후, line에 담긴 operator 정보를 구조체 내부에 저장
		token_table[token_line]->operator = malloc(sizeof(char) * 30);
		strcpy(token_table[token_line]->operator, line);

		// 3형식 또는 4형식을 지원하는 명령어인 경우, 
		if (op_index >= 0 && inst_table[op_index]->form == 3)
		{
			// 4형식 명령어인 경우 nixbpe 중 e 표시
			if (line[0] == '+')
				token_table[token_line]->nixbpe |= E_NIXBPE;
			else // 아니라면 pc relative를 위해 nixbpe 중 p에 표시
				token_table[token_line]->nixbpe |= P_NIXBPE;
		}

		line = strtok(NULL, "\t\n");  // 다음 저장할 문자열을 가져옴
	}
	else  // 다음 저장할 문자열이 없는 경우 함수 종료
	{
		return 0;
	}

	if (line != NULL)  // 다음 저장할 문자열이 있는 경우 line에 피연산자나 코멘트가 들어옴
	{
		// 현재 소스코드에 명령어가 있으나, 해당 명령어의 피연산자가 없다면 line의 내용은 코멘트이므로
		// 피연산자 저장 코드를 건너 뜀
		// ex) RSUB
		// 그 이외에는 피연산자 저장
		if (!(op_index >= 0 && inst_table[op_index]->oprnd_num == 0))
		{
			strcpy(temp, line);  // 피연산자를 분리하여 저장해야하므로 line의 내용을 temp에 복사
			line = strtok(NULL, "\t\n");  // 피연산자를 임시공간에 저장했으므로 다음 저장할 문자열 가져옴

			char * operand = strtok(temp, ",");  // 피연산자를 콤마(,) 기준으로 분리하여 하나씩 가져옴

			// 구조체 내부의 피연산자를 담는 배열에 분리한 피연산자를 하나씩 저장
			for (int j = 0; operand != NULL; j++)
			{
				token_table[token_line]->operand[j] = malloc(sizeof(char) * 256);
				strcpy(token_table[token_line]->operand[j], operand);
				operand = strtok(NULL, ",");
			}

			//3 또는 4형식을 지원하는 명령어의 경우에 따라 nixpbe를 표시
			if (op_index >= 0 && inst_table[op_index]->form == 3)
			{
				// X레지스터를 이용해 루핑을 하는 경우 x 표시
				if ((token_table[token_line]->operand[1]!= NULL) && (!strcmp(token_table[token_line]->operand[1], "X")))
				{
					token_table[token_line]->nixbpe |= X_NIXBPE;
				}
				
				// indirect addressing을 사용하는 경우 n 표시
				if (strstr(token_table[token_line]->operand[0], "@"))
				{
					token_table[token_line]->nixbpe |= N_NIXBPE;
				}
				else if (strstr(token_table[token_line]->operand[0], "#")) // immediate addressing을 사용하는 경우 i표시 및 p 표시 삭제
				{
					token_table[token_line]->nixbpe |= I_NIXBPE;
					token_table[token_line]->nixbpe ^= P_NIXBPE;
				}
				else // 그 외의 경우 simple addressing 모드이므로 n과 i 표시
				{
					token_table[token_line]->nixbpe |= N_NIXBPE + I_NIXBPE;
				}
			}
		}
		else  //이 외의 경우 simple addressing 모드 이므로 n과 i 표시
		{
			token_table[token_line]->nixbpe |= N_NIXBPE + I_NIXBPE;
		}

		if (line != NULL)  // 다음 저장할 문자열이 있는 경우 line에 코멘트가 들어옴
		{
			// 구조체 내부에 코멘트를 저장하는 포인터에 동적할당을 하여 코멘트를 저장함
			token_table[token_line]->comment = malloc(sizeof(char) * 1024);
			strcpy(token_table[token_line]->comment, line);
		}

		free(input);
	}
	// 한 라인의 소스코드를 토큰별로 모두 분리했으므로 토큰의 수를 저장하는 token_line의 값을 1 올림
	token_line++;
	
	return 0;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다. 
 * 매계 : 토큰 단위로 구분된 문자열 
 * 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0 
 * 주의 : 
 *		
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char *str) 
{
	int i;  // for문을 사용하기 위한 임시 변수
	// 명령어인지 여부를 확인하기 전에, 문자열 str의 첫번째 문자가 '+'인 경우 
	// '+' 다음을 가리키도록 함
	// '+'가 있으면 명령어인지 여부를 확인할 수 없기 때문에 처리함
	if (str[0] == '+')
	{
		str = str + 1;
	}

	// inst_table의 명령어 이름을 하나씩 모두 비교
	// inst_table에 존재하는 명령어가 맞다면 해당 명령어가 inst_table 상에서 몇번째인지 인덱스를 반환
	for (i = 0; i < inst_index; i++)
	{
		if (strcmp(str, inst_table[i]->inst) == 0)
		{
			return i;
		}
	}

	// 찾을 수 없다면 -1을 반환
	return -1;
}

/* ----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다.
*		   패스1에서는..
*		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
*		   테이블을 생성한다.
*
* 매계 : 없음
* 반환 : 정상 종료 = 0 , 에러 = < 0
* 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
*	  따라서 에러에 대한 검사 루틴을 추가해야 한다.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	/* add your code here */

	/* input_data의 문자열을 한줄씩 입력 받아서 
	 * token_parsing()을 호출하여 token_unit에 저장
	 */

	// input 파일에서 읽어들인 라인 수 만큼 토큰 분리 함수를 호출하여 작업
	for (int i = 0; i < line_num; i++)
	{
		token_parsing(input_data[i]);
	}

	//첫번째 토큰의 연산자가 START라는 지시어라면 피연산자로 들어온 주소값으로 locctr을 초기화
	//첫번째 프로그램임을 표시 (sub_prog_num = 0)
	if (token_table[0] != NULL && !strcmp(token_table[0]->operator,"START"))
	{
		locctr = atoi(token_table[0]->operand[0]);
		sub_prog_num = 0;
	}
	else
		return -1;

	//첫번째 토큰의 레이블을 심볼 테이블에 저장
	insert_symbol(token_table[0]);

	// 토큰을 하나씩 가져와 Pass1 수행
	for (int i = 1; i < token_line; i++)
	{
		// 해당 토큰의 연산자가 CSECT라면 새로운 서브루틴의 시작임
		// 이전 프로그램의 마지막 주소를 배열에 저장
		// locctr의 값을 0으로 초기화
		// 프로그램 표시값을 1 올림
		if (!strcmp(token_table[i]->operator, "CSECT"))
		{
			end_addr[sub_prog_num] = locctr;
			locctr = 0;
			sub_prog_num++;
		}

		// 레이블 정보가 있는 토큰인 경우 심볼을 테이블에 저장하도록 함
		insert_symbol(token_table[i]);

		// 피연산자로 리터럴이 들어온 경우, 리터럴 정보를 테이블에 저장하도록 함
		if (token_table[i]->operand[0] != NULL && strstr(token_table[i]->operand[0], "="))
			insert_literal_littab(token_table[i]->operand[0]);

		//읽어들인 연산자가 LTORG나 END 지시어라면 리터럴 주소를 리터럴 테이블에 저장
		if (!strcmp(token_table[i]->operator, "LTORG") || !strcmp(token_table[i]->operator,"END"))
		{
			insert_addr_littab();
		}
	}

	// 마지막 서브 루틴 프로그램의 끝 주소를 저장
	end_addr[sub_prog_num] = locctr;

	return 0;
}


/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 명령어 옆에 OPCODE가 기록된 표(과제 4번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*        또한 과제 4번에서만 쓰이는 함수이므로 이후의 프로젝트에서는 사용되지 않는다.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char *file_name)
{
	/* add your code here */
	
	// file: 인자로 받은 파일명으로 파일을 열기 위한 파일 포인터
	// op_index: 현재 출력하는 소스코드에 명령어가 포함된 경우 해당 명령어가 inst_table 상으로 어디에 있는지 저장
	// op_cnt: 현재 출력하는 소스코드의 피연산자의 개수를 세기 위함
	// label, operator, operand: 출력하기 위한 레이블, 연산자, 피연산자를 각각 임시로 담는 용도
	// output: 출력하기 위해 문자열들을 담는 용도
	// init: 앞선 네 배열을 초기화하기 위한 용도
	// isFile: 파일에 출력할 것인지(1), 표준 출력으로 보여줄 것인지(0)
	FILE * file = NULL;  
	int op_index, op_cnt;
	char lable[20], operator[8], operand[255], output[1024], init[1024] = { 0 };
	_Bool isFile = 1;

	if (file_name == NULL)  // 인자에 NULL이 들어온 경우 표준 출력
	{
		isFile = 0;
	}
	else
	{
		fopen_s(&file, file_name, "w");  // 인자로 받은 파일명으로 파일을 쓰기용으로 엶

		if (file == NULL) // 파일을 열 수 없다면 표준 출력 후 함수를 끝냄
		{
			printf("해당 이름의 파일을 작성할 수 없습니다.");
			return;
		}
	}
	
	// 토큰으로 분리했던 소스코드를 출력
	for (int i = 0; i < token_line; i++)
	{
		// 출력할 레이블, 연산자, 피연산자를 담기 전에 배열 초기화
		strcpy(lable, init);
		strcpy(operator, init);
		strcpy(operand, init);
		strcpy(output, init);
		op_cnt = 0;  // 피연산자 개수 초기화

		// 토큰에 레이블 정보가 비어있지 않다면(NULL이 아니라면)
		// 레이블 정보를 lable에 복사
		if (token_table[i]->label != NULL)
			strcpy(lable, token_table[i]->label);

		// 토큰의 연산자 정보에 담긴 내용이 명령어라면 inst_table 상의 인덱스를 구한 후 operator에 복사
		op_index = search_opcode(token_table[i]->operator);
		strcpy(operator, token_table[i]->operator);
		
		// 토큰에 저장된 피연산자 개수를 구함
		for (int j = 0; token_table[i]->operand[j] != NULL; j++)
		{
			op_cnt++;
		}

		// 토큰의 저장된 피연산자들을 operand에 저장
		// 피연산자가 여러 개인 경우 피연산자들 사이에 콤마(,) 입력
		for (int j = 0; j < op_cnt; j++)
		{
			strcat(operand, token_table[i]->operand[j]);

			if (j < op_cnt - 1)
				strcat(operand, ",");
		}

		// 출력을 위해 위에서 저장한 레이블, 연산자, 피연산자 정보를 배열 output에 모두 담음
		sprintf(output, "%-10s\t%s\t%-20s", lable, operator, operand);

		if (op_index >= 0)  // 명령어 인덱스를 저장하는 op_index의 값이 0보다 같거나 큰 경우 (테이블에 존재하는 명령어인 경우)
		{
			// 해당 코드의 기계어 코드를 출력할 문자열 output에 포함시킴
			sprintf(output, "%s\t%s", output, inst_table[op_index]->opcode);
		}

		// 파일에서 출력하는 경우, fprintf로 파일에 명령어 출력
		if (isFile)
		{
			fprintf(file, "%s\n", output);
		}
		else // 파일에 출력하는 경우가 아니라면 표준출력으로 명령어 출력
		{
			printf("%s\n", output);
		}
	}
	if(file != NULL)
		fclose(file);  // 연 파일을 닫음

	//freeAll();  // 메모리 동적할당 모두 반환
}

// 메모리 동적할당 모두 반환하는 함수
void freeAll(void) {
	int i;

	// 기계어 코드 테이블 반환
	for (i = 0; i < inst_index; i++)
	{
		free(inst_table[i]);
	}

	// 토큰 테이블 반환
	for (i = 0; i < token_line; i++)
	{
		if (token_table[i]->label != NULL)
			free(token_table[i]->label);

		if (token_table[i]->operator != NULL)
			free(token_table[i]->operator);

		for (int j = 0; j < MAX_OPERAND; j++)
			if (token_table[i]->operand[j] != NULL)
				free(token_table[i]->operand[j]);

		if (token_table[i]->comment != NULL)
			free(token_table[i]->comment);

		free(token_table[i]);
	}

	// 소스코드 테이블 반환
	for (i = 0; i < line_num; i++)
	{
		free(input_data[i]);
	}
}


/* --------------------------------------------------------------------------------*
* ------------------------- 추후 프로젝트에서 사용할 함수 --------------------------*
* --------------------------------------------------------------------------------*/

// 심볼 테이블의 정보를 파일에 출력하는 함수
void make_symtab_output(char *fileName)
{
	FILE * file;

	// 인자의 문자열을 이름으로 한 파일을 쓰기용으로 오픈
	file = fopen(fileName, "w");

	if (file == NULL)
	{
		return;
	}

	// 심볼 테이블에 저장된 심볼 이름과 주소를 출력
	for (int i = 0; i < sym_num; i++)
	{
		fprintf(file, "%s\t%X\n", sym_table[i].symbol, sym_table[i].addr);
		//printf("%s\t%X\n", sym_table[i].symbol, sym_table[i].addr);
	}
}

// 인자로 받은 토큰에서 레이블 정보를 심볼 테이블에 심볼 이름으로 넣는 함수
void insert_symbol(token * inputToken)
{
	int op_index = -1; 

	// 토큰의 레이블 정보가 있는 경우 저장할 심볼이 있다고 파악
	if (inputToken->label != NULL)
	{
		// 레이블을 심볼 테이블에 이름으로 저장
		strcpy(sym_table[sym_num].symbol, inputToken->label);

		// 토큰의 명령어 연산자가 EQU 지시어라면 주소값으로 피연산자 값을 넣어야 함
		if (!strcmp(inputToken->operator, "EQU"))
		{
			// 피연산자가 *인 경우 현재 locctr의 값을 주소로 저장
			if (!strcmp(inputToken->operand[0], "*"))
				sym_table[sym_num].addr = locctr;
			else  // 그렇지 않은 경우 피연산자를 통해 주소값을 계산하여 저장
				sym_table[sym_num].addr = operate_address(inputToken->operand[0]);
		}
		else  // 그 외의 경우 현재 주소값을 심볼 주소값으로 저장
			sym_table[sym_num].addr = locctr;

		sym_num++; // 하나를 온전하게 저장했으므로 저장 수를 1 올림 
	}

	increase_locctr(inputToken);  // 현재 locctr를 증가시킴
}

// 인자로 받은 리터럴을 리터럴 테이블에 넣는 함수
void insert_literal_littab(char * input_literal)
{
	char data[255];
	char *ptr;

	// 문자열 분리를 위해 리터럴 정보를 임시로 복사함
	strcpy(data, input_literal);

	// 리터럴 정보에 첫번째 문자로 = 이 포함된 경우 이를 없앰
	if (data[0] == '=')
		ptr = strtok(data, "=");

	// 이전에 저장된 리터럴인지 확인
	for (int i = lit_index; i < lit_num; i++)
	{
		if (!strcmp(lit_table[i].literal, ptr))
			return;
	}
	
	//새로운 리터럴이라면 리터럴 테이블에 저장
	strcpy(lit_table[lit_num].literal, ptr);

	lit_num++;  // 리터럴 개수를 1 올림
}

// 피연산자에 연산이 포함된 경우 주소를 계산하여 반환하는 함수
int operate_address(char * input_operand)
{
	char expression[1024];
	char * symbol;
	int result = -1;

	// 계산할 피연산자 정보를 임시로 복사
	strcpy(expression, input_operand);

	// - 연산이 있는 경우, 해당 심볼의 주소값을 가져와 - 연산 수행
	if (strstr(expression, "-"))
	{
		symbol = strtok(expression, "-");
		result = search_symbol_address(symbol);
		symbol = strtok(NULL, "-");
		result -= search_symbol_address(symbol);
	}

	// 계산된 주소값 반환
	return result;
}

// 인자로 받은 심볼 이름을 가지고 테이블에서 주소값을 찾아 반환하는 함수
int search_symbol_address(char * symbol)
{
	// 프로그램 구분을 위한 변수
	int program_num = 0;

	// 인자로 받은 심볼에 @ 표시가 있다면 이를 없애줌
	if (symbol[0] == '@')
		symbol++;

	// 심볼 테이블에서 인자로 받은 심볼과 이름이 일치하는 심볼의 주소값을 반환
	for (int i = 0; i < sym_num; i++)
	{
		// 찾는 도중 심볼 테이블의 주소값이 0으로 바뀌었다면 새로운 프로그램의 시작이므로
		// 프로그램을 구분하는 변수의 값을 하나 올림
		if (i != 0 && i != 1 && sym_table[i].addr == 0)
			program_num++;

		// 현재 서브 루틴 프로그램의 정보와 해당 심볼이 정의된 프로그램의 정보가 일치하고,
		// 심볼 이름이 일치하다면 주소값 반환
		if (sub_prog_num == program_num && !strcmp(sym_table[i].symbol, symbol))
			return sym_table[i].addr;
	}

	// 찾지 못한 경우 에러의 표시로 -1 반환
	return -1;
}

// 리터럴 테이블에서 인자로 받은 리터럴의 주소값을 반환하는 함수
int search_lit_address(char * literal)
{
	// 인자로 받은 리터럴 정보에 =이 포함된 경우 이를 없앰
	if (literal[0] == '=')
		literal++;

	//리터럴 테이블에서 리터럴 이름이 일치하는 경우의 주소값을 반환
	for (int i = 0; i < lit_num; i++)
		if (!strcmp(lit_table[i].literal, literal))
			return lit_table[i].addr;

	// 찾지 못한 경우 에러의 표시로 -1 반환
	return -1;
}

// 인자로 받은 토큰의 정보를 가지고 locctr의 값을 증가시키는 함수
void increase_locctr(token * inputToken)
{
	// 인자로 들어온 토큰의 연산자가 명령어인 경우 해당 명령어의 인덱스를 구함
	int op_index = search_opcode(inputToken->operator);

	// 인덱스를 구한 경우(명령어인 경우)
	if (op_index >= 0)
	{
		// 해당 명령어가 지원하는 형식만큼 locctr를 증가
		locctr += inst_table[op_index]->form;

		// 연산자에 +가 들어간 4형식인 경우 locctr의 값을 하나 더 증가시킴
		if (strstr(inputToken->operator, "+") && inst_table[op_index]->form == 3)
			locctr++;
	}
	else if (!strcmp(inputToken->operator, "RESW")) // 토큰의 연산자가 RESW 지시어인 경우 크기를 나타내는 피연산자 값*3 만큼 증가
	{
		locctr += atoi(inputToken->operand[0]) * 3;
	}
	else if (!strcmp(inputToken->operator, "RESB"))  // 토큰의 연산자가 RESB 지시어인 경우 크기를 나타내는 피연산자 값만큼 증가
	{
		locctr += atoi(inputToken->operand[0]);
	}
	else if (!strcmp(inputToken->operator, "BYTE"))  // 토큰의 연산자가 BTYE 지시어인 경우 1 증가 (1byte)
	{
		locctr++;
	}
	else if (!strcmp(inputToken->operator, "WORD"))  // 토큰의 연산자가 WORD 지시어인 경우 3 증가 (3byte)
	{
		locctr += 3;
	}
}

// PC 값을 증가시키는 함수 (위 locctr을 증가시키는 함수와 동작방식 동일)
void increase_program_cnt(token * inputToken)
{
	int op_index = search_opcode(inputToken->operator);

	if (op_index >= 0)
	{
		program_cnt += inst_table[op_index]->form;

		if (strstr(inputToken->operator, "+") && inst_table[op_index]->form == 3)
			program_cnt++;
	}
	else if (!strcmp(inputToken->operator, "RESW"))
	{
		program_cnt += atoi(inputToken->operand[0]) * 3;
	}
	else if (!strcmp(inputToken->operator, "RESB"))
	{
		program_cnt += atoi(inputToken->operand[0]);
	}
	else if (!strcmp(inputToken->operator, "BYTE"))
	{
		program_cnt++;
	}
	else if (!strcmp(inputToken->operator, "WORD"))
	{
		program_cnt += 3;
	}
}

// 리터럴 테이블에 적절한 주소값을 넣는 함수
// 리터럴 정보는 = 문자가 없는 상태로 들어옴
void insert_addr_littab(void)
{
	// lit_input: 해당 인덱스의 리터럴을 임시로 저장하기 위한 문자열
	// lit_type: X, C 등의 데이터 타입 정보를 저장하기 위한 변수
	// lit_data: 리터럴 문자열을 분리하여 데이터 부분만을 가리키기 위한 포인터 변수
	char lit_input[20];
	char lit_type;
	char *lit_data;

	// 이전에 주소를 넣은 인덱스 이후부터 현재까지 저장된 인덱스까지 주소값을 넣기 시작
	for (int i = lit_index; i < lit_num; i++)
	{
		// 현재 주소를 해당 리터럴 주소로 저장하고
		// 해당 리터럴의 타입을 저장 후
		// 데이터 부분만을 문자열 분리로 포인트 지정
		lit_table[i].addr = locctr;
		strcpy(lit_input, lit_table[i].literal);
		lit_type = lit_input[0];
		lit_data = strtok(lit_input, "'");
		lit_data = strtok(NULL, "'");

		// 리터럴 타입이 C(char)인 경우, 데이터 길이만큼 locctr 값 증가
		if (lit_type == 'C')
		{
			locctr += strlen(lit_data);
		}
		else if (lit_type == 'X') // X(16진수 데이터)인 경우, locctr 값 1 증가
		{
			locctr++;
		}
	}
	// 현재까지 저장된 인덱스까지 모두 주소를 저장했으므로 lit_index 값 현재 인덱스 값으로 변경
	lit_index = lit_num;
}

// short 형의 주소 값을 배열 형태로 바꾸는 함수
void address_to_array(short address, char * arr, int arr_num)
{
	//short 형의 주소 값을 문자열 형태로 임시 담아두는 배열
	char temp[9];

	// 주소 값이 마이너스인 경우
	if (address < 0)
	{
		//16진수로 temp에 문자열 출력 후 필요한 만큼만 arr 배열에 담는다
		// FFFFFA -> FFA(3자리만 필요한 경우)
		sprintf(temp, "%0X", address);
		for (int i = 0; i < arr_num+1; i++)
		{
			arr[i] = temp[strlen(temp) - arr_num + i];
		}
	}
	else // 주소값이 0 이상인 경우 필요한 자리수 만큼 arr에 문자열 출력
	{
		if(arr_num == 3)
			sprintf(arr, "%03X", address);
		else if(arr_num == 5)
			sprintf(arr, "%05X", address);
	}
}

/* ----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다.
*		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다.
*		   다음과 같은 작업이 수행되어 진다.
*		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다.
* 매계 : 없음
* 반환 : 정상종료 = 0, 에러발생 = < 0
* 주의 :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{
	/* add your code here */
	// target_addr: 현재 명령어의 피연산자 주소
	// obj_index: 오브젝트 코드 테이블의 인덱스를 표현하는 변수/ op_index: inst_table상의 인덱스를 표현하는 변수
	// opcode: 해당 명령어의 16진수로 표현된 OPCODE를 담는 변수 
	// r1, r2: 레지스터 연산인 경우 피연산자로 들어온 레지스터의 번호를 담기위한 변수
	// xbpe: xbpe의 값을 10진수 형의 수로 저장하는 변수
	// lit_cnt: 리터럴 카운터 / ref_cnt: 참조 심볼 카운터
	// operand: 피연산자를 임시로 저장하는 문자열 / output_addr: 출력할 주소값을 담는 문자열
	// liter: 리터럴 데이터를 임시로 저장하기 위한 문자열 / ptr: 리터럴의 데이터 부분만을 지정하기 위한 포인터
	// op_sign: 피연산자로 식이 들어온 경우 사용하는 연산자를 담기 위한 문자 변수
	short target_addr;
	int obj_index = 0, op_index;
	int opcode, r1, r2, xbpe;
	int lit_cnt = 0, ref_cnt;
	char operand[255], output_addr[9], liter[20], *ptr;
	char op_sign;

	// 리터럴 테이블 인덱스, 프로그램 구분, locctr 초기화
	lit_index = 0;
	sub_prog_num = 0;
	locctr = 0;

	// 토큰별로 하나씩 오브젝트 코드를 생성함
	for (int i = 1; i < token_line; i++)
	{
		// 오브젝트 코드를 만들기 전, 부수 변수들을 초기화
		// inst_table 상의 연산자 인덱스 값 구함
		ptr = NULL;
		r1 = r2 = 0;
		xbpe = 0;
		target_addr = 0;
		op_index = search_opcode(token_table[i]->operator);

		// 연산자가 CSECT 지시어인 경우,
		// pc를 0으로 초기화하고 프로그램 구분 값을 1 올림(다음 프로그램)
		if (!strcmp(token_table[i]->operator,"CSECT"))
		{
			program_cnt = 0;
			sub_prog_num++;
		}
		// 연산자가 EXTREF 지시어인 경우,
		// 참조 심볼을 담는 extref 배열에 참조 심볼 이름을 담음
		else if (!strcmp(token_table[i]->operator,"EXTREF"))
		{
			for (ref_cnt = 0; token_table[i]->operand[ref_cnt] != NULL; ref_cnt++)
				strcpy(extref[ref_cnt], token_table[i]->operand[ref_cnt]);
		}
		// 명령어인 경우,
		if (op_index >= 0)
		{
			// 이전의 PC를 locctr로 지정
			locctr = program_cnt;

			// 명령어의 피연산자로 참조심볼이 들어온 경우
			// 후에 Modification record 작성을 위해
			// modif_table에 수정을 시작할 주소와 프로그램 구분값,
			// 명령어 시작주소로부터 고쳐야할 부분이 얼만큼 떨어져 있는지 저장
			for (int j = 0; j < ref_cnt; j++)
			{
				if (inst_table[op_index]->oprnd_num > 0 && strstr(token_table[i]->operand[0], extref[j]))
				{ 
					sprintf(modif_table[modif_cnt].ref_symbol.symbol, "+%s", extref[j]);
					modif_table[modif_cnt].ref_symbol.addr = locctr+1;
					modif_table[modif_cnt].program_num = sub_prog_num;
					modif_table[modif_cnt].op_or_dif = 3;
					modif_cnt++;
				}
			}
			
			// 다음 명령어 주소(PC) 값을 구하기 위해
			// 가장 가까운 메모리를 차지하는 명령어를 찾아 pc 값을 증가시킴
			for (int j = i + 1; j < token_line; j++)
			{
				if (search_opcode(token_table[j]->operator) >= 0
					|| (!strcmp(token_table[j]->operator, "RESW")) || (!strcmp(token_table[j]->operator, "RESB"))
					|| (!strcmp(token_table[j]->operator, "WORD")) || (!strcmp(token_table[j]->operator, "BYTE")))
				{
					increase_program_cnt(token_table[i]);
					break;
				}
			}
			// 명령어의 16진수 형태의 OPCODE를 opcode에 저장
			opcode = strtol(inst_table[op_index]->opcode, NULL, 16);

			//2형식을 지원하는 레지스터 연산자인 경우
			if (inst_table[op_index]->form == 2)
			{
				// 해당 명령어가 1개의 피연산자를 지원하는 경우
				if (inst_table[op_index]->oprnd_num == 1)
				{
					// 첫번째 피연산자의 레지스터에 종류에 따라 해당 레지스터 번호를 r1에 저장
					if (!strcmp(token_table[i]->operand[0], "A"))
						r1 = A_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "X"))
						r1 = X_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "L"))
						r1 = L_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "S"))
						r1 = S_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "T"))
						r1 = T_REGISTER;
				}
				// 해당 명령어가 2개의 피연산자를 지원하는 경우
				else if (inst_table[op_index]->oprnd_num == 2)
				{
					// 첫번째 피연산자의 레지스터에 종류에 따라 해당 레지스터 번호를 r1에 저장
					if (!strcmp(token_table[i]->operand[0], "A"))
						r1 = A_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "X"))
						r1 = X_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "L"))
						r1 = L_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "S"))
						r1 = S_REGISTER;
					else if (!strcmp(token_table[i]->operand[0], "T"))
						r1 = T_REGISTER;
					// 두번째 피연산자의 레지스터에 종류에 따라 해당 레지스터 번호를 r2에 저장
					if (!strcmp(token_table[i]->operand[1], "A"))
						r2 = A_REGISTER;
					else if (!strcmp(token_table[i]->operand[1], "X"))
						r2 = X_REGISTER;
					else if (!strcmp(token_table[i]->operand[1], "L"))
						r2 = L_REGISTER;
					else if (!strcmp(token_table[i]->operand[1], "S"))
						r2 = S_REGISTER;
					else if (!strcmp(token_table[i]->operand[1], "T"))
						r2 = T_REGISTER;
				}
				// opcode, 레지스터 번호들을 조합하여 오브젝트 코드로 배열에 저장
				sprintf(object_codes[obj_index], "%02X%d%d", opcode, r1, r2);
				obj_index++;
			}
			else // 2형식을 지원하는 명령어가 아닌 경우
			{
				// 연산자가 가질 수 있는 피연산자 개수가 1개인 경우
				if (inst_table[op_index]->oprnd_num == 1)
				{
					// 피연산자 임시 저장
					strcpy(operand, token_table[i]->operand[0]);
					// 피연산자 첫번째 문자가 #(immediate addressing)인 경우
					// #을 제외한 숫자값을 타겟 주소로 지정
					if (operand[0] == '#')
					{
						ptr = strtok(operand, "#");
						target_addr = atoi(ptr);
					}
					else // 그 외의 경우
					{
						// 참조 심볼을 피연산자로 쓰는지 확인
						int j;
						for (j = 0; j < ref_cnt; j++)
						{
							if (extref[j] != NULL && !strcmp(operand, extref[j]))
								break;
						}
						// 참조 심볼을 피연산자로 쓰는 경우 타겟 주소를 0으로 지정
						if (j < ref_cnt)
						{
							target_addr = 0;
						}
						// 피연산자가 심볼인 경우, 심볼 테이블에 저장된 주소를 타겟 주소로 지정
						else if (search_symbol_address(operand) >= 0)
						{
							target_addr = search_symbol_address(operand);
						}
						// 피연산자가 리터럴인 경우, 리터럴 테이블에 저장된 주소를 타겟 주소로 지정
						else if (search_lit_address(operand) >= 0)
						{
							target_addr = search_lit_address(operand);
							
							int k;
							for (k = lit_index; k < lit_index + lit_cnt; k++)
							{
								if (!strcmp(lit_table[k].literal, operand + 1))
									break;
							}
							if (k >= lit_index + lit_cnt)
								lit_cnt++;
						}
						// 그 외의 경우 주소의 값을 모두 0으로 지정
						else
							target_addr = 0;
					}

					// 토큰의 nixbpe 중 n이 1인 경우, 출력할 opcode 부분에 2 더함
					if ((token_table[i]->nixbpe & N_NIXBPE) == N_NIXBPE)
					{
						opcode += 2;
					}
					// 토큰의 nixbpe 중 i이 1인 경우, 출력할 opcode 부분에 1 더함
					if ((token_table[i]->nixbpe & I_NIXBPE) == I_NIXBPE)
					{
						opcode++;
					}
					// 토큰의 nixbpe 중 x이 1인 경우, 출력할 xbpe 부분에 8 더함
					if ((token_table[i]->nixbpe & X_NIXBPE) == X_NIXBPE)
					{
						xbpe += 8;
					}
					// 토큰의 nixbpe 중 b이 1인 경우, 출력할 xbpe 부분에 4 더함
					if ((token_table[i]->nixbpe & B_NIXBPE) == B_NIXBPE)
					{
						xbpe += 4;
					}
					// 토큰의 nixbpe 중 p이 1인 경우, 출력할 xbpe 부분에 2 더함
					// PC relative이므로 타겟 주소에서 PC 주소 값을 뺌
					if ((token_table[i]->nixbpe & P_NIXBPE) == P_NIXBPE)
					{
						xbpe += 2;
						target_addr -= program_cnt;
					}
					// 토큰의 nixbpe 중 e이 1인 경우, 출력할 xbpe 부분에 1 더함
					// 주소표현 비트 수가 늘어난 4형식이므로 주소 표현 자리수를 5로 하여 주소값을 배열로 표현함
					if ((token_table[i]->nixbpe & E_NIXBPE) == E_NIXBPE)
					{
						xbpe += 1;
						address_to_array(target_addr, output_addr, 5);
					}
					// 3형식이므로 주소 표현 자리수를 3으로 하여 주소값을 배열로 표현함
					else
						address_to_array(target_addr, output_addr, 3);
				}
				else // 연산자가 가질 수 있는 피연산자 개수가 1개가 아닌 경우(0개)
				{
					// nixbpe 중 n,i,x가 표시되어있는 경우 위와 같이 opcode나 xbpe에 적절히 더함
					if ((token_table[i]->nixbpe & N_NIXBPE) == N_NIXBPE)
					{
						opcode += 2;
					}

					if ((token_table[i]->nixbpe & I_NIXBPE) == I_NIXBPE)
					{
						opcode++;
					}

					if ((token_table[i]->nixbpe & X_NIXBPE) == X_NIXBPE)
					{
						xbpe += 8;
					}

					address_to_array(target_addr, output_addr, 3);
				}
				
				// 오브젝트 코드를 배열에 넣어줌
				sprintf(object_codes[obj_index], "%02X%X%s", opcode, xbpe, output_addr);
				obj_index++;
			}
		}
		// 현재 연산자가 LTORG나 END인 경우 리터럴에 대한 오브젝트 코드 생성
		else if (!strcmp(token_table[i]->operator, "LTORG") || !strcmp(token_table[i]->operator, "END"))
		{
			// 이전에 오브젝트 코드로 작성된 리터럴 이후부터
			// 현재까지 나온 리터럴 수만큼 오브젝트 코드 생성
			// 오브젝트 코드는 리터럴 이름으로 저장된 리터럴 데이터를 16진수로 표현하여 저장
			for (int j = lit_index; j < lit_index + lit_cnt; j++)
			{
				strcpy(liter, lit_table[j].literal);
				ptr = strtok(liter, "'");

				// 리터럴 데이터가 C(char) 타입인 경우
				if (!strcmp(ptr, "C"))
				{
					ptr = strtok(NULL, "'");

					for (int k = 0; ptr[k] != NULL; k++)
					{
						sprintf(object_codes[obj_index], "%s%02X", object_codes[obj_index], ptr[k]);
					}
					obj_index++;
				}
				// 리터럴 데이터가 X(16진수 수) 타입인 경우
				else if (!strcmp(ptr, "X"))
				{
					ptr = strtok(NULL, "'");
					sprintf(object_codes[obj_index], "%s", ptr);
					obj_index++;
				}
			}
			//현재까지 오브젝트 코드로 만든 lit_index를 저장함
			lit_index += lit_cnt;
			lit_cnt = 0;
		}
		// 연산자가 "BYTE"나 "WORD"라면
		else if (!strcmp(token_table[i]->operator, "BYTE") || !strcmp(token_table[i]->operator, "WORD"))
		{
			locctr = program_cnt;
			
			// 피연산자의 주소값으로 심볼간의 표현식으로 들어온 경우,
			// 심볼의 주소값을 구한 후 식을 계산함
			strcpy(operand, token_table[i]->operand[0]);
			ptr = strtok(operand, "-+/*");
			op_sign = '+';
			for (int j = 0; j < ref_cnt; j++)
			{
				if (strstr(ptr, extref[j]))
				{
					
					sprintf(modif_table[modif_cnt].ref_symbol.symbol, "%c%s", op_sign, ptr);
					modif_table[modif_cnt].ref_symbol.addr = locctr;
					modif_table[modif_cnt].program_num = sub_prog_num;
					modif_table[modif_cnt].op_or_dif = 0;
					modif_cnt++;

					if (strstr(token_table[i]->operand[0], "-"))
					{
						op_sign = '-';
						ptr = strtok(NULL, "-+/*");
						
						if (ptr == NULL)
							break;
						else
							j = -1;
					}
				}
			}

			//PC 값을 구함
			for (int j = i + 1; j < token_line; j++)
			{
				if (search_opcode(token_table[j]->operator) >= 0
					|| (!strcmp(token_table[j]->operator, "RESW")) || (!strcmp(token_table[j]->operator, "RESB"))
					|| (!strcmp(token_table[j]->operator, "WORD")) || (!strcmp(token_table[j]->operator, "BYTE")))
				{
					increase_program_cnt(token_table[i]);
					break;
				}
			}

			// 상수 피연산자의 데이터값만을 가지고 16진수로 변환하여 오브젝트 코드 생성
			strcpy(liter, token_table[i]->operand[0]);
			ptr = strtok(liter, "'");

			if (!strcmp(ptr, "C"))
			{
				ptr = strtok(NULL, "'");

				for (int k = 0; ptr[k] != NULL; k++)
				{
					sprintf(object_codes[obj_index], "%s%02X", object_codes[obj_index], ptr[k]);
				}
				obj_index++;
			}
			else if (!strcmp(ptr, "X"))
			{
				ptr = strtok(NULL, "'");
				sprintf(object_codes[obj_index], "%s", ptr);
				obj_index++;
			}
			else
			{
				sprintf(object_codes[obj_index], "%06d", 0);
				obj_index++;
			}
		}
	}
	return 0;
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 object code (프로젝트 1번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */
	// file: 파일을 쓰기 위한 파일 포인터
	// char_len: 파일에 쓰일 오브젝트 코드의 바이트 수를 세는 변수
	// ref_cnt: 프로그램 상단에 정의하는 또는 참조 받는 심볼의 수
	// op_index: 해당 명령어의 인덱스
	// wr_code_cnt: 한 라인에 쓰인 오브젝트 코드의 수
	// lit_cnt: 한 프로그램 상에서 쓰인 리터럴의 수
	// output: 한 라인씩 출력할 문자열을 담음
	// isEnd: END 지시어를 만나 끝나는 상태임을 표시
	// isFile: 파일에 출력하는 것인지 표준 출력인지 표시
	FILE * file;
	int char_len = 0, ref_cnt, op_index, obj_index = 0, wr_code_cnt, lit_cnt = 0;
	char output[1024] = { 0 };
	_Bool isEnd = 0, isFile = 1; 

	// 인자로 받은 파일 이름으로 쓰기용으로 파일을 열기

	if (file_name == NULL)
		isFile = 0;

	if (isFile)
	{
		file = fopen(file_name, "w");

		if (file == NULL)
			return;
	}

	// 부가적으로 쓰기 위한 변수들 초기화
	sub_prog_num = 0;
	locctr = 0;
	lit_index = 0;

	// 토큰 테이블에서 하나씩 읽어와 작성함
	for (int i = 0; i < token_line; i++)
	{
		// START 지시어인 경우
		if (!strcmp(token_table[i]->operator, "START"))
		{
			// Header record를 문자열 output에 작성
			sprintf(output, "H%s\t%06s%06X", token_table[i]->label, token_table[i]->operand[0], end_addr[sub_prog_num]);
		}
		// CSECT 지시어인 경우
		// 새로운 섹션 프로그램의 시작을 표시하는 것이므로
		// 이전 프로그램의 modification record와 end record를 출력한 후
		// 새로운 섹션 프로그램의 header record 출력
		else if (!strcmp(token_table[i]->operator, "CSECT"))
		{
			// 이전 프로그램의 modirication record 출력
			for (int j = 0; j < modif_cnt; j++)
			{
				if (modif_table[j].program_num == sub_prog_num)
				{
					int byte_num = 5;

					if (modif_table[j].op_or_dif == 0)
						byte_num = 6;

					if(isFile)
						fprintf(file, "M%06X%02X%s\n", modif_table[j].ref_symbol.addr, byte_num, modif_table[j].ref_symbol.symbol);
					else
						printf("M%06X%02X%s\n", modif_table[j].ref_symbol.addr, byte_num, modif_table[j].ref_symbol.symbol);

				}
			}

			// 이전 프로그램이 첫번째 프로그램이었다면 end rocord와 함께 명령어 시작 주소도 같이 출력
			if (sub_prog_num == 0)
			{
				if(isFile)
					fprintf(file, "E%06s\n\n", token_table[0]->operand[0]);
				else
					printf("E%06s\n\n", token_table[0]->operand[0]);
			}
			else // 그외 프로그램이라면 end record만 출력
			{
				if (isFile)
					fprintf(file, "E\n\n");
				else
					printf("E\n\n");
			}

			// 새로운 프로그램이 시작됐으므로 locctr를 초기화 하고 프로그램 구분 변수를 1 올림
			locctr = 0;
			sub_prog_num++;
			// 새 프로그램의 header record를 output에 작성
			sprintf(output, "H%s\t%06X%06X", token_table[i]->label, 0, end_addr[sub_prog_num]);
		}
		// EXTDEF 지시어인 경우
		else if (!strcmp(token_table[i]->operator, "EXTDEF"))
		{
			// define record output에 작성
			ref_cnt = 0;
			sprintf(output, "D%s%06X", token_table[i]->operand[0], search_symbol_address(token_table[i]->operand[0]));
			ref_cnt++;

			for (int j = 1; token_table[i]->operand[j] != NULL && j < 3; j++)
			{
				sprintf(output, "%s%s%06X", output, token_table[i]->operand[j], search_symbol_address(token_table[i]->operand[j]));
				ref_cnt++;
			}
		}
		//EXTREF 지시어인 경우
		else if (!strcmp(token_table[i]->operator, "EXTREF"))
		{
			// refer record output에 작성
			strcpy(extref[0], token_table[i]->operand[0]);
			sprintf(output, "R%s", token_table[i]->operand[0]);

			for (ref_cnt = 1; token_table[i]->operand[ref_cnt] != NULL; ref_cnt++)
			{
				strcpy(extref[ref_cnt], token_table[i]->operand[ref_cnt]);
				sprintf(output, "%s%s", output, token_table[i]->operand[ref_cnt]);
			}
		}
		// END 지시어인 경우 프로그램이 끝났음을 표시
		else if (!strcmp(token_table[i]->operator, "END"))
		{
			isEnd = 1;
		}
		// 명령어인 경우
		else if ((op_index = search_opcode(token_table[i]->operator)) >= 0)
		{
			wr_code_cnt = 0;
			char_len = 0;

			// 한 라인에 쓸 오브젝트 코드의 바이트 수와 오브젝트 코드의 수를 셈
			for (int j = 0; j + i < token_line; j++)
			{
				// 메모리상에서 공간을 차지하는 명령어인 경우(오브젝트 코드로 변환된 명령어인 경우)의 수를 셈
				if ((op_index = search_opcode(token_table[j + i]->operator)) >= 0
					|| !strcmp(token_table[j + i]->operator, "BYTE") || !strcmp(token_table[j + i]->operator, "WORD"))
				{
					char_len += strlen(object_codes[j + obj_index]) / 2;
					wr_code_cnt++;
					
					// 만약 해당 명령어에 리터럴이 포함되어 있다면 리터럴의 수도 셈
					if (op_index >= 0 && (inst_table[op_index]->oprnd_num >= 1 && search_lit_address(token_table[i + j]->operand[0]) >= 0))
					{
						int k;

						for (k = lit_index; k < lit_index + lit_cnt; k++)
						{
							if (!strcmp(lit_table[k].literal, token_table[i + j]->operand[0] + 1))
								break;
						}
						if (k >= lit_index + lit_cnt)
							lit_cnt++;
					}
				}
				// 혹은 END 지시어를 만날 경우 프로그램 종료를 표시한 후 출력하지 못한 남은 리터럴도 포함하여 개수를 셈
				else if (!strcmp(token_table[j + i]->operator, "END"))
				{
					isEnd = 1;
					for (int k = 0; k < lit_num - lit_index; k++)
					{
						char_len += strlen(object_codes[j+ k + obj_index]) / 2;
						wr_code_cnt++;
					}
				}
				else // 그 이외의 코드를 만나면 더이상 수를 세지 않음
					break;

				// 출력할 바이트의 수가 30을 넘어간 경우 30을 넘기 전 오브젝트코드까지만 세고 멈춤
				if (char_len >= 30)
				{
					char_len -= strlen(object_codes[j + obj_index]) / 2;
					wr_code_cnt--;
					break;
				}
			}

			// text record 표시와 첫번째 명령어 주소, 앞으로 쓸 오브젝트 코드의 바이트 수를 output에 표시
			sprintf(output, "T%06X%02X", locctr, char_len);

			// 센 오브젝트 코드 수만큼 output에 담음
			for (int j = 0; j < wr_code_cnt; j++)
			{
				sprintf(output, "%s%s", output, object_codes[j + obj_index]);
				increase_locctr(token_table[i + j]);
			}

			// 작성한 수 만큼 토큰 수를 세는 i와 obj_index의 값을 올림
			i += wr_code_cnt - 1;
			obj_index += wr_code_cnt;
		}
		// LTORG 지시어인 경우
		else if (!strcmp(token_table[i]->operator, "LTORG"))
		{
			wr_code_cnt = 0;
			char_len = 0;

			// 해당 명령어에서 명시된 리터럴 수만큼 출력할 바이트 수를 셈
			for (int j = 0; j < lit_cnt; j++)
			{
				char_len += strlen(object_codes[j + obj_index]) / 2;
			}

			// 이후 리터럴이 아닌 메모리를 차지하는 명령어가 나올수도 있으므로
			// 위와 동일한 방식으로 오브젝트 코드의 바이트 수와 출력할 오브젝트 코드 수를 셈
			for (int j = 0; j + i < token_line; j++)
			{

				if ((op_index = search_opcode(token_table[j + i]->operator)) >= 0
					|| !strcmp(token_table[j + i]->operator, "BYTE") || !strcmp(token_table[j + i]->operator, "WORD"))
				{
					char_len += strlen(object_codes[j + obj_index]) / 2;
					wr_code_cnt++;

					if (inst_table[op_index]->oprnd_num >= 1 && search_lit_address(token_table[i + j]->operand[0]) >= 0)
					{
						int k;

						for (k = lit_index; k < lit_index + lit_cnt; k++)
						{
							if (!strcmp(lit_table[k].literal, token_table[i + j]->operand[0] + 1))
								break;
						}
						if (k >= lit_index + lit_cnt)
							lit_cnt++;
					}
				}
				else if (!strcmp(token_table[j + i]->operator, "END"))
				{
					isEnd = 1;

					for (int k = 0; k < lit_num - lit_index; k++)
					{
						char_len += strlen(object_codes[j + k + obj_index]) / 2;
						wr_code_cnt++;
					}
				}
				else
					break;
			}

			// text record output에 작성
			sprintf(output, "T%06X%02X", locctr, char_len);

			// 리터럴 오브젝트 코드 output에 출력
			for (int j = 0; j < lit_cnt; j++)
			{
				sprintf(output, "%s%s", output, object_codes[j + obj_index]);
				locctr += strlen(object_codes[j + obj_index]) / 2;
			}
			lit_index += lit_cnt;
			obj_index += lit_cnt;
			lit_cnt = 0;
		}
		// RESW나 RESB 지시어인 경우 locctr 값만 증가시킴
		else if (!strcmp(token_table[i]->operator, "RESW") || !strcmp(token_table[i]->operator, "RESB"))
		{
			increase_locctr(token_table[i]);
			continue;
		}
		else
			continue;

		//이제까지 출력하기 위해 작성했던 문자열 output을 파일에 출력
		if(isFile)
			fprintf(file, "%s\n", output);
		else
			printf("%s\n", output);

		// 프로그램이 종료되는 경우
		// 마지막 프로그램의 modification record를 출력
		// end record 출력
		if (isEnd)
		{
			for (int j = 0; j < modif_cnt; j++)
			{
				if (modif_table[j].program_num == sub_prog_num)
				{
					int byte_num = 5;

					if (modif_table[j].op_or_dif == 0)
						byte_num = 6;

					if(isFile)
						fprintf(file, "M%06X%02X%s\n", modif_table[j].ref_symbol.addr, byte_num, modif_table[j].ref_symbol.symbol);
					else
						printf("M%06X%02X%s\n", modif_table[j].ref_symbol.addr, byte_num, modif_table[j].ref_symbol.symbol);
				}
			}

			if(isFile)
				fprintf(file, "E\n");
			else
				printf("E\n");
		}
	}
	if(isFile)
		fclose(file);
	freeAll();
}