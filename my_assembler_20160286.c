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
	make_opcode_output("output_20160286.txt");
	//make_opcode_output(NULL);

	make_symtab_output("symtab_20160286");
	
	assem_pass2();
	make_objectcode_output("output_20160286");
	

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

		if (op_index >= 0 && inst_table[op_index]->form != 2)
		{
			if (line[0] == '+')
				token_table[token_line]->nixbpe |= E_NIXBPE;
			else
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

			if (op_index >= 0 && inst_table[op_index]->form != 2)
			{
				if ((token_table[token_line]->operand[1]!= NULL) && (!strcmp(token_table[token_line]->operand[1], "X")))
				{
					token_table[token_line]->nixbpe |= X_NIXBPE;
				}
				if (strstr(token_table[token_line]->operand[0], "@"))
				{
					token_table[token_line]->nixbpe |= N_NIXBPE;
				}
				else if (strstr(token_table[token_line]->operand[0], "#"))
				{
					token_table[token_line]->nixbpe |= I_NIXBPE;
					token_table[token_line]->nixbpe ^= P_NIXBPE;
				}
				else
				{
					token_table[token_line]->nixbpe |= N_NIXBPE + I_NIXBPE;
				}
			}
		}
		else
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
	// 기계어 코드인지 여부를 확인하기 전에, 문자열 str의 첫번째 문자가 '+'인 경우 
	// '+' 다음을 가리키도록 함
	// '+'가 있으면 기계어 코드인지 여부를 확인할 수 없기 때문에 처리함
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

	if (token_table[0] != NULL && !strcmp(token_table[0]->operator,"START"))
	{
		locctr = atoi(token_table[0]->operand[0]);
		sub_prog_num = 0;
	}
	else
		return -1;


	insert_symbol(token_table[0]);/////////////////////////////////////////////////////////

	for (int i = 1; i < token_line; i++)
	{

		if (!strcmp(token_table[i]->operator, "CSECT"))
		{
			end_addr[sub_prog_num] = locctr;
			locctr = 0;
			sub_prog_num++;
		}

		insert_symbol(token_table[i]);/////////////////////////////////////////////////////////

		if (token_table[i]->operand[0] != NULL && strstr(token_table[i]->operand[0], "="))
			insert_literal_littab(token_table[i]->operand[0]);

		if (!strcmp(token_table[i]->operator, "LTORG") || !strcmp(token_table[i]->operator,"END"))
		{
			insert_addr_littab();
		}
	}

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
			return 0;
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
			// 해당 명령어의 기계어 코드를 출력할 문자열 output에 포함시킴
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

void make_symtab_output(char *fileName)
{
	FILE * file;

	file = fopen(fileName, "w");

	if (file == NULL)
	{
		return;
	}

	for (int i = 0; i < sym_num; i++)
	{
		fprintf(file, "%s\t%X\n", sym_table[i].symbol, sym_table[i].addr);
		printf("%s\t%X\n", sym_table[i].symbol, sym_table[i].addr);
	}
}

void insert_symbol(token * inputToken)
{
	int op_index = -1;

	if (inputToken->label != NULL)
	{
		strcpy(sym_table[sym_num].symbol, inputToken->label);

		if (!strcmp(inputToken->operator, "EQU"))
		{
			if (!strcmp(inputToken->operand[0], "*"))
				sym_table[sym_num].addr = locctr;
			else
				sym_table[sym_num].addr = operate_address(inputToken->operand[0]); /////////////////////////////////////
		}
		else
			sym_table[sym_num].addr = locctr;
		sym_num++;
	}

	increase_locctr(inputToken);
}

void insert_literal_littab(char * input_literal)
{
	char data[255];
	char *ptr;

	strcpy(data, input_literal);

	if (data[0] == '=')
		ptr = strtok(data, "=");

	for (int i = lit_index; i < lit_num; i++)
	{
		if (!strcmp(lit_table[i].literal, ptr))
			return;
	}

	strcpy(lit_table[lit_num].literal, ptr);

	lit_num++;
}

int operate_address(char * input_operand)
{
	char expression[1024];
	char * symbol;
	int result = -1;

	strcpy(expression, input_operand);

	if (strstr(expression, "-"))
	{
		symbol = strtok(expression, "-");
		result = search_symbol_address(symbol);
		symbol = strtok(NULL, "-");
		result -= search_symbol_address(symbol);
	}

	return result;
}

