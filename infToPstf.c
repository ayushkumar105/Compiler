#include <stdlib.h>
#include <stdio.h>
#include "infToPstf.h"
#include <stdbool.h>

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
int main(int argc, char** argv)
{
	
	return 0;
}
