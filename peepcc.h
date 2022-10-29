
#include "infToPstf.h"
struct tableEntry { 
    int symbol;
    char type;      /* 'C', 'L' or 'V' */
    int location;   /* 00 to FF */ 
};
bool lookUpSymbol(int symbol, TableEntry *table);
TableEntry getSymbol(int symbol, TableEntry *table);
typedef struct tableEntry TableEntry;

struct pCompiler {
    int memsize;
	TableEntry *symTab;         /* the symbol table */
	unsigned short int *hml;    /* the hml instructions */
	int *flag;                  /* the flag array */
	char *file;                 /* name of file to be compiled*/
	int ln;                     /* current line of file to be compiled */
	int inscount;               /* instruction count for all the hml statements created */
	int datacount = 255;              /* data count to keep track for space in the hml memory*/
	int symSize;				/*Symbol table size tracker*/
};

typedef struct pCompiler PeepCompiler;
