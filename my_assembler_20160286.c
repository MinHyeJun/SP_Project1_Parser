/*
 * ȭ�ϸ� : my_assembler_00000000.c 
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 * ���� ������ ���Ǵ� ���ڿ� "00000000"���� �ڽ��� �й��� �����Ѵ�.
 */

/*
 *
 * ���α׷��� ����� �����Ѵ�. 
 *
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// ���ϸ��� "00000000"�� �ڽ��� �й����� ������ ��.
#include "my_assembler_20160286.h"

/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ���� 
 * ��ȯ : ���� = 0, ���� = < 0 
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�. 
 *		   ���� �߰������� �������� �ʴ´�. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[]) 
{
	// ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ���� �о��
	if(init_my_assembler()< 0)
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n"); 
		return -1 ; 
	}

	// ����� �ڵ��� Pass1 ����
	if(assem_pass1() < 0 ){
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n") ; 
		return -1 ; 
	}

	// ����� �ڵ��� ��ɾ� Opcode�� �Բ� ���Ͽ� ���
	make_opcode_output("output_20160286.txt");
	//make_opcode_output(NULL);

	make_symtab_output("symtab_20160286");
	/*
	assem_pass2();
	make_objectcode_output("output_20160286");
	*/

	/*
	* ���� ������Ʈ���� ���Ǵ� �κ�
	*
	if(assem_pass2() < 0 ){
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n") ; 
		 return -1 ; 
	}

	make_objectcode_output("output") ; 
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� 
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���. 
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
 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)�� 
 *        �����ϴ� �Լ��̴�. 
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : ���� ������� ������ �����Ӱ� �����Ѵ�. ���ô� ������ ����.
 *	
 *	===============================================================================
 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
 *	===============================================================================	   
 *		
 * ----------------------------------------------------------------------------------
 */                                                      
int init_inst_file(char *inst_file)
{
	FILE * file;  // ���ڷ� ���� ���ϸ����� ������ ���� ���� ���� ������
	int errno;  // ���� üũ 

	/* add your code here */

	char str[1024];  // �� ������ �о���� ���� ���ڿ�
	char *ptr;  // �о�� ���ڿ��� ���� ������ �и��Ͽ� �о���� ���� ������

	fopen_s(&file, inst_file, "r");  // ���ڷ� ���� ���ϸ����� ������ �б������ ��

	if (file == NULL)  // ������ �� �� ���ٸ� ǥ�� ��� ��, ��������(-1)�� ǥ��
	{
		printf("������ ���� �� �����ϴ�.");
		errno =  -1;
	}
	else  // ������ �����ٸ� �� ���ξ� �о� �̸�, ����, ���� �ڵ�, ���۷����� ���� ������ ������ ����ü�� ����
	{

		while (0 == feof(file))  // ������ ��(EOF)�� �����ϱ� ������ �ݺ�
		{
			fgets(str, 1024, file);  // ������ �� ���ξ� str�� �о����

			ptr = strtok(str, " ");  // ������ �������� ���ڿ� str�� �и�
			
			// ��ɾ���� �����ϴ� ���̺��� �� ���ҿ� ����ü inst �ϳ���ŭ�� ũ�� ���� �Ҵ�
			inst_table[inst_index] = malloc(sizeof(inst));  

			// ��ɾ� �̸�, ����, ���� �ڵ�, ���۷��� ���� ������ �����ϰ� �����⸦ �ݺ�
			strcpy(inst_table[inst_index]->inst, ptr);  // ��ɾ� �̸� ����
			ptr = strtok(NULL, " ");
			inst_table[inst_index]->form = atoi(ptr);  // ���� ����
			ptr = strtok(NULL, " ");
			strcpy(inst_table[inst_index]->opcode, ptr);  // ���� �ڵ� ����
			ptr = strtok(NULL, " ");
			inst_table[inst_index]->oprnd_num = atoi(ptr);  // ���۷��� ���� ����

			// �ϳ��� ������ �����ߴٸ� inst ������ �����ϴ� inst_index�� ���� �ϳ� �ø�
			inst_index++;  
		}

		errno = 0;  // �������� ���Ͽ��� �о���⸦ �Ϸ������Ƿ� ����(0)�� ������ ǥ��
	}
	fclose(file);  // �� ������ ����

	return errno;  // ���� ���� ��ȯ
}

