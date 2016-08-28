#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 50//максимальный размер выражений

//элемент стека
typedef struct node
{
	char data;
	struct node *next;
} Nd;
// метод, который добавляет элемент в стек
void Push(Nd **head, char value)
{
	Nd *tmp = malloc(sizeof(Nd));
	tmp->next = *head;
	tmp->data = value;
	*head = tmp;
}
//проверка пуст ли стек
int IsEmpty(Nd *head)
{
	return head == NULL;
}
//метод, который достает элемент из стека
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
//метод для продолжения работы с программой
int WeContinue()
{
	char answer = ' ';
	while (1)
	{
		system("cls");
		printf("Еще раз?(y/n)\n");
		answer = getchar();
		if (answer != 'y' && answer != 'n')
		{
			printf("Некорректный  вод!!! Попробуйте еще раз(y - да /n - нет)\n");
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
//проверка является ли текущий символ операцией
int IsOperator(char operator)
{
	if (operator == '+' || operator == '-' || operator == '*' || operator == '/' || operator == '_')//'_' - символ, заменяющий унарный минус
		return 1;
	else
		return 0;
}
//метод для отлавливания недопустимых выражений
int SearchingErrors(char *expr, char *error)
{
	int err = 0, i,j = 0;
	for (i = 0; expr[i] != '\n'; i++)
	{
		if (expr[i] != ' ' && expr[i] != '.' && expr[i] != ',' &&
			expr[i] != '(' && expr[i] != ')' &&
			IsOperator(expr[i]) == 0 && isdigit(expr[i]) == 0)//если текущий символ является недопустимым
		{
			error[j++] = expr[i];//его запоминаем
			err = 1;//флаг, говорящий, что был найден недопустимый символ
		}
	}
	error[j] = '\0';
	return err;
}
//удаление пробелов
void DeletingSpaces(char *expr)
{
	char buf[MAX]; int i, j = 0, isSpace = 0;// флаг, чтобы отметить был ли пробел
	for (i = 0; expr[i] != '\n'; i++)
	{
		if (expr[i] != ' ')
			buf[j++] = expr[i];
		else
			isSpace = 1;
	}
	buf[j] = '\0';
	if (isSpace != 0)//если не было пробела, то не копируем обртано
		for (i = 0; buf[i] != '\0'; i++)
			expr[i] = buf[i];
	expr[i] = '\0';
}
//проверка скобок
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
//метод, позволяюий узнать приоритет операции
int GetingPriority(char operator)
{
	if (operator == '_')//если унарный оператор
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
//проверка является ли символ числом
int IsConst(char ch)
{
	if (isdigit(ch) != 0 || ch == '.' || ch == ',')
		return 1;
	else
		return 0;
}
//проверка является ли операция унарной
int IsUnary(char ch)
{
	return ch == '(' || IsOperator(ch);
	//обычно унарная операция стоит перед открывающей скобкой, перед бинарной операцией или в начале выражения
}
//перевод исходного выражения в "обратную польскую запись"
void ConvertToRPN(char *expr, char *RPN)
{
	Nd *head = NULL;//начало стека
	int j = 0, mayUnary = 1;//mayUnary-флаг, который указывается, что следующая операция может быть унарной
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
					if (mayUnary != 0 && ((i-1) < 0 | IsUnary(expr[i-1]) != 0))//если операция - унарный минус
						currentOperator = '_';//помечаем ее символом '_'
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
//перевод из символьного типа в тип double
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
//вычисления обратной польской записи
double Calculation(char *RPN)
{
	char operand[10];//переменная для хранения чисел в символьном виде
	double stack[MAX];// стек для хранения чисел
	int top = -1, k;//top - вершина стека
	for (int i = 0; i < (int)strlen(RPN); i++)
	{
		if (IsConst(RPN[i]) != 0)
		{
			k = 0;
			while (IsConst(RPN[i]) != 0)
				operand[k++] = RPN[i++];//достаем из строки числа в символьном виде
			operand[k] = '\0';
			stack[++top] = ConvertToDouble(operand);
		}
		else
		{
			if (IsOperator(RPN[i]) != 0)//если операция 
			{
				switch (RPN[i])//выполняем то или иное действие
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
//очистка потока stdin
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
	char expression[MAX], wrongExpression[MAX], RPN[MAX];//Reverse Polish Notation(обратная польская нотация(запись))
	int exit = 0;//флаг для выхода из программы
	while (exit == 0)
	{
		system("cls");
		printf("\t\t\t\t******Калькулятор******\n");
		printf("Вводите арифметическое выражение...\n");
		CleaningStdin();
		fgets(expression, MAX, stdin);
		if (expression[0] == '\n')
		{
			printf("Некорректный ввод, вы не ввели арифметическое выражение!!!\nДля продолжения нажмите любую клавишу...\n");
			getchar();
		}
		else
		{
			if (SearchingErrors(expression, wrongExpression) != 0)
			{
				printf("Некорректный ввод, строка содержит недопустимое выражение \"%s\"!!!\nДля продолжения нажмите любую клавишу...\n", wrongExpression);
				getchar();
			}
			else
			{
				if (CheckingBrackets(expression) == 0)
				{
					printf("Некорректный ввод, что-то напутанно со скобками!!!\nДля продолжения нажмите любую клавишу...\n");
					getchar();
				}
				else
				{
					DeletingSpaces(expression);
					ConvertToRPN(expression, RPN);
					printf("Результат: %.2f\nДля продолжения нажмите любую клавишу...\n", Calculation(RPN));
					getchar();
				}
			}
		}
		exit = WeContinue();
	}
	getchar();
}