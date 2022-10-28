#include "infToPstf.h"
struct tableEntry { 
    int symbol;
    char type;      /* 'C', 'L' or 'V' */
    int location;   /* 00 to FF */ 
};

typedef struct tableEntry TableEntry;

struct pCompiler {
    int memsize;
	TableEntry *symTab;         /* the symbol table */
	unsigned short int *hml;    /* the hml instructions */
	int *flag;                  /* the flag array */
	char *file;                 /* name of file to be compiled*/
	int ln;                  /* current line of file to be compiled */
	int inscount;            /* instruction count for the specific peep statement */
	int datacount;           /* data count to keep track for space in the hml memory*/
};

typedef struct pCompiler PeepCompiler;
