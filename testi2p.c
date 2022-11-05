#include "peepcc.h"
#include "infToPstf.h"

void test()
{
    PeepCompiler compiler;
    initCompiler(&compiler);
    char infix[50] = "( 6 + 2 ) * 5 - 8 / 4 \0";
    char postfix[50];
    infixToPostfix(infix, postfix);
    printf("Infix given: %s \n", infix);
    printf("Postfix: %s \n", postfix);
    int result = evaluatePostfixExpression(&compiler, postfix);
    printf("Evaluates to the address: %04xH", result);
}