// 중복되는 심볼이 있을 경우 어느 프로그램 내에 있는 심볼을 가져올 것인지 파악해서 가져오는 코드 필요
int search_symbol_address(char * symbol)
{
	int program_num = 0;

	if (symbol[0] == '@')
		symbol++;

	for (int i = 0; i < sym_num; i++)
	{
		if (i != 0 && i != 1 && sym_table[i].addr == 0)
			program_num++;

		if (sub_prog_num == program_num && !strcmp(sym_table[i].symbol, symbol))
			return sym_table[i].addr;
	}

	return -1;
}

int search_lit_address(char * literal)
{
	if (literal[0] == '=')
		literal++;

	for (int i = 0; i < lit_num; i++)
		if (!strcmp(lit_table[i].literal, literal))
			return lit_table[i].addr;

	return -1;
}

void increase_locctr(token * inputToken)
{
	int op_index = search_opcode(inputToken->operator);

	if (op_index >= 0)
	{
		locctr += inst_table[op_index]->form;

		if (strstr(inputToken->operator, "+") && inst_table[op_index]->form == 3)
			locctr++;
	}
	else if (!strcmp(inputToken->operator, "RESW"))
	{
		locctr += atoi(inputToken->operand[0]) * 3;
	}
	else if (!strcmp(inputToken->operator, "RESB"))
	{
		locctr += atoi(inputToken->operand[0]);
	}
	else if (!strcmp(inputToken->operator, "BYTE"))
	{
		locctr++;
	}
	else if (!strcmp(inputToken->operator, "WORD"))
	{
		locctr += 3;
	}
}

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

void insert_addr_littab(void)
{
	char lit_input[20];
	char lit_type;
	char *lit_data;

	for (int i = lit_index; i < lit_num; i++)
	{
		lit_table[i].addr = locctr;
		strcpy(lit_input, lit_table[i].literal);
		lit_type = lit_input[0];
		lit_data = strtok(lit_input, "'");
		lit_data = strtok(NULL, "'");

		if (lit_type == 'C')
		{
			locctr += strlen(lit_data);
		}
		else if (lit_type == 'X')
		{
			locctr++;
		}
	}

	lit_index = lit_num;
}

