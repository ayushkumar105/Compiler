#ifndef PEEPCC_H_
#define PEEPCC_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



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
	int flag[256];                  /* the flag array */
	char *file;                 /* name of file to be compiled*/
	int ln;                     /* current line of file to be compiled */
	int inscount;               /* instruction count for all the hml statements created */
	int datacount;              /* data count to keep track for space in the hml memory*/
	int symSize;				/*Symbol table size tracker*/
};


typedef struct pCompiler PeepCompiler;



void initCompiler(PeepCompiler *compiler);
void firstPass(PeepCompiler *compiler, char file[]);
void secondPass(PeepCompiler *compiler);
void writeToFile(PeepCompiler *compiler, char fileName[]);
bool lookUpSymbol(int symbol, PeepCompiler* comp);
int ifConverter(PeepCompiler *comp, char *var1, char *op, char *var2, int lineNumber);
int letConverter(PeepCompiler *comp, char res, char expr[]);
int printConverter(PeepCompiler *comp, int var);
int gotoConverter(PeepCompiler *comp, int lineNumber);
int endConverter(PeepCompiler *compiler);
int inputConverter(PeepCompiler *comp, int var);
void printMem(unsigned short int mem[256]);
void printSymTab(TableEntry* tab, int size);
#endif
