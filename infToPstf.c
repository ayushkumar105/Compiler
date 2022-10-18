#include <stdlib.h>
#include <stdio.h>
#include "infToPstf.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*
***********************************
* Function declarations
***********************************
*/
void convertToPostfix(char infix[], char postfix[]);

int isOperator(char c);

int precedence(char operator1, char operator2);

Stack_t* newStack();

bool push(Stack_t *s, char value);

char pop(Stack_t *s);

char stackTop(Stack_t *s);

int isEmpty(Stack_t *s);

void printStack(StackNodePtr topPtr);

int evaluatePostfixExpression(char *expr);

int calculate(int op1, int op2, char operator);

/*
*************************************
* Function Implementations
*************************************
*/
Stack_t* newStack()
{
	Stack_t *s = malloc(sizeof(Stack_t));
	if (s == NULL)
	{
		return NULL;
	}
	s->top = NULL;
	s->size = 0;
	return s;
}

bool push(Stack_t *s, char value)
{
	if (s == NULL)
	{
		return false;
	}
	StackNodePtr newTop = malloc(sizeof(StackNode));
	if (newTop == NULL)
	{
		return false;
	}
	newTop->data = value;
	newTop->nextPtr = s->top;
	s->top = newTop;
	s->size += 1;
	return true;
}

char pop(Stack_t *s)
{
	if (s == NULL)
	{
		return '\0';
	}
	StackNodePtr oldTop;
	oldTop = s->top;
	s->top = s->top->nextPtr;
	s->size -= 1;
	char val = oldTop->data;
	free(oldTop);
	return val;
}

char stackTop(Stack_t *s)
{
	if (s == NULL)
	{
		return '\0';
	}
	StackNodePtr t;
	t = s->top;
	return t->data;
}

int isEmpty(Stack_t* s)
{
	if (s->size == 0)
	{
		return 1;
	}
	return 0;
}

void printStack(StackNodePtr topPtr)
{
	StackNode *current_node = topPtr;
	printf("%c ", '[');
   	while ( current_node != NULL) {
        printf("%c ", current_node->data);
        current_node = current_node->nextPtr;
    }
	printf("%c", ']');
	printf("\n");
}

int isOperator(char c)
{
	if (c == '/' || c == '*' || c == '+' || c == '-')
	{
		return 1;
	}
	return 0;
}

int precedence(char operator1, char operator2)
{
	if (operator1 == '*')
	{
		if(operator2 == '*')
		{
			return 0;
		}
		return 1;
	}
	else if (operator1 == '/')
	{
		if(operator2 == '*')
		{
			return -1;
		}
		else if(operator2 == '/')
		{
			return 0;
		}
		return 1;
	}
	else if (operator1 == '+')
	{
		if(operator2 == '*' || operator2 == '/')
		{
			return -1;
		}
		else if(operator2 == '+')
		{
			return 0;
		}
		return 1;
	}
	else if(operator1 == '-')
	{
		if(operator2 == '*' || operator2 == '/' || operator2 == '+')
		{
			return -1;
		}
		else if(operator2 == '-')
		{
			return 0;
		}
	}
	else
	{
		printf("%s", "Not an operator");
		return 0;
	}
}

int calculate(int op1, int op2, char operator)
{
	if (operator == '+')
	{
		return op1 + op2;
	}
	else if(operator == '-')
	{
		return op1 - op2;
	}
	else if(operator == '/')
	{
		return op1 / op2;
	}
	else if(operator == '*')
	{
		return op1 * op2;
	}
	return 0;
}

void infixToPostfix(char infix_exp[], char postfix_exp[])
{
	int i, j;
	char item;
	char x;
	Stack_t *stack = newStack();
	push(stack, '(');
	strcat(infix_exp,")");

	i=0;
	j=0;
	item=infix_exp[i];

	while(item != '\0')
	{
		if(item == '(')
		{
			push(stack, item);
		}
		else if( isdigit(item) || isalpha(item))
		{
			postfix_exp[j] = item;              /
			j++;
		}
		else if(isOperator(item) == 1)       
		{
			x=pop(stack);
			while(isOperator(x) == 1 && (precedence(x, item)== 1 || precedence(x, item) == 0))
			{
				postfix_exp[j] = x;                
				j++;
				x = pop(stack);                       
			}
			push(stack, x);

			push(stack, item);
		}
		else if(item == ')')       
		{
			x = pop(stack);                   /* pop and keep popping until */
			while(x != '(')                /* '(' encounterd */
			{
				postfix_exp[j] = x;
				j++;
				x = pop(stack);
			}
		}
		else
		{ /* if current symbol is neither operand not '(' nor ')' and nor
			operator */
			printf("\nInvalid infix Expression.\n");        /* the it is illegeal  symbol */
			getchar();
			exit(1);
		}
		i++;


		item = infix_exp[i]; /* go to next symbol of infix expression */
	} /* while loop ends here */

	postfix_exp[j] = '\0'; /* add sentinel else puts() fucntion */
	/* will print entire postfix[] array upto SIZE */

}


int main(int argc, char** argv)
{
	char in[12] = "(6+2)*5-8/4";
	char post[10];
	infixToPostfix(in, post);
	for (int i = 0; i<10; ++i)
	{
		printf("%c ", post[i]);
	}
	printf("\n");
	return 0;
}