/* ----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�. 
 * �Ű� : ������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0  
 * ���� : ���δ����� �����Ѵ�.
 *		
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{
	FILE * file;  // ���ڷ� ���� ���ϸ����� ������ ���� ���� ���� ������
	int errno;  // ���� üũ 

	/* add your code here */

	fopen_s(&file, input_file, "r");  // ���ڷ� ���� ���ϸ����� ������ �б������ ��

	if (file == NULL)  // ������ �� �� ���ٸ� ǥ�� ��� ��, ��������(-1)�� ǥ��
	{
		printf("������ ���� �� �����ϴ�.");
		errno = -1;
	}
	else  // ������ �����ٸ� �� ���ξ� �о� ����
	{
		while (0 == feof(file))  // ������ ��(EOF)�� �����ϱ� ������ �ݺ� 
		{
			// input_data �迭�� �� ���ҿ� ���ڿ� ���� �Ҵ� �� ������� �ҽ��ڵ� ����
			input_data[line_num] = malloc(sizeof(char) * 1024);  
			fgets(input_data[line_num], 1024, file); 

			line_num++;  // �ϳ��� ������ �����ߴٸ� �ҽ��ڵ� ���� ���� �����ϴ� line_num�� ���� �ϳ� �ø�
		}

		errno = 0;  // �������� ���Ͽ��� �о���⸦ �Ϸ������Ƿ� ����(0)�� ������ ǥ��
	}
	fclose(file);  // �� ������ ����
	
	return errno;  // ���� ���� ��ȯ
}

