#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 50//ìàêñèìàëüíûé ðàçìåð âûðàæåíèé

//ýëåìåíò ñòåêà
typedef struct node
{
	char data;
	struct node *next;
} Nd;
// ìåòîä, êîòîðûé äîáàâëÿåò ýëåìåíò â ñòåê
void Push(Nd **head, char value)
{
	Nd *tmp = malloc(sizeof(Nd));
	tmp->next = *head;
	tmp->data = value;
	*head = tmp;
}
//ïðîâåðêà ïóñò ëè ñòåê
int IsEmpty(Nd *head)
{
	return head == NULL;
}
//ìåòîä, êîòîðûé äîñòàåò ýëåìåíò èç ñòåêà
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
//ìåòîä äëÿ ïðîäîëæåíèÿ ðàáîòû ñ ïðîãðàììîé
int WeContinue()
{
	char answer = ' ';
	while (1)
	{
		printf("We continue?(y/n)\n");
		answer = getchar();
		if (answer != 'y' && answer != 'n')
		{
			printf("Wrong input!!! Try again(y - yes /n - no)\nTo continue press any key...\n");
			getchar();
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
//ïðîâåðêà ÿâëÿåòñÿ ëè òåêóùèé ñèìâîë îïåðàöèåé
int IsOperator(char operator)
{
	if (operator == '+' || operator == '-' || operator == '*' || operator == '/' || operator == '_')//'_' - ñèìâîë, çàìåíÿþùèé óíàðíûé ìèíóñ
		return 1;
	else
		return 0;
}
//ìåòîä äëÿ îòëàâëèâàíèÿ íåäîïóñòèìûõ âûðàæåíèé
int SearchingErrors(char *expr, char *error)
{
	int err = 0, i,j = 0;
	for (i = 0; expr[i] != '\n'; i++)
	{
		if (expr[i] != ' ' && expr[i] != '.' && expr[i] != ',' &&
			expr[i] != '(' && expr[i] != ')' &&
			IsOperator(expr[i]) == 0 && isdigit(expr[i]) == 0)//åñëè òåêóùèé ñèìâîë ÿâëÿåòñÿ íåäîïóñòèìûì
		{
			error[j++] = expr[i];//åãî çàïîìèíàåì
			err = 1;//ôëàã, ãîâîðÿùèé, ÷òî áûë íàéäåí íåäîïóñòèìûé ñèìâîë
		}
	}
	error[j] = '\0';
	return err;
}
//óäàëåíèå ïðîáåëîâ
void DeletingSpaces(char *expr)
{
	char buf[MAX]; int i, j = 0, isSpace = 0;// ôëàã, ÷òîáû îòìåòèòü áûë ëè ïðîáåë
	for (i = 0; expr[i] != '\n'; i++)
	{
		if (expr[i] != ' ')
			buf[j++] = expr[i];
		else
			isSpace = 1;
	}
	buf[j] = '\0';
	if (isSpace != 0)//åñëè íå áûëî ïðîáåëà, òî íå êîïèðóåì îáðòàíî
		for (i = 0; buf[i] != '\0'; i++)
			expr[i] = buf[i];
	expr[i] = '\0';
}
//ïðîâåðêà ñêîáîê
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
//ìåòîä, ïîçâîëÿþèé óçíàòü ïðèîðèòåò îïåðàöèè
int GetingPriority(char operator)
{
	if (operator == '_')//åñëè óíàðíûé îïåðàòîð
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
//ïðîâåðêà ÿâëÿåòñÿ ëè ñèìâîë ÷èñëîì
int IsConst(char ch)
{
	if (isdigit(ch) != 0 || ch == '.' || ch == ',')
		return 1;
	else
		return 0;
}
//ïðîâåðêà ÿâëÿåòñÿ ëè îïåðàöèÿ óíàðíîé
int IsUnary(char ch)
{
	return ch == '(' || IsOperator(ch);
	//îáû÷íî óíàðíàÿ îïåðàöèÿ ñòîèò ïåðåä îòêðûâàþùåé ñêîáêîé, ïåðåä áèíàðíîé îïåðàöèåé èëè â íà÷àëå âûðàæåíèÿ
}
//ïåðåâîä èñõîäíîãî âûðàæåíèÿ â "îáðàòíóþ ïîëüñêóþ çàïèñü"
void ConvertToRPN(char *expr, char *RPN)
{
	Nd *head = NULL;//íà÷àëî ñòåêà
	int j = 0, mayUnary = 1;//mayUnary-ôëàã, êîòîðûé óêàçûâàåòñÿ, ÷òî ñëåäóþùàÿ îïåðàöèÿ ìîæåò áûòü óíàðíîé
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
					if (mayUnary != 0 && ((i-1) < 0 | IsUnary(expr[i-1]) != 0))//åñëè îïåðàöèÿ - óíàðíûé ìèíóñ
						currentOperator = '_';//ïîìå÷àåì åå ñèìâîëîì '_'
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
//ïåðåâîä èç ñèìâîëüíîãî òèïà â òèï double
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
//âû÷èñëåíèÿ îáðàòíîé ïîëüñêîé çàïèñè
double Calculation(char *RPN)
{
	char operand[10];//ïåðåìåííàÿ äëÿ õðàíåíèÿ ÷èñåë â ñèìâîëüíîì âèäå
	double stack[MAX];// ñòåê äëÿ õðàíåíèÿ ÷èñåë
	int top = -1, k;//top - âåðøèíà ñòåêà
	for (int i = 0; i < (int)strlen(RPN); i++)
	{
		if (IsConst(RPN[i]) != 0)
		{
			k = 0;
			while (IsConst(RPN[i]) != 0)
				operand[k++] = RPN[i++];//äîñòàåì èç ñòðîêè ÷èñëà â ñèìâîëüíîì âèäå
			operand[k] = '\0';
			stack[++top] = ConvertToDouble(operand);
		}
		else
		{
			if (IsOperator(RPN[i]) != 0)//åñëè îïåðàöèÿ 
			{
				switch (RPN[i])//âûïîëíÿåì òî èëè èíîå äåéñòâèå
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
//î÷èñòêà ïîòîêà stdin
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
	char expression[MAX], wrongExpression[MAX], RPN[MAX];//Reverse Polish Notation(îáðàòíàÿ ïîëüñêàÿ íîòàöèÿ(çàïèñü))
	int exit = 0;//ôëàã äëÿ âûõîäà èç ïðîãðàììû
	while (exit == 0)
	{
		printf("\t\t\t\t******Calculator******\n");
		printf("Please, enter expression...\n");
		CleaningStdin();
		fgets(expression, MAX, stdin);
		if (expression[0] == '\n')
		{
			printf("Wrong input, You did not enter expression!!!\nTo continue press any key...\n");
			getchar();
		}
		else
		{
			if (SearchingErrors(expression, wrongExpression) != 0)
			{
				printf("Wrong input, string contains invalid expression \"%s\"!!!\nTo continue press any key...\n\n", wrongExpression);
				getchar();
			}
			else
			{
				if (CheckingBrackets(expression) == 0)
				{
					printf("Wrong input, Please, check brackets!!!\nTo continue press any key...\n");
					getchar();
				}
				else
				{
					DeletingSpaces(expression);
					ConvertToRPN(expression, RPN);
					printf("Result: %.2f\nTo continue press any key...\n", Calculation(RPN));
					getchar();
				}
			}
		}
		exit = WeContinue();
	}
	getchar();
}
