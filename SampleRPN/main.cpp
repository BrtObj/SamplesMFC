#include "string.h"
#include "stdlib.h"
#include "stdio.h"

/*
������Ф����Ϥ�һ���g���ʤΤ���ݩ`����ӛ�����ä��Q���Ƥ��ޤ����ȤǤ���
��ͨ������ʽ���Q����ȴΤΤ褦�ˤʤ�ޤ���

1+2 �� 1 2 +
1+3*4+5  ��  1 3 4 * + 5 +
1+(2+3)*4 �� 1 2 3 + 4 * +

�Ȥ���ʸФ��Ǥ���

Ӌ����˷��������ӤΈ��Ϥˣ���ǰ�ȣ���ǰ�΂���Ӌ�㤷�ޤ���
������ "1 2 3 + *" (1*(2+3))��ʽ��Ӌ����혤�

1 2 3 + *     +��ǰ��2��3���㤷��2�β��֤�Ӌ��Y�������ޤ�
1 5 *         *��ǰ��1��5��줱��1�β��֤�Ӌ��Y�������ޤ�
5             Ӌ��δ�

�Ȥ����Ф��ǅg����Ӌ�㤬�Ǥ��ޤ���

��ݩ`����ӛ���Ȥ����褦�˥ݩ`����ӛ���⤢��櫓�Ǥ��������ä��������Ӥ�
�Ȥ�����ӛ�����ä���˼���ޤ���Ӌ�㤵���褦�Ȥ�����Ϥ���ݩ`����ӛ���η���
�S�ʤΤ�ʹ���ޤ���

�����ޤǤ���ݩ`����ӛ���λ��A�Ǥ���

���`�����`�ɤϴ󤭤��֤��ƣ��Ĥβ��֤ǘ��ɤ���Ƥ��ޤ���
ǰ�벿�֤���ͨ������ʽ����ݩ`����ӛ���ˉ�Q���벿�֣�
��뤬��ݩ`����ӛ����Ӌ�㤹�벿�֤Ǥ���

��Q���֤Ǥ�+,-,/,*,(,)�򥵥ݩ`�Ȥ��Ƥ���ޤ���
Ӌ��ζ����ϡ��ɤ��줿�����Ф����^��"("��׷�Ӥ��������")"��׷�Ӥ��Ƥ��ޤ���

Ӌ��β��֤Ǥυg����Ӌ�㤷�Ƥ�������Ǥ���
*/

