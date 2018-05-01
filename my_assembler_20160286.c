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
	//make_opcode_output("output_20160286.txt");
	//make_opcode_output(NULL);

	make_symtab_output("symtab_20160286");
	assem_pass2();
	make_objectcode_output("output_20160286");
	//make_objectcode_output(NULL);
	

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
	token_table[token_line]->nixbpe = 0;

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

		// 3���� �Ǵ� 4������ �����ϴ� ��ɾ��� ���, 
		if (op_index >= 0 && inst_table[op_index]->form == 3)
		{
			// 4���� ��ɾ��� ��� nixbpe �� e ǥ��
			if (line[0] == '+')
				token_table[token_line]->nixbpe |= E_NIXBPE;
			else // �ƴ϶�� pc relative�� ���� nixbpe �� p�� ǥ��
				token_table[token_line]->nixbpe |= P_NIXBPE;
		}

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

			//3 �Ǵ� 4������ �����ϴ� ��ɾ��� ��쿡 ���� nixpbe�� ǥ��
			if (op_index >= 0 && inst_table[op_index]->form == 3)
			{
				// X�������͸� �̿��� ������ �ϴ� ��� x ǥ��
				if ((token_table[token_line]->operand[1]!= NULL) && (!strcmp(token_table[token_line]->operand[1], "X")))
				{
					token_table[token_line]->nixbpe |= X_NIXBPE;
				}
				
				// indirect addressing�� ����ϴ� ��� n ǥ��
				if (strstr(token_table[token_line]->operand[0], "@"))
				{
					token_table[token_line]->nixbpe |= N_NIXBPE;
				}
				else if (strstr(token_table[token_line]->operand[0], "#")) // immediate addressing�� ����ϴ� ��� iǥ�� �� p ǥ�� ����
				{
					token_table[token_line]->nixbpe |= I_NIXBPE;
					token_table[token_line]->nixbpe ^= P_NIXBPE;
				}
				else // �� ���� ��� simple addressing ����̹Ƿ� n�� i ǥ��
				{
					token_table[token_line]->nixbpe |= N_NIXBPE + I_NIXBPE;
				}
			}
		}
		else  //�� ���� ��� simple addressing ��� �̹Ƿ� n�� i ǥ��
		{
			token_table[token_line]->nixbpe |= N_NIXBPE + I_NIXBPE;
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
	
	return 0;
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
	// ��ɾ����� ���θ� Ȯ���ϱ� ����, ���ڿ� str�� ù��° ���ڰ� '+'�� ��� 
	// '+' ������ ����Ű���� ��
	// '+'�� ������ ��ɾ����� ���θ� Ȯ���� �� ���� ������ ó����
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

	//ù��° ��ū�� �����ڰ� START��� ���þ��� �ǿ����ڷ� ���� �ּҰ����� locctr�� �ʱ�ȭ
	//ù��° ���α׷����� ǥ�� (sub_prog_num = 0)
	if (token_table[0] != NULL && !strcmp(token_table[0]->operator,"START"))
	{
		locctr = atoi(token_table[0]->operand[0]);
		sub_prog_num = 0;
	}
	else
		return -1;

	//ù��° ��ū�� ���̺��� �ɺ� ���̺� ����
	insert_symbol(token_table[0]);

	// ��ū�� �ϳ��� ������ Pass1 ����
	for (int i = 1; i < token_line; i++)
	{
		// �ش� ��ū�� �����ڰ� CSECT��� ���ο� �����ƾ�� ������
		// ���� ���α׷��� ������ �ּҸ� �迭�� ����
		// locctr�� ���� 0���� �ʱ�ȭ
		// ���α׷� ǥ�ð��� 1 �ø�
		if (!strcmp(token_table[i]->operator, "CSECT"))
		{
			end_addr[sub_prog_num] = locctr;
			locctr = 0;
			sub_prog_num++;
		}

		// ���̺� ������ �ִ� ��ū�� ��� �ɺ��� ���̺� �����ϵ��� ��
		insert_symbol(token_table[i]);

		// �ǿ����ڷ� ���ͷ��� ���� ���, ���ͷ� ������ ���̺� �����ϵ��� ��
		if (token_table[i]->operand[0] != NULL && strstr(token_table[i]->operand[0], "="))
			insert_literal_littab(token_table[i]->operand[0]);

		//�о���� �����ڰ� LTORG�� END ���þ��� ���ͷ� �ּҸ� ���ͷ� ���̺� ����
		if (!strcmp(token_table[i]->operator, "LTORG") || !strcmp(token_table[i]->operator,"END"))
		{
			insert_addr_littab();
		}
	}

	// ������ ���� ��ƾ ���α׷��� �� �ּҸ� ����
	end_addr[sub_prog_num] = locctr;

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
			return;
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
			// �ش� �ڵ��� ���� �ڵ带 ����� ���ڿ� output�� ���Խ�Ŵ
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

	//freeAll();  // �޸� �����Ҵ� ��� ��ȯ
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

// �ɺ� ���̺��� ������ ���Ͽ� ����ϴ� �Լ�
void make_symtab_output(char *fileName)
{
	FILE * file;

	// ������ ���ڿ��� �̸����� �� ������ ��������� ����
	file = fopen(fileName, "w");

	if (file == NULL)
	{
		return;
	}

	// �ɺ� ���̺� ����� �ɺ� �̸��� �ּҸ� ���
	for (int i = 0; i < sym_num; i++)
	{
		fprintf(file, "%s\t%X\n", sym_table[i].symbol, sym_table[i].addr);
		//printf("%s\t%X\n", sym_table[i].symbol, sym_table[i].addr);
	}
}

// ���ڷ� ���� ��ū���� ���̺� ������ �ɺ� ���̺� �ɺ� �̸����� �ִ� �Լ�
void insert_symbol(token * inputToken)
{
	int op_index = -1; 

	// ��ū�� ���̺� ������ �ִ� ��� ������ �ɺ��� �ִٰ� �ľ�
	if (inputToken->label != NULL)
	{
		// ���̺��� �ɺ� ���̺� �̸����� ����
		strcpy(sym_table[sym_num].symbol, inputToken->label);

		// ��ū�� ��ɾ� �����ڰ� EQU ���þ��� �ּҰ����� �ǿ����� ���� �־�� ��
		if (!strcmp(inputToken->operator, "EQU"))
		{
			// �ǿ����ڰ� *�� ��� ���� locctr�� ���� �ּҷ� ����
			if (!strcmp(inputToken->operand[0], "*"))
				sym_table[sym_num].addr = locctr;
			else  // �׷��� ���� ��� �ǿ����ڸ� ���� �ּҰ��� ����Ͽ� ����
				sym_table[sym_num].addr = operate_address(inputToken->operand[0]);
		}
		else  // �� ���� ��� ���� �ּҰ��� �ɺ� �ּҰ����� ����
			sym_table[sym_num].addr = locctr;

		sym_num++; // �ϳ��� �����ϰ� ���������Ƿ� ���� ���� 1 �ø� 
	}

	increase_locctr(inputToken);  // ���� locctr�� ������Ŵ
}

// ���ڷ� ���� ���ͷ��� ���ͷ� ���̺� �ִ� �Լ�
void insert_literal_littab(char * input_literal)
{
	char data[255];
	char *ptr;

	// ���ڿ� �и��� ���� ���ͷ� ������ �ӽ÷� ������
	strcpy(data, input_literal);

	// ���ͷ� ������ ù��° ���ڷ� = �� ���Ե� ��� �̸� ����
	if (data[0] == '=')
		ptr = strtok(data, "=");

	// ������ ����� ���ͷ����� Ȯ��
	for (int i = lit_index; i < lit_num; i++)
	{
		if (!strcmp(lit_table[i].literal, ptr))
			return;
	}
	
	//���ο� ���ͷ��̶�� ���ͷ� ���̺� ����
	strcpy(lit_table[lit_num].literal, ptr);

	lit_num++;  // ���ͷ� ������ 1 �ø�
}

// �ǿ����ڿ� ������ ���Ե� ��� �ּҸ� ����Ͽ� ��ȯ�ϴ� �Լ�
int operate_address(char * input_operand)
{
	char expression[1024];
	char * symbol;
	int result = -1;

	// ����� �ǿ����� ������ �ӽ÷� ����
	strcpy(expression, input_operand);

	// - ������ �ִ� ���, �ش� �ɺ��� �ּҰ��� ������ - ���� ����
	if (strstr(expression, "-"))
	{
		symbol = strtok(expression, "-");
		result = search_symbol_address(symbol);
		symbol = strtok(NULL, "-");
		result -= search_symbol_address(symbol);
	}

	// ���� �ּҰ� ��ȯ
	return result;
}

// ���ڷ� ���� �ɺ� �̸��� ������ ���̺��� �ּҰ��� ã�� ��ȯ�ϴ� �Լ�
int search_symbol_address(char * symbol)
{
	// ���α׷� ������ ���� ����
	int program_num = 0;

	// ���ڷ� ���� �ɺ��� @ ǥ�ð� �ִٸ� �̸� ������
	if (symbol[0] == '@')
		symbol++;

	// �ɺ� ���̺��� ���ڷ� ���� �ɺ��� �̸��� ��ġ�ϴ� �ɺ��� �ּҰ��� ��ȯ
	for (int i = 0; i < sym_num; i++)
	{
		// ã�� ���� �ɺ� ���̺��� �ּҰ��� 0���� �ٲ���ٸ� ���ο� ���α׷��� �����̹Ƿ�
		// ���α׷��� �����ϴ� ������ ���� �ϳ� �ø�
		if (i != 0 && i != 1 && sym_table[i].addr == 0)
			program_num++;

		// ���� ���� ��ƾ ���α׷��� ������ �ش� �ɺ��� ���ǵ� ���α׷��� ������ ��ġ�ϰ�,
		// �ɺ� �̸��� ��ġ�ϴٸ� �ּҰ� ��ȯ
		if (sub_prog_num == program_num && !strcmp(sym_table[i].symbol, symbol))
			return sym_table[i].addr;
	}

	// ã�� ���� ��� ������ ǥ�÷� -1 ��ȯ
	return -1;
}

// ���ͷ� ���̺��� ���ڷ� ���� ���ͷ��� �ּҰ��� ��ȯ�ϴ� �Լ�
int search_lit_address(char * literal)
{
	// ���ڷ� ���� ���ͷ� ������ =�� ���Ե� ��� �̸� ����
	if (literal[0] == '=')
		literal++;

	//���ͷ� ���̺��� ���ͷ� �̸��� ��ġ�ϴ� ����� �ּҰ��� ��ȯ
	for (int i = 0; i < lit_num; i++)
		if (!strcmp(lit_table[i].literal, literal))
			return lit_table[i].addr;

	// ã�� ���� ��� ������ ǥ�÷� -1 ��ȯ
	return -1;
}

// ���ڷ� ���� ��ū�� ������ ������ locctr�� ���� ������Ű�� �Լ�
void increase_locctr(token * inputToken)
{
	// ���ڷ� ���� ��ū�� �����ڰ� ��ɾ��� ��� �ش� ��ɾ��� �ε����� ����
	int op_index = search_opcode(inputToken->operator);

	// �ε����� ���� ���(��ɾ��� ���)
	if (op_index >= 0)
	{
		// �ش� ��ɾ �����ϴ� ���ĸ�ŭ locctr�� ����
		locctr += inst_table[op_index]->form;

		// �����ڿ� +�� �� 4������ ��� locctr�� ���� �ϳ� �� ������Ŵ
		if (strstr(inputToken->operator, "+") && inst_table[op_index]->form == 3)
			locctr++;
	}
	else if (!strcmp(inputToken->operator, "RESW")) // ��ū�� �����ڰ� RESW ���þ��� ��� ũ�⸦ ��Ÿ���� �ǿ����� ��*3 ��ŭ ����
	{
		locctr += atoi(inputToken->operand[0]) * 3;
	}
	else if (!strcmp(inputToken->operator, "RESB"))  // ��ū�� �����ڰ� RESB ���þ��� ��� ũ�⸦ ��Ÿ���� �ǿ����� ����ŭ ����
	{
		locctr += atoi(inputToken->operand[0]);
	}
	else if (!strcmp(inputToken->operator, "BYTE"))  // ��ū�� �����ڰ� BTYE ���þ��� ��� 1 ���� (1byte)
	{
		locctr++;
	}
	else if (!strcmp(inputToken->operator, "WORD"))  // ��ū�� �����ڰ� WORD ���þ��� ��� 3 ���� (3byte)
	{
		locctr += 3;
	}
}

// PC ���� ������Ű�� �Լ� (�� locctr�� ������Ű�� �Լ��� ���۹�� ����)
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

// ���ͷ� ���̺� ������ �ּҰ��� �ִ� �Լ�
// ���ͷ� ������ = ���ڰ� ���� ���·� ����
void insert_addr_littab(void)
{
	// lit_input: �ش� �ε����� ���ͷ��� �ӽ÷� �����ϱ� ���� ���ڿ�
	// lit_type: X, C ���� ������ Ÿ�� ������ �����ϱ� ���� ����
	// lit_data: ���ͷ� ���ڿ��� �и��Ͽ� ������ �κи��� ����Ű�� ���� ������ ����
	char lit_input[20];
	char lit_type;
	char *lit_data;

	// ������ �ּҸ� ���� �ε��� ���ĺ��� ������� ����� �ε������� �ּҰ��� �ֱ� ����
	for (int i = lit_index; i < lit_num; i++)
	{
		// ���� �ּҸ� �ش� ���ͷ� �ּҷ� �����ϰ�
		// �ش� ���ͷ��� Ÿ���� ���� ��
		// ������ �κи��� ���ڿ� �и��� ����Ʈ ����
		lit_table[i].addr = locctr;
		strcpy(lit_input, lit_table[i].literal);
		lit_type = lit_input[0];
		lit_data = strtok(lit_input, "'");
		lit_data = strtok(NULL, "'");

		// ���ͷ� Ÿ���� C(char)�� ���, ������ ���̸�ŭ locctr �� ����
		if (lit_type == 'C')
		{
			locctr += strlen(lit_data);
		}
		else if (lit_type == 'X') // X(16���� ������)�� ���, locctr �� 1 ����
		{
			locctr++;
		}
	}
	// ������� ����� �ε������� ��� �ּҸ� ���������Ƿ� lit_index �� ���� �ε��� ������ ����
	lit_index = lit_num;
}

// short ���� �ּ� ���� �迭 ���·� �ٲٴ� �Լ�
void address_to_array(short address, char * arr, int arr_num)
{
	//short ���� �ּ� ���� ���ڿ� ���·� �ӽ� ��Ƶδ� �迭
	char temp[9];

	// �ּ� ���� ���̳ʽ��� ���
	if (address < 0)
	{
		//16������ temp�� ���ڿ� ��� �� �ʿ��� ��ŭ�� arr �迭�� ��´�
		// FFFFFA -> FFA(3�ڸ��� �ʿ��� ���)
		sprintf(temp, "%0X", address);
		for (int i = 0; i < arr_num+1; i++)
		{
			arr[i] = temp[strlen(temp) - arr_num + i];
		}
	}
	else // �ּҰ��� 0 �̻��� ��� �ʿ��� �ڸ��� ��ŭ arr�� ���ڿ� ���
	{
		if(arr_num == 3)
			sprintf(arr, "%03X", address);
		else if(arr_num == 5)
			sprintf(arr, "%05X", address);
	}
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
	// target_addr: ���� ��ɾ��� �ǿ����� �ּ�
	// obj_index: ������Ʈ �ڵ� ���̺��� �ε����� ǥ���ϴ� ����/ op_index: inst_table���� �ε����� ǥ���ϴ� ����
	// opcode: �ش� ��ɾ��� 16������ ǥ���� OPCODE�� ��� ���� 
	// r1, r2: �������� ������ ��� �ǿ����ڷ� ���� ���������� ��ȣ�� ������� ����
	// xbpe: xbpe�� ���� 10���� ���� ���� �����ϴ� ����
	// lit_cnt: ���ͷ� ī���� / ref_cnt: ���� �ɺ� ī����
	// operand: �ǿ����ڸ� �ӽ÷� �����ϴ� ���ڿ� / output_addr: ����� �ּҰ��� ��� ���ڿ�
	// liter: ���ͷ� �����͸� �ӽ÷� �����ϱ� ���� ���ڿ� / ptr: ���ͷ��� ������ �κи��� �����ϱ� ���� ������
	// op_sign: �ǿ����ڷ� ���� ���� ��� ����ϴ� �����ڸ� ��� ���� ���� ����
	short target_addr;
	int obj_index = 0, op_index;
	int opcode, r1, r2, xbpe;
	int lit_cnt = 0, ref_cnt;
	char operand[255], output_addr[9], liter[20], *ptr;
	char op_sign;

	// ���ͷ� ���̺� �ε���, ���α׷� ����, locctr �ʱ�ȭ
	lit_index = 0;
	sub_prog_num = 0;
	locctr = 0;

	// ��ū���� �ϳ��� ������Ʈ �ڵ带 ������
	for (int i = 1; i < token_line; i++)
	{
		// ������Ʈ �ڵ带 ����� ��, �μ� �������� �ʱ�ȭ
		// inst_table ���� ������ �ε��� �� ����
		ptr = NULL;
		r1 = r2 = 0;
		xbpe = 0;
		target_addr = 0;
		op_index = search_opcode(token_table[i]->operator);

		// �����ڰ� CSECT ���þ��� ���,
		// pc�� 0���� �ʱ�ȭ�ϰ� ���α׷� ���� ���� 1 �ø�(���� ���α׷�)
		if (!strcmp(token_table[i]->operator,"CSECT"))
		{
			program_cnt = 0;
			sub_prog_num++;
		}
		// �����ڰ� EXTREF ���þ��� ���,
		// ���� �ɺ��� ��� extref �迭�� ���� �ɺ� �̸��� ����
		else if (!strcmp(token_table[i]->operator,"EXTREF"))
		{
			for (ref_cnt = 0; token_table[i]->operand[ref_cnt] != NULL; ref_cnt++)
				strcpy(extref[ref_cnt], token_table[i]->operand[ref_cnt]);
		}
		// ��ɾ��� ���,
		if (op_index >= 0)
		{
			// ������ PC�� locctr�� ����
			locctr = program_cnt;

			// ��ɾ��� �ǿ����ڷ� �����ɺ��� ���� ���
			// �Ŀ� Modification record �ۼ��� ����
			// modif_table�� ������ ������ �ּҿ� ���α׷� ���а�,
			// ��ɾ� �����ּҷκ��� ���ľ��� �κ��� ��ŭ ������ �ִ��� ����
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
			
			// ���� ��ɾ� �ּ�(PC) ���� ���ϱ� ����
			// ���� ����� �޸𸮸� �����ϴ� ��ɾ ã�� pc ���� ������Ŵ
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
			// ��ɾ��� 16���� ������ OPCODE�� opcode�� ����
			opcode = strtol(inst_table[op_index]->opcode, NULL, 16);

			//2������ �����ϴ� �������� �������� ���
			if (inst_table[op_index]->form == 2)
			{
				// �ش� ��ɾ 1���� �ǿ����ڸ� �����ϴ� ���
				if (inst_table[op_index]->oprnd_num == 1)
				{
					// ù��° �ǿ������� �������Ϳ� ������ ���� �ش� �������� ��ȣ�� r1�� ����
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
				// �ش� ��ɾ 2���� �ǿ����ڸ� �����ϴ� ���
				else if (inst_table[op_index]->oprnd_num == 2)
				{
					// ù��° �ǿ������� �������Ϳ� ������ ���� �ش� �������� ��ȣ�� r1�� ����
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
					// �ι�° �ǿ������� �������Ϳ� ������ ���� �ش� �������� ��ȣ�� r2�� ����
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
				// opcode, �������� ��ȣ���� �����Ͽ� ������Ʈ �ڵ�� �迭�� ����
				sprintf(object_codes[obj_index], "%02X%d%d", opcode, r1, r2);
				obj_index++;
			}
			else // 2������ �����ϴ� ��ɾ �ƴ� ���
			{
				// �����ڰ� ���� �� �ִ� �ǿ����� ������ 1���� ���
				if (inst_table[op_index]->oprnd_num == 1)
				{
					// �ǿ����� �ӽ� ����
					strcpy(operand, token_table[i]->operand[0]);
					// �ǿ����� ù��° ���ڰ� #(immediate addressing)�� ���
					// #�� ������ ���ڰ��� Ÿ�� �ּҷ� ����
					if (operand[0] == '#')
					{
						ptr = strtok(operand, "#");
						target_addr = atoi(ptr);
					}
					else // �� ���� ���
					{
						// ���� �ɺ��� �ǿ����ڷ� ������ Ȯ��
						int j;
						for (j = 0; j < ref_cnt; j++)
						{
							if (extref[j] != NULL && !strcmp(operand, extref[j]))
								break;
						}
						// ���� �ɺ��� �ǿ����ڷ� ���� ��� Ÿ�� �ּҸ� 0���� ����
						if (j < ref_cnt)
						{
							target_addr = 0;
						}
						// �ǿ����ڰ� �ɺ��� ���, �ɺ� ���̺� ����� �ּҸ� Ÿ�� �ּҷ� ����
						else if (search_symbol_address(operand) >= 0)
						{
							target_addr = search_symbol_address(operand);
						}
						// �ǿ����ڰ� ���ͷ��� ���, ���ͷ� ���̺� ����� �ּҸ� Ÿ�� �ּҷ� ����
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
						// �� ���� ��� �ּ��� ���� ��� 0���� ����
						else
							target_addr = 0;
					}

					// ��ū�� nixbpe �� n�� 1�� ���, ����� opcode �κп� 2 ����
					if ((token_table[i]->nixbpe & N_NIXBPE) == N_NIXBPE)
					{
						opcode += 2;
					}
					// ��ū�� nixbpe �� i�� 1�� ���, ����� opcode �κп� 1 ����
					if ((token_table[i]->nixbpe & I_NIXBPE) == I_NIXBPE)
					{
						opcode++;
					}
					// ��ū�� nixbpe �� x�� 1�� ���, ����� xbpe �κп� 8 ����
					if ((token_table[i]->nixbpe & X_NIXBPE) == X_NIXBPE)
					{
						xbpe += 8;
					}
					// ��ū�� nixbpe �� b�� 1�� ���, ����� xbpe �κп� 4 ����
					if ((token_table[i]->nixbpe & B_NIXBPE) == B_NIXBPE)
					{
						xbpe += 4;
					}
					// ��ū�� nixbpe �� p�� 1�� ���, ����� xbpe �κп� 2 ����
					// PC relative�̹Ƿ� Ÿ�� �ּҿ��� PC �ּ� ���� ��
					if ((token_table[i]->nixbpe & P_NIXBPE) == P_NIXBPE)
					{
						xbpe += 2;
						target_addr -= program_cnt;
					}
					// ��ū�� nixbpe �� e�� 1�� ���, ����� xbpe �κп� 1 ����
					// �ּ�ǥ�� ��Ʈ ���� �þ 4�����̹Ƿ� �ּ� ǥ�� �ڸ����� 5�� �Ͽ� �ּҰ��� �迭�� ǥ����
					if ((token_table[i]->nixbpe & E_NIXBPE) == E_NIXBPE)
					{
						xbpe += 1;
						address_to_array(target_addr, output_addr, 5);
					}
					// 3�����̹Ƿ� �ּ� ǥ�� �ڸ����� 3���� �Ͽ� �ּҰ��� �迭�� ǥ����
					else
						address_to_array(target_addr, output_addr, 3);
				}
				else // �����ڰ� ���� �� �ִ� �ǿ����� ������ 1���� �ƴ� ���(0��)
				{
					// nixbpe �� n,i,x�� ǥ�õǾ��ִ� ��� ���� ���� opcode�� xbpe�� ������ ����
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
				
				// ������Ʈ �ڵ带 �迭�� �־���
				sprintf(object_codes[obj_index], "%02X%X%s", opcode, xbpe, output_addr);
				obj_index++;
			}
		}
		// ���� �����ڰ� LTORG�� END�� ��� ���ͷ��� ���� ������Ʈ �ڵ� ����
		else if (!strcmp(token_table[i]->operator, "LTORG") || !strcmp(token_table[i]->operator, "END"))
		{
			// ������ ������Ʈ �ڵ�� �ۼ��� ���ͷ� ���ĺ���
			// ������� ���� ���ͷ� ����ŭ ������Ʈ �ڵ� ����
			// ������Ʈ �ڵ�� ���ͷ� �̸����� ����� ���ͷ� �����͸� 16������ ǥ���Ͽ� ����
			for (int j = lit_index; j < lit_index + lit_cnt; j++)
			{
				strcpy(liter, lit_table[j].literal);
				ptr = strtok(liter, "'");

				// ���ͷ� �����Ͱ� C(char) Ÿ���� ���
				if (!strcmp(ptr, "C"))
				{
					ptr = strtok(NULL, "'");

					for (int k = 0; ptr[k] != NULL; k++)
					{
						sprintf(object_codes[obj_index], "%s%02X", object_codes[obj_index], ptr[k]);
					}
					obj_index++;
				}
				// ���ͷ� �����Ͱ� X(16���� ��) Ÿ���� ���
				else if (!strcmp(ptr, "X"))
				{
					ptr = strtok(NULL, "'");
					sprintf(object_codes[obj_index], "%s", ptr);
					obj_index++;
				}
			}
			//������� ������Ʈ �ڵ�� ���� lit_index�� ������
			lit_index += lit_cnt;
			lit_cnt = 0;
		}
		// �����ڰ� "BYTE"�� "WORD"���
		else if (!strcmp(token_table[i]->operator, "BYTE") || !strcmp(token_table[i]->operator, "WORD"))
		{
			locctr = program_cnt;
			
			// �ǿ������� �ּҰ����� �ɺ����� ǥ�������� ���� ���,
			// �ɺ��� �ּҰ��� ���� �� ���� �����
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

			//PC ���� ����
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

			// ��� �ǿ������� �����Ͱ����� ������ 16������ ��ȯ�Ͽ� ������Ʈ �ڵ� ����
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
	// file: ������ ���� ���� ���� ������
	// char_len: ���Ͽ� ���� ������Ʈ �ڵ��� ����Ʈ ���� ���� ����
	// ref_cnt: ���α׷� ��ܿ� �����ϴ� �Ǵ� ���� �޴� �ɺ��� ��
	// op_index: �ش� ��ɾ��� �ε���
	// wr_code_cnt: �� ���ο� ���� ������Ʈ �ڵ��� ��
	// lit_cnt: �� ���α׷� �󿡼� ���� ���ͷ��� ��
	// output: �� ���ξ� ����� ���ڿ��� ����
	// isEnd: END ���þ ���� ������ �������� ǥ��
	// isFile: ���Ͽ� ����ϴ� ������ ǥ�� ������� ǥ��
	FILE * file;
	int char_len = 0, ref_cnt, op_index, obj_index = 0, wr_code_cnt, lit_cnt = 0;
	char output[1024] = { 0 };
	_Bool isEnd = 0, isFile = 1; 

	// ���ڷ� ���� ���� �̸����� ��������� ������ ����

	if (file_name == NULL)
		isFile = 0;

	if (isFile)
	{
		file = fopen(file_name, "w");

		if (file == NULL)
			return;
	}

	// �ΰ������� ���� ���� ������ �ʱ�ȭ
	sub_prog_num = 0;
	locctr = 0;
	lit_index = 0;

	// ��ū ���̺��� �ϳ��� �о�� �ۼ���
	for (int i = 0; i < token_line; i++)
	{
		// START ���þ��� ���
		if (!strcmp(token_table[i]->operator, "START"))
		{
			// Header record�� ���ڿ� output�� �ۼ�
			sprintf(output, "H%s\t%06s%06X", token_table[i]->label, token_table[i]->operand[0], end_addr[sub_prog_num]);
		}
		// CSECT ���þ��� ���
		// ���ο� ���� ���α׷��� ������ ǥ���ϴ� ���̹Ƿ�
		// ���� ���α׷��� modification record�� end record�� ����� ��
		// ���ο� ���� ���α׷��� header record ���
		else if (!strcmp(token_table[i]->operator, "CSECT"))
		{
			// ���� ���α׷��� modirication record ���
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

			// ���� ���α׷��� ù��° ���α׷��̾��ٸ� end rocord�� �Բ� ��ɾ� ���� �ּҵ� ���� ���
			if (sub_prog_num == 0)
			{
				if(isFile)
					fprintf(file, "E%06s\n\n", token_table[0]->operand[0]);
				else
					printf("E%06s\n\n", token_table[0]->operand[0]);
			}
			else // �׿� ���α׷��̶�� end record�� ���
			{
				if (isFile)
					fprintf(file, "E\n\n");
				else
					printf("E\n\n");
			}

			// ���ο� ���α׷��� ���۵����Ƿ� locctr�� �ʱ�ȭ �ϰ� ���α׷� ���� ������ 1 �ø�
			locctr = 0;
			sub_prog_num++;
			// �� ���α׷��� header record�� output�� �ۼ�
			sprintf(output, "H%s\t%06X%06X", token_table[i]->label, 0, end_addr[sub_prog_num]);
		}
		// EXTDEF ���þ��� ���
		else if (!strcmp(token_table[i]->operator, "EXTDEF"))
		{
			// define record output�� �ۼ�
			ref_cnt = 0;
			sprintf(output, "D%s%06X", token_table[i]->operand[0], search_symbol_address(token_table[i]->operand[0]));
			ref_cnt++;

			for (int j = 1; token_table[i]->operand[j] != NULL && j < 3; j++)
			{
				sprintf(output, "%s%s%06X", output, token_table[i]->operand[j], search_symbol_address(token_table[i]->operand[j]));
				ref_cnt++;
			}
		}
		//EXTREF ���þ��� ���
		else if (!strcmp(token_table[i]->operator, "EXTREF"))
		{
			// refer record output�� �ۼ�
			strcpy(extref[0], token_table[i]->operand[0]);
			sprintf(output, "R%s", token_table[i]->operand[0]);

			for (ref_cnt = 1; token_table[i]->operand[ref_cnt] != NULL; ref_cnt++)
			{
				strcpy(extref[ref_cnt], token_table[i]->operand[ref_cnt]);
				sprintf(output, "%s%s", output, token_table[i]->operand[ref_cnt]);
			}
		}
		// END ���þ��� ��� ���α׷��� �������� ǥ��
		else if (!strcmp(token_table[i]->operator, "END"))
		{
			isEnd = 1;
		}
		// ��ɾ��� ���
		else if ((op_index = search_opcode(token_table[i]->operator)) >= 0)
		{
			wr_code_cnt = 0;
			char_len = 0;

			// �� ���ο� �� ������Ʈ �ڵ��� ����Ʈ ���� ������Ʈ �ڵ��� ���� ��
			for (int j = 0; j + i < token_line; j++)
			{
				// �޸𸮻󿡼� ������ �����ϴ� ��ɾ��� ���(������Ʈ �ڵ�� ��ȯ�� ��ɾ��� ���)�� ���� ��
				if ((op_index = search_opcode(token_table[j + i]->operator)) >= 0
					|| !strcmp(token_table[j + i]->operator, "BYTE") || !strcmp(token_table[j + i]->operator, "WORD"))
				{
					char_len += strlen(object_codes[j + obj_index]) / 2;
					wr_code_cnt++;
					
					// ���� �ش� ��ɾ ���ͷ��� ���ԵǾ� �ִٸ� ���ͷ��� ���� ��
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
				// Ȥ�� END ���þ ���� ��� ���α׷� ���Ḧ ǥ���� �� ������� ���� ���� ���ͷ��� �����Ͽ� ������ ��
				else if (!strcmp(token_table[j + i]->operator, "END"))
				{
					isEnd = 1;
					for (int k = 0; k < lit_num - lit_index; k++)
					{
						char_len += strlen(object_codes[j+ k + obj_index]) / 2;
						wr_code_cnt++;
					}
				}
				else // �� �̿��� �ڵ带 ������ ���̻� ���� ���� ����
					break;

				// ����� ����Ʈ�� ���� 30�� �Ѿ ��� 30�� �ѱ� �� ������Ʈ�ڵ������ ���� ����
				if (char_len >= 30)
				{
					char_len -= strlen(object_codes[j + obj_index]) / 2;
					wr_code_cnt--;
					break;
				}
			}

			// text record ǥ�ÿ� ù��° ��ɾ� �ּ�, ������ �� ������Ʈ �ڵ��� ����Ʈ ���� output�� ǥ��
			sprintf(output, "T%06X%02X", locctr, char_len);

			// �� ������Ʈ �ڵ� ����ŭ output�� ����
			for (int j = 0; j < wr_code_cnt; j++)
			{
				sprintf(output, "%s%s", output, object_codes[j + obj_index]);
				increase_locctr(token_table[i + j]);
			}

			// �ۼ��� �� ��ŭ ��ū ���� ���� i�� obj_index�� ���� �ø�
			i += wr_code_cnt - 1;
			obj_index += wr_code_cnt;
		}
		// LTORG ���þ��� ���
		else if (!strcmp(token_table[i]->operator, "LTORG"))
		{
			wr_code_cnt = 0;
			char_len = 0;

			// �ش� ��ɾ�� ��õ� ���ͷ� ����ŭ ����� ����Ʈ ���� ��
			for (int j = 0; j < lit_cnt; j++)
			{
				char_len += strlen(object_codes[j + obj_index]) / 2;
			}

			// ���� ���ͷ��� �ƴ� �޸𸮸� �����ϴ� ��ɾ ���ü��� �����Ƿ�
			// ���� ������ ������� ������Ʈ �ڵ��� ����Ʈ ���� ����� ������Ʈ �ڵ� ���� ��
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

			// text record output�� �ۼ�
			sprintf(output, "T%06X%02X", locctr, char_len);

			// ���ͷ� ������Ʈ �ڵ� output�� ���
			for (int j = 0; j < lit_cnt; j++)
			{
				sprintf(output, "%s%s", output, object_codes[j + obj_index]);
				locctr += strlen(object_codes[j + obj_index]) / 2;
			}
			lit_index += lit_cnt;
			obj_index += lit_cnt;
			lit_cnt = 0;
		}
		// RESW�� RESB ���þ��� ��� locctr ���� ������Ŵ
		else if (!strcmp(token_table[i]->operator, "RESW") || !strcmp(token_table[i]->operator, "RESB"))
		{
			increase_locctr(token_table[i]);
			continue;
		}
		else
			continue;

		//�������� ����ϱ� ���� �ۼ��ߴ� ���ڿ� output�� ���Ͽ� ���
		if(isFile)
			fprintf(file, "%s\n", output);
		else
			printf("%s\n", output);

		// ���α׷��� ����Ǵ� ���
		// ������ ���α׷��� modification record�� ���
		// end record ���
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