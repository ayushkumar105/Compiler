#include <stdlib.h>
#include <stdio.h>
#include "peepcc.h"


struct stackNode {
    int data;
    struct stackNode *nextPtr;
};
typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;

typedef struct {
    StackNodePtr top;  /* Linked list of elements */
	int size;
} Stack_t;

static const int MEMSIZE = 256;

bool lookUpSymbol(int symbol, PeepCompiler *comp);
TableEntry getSymbol(int symbol, TableEntry *table);
int evaluatePostfixExpression(PeepCompiler *compiler, char *expr);
void infixToPostfix(char infix_exp[], char postfix_exp[]);
void convertToPostfix(char infix[], char postfix[]);

int isOperator(char c);

int precedence(char operator1, char operator2);

Stack_t* newStack();

bool push(Stack_t *s, int value);

int pop(Stack_t *s);

int stackTop(Stack_t *s);

int isEmpty(Stack_t *s);

void printStack(StackNodePtr topPtr);

int evaluatePostfixExpression(PeepCompiler *compiler, char *expr);

int calculate(int op1, int op2, char operator);