///////////////////////////////////////////////////////////////////////////////
//���`�����`��
//���L98���֤� 0123456789.+-*/() �Ǖ�������ʽ��Ӌ�㤷�ޤ���
double CalcString(char str[100])
{
	char varstack[100]; //�������äΥ����å�
	char porstr[100][20]; //��ݩ`����ӛ�����У������������С�����1���֣�
	char numstr[20]; //����

	int i;
	int k = 0;//stacksize
	int varstacksize = 0;
	int numstrsize = 0;

	int strLen = strlen(str);
	if (strLen == 0)
	{
		printf("�����Ф��դǤ���\n");
		return 0.0;
	}
	if (strLen >= 98)
	{
		printf("�����Ф��L�����ޤ���\n");
		return 0.0;
	}

	//ǰ��� '(', ')' ��׷��
	for (i = strLen; i > 0; i--)
		str[i] = str[i - 1];
	str[0] = '(';
	strLen += 2;
	str[strLen - 1] = ')';
	str[strLen] = '\0';

	for (i = 0; i < strLen; i++)
	{
		switch (str[i])
		{//���������Ф����^����
		case '0': case '1': case '2': case '3': case '4': //�����Έ���
		case '5': case '6': case '7': case '8': case '9': case '.':
			numstr[numstrsize] = str[i];//���������Ф�׷��
			numstrsize++;
			break;
		case '+': //+,-�Έ��Ϥ������ӥ����å���ߤ�����
		case '-':
			if (numstrsize > 0)
			{
				numstr[numstrsize] = '\0';
				strcpy_s(porstr[k], numstr);
				k++;
				numstrsize = 0;
			}
			while (varstacksize > 0)
			{//�����ӥ����å����դǤʤ�����
				if (varstack[varstacksize - 1] == '(')
				{//stack �� top
					break;//�ȥåפ��_�������ΤȤ��ϥ֥쥤��
				}
				//�����ӥ����å�����ݩ`����ӛ����
				porstr[k][0] = varstack[varstacksize - 1];
				porstr[k][1] = '\0';
				k++;
				varstacksize--;//�����ӥ����å�����1��������
			}
			varstack[varstacksize] = str[i];
			varstacksize++;
			break;
		case '*':
		case '/': //*,/�Έ��Ϥ������ӥ����å���׷��
			if (numstrsize > 0)
			{
				//���������Ф�����Хݩ`����ӛ���˼Ӥ���
				numstr[numstrsize] = '\0';
				strcpy_s(porstr[k], numstr);
				k++;
			}
			numstrsize = 0;//���������Фγ��ڻ����դˤ��룩
			if (varstacksize > 0 && (varstack[varstacksize - 1] == '*' || varstack[varstacksize - 1] == '/'))
			{
				//�����ӥ����å�����ݩ`����ӛ����
				porstr[k][0] = varstack[varstacksize - 1];
				porstr[k][1] = '\0';
				k++;
				varstacksize--;//�����ӥ����å�����1��������
			}
			varstack[varstacksize] = str[i];
			varstacksize++;
			break;
		case '(':
			varstack[varstacksize] = str[i];//�����ӥ����å���
			varstacksize++;
			break;
		case ')':
			if (numstrsize > 0)
			{
				//���������Ф�����Хݩ`����ӛ���˼Ӥ���
				numstr[numstrsize] = '\0';
				strcpy_s(porstr[k], numstr);
				k++;
			}
			numstrsize = 0;//���������Фγ��ڻ����դˤ��룩
			while (varstacksize > 0)
			{//�����ӥ����å����դǤʤ��g
				if (varstack[varstacksize - 1] == '(')
				{
					varstacksize--;//�_�������Εr�ϥ����å����������ƥ֥쥤��
					break;
				}
				porstr[k][0] = varstack[varstacksize - 1];//�ݩ`����ӛ�������Ф�
				porstr[k][1] = '\0';
				k++;
				varstacksize--;//�����ӥ����å�����1��������
			}
			break;
		}
	}

	//  ��ݩ`����ӛ����Ӌ��
	double numstack[100];
	int numstacksize = 0;

	for (i = 0; i < k; i++)
	{
		if (porstr[i][0] >= '0' && porstr[i][0] <= '9')
		{
			numstack[numstacksize] = atof(porstr[i]);
			numstacksize++;
		}
		else
		{
			if (numstacksize < 2)
			{
				printf("����`�Ǥ���\n");
				return 0.0;
			}
			double ope1, ope2;
			ope1 = numstack[numstacksize - 2];
			ope2 = numstack[numstacksize - 1];
			switch (porstr[i][0])
			{
			case '-': numstack[numstacksize - 2] = (ope1 - ope2);
				break;
			case '+': numstack[numstacksize - 2] = (ope1 + ope2);
				break;
			case '/': numstack[numstacksize - 2] = (ope1 / ope2);
				break;
			case '*': numstack[numstacksize - 2] = (ope1 * ope2);
				break;
			}
			numstacksize--;
		}
	}
	if (numstacksize != 1)
	{
		printf("����`�Ǥ���\n");
		return 0.0;
	}

	return numstack[0];
}

/// <summary>
/// Mains this instance.
/// </summary>
void main()
{
	char str[100];
	strcpy_s(str, "(1+2)*3");//������ʽ
	printf("%f", CalcString(str));
	getchar();
}
