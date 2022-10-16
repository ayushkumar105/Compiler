import <stdlib.h>
import <stdio.h>
import "infToPstf.h"

/*
***********************************
* Function declarations
***********************************
*/
void convertToPostfix(char infix[], char postfix[]);

int isOperator(char c);

int precedence(char operator1, char operator2);

void newStack();

bool push(StackNodePtr *topPtr, char value);

char pop(StackNodePtr *topPtr);

char stackTop(StackNodePtr topPtr);

int isEmpty(StackNodePtr topPtr);

void printStack(StackNodePtr topPtr);

int evaluatePostfixExpression(char *expr);

int calculate(int op1, int op2, char operator);

/*
*************************************
* Function Implementations
*************************************
*/
void newStack()
{
	StackNode *s = malloc(sizeof(StackNode));
	if (s == NULL)
	{
		return NULL;
	}
	s->data = '';
	s->size = 0;
	s->nextPtr = NULL;
	return s;
}

bool push(StackNodePtr *topPtr, char value)
{
	if (topPtr == NULL)
	{
		return false;
	}
	StackNodePtr *newTop = malloc(sizeof(StackNode));
	if (newTop == NULL)
	{
		return false;
	}
	newTop->data = value;
	newTop->nextPtr = topPtr;
	topPtr = newTop;
	return true;
}

char pop(StackNodePtr *topPtr)
{
	if (topPtr == NULL)
	{
		return '';
	}
	char value = topPtr->data;
	StackNode *p;
	p = topPtr;
	topPtr = topPtr->nextPtr;
	free(p);
	return value;
}

char stackTop(StackNodePtr topPtr)
{
	if (topPtr == NULL)
	{
		return '';
	}
	return topPtr->data;
}

int isEmpty(StackNodePtr topPtr)
{
	if (topPtr->size == 0)
	{
		return 1;
	}
	return 0;
}

void printStack(StackNodePtr topPtr)
{
	StackNode *current_node = topPtr;
   	while ( current_node != NULL) {
        printf("%d ", current_node->data);
        current_node = current_node->nextPtr;
    }
}