/* ----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. 
 *        �н� 1�� ���� ȣ��ȴ�. 
 * �Ű� : �Ľ��� ���ϴ� ���ڿ�  
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�. 
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
	// op_index: ��ū ���̺��� �ۼ��� �ҽ� �ڵ��� ��ɾ inst_table ������ ��� �ִ��� ����
	// input: ���ڷ� ���� ���ڿ� str�� �ӽ÷� �����ϱ� ���� ������
	// line: �ǰ� ���� �������� ���ڿ� input�� �и��� ��, �и��� ���ڿ��� �ӽ÷� ��� ���� ������ 
	// temp: ���۷��带 �����ϱ� ���� �ӽ� ���ڿ�
	int op_index = -1;
	char  * input, *line, temp[256];

	if (str[0] == '.')  // �ҽ��ڵ��� ������ "."�̶�� ��ū�� �и����� �ʰ� �Լ��� ����
		return 0;

	// ���ڿ� str�� ������ �����ϱ� ���� input�� ���� �Ҵ�
	input = malloc(sizeof(char) * 1024);

	strcpy(input, str);  // ���ڿ� str�� ������ input���� ����

	// ��ū�� ������ ���� ��ū ���̺��� �� ���ҿ� ����ü token �ϳ���ŭ ���� �Ҵ�
	// �� ����ü ������ �������� NULL�� �ʱ�ȭ��
	token_table[token_line] = malloc(sizeof(token));
	token_table[token_line]->label = NULL;
	token_table[token_line]->operator = NULL;
	for (int i = 0; i < MAX_OPERAND; i++)
		token_table[token_line]->operand[i] = NULL;
	token_table[token_line]->comment = NULL;

	line = strtok(input, "\t\n");  // �ǰ� ������ �������� ���ڿ��� �и��Ͽ� ���ʷ� ������

	if (input[0] != '\t')  // �ҽ��ڵ��� ù ���ڰ� ���� �ƴ� ���, ���̺��� �����Ͽ� line�� ���̺��� ��ȯ��
	{
		// ����ü ���ο� ���̺� �����ϴ� ���� ���� �Ҵ� ��, line�� ��� ���̺� ������ ����ü ���ο� ����
		token_table[token_line]->label = malloc(sizeof(char) * 30);
		strcpy(token_table[token_line]->label, line);
		line = strtok(NULL, "\t\n");  // ���̺��� ���������Ƿ� ���� ������ ���ڿ��� ������
	}

	if (line != NULL)  // ���� ������ ���ڿ��� �ִ� ��� line�� ��ɾ� Ȥ�� ���þ ��ȯ��
	{
		// line�� ���ڿ��� ��ɾ��� ���, �ش� ��ɾ��� inst_table ���� index ��ȯ
		op_index = search_opcode(line);

		// ����ü ���ο� ���̺� �����ϴ� ���� ���� �Ҵ� ��, line�� ��� operator ������ ����ü ���ο� ����
		token_table[token_line]->operator = malloc(sizeof(char) * 30);
		strcpy(token_table[token_line]->operator, line);
		line = strtok(NULL, "\t\n");  // ���� ������ ���ڿ��� ������
	}
	else  // ���� ������ ���ڿ��� ���� ��� �Լ� ����
	{
		return 0;
	}

	if (line != NULL)  // ���� ������ ���ڿ��� �ִ� ��� line�� �ǿ����ڳ� �ڸ�Ʈ�� ����
	{
		// ���� �ҽ��ڵ忡 ��ɾ ������, �ش� ��ɾ��� �ǿ����ڰ� ���ٸ� line�� ������ �ڸ�Ʈ�̹Ƿ�
		// �ǿ����� ���� �ڵ带 �ǳ� ��
		// ex) RSUB
		// �� �̿ܿ��� �ǿ����� ����
		if (!(op_index >= 0 && inst_table[op_index]->oprnd_num == 0))
		{
			strcpy(temp, line);  // �ǿ����ڸ� �и��Ͽ� �����ؾ��ϹǷ� line�� ������ temp�� ����
			line = strtok(NULL, "\t\n");  // �ǿ����ڸ� �ӽð����� ���������Ƿ� ���� ������ ���ڿ� ������

			char * operand = strtok(temp, ",");  // �ǿ����ڸ� �޸�(,) �������� �и��Ͽ� �ϳ��� ������

			// ����ü ������ �ǿ����ڸ� ��� �迭�� �и��� �ǿ����ڸ� �ϳ��� ����
			for (int j = 0; operand != NULL; j++)
			{
				token_table[token_line]->operand[j] = malloc(sizeof(char) * 256);
				strcpy(token_table[token_line]->operand[j], operand);
				operand = strtok(NULL, ",");
			}
		}

		if (line != NULL)  // ���� ������ ���ڿ��� �ִ� ��� line�� �ڸ�Ʈ�� ����
		{
			// ����ü ���ο� �ڸ�Ʈ�� �����ϴ� �����Ϳ� �����Ҵ��� �Ͽ� �ڸ�Ʈ�� ������
			token_table[token_line]->comment = malloc(sizeof(char) * 1024);
			strcpy(token_table[token_line]->comment, line);
		}

		free(input);
	}
	// �� ������ �ҽ��ڵ带 ��ū���� ��� �и������Ƿ� ��ū�� ���� �����ϴ� token_line�� ���� 1 �ø�
	token_line++;
	
}

/* ----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. 
 * �Ű� : ��ū ������ ���е� ���ڿ� 
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0 
 * ���� : 
 *		
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char *str) 
{
	int i;  // for���� ����ϱ� ���� �ӽ� ����
	// ���� �ڵ����� ���θ� Ȯ���ϱ� ����, ���ڿ� str�� ù��° ���ڰ� '+'�� ��� 
	// '+' ������ ����Ű���� ��
	// '+'�� ������ ���� �ڵ����� ���θ� Ȯ���� �� ���� ������ ó����
	if (str[0] == '+')
	{
		str = str + 1;
	}

	// inst_table�� ��ɾ� �̸��� �ϳ��� ��� ��
	// inst_table�� �����ϴ� ��ɾ �´ٸ� �ش� ��ɾ inst_table �󿡼� ���°���� �ε����� ��ȯ
	for (i = 0; i < inst_index; i++)
	{
		if (strcmp(str, inst_table[i]->inst) == 0)
		{
			return i;
		}
	}

	// ã�� �� ���ٸ� -1�� ��ȯ
	return -1;
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	/* add your code here */

	/* input_data�� ���ڿ��� ���پ� �Է� �޾Ƽ� 
	 * token_parsing()�� ȣ���Ͽ� token_unit�� ����
	 */

	// input ���Ͽ��� �о���� ���� �� ��ŭ ��ū �и� �Լ��� ȣ���Ͽ� �۾�
	for (int i = 0; i < line_num; i++)
	{
		token_parsing(input_data[i]);
	}

	if (token_table[0] != NULL && !strcmp(token_table[0]->operator,"START"))
		locctr = atoi(token_table[0]->operand);
	else
		return -1;


	insert_symbol(token_table[0]);/////////////////////////////////////////////////////////

	for (int i = 1; i < token_line; i++)
	{

		if (!strcmp(token_table[i]->operator, "CSECT"))
			locctr = 0;

		insert_symbol(token_table[i]);/////////////////////////////////////////////////////////

		if (token_table[i]->operand[0] != NULL && strstr(token_table[i]->operand[0], "="))
			insert_literal_littab(token_table[i]->operand[0]);

		if (!strcmp(token_table[i]->operator, "LTORG") || !strcmp(token_table[i]->operator,"END"))
		{
			insert_addr_littab();
		}
	}

	return 0;
}