void address_to_array(short address, char * arr, int arr_num)
{
	char temp[9];

	if (address < 0)
	{
		sprintf(temp, "%0X", address);
		for (int i = 0; i < arr_num+1; i++)
		{
			arr[i] = temp[strlen(temp) - arr_num + i];
		}
	}
	else
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
	short target_addr;
	int obj_index = 0, op_index;
	int opcode, r1, r2, xbpe;
	int lit_cnt = 0, ref_cnt;
	char operand[255], output_addr[9], liter[20], *ptr;

	lit_index = 0;
	sub_prog_num = 0;
	locctr = 0;

	for (int i = 1; i < token_line; i++)
	{
		ptr = NULL;
		r1 = r2 = 0;
		xbpe = 0;
		target_addr = 0;
		op_index = search_opcode(token_table[i]->operator);

		if (!strcmp(token_table[i]->operator,"CSECT"))
		{
			program_cnt = 0;
			sub_prog_num++;
		}
		else if (!strcmp(token_table[i]->operator,"EXTREF"))
		{
			for (ref_cnt = 0; token_table[i]->operand[ref_cnt] != NULL; ref_cnt++)
				strcpy(extref[ref_cnt], token_table[i]->operand[ref_cnt]);
		}

		if (op_index >= 0)
		{
			locctr = program_cnt;

			for (int j = 0; j < ref_cnt; j++)
			{
				if (inst_table[op_index]->oprnd_num > 0 && strstr(token_table[i]->operand[0], extref[j]))
				{
					strcpy(modif_table[modif_cnt].ref_symbol.symbol, token_table[i]->operand[0]);
					modif_table[modif_cnt].ref_symbol.addr = locctr+1;
					modif_table[modif_cnt].program_num = sub_prog_num;
					modif_table[modif_cnt].op_or_dif = 3;
					modif_cnt++;
					break;
				}
			}

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
			opcode = strtol(inst_table[op_index]->opcode, NULL, 16);

			if (inst_table[op_index]->form == 2)
			{

				if (inst_table[op_index]->oprnd_num == 1)
				{
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
				else if (inst_table[op_index]->oprnd_num == 2)
				{
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

				sprintf(object_codes[obj_index], "%02X%d%d", opcode, r1, r2);
				obj_index++;
			}
			else
			{
				if (inst_table[op_index]->oprnd_num == 1)
				{
					strcpy(operand, token_table[i]->operand[0]);
					if (operand[0] == '#')
					{
						ptr = strtok(operand, "#");
						target_addr = atoi(ptr);
					}
					else
					{
						int j;
						for (j = 0; j < ref_cnt; j++)
						{
							if (extref[j] != NULL && !strcmp(operand, extref[j]))
								break;
						}
						if (j < ref_cnt)
						{
							target_addr = 0;
						}
						else if (search_symbol_address(operand) >= 0)
						{
							target_addr = search_symbol_address(operand);
						}
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
						else
							target_addr = 0;
					}

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

					if ((token_table[i]->nixbpe & B_NIXBPE) == B_NIXBPE)
					{
						xbpe += 4;
					}

					if ((token_table[i]->nixbpe & P_NIXBPE) == P_NIXBPE)
					{
						xbpe += 2;
						target_addr -= program_cnt;
					}

					if ((token_table[i]->nixbpe & E_NIXBPE) == E_NIXBPE)
					{
						xbpe += 1;
						address_to_array(target_addr, output_addr, 5);
					}
					else
						address_to_array(target_addr, output_addr, 3);
				}
				else
				{
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

				sprintf(object_codes[obj_index], "%02X%X%s", opcode, xbpe, output_addr);
				obj_index++;
			}
		}
		else if (!strcmp(token_table[i]->operator, "LTORG") || !strcmp(token_table[i]->operator, "END"))
		{
			for (int j = lit_index; j < lit_index + lit_cnt; j++)
			{
				strcpy(liter, lit_table[j].literal);
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
			}
			lit_index += lit_cnt;
			lit_cnt = 0;
		}
		else if (!strcmp(token_table[i]->operator, "BYTE") || !strcmp(token_table[i]->operator, "WORD"))
		{
			locctr = program_cnt;

			for (int j = 0; j < ref_cnt; j++)
			{
				if (strstr(token_table[i]->operand[0], extref[j]))
				{
					strcpy(modif_table[modif_cnt].ref_symbol.symbol, token_table[i]->operand[0]);
					modif_table[modif_cnt].ref_symbol.addr = locctr;
					modif_table[modif_cnt].program_num = sub_prog_num;
					modif_table[modif_cnt].op_or_dif = 0;
					modif_cnt++;
					break;
				}
			}

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
	for (int i = 0; i < obj_index; i++)
		printf("%s\n", object_codes[i]);

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
	FILE * file;
	int char_len = 0, ref_cnt, op_index, obj_index = 0, wr_code_cnt, lit_cnt = 0;
	char output[1024] = { 0 };
	char code_line[1024] = { 0 };
	_Bool isEnd = 0;

	file = fopen(file_name, "w");

	if (file == NULL)
		return;

	sub_prog_num = 0;
	locctr = 0;
	lit_index = 0;

	for (int i = 0; i < token_line; i++)
	{
		if (!strcmp(token_table[i]->operator, "START"))
		{
			sprintf(output, "H%s\t%06s%06X", token_table[i]->label, token_table[i]->operand[0], end_addr[sub_prog_num]);
		}
		else if (!strcmp(token_table[i]->operator, "CSECT"))
		{
			for (int j = 0; j < modif_cnt; j++)
			{
				if (modif_table[j].program_num == sub_prog_num)
				{
					int byte_num = 5;

					if (modif_table[j].op_or_dif == 0)
						byte_num = 6;

					fprintf(file, "M%06X%02X+%s\n", modif_table[j].ref_symbol.addr, byte_num, modif_table[j].ref_symbol.symbol);
					printf("M%06X%02X+%s\n", modif_table[j].ref_symbol.addr, byte_num, modif_table[j].ref_symbol.symbol);
				}
			}

			if (sub_prog_num == 0)
			{
				fprintf(file, "E%06s\n\n", token_table[0]->operand[0]);
				printf("E%06s\n\n", token_table[0]->operand[0]);
			}
			else
			{
				fprintf(file, "E\n\n");
				printf("E\n\n");
			}

			if (!strcmp(token_table[i]->operator, "CSECT"))
			{
				locctr = 0;
				sub_prog_num++;

				sprintf(output, "H%s\t%06X%06X", token_table[i]->label, 0, end_addr[sub_prog_num]);
			}
		}
		else if (!strcmp(token_table[i]->operator, "EXTDEF"))
		{
			ref_cnt = 0;
			sprintf(output, "D%s%06X", token_table[i]->operand[0], search_symbol_address(token_table[i]->operand[0]));
			ref_cnt++;

			for (int j = 1; token_table[i]->operand[j] != NULL && j < 3; j++)
			{
				sprintf(output, "%s%s%06X", output, token_table[i]->operand[j], search_symbol_address(token_table[i]->operand[j]));
				ref_cnt++;
			}
		}
		else if (!strcmp(token_table[i]->operator, "EXTREF"))
		{
			strcpy(extref[0], token_table[i]->operand[0]);
			sprintf(output, "R%s", token_table[i]->operand[0]);

			for (ref_cnt = 1; token_table[i]->operand[ref_cnt] != NULL; ref_cnt++)
			{
				strcpy(extref[ref_cnt], token_table[i]->operand[ref_cnt]);
				sprintf(output, "%s%s", output, token_table[i]->operand[ref_cnt]);
			}
		}
		else if (!strcmp(token_table[i]->operator, "END"))
		{
			isEnd = 1;
		}
		else if ((op_index = search_opcode(token_table[i]->operator)) >= 0)
		{
			wr_code_cnt = 0;
			char_len = 0;

			for (int j = 0; j + i < token_line; j++)
			{

				if ((op_index = search_opcode(token_table[j + i]->operator)) >= 0
					|| !strcmp(token_table[j + i]->operator, "BYTE") || !strcmp(token_table[j + i]->operator, "WORD"))
				{
					char_len += strlen(object_codes[j + obj_index]) / 2;
					wr_code_cnt++;
					
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
				else if (!strcmp(token_table[j + i]->operator, "END"))
				{
					isEnd = 1;
					for (int k = 0; k < lit_num - lit_index; k++)
					{
						char_len += strlen(object_codes[j+ k + obj_index]) / 2;
						wr_code_cnt++;
					}
				}
				else
					break;

				if (char_len >= 30)
				{
					char_len -= strlen(object_codes[j + obj_index]) / 2;
					wr_code_cnt--;
					break;
				}
			}

			sprintf(output, "T%06X%02X", locctr, char_len);

			for (int j = 0; j < wr_code_cnt; j++)
			{
				sprintf(output, "%s%s", output, object_codes[j + obj_index]);
				increase_locctr(token_table[i + j]);
			}

			i += wr_code_cnt - 1;
			obj_index += wr_code_cnt;
		}
		else if (!strcmp(token_table[i]->operator, "LTORG"))
		{
			wr_code_cnt = 0;
			char_len = 0;

			for (int j = 0; j < lit_cnt; j++)
			{
				char_len += strlen(object_codes[j + obj_index]) / 2;
			}

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

			sprintf(output, "T%06X%02X", locctr, char_len);

			for (int j = 0; j < lit_cnt; j++)
			{
				sprintf(output, "%s%s", output, object_codes[j + obj_index]);
				locctr += strlen(object_codes[j + obj_index]) / 2;
			}
			lit_index += lit_cnt;
			obj_index += lit_cnt;
			lit_cnt = 0;
		}
		else if (!strcmp(token_table[i]->operator, "RESW") || !strcmp(token_table[i]->operator, "RESB"))
		{
			increase_locctr(token_table[i]);
			continue;
		}
		else
			continue;

		fprintf(file, "%s\n", output);
		printf("%s\n", output);

		if (isEnd)
		{
			for (int j = 0; j < modif_cnt; j++)
			{
				if (modif_table[j].program_num == sub_prog_num)
				{
					fprintf(file, "M%06X+%s\n", modif_table[j].ref_symbol.addr, modif_table[j].ref_symbol.symbol);
					printf("M%06X+%s\n", modif_table[j].ref_symbol.addr, modif_table[j].ref_symbol.symbol);
				}
			}

			fprintf(file, "E\n");
			printf("E\n");
		}
	}

	fclose(file);
	freeAll();
}