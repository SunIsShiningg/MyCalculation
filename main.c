#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 50//������������ ������ ���������

//������� �����
typedef struct node
{
	char data;
	struct node *next;
} Nd;
// �����, ������� ��������� ������� � ����
void Push(Nd **head, char value)
{
	Nd *tmp = malloc(sizeof(Nd));
	tmp->next = *head;
	tmp->data = value;
	*head = tmp;
}
//�������� ���� �� ����
int IsEmpty(Nd *head)
{
	return head == NULL;
}
//�����, ������� ������� ������� �� �����
char Pop(Nd **head)
{
	Nd *tmp;
	char value;
	tmp = *head;
	*head = (*head)->next;
	value = tmp->data;
	free(tmp);
	return value;
}
//����� ��� ����������� ������ � ����������
int WeContinue()
{
	char answer = ' ';
	while (1)
	{
		system("cls");
		printf("��� ���?(y/n)\n");
		answer = getchar();
		if (answer != 'y' && answer != 'n')
		{
			printf("������������  ���!!! ���������� ��� ���(y - �� /n - ���)\n");
			system("pause");
		}
		else
		{
			if (answer == 'n')
				return 1;
			else
			{
				return 0;
			}
		}
	}
	
}
//�������� �������� �� ������� ������ ���������
int IsOperator(char operator)
{
	if (operator == '+' || operator == '-' || operator == '*' || operator == '/' || operator == '_')//'_' - ������, ���������� ������� �����
		return 1;
	else
		return 0;
}
//����� ��� ������������ ������������ ���������
int SearchingErrors(char *expr, char *error)
{
	int err = 0, i,j = 0;
	for (i = 0; expr[i] != '\n'; i++)
	{
		if (expr[i] != ' ' && expr[i] != '.' && expr[i] != ',' &&
			expr[i] != '(' && expr[i] != ')' &&
			IsOperator(expr[i]) == 0 && isdigit(expr[i]) == 0)//���� ������� ������ �������� ������������
		{
			error[j++] = expr[i];//��� ����������
			err = 1;//����, ���������, ��� ��� ������ ������������ ������
		}
	}
	error[j] = '\0';
	return err;
}
//�������� ��������
void DeletingSpaces(char *expr)
{
	char buf[MAX]; int i, j = 0, isSpace = 0;// ����, ����� �������� ��� �� ������
	for (i = 0; expr[i] != '\n'; i++)
	{
		if (expr[i] != ' ')
			buf[j++] = expr[i];
		else
			isSpace = 1;
	}
	buf[j] = '\0';
	if (isSpace != 0)//���� �� ���� �������, �� �� �������� �������
		for (i = 0; buf[i] != '\0'; i++)
			expr[i] = buf[i];
	expr[i] = '\0';
}
//�������� ������
int CheckingBrackets(char *expr)
{
	int counter = 0;
	for (int i = 0; expr[i] != '\n'; i++)
	{
		if (expr[i] == '(')
			counter++;
		if (expr[i] == ')')
			counter--;
	}
	return counter == 0;
}
//�����, ���������� ������ ��������� ��������
int GetingPriority(char operator)
{
	if (operator == '_')//���� ������� ��������
		return 4;
	else
	{
		if (operator == '*' || operator == '/')
			return 3;
		else
		{
			if (operator == '+' || operator == '-')
				return 2;
			else
			if (operator == '(')
				return 1;
		}
	}
	return 0;
}
//�������� �������� �� ������ ������
int IsConst(char ch)
{
	if (isdigit(ch) != 0 || ch == '.' || ch == ',')
		return 1;
	else
		return 0;
}
//�������� �������� �� �������� �������
int IsUnary(char ch)
{
	return ch == '(' || IsOperator(ch);
	//������ ������� �������� ����� ����� ����������� �������, ����� �������� ��������� ��� � ������ ���������
}
//������� ��������� ��������� � "�������� �������� ������"
void ConvertToRPN(char *expr, char *RPN)
{
	Nd *head = NULL;//������ �����
	int j = 0, mayUnary = 1;//mayUnary-����, ������� �����������, ��� ��������� �������� ����� ���� �������
	for (int i = 0; i < (int)strlen(expr); i++)
	{			
		if (expr[i] == '(')
		{
			Push(&head, '(');
			mayUnary = 1;
		}
		else
		{
			if (expr[i] == ')')
			{
				while (head->data != '(')
					RPN[j++] = Pop(&head);
				Pop(&head);
				mayUnary = 0;
			}
			else
			{
				if (IsConst(expr[i]) != 0)
				{
					while (IsConst(expr[i]) != 0)
						RPN[j++] = expr[i++];
					--i;
					RPN[j++] = ' ';
					mayUnary = 0;
				}
				else
				{
					char currentOperator = expr[i];
					if (mayUnary != 0 && ((i-1) < 0 | IsUnary(expr[i-1]) != 0))//���� �������� - ������� �����
						currentOperator = '_';//�������� �� �������� '_'
					if (IsEmpty(head) != 0 || GetingPriority(head->data) < GetingPriority(currentOperator))
						Push(&head, currentOperator);
					else
					{
						while (head && GetingPriority(head->data) >= GetingPriority(currentOperator))
							RPN[j++] = Pop(&head);
						Push(&head, currentOperator);
						mayUnary = 1;
					}
				}
			}
		}
	}
	while (IsEmpty(head) == 0)
	{
		RPN[j++] = Pop(&head);
	}
	RPN[j] = '\0';
}
//������� �� ����������� ���� � ��� double
double ConvertToDouble(char *ch)
{
	int intNumber = 0, counter = 0;
	double dbNumber = 0.0;
	for (int i = 0; i < (int)strlen(ch); i++)
	{
		if (ch[i] != '.' && ch[i] != ',')
		{
			int digit = (int)ch[i] - (int)'0';
			intNumber = intNumber * 10 + digit;
		}
		else
		{
			i++;
			while (i < (int)strlen(ch))
			{
				int digit = (int)ch[i] - (int)'0';
				dbNumber = dbNumber * 10 + digit;
				counter++;
				i++;
			}
			dbNumber /= pow(10, counter);
		}
	}
	return intNumber + dbNumber;
}
//���������� �������� �������� ������
double Calculation(char *RPN)
{
	char operand[10];//���������� ��� �������� ����� � ���������� ����
	double stack[MAX];// ���� ��� �������� �����
	int top = -1, k;//top - ������� �����
	for (int i = 0; i < (int)strlen(RPN); i++)
	{
		if (IsConst(RPN[i]) != 0)
		{
			k = 0;
			while (IsConst(RPN[i]) != 0)
				operand[k++] = RPN[i++];//������� �� ������ ����� � ���������� ����
			operand[k] = '\0';
			stack[++top] = ConvertToDouble(operand);
		}
		else
		{
			if (IsOperator(RPN[i]) != 0)//���� �������� 
			{
				switch (RPN[i])//��������� �� ��� ���� ��������
				{
				case '+':
					stack[top - 1] += stack[top];
					top--;
					break;
				case '-':
					stack[top - 1] -= stack[top];
					top--;
					break;
				case '*':
					stack[top - 1] *= stack[top];
					top--;
					break;
				case '/':
					stack[top - 1] /= stack[top];
					top--;
					break;
				case '_':
					stack[top] = -stack[top];
					break;
				default:
					break;
				}
			}
		}
	}
	return stack[0];
}
//������� ������ stdin
void CleaningStdin()
{
	int c;
	do
	{
		c = getchar();
	} while (c != '\n' && c != EOF);
}

void main()
{
	setlocale(LC_ALL, "Russian");
	char expression[MAX], wrongExpression[MAX], RPN[MAX];//Reverse Polish Notation(�������� �������� �������(������))
	int exit = 0;//���� ��� ������ �� ���������
	while (exit == 0)
	{
		system("cls");
		printf("\t\t\t\t******�����������******\n");
		printf("������� �������������� ���������...\n");
		CleaningStdin();
		fgets(expression, MAX, stdin);
		if (expression[0] == '\n')
		{
			printf("������������ ����, �� �� ����� �������������� ���������!!!\n��� ����������� ������� ����� �������...\n");
			getchar();
		}
		else
		{
			if (SearchingErrors(expression, wrongExpression) != 0)
			{
				printf("������������ ����, ������ �������� ������������ ��������� \"%s\"!!!\n��� ����������� ������� ����� �������...\n", wrongExpression);
				getchar();
			}
			else
			{
				if (CheckingBrackets(expression) == 0)
				{
					printf("������������ ����, ���-�� ��������� �� ��������!!!\n��� ����������� ������� ����� �������...\n");
					getchar();
				}
				else
				{
					DeletingSpaces(expression);
					ConvertToRPN(expression, RPN);
					printf("���������: %.2f\n��� ����������� ������� ����� �������...\n", Calculation(RPN));
					getchar();
				}
			}
		}
		exit = WeContinue();
	}
	getchar();
}