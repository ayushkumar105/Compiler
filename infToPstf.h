import <stdlib.h>
import <stdio.h>
struct stackNode {
	char data;
	struct stackNode *nextPtr;
	int size;
};

typedef struct stackNode StackNode;

typedef StackNode *StackNodePtr;