/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ��ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 4��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*        ���� ���� 4�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char *file_name)
{
	/* add your code here */
	
	// file: ���ڷ� ���� ���ϸ����� ������ ���� ���� ���� ������
	// op_index: ���� ����ϴ� �ҽ��ڵ忡 ��ɾ ���Ե� ��� �ش� ��ɾ inst_table ������ ��� �ִ��� ����
	// op_cnt: ���� ����ϴ� �ҽ��ڵ��� �ǿ������� ������ ���� ����
	// label, operator, operand: ����ϱ� ���� ���̺�, ������, �ǿ����ڸ� ���� �ӽ÷� ��� �뵵
	// output: ����ϱ� ���� ���ڿ����� ��� �뵵
	// init: �ռ� �� �迭�� �ʱ�ȭ�ϱ� ���� �뵵
	// isFile: ���Ͽ� ����� ������(1), ǥ�� ������� ������ ������(0)
	FILE * file = NULL;  
	int op_index, op_cnt;
	char lable[20], operator[8], operand[255], output[1024], init[1024] = { 0 };
	_Bool isFile = 1;

	if (file_name == NULL)  // ���ڿ� NULL�� ���� ��� ǥ�� ���
	{
		isFile = 0;
	}
	else
	{
		fopen_s(&file, file_name, "w");  // ���ڷ� ���� ���ϸ����� ������ ��������� ��

		if (file == NULL) // ������ �� �� ���ٸ� ǥ�� ��� �� �Լ��� ����
		{
			printf("�ش� �̸��� ������ �ۼ��� �� �����ϴ�.");
			return 0;
		}
	}
	
	// ��ū���� �и��ߴ� �ҽ��ڵ带 ���
	for (int i = 0; i < token_line; i++)
	{
		// ����� ���̺�, ������, �ǿ����ڸ� ��� ���� �迭 �ʱ�ȭ
		strcpy(lable, init);
		strcpy(operator, init);
		strcpy(operand, init);
		strcpy(output, init);
		op_cnt = 0;  // �ǿ����� ���� �ʱ�ȭ

		// ��ū�� ���̺� ������ ������� �ʴٸ�(NULL�� �ƴ϶��)
		// ���̺� ������ lable�� ����
		if (token_table[i]->label != NULL)
			strcpy(lable, token_table[i]->label);

		// ��ū�� ������ ������ ��� ������ ��ɾ��� inst_table ���� �ε����� ���� �� operator�� ����
		op_index = search_opcode(token_table[i]->operator);
		strcpy(operator, token_table[i]->operator);
		
		// ��ū�� ����� �ǿ����� ������ ����
		for (int j = 0; token_table[i]->operand[j] != NULL; j++)
		{
			op_cnt++;
		}

		// ��ū�� ����� �ǿ����ڵ��� operand�� ����
		// �ǿ����ڰ� ���� ���� ��� �ǿ����ڵ� ���̿� �޸�(,) �Է�
		for (int j = 0; j < op_cnt; j++)
		{
			strcat(operand, token_table[i]->operand[j]);

			if (j < op_cnt - 1)
				strcat(operand, ",");
		}

		// ����� ���� ������ ������ ���̺�, ������, �ǿ����� ������ �迭 output�� ��� ����
		sprintf(output, "%-10s\t%s\t%-20s", lable, operator, operand);

		if (op_index >= 0)  // ��ɾ� �ε����� �����ϴ� op_index�� ���� 0���� ���ų� ū ��� (���̺� �����ϴ� ��ɾ��� ���)
		{
			// �ش� ��ɾ��� ���� �ڵ带 ����� ���ڿ� output�� ���Խ�Ŵ
			sprintf(output, "%s\t%s", output, inst_table[op_index]->opcode);
		}

		// ���Ͽ��� ����ϴ� ���, fprintf�� ���Ͽ� ��ɾ� ���
		if (isFile)
		{
			fprintf(file, "%s\n", output);
		}
		else // ���Ͽ� ����ϴ� ��찡 �ƴ϶�� ǥ��������� ��ɾ� ���
		{
			printf("%s\n", output);
		}
	}
	if(file != NULL)
		fclose(file);  // �� ������ ����

	freeAll();  // �޸� �����Ҵ� ��� ��ȯ
}

// �޸� �����Ҵ� ��� ��ȯ�ϴ� �Լ�
void freeAll(void) {
	int i;

	// ���� �ڵ� ���̺� ��ȯ
	for (i = 0; i < inst_index; i++)
	{
		free(inst_table[i]);
	}

	// ��ū ���̺� ��ȯ
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

	// �ҽ��ڵ� ���̺� ��ȯ
	for (i = 0; i < line_num; i++)
	{
		free(input_data[i]);
	}
}


/* --------------------------------------------------------------------------------*
* ------------------------- ���� ������Ʈ���� ����� �Լ� --------------------------*
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
		fprintf(file, "%s\t%x\n", sym_table[i].symbol, sym_table[i].addr);
		printf("%s\t%x\n", sym_table[i].symbol, sym_table[i].addr);
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
	char data_type;
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

// �ߺ��Ǵ� �ɺ��� ���� ��� ��� ���α׷� ���� �ִ� �ɺ��� ������ ������ �ľ��ؼ� �������� �ڵ� �ʿ�
int search_symbol_address(char * symbol)
{
	for (int i = 0; i < sym_num; i++)
		if (!strcmp(sym_table[i].symbol, symbol))
			return sym_table[i].addr;

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

void insert_addr_littab()
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

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

	/* add your code here */

}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */
	
}