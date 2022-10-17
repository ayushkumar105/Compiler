#include <stdlib.h>
#include <stdio.h>
struct stackNode {
	char data;
	struct stackNode *nextPtr;
};

typedef struct stackNode StackNode;

typedef StackNode *StackNodePtr;

typedef struct {
    StackNodePtr top;  /* Linked list of elements */
  /*
      You will need to add more fields to this structure
      to efficiently implement q_size and q_insert_tail
    */
	int size;
} Stack_t;
