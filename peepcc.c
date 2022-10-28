#include <stdlib.h>
#include <stdio.h>
#include "infToPstf.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "peep.h"


#define ADD     0x10    // Add a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define SUB     0x11    // Subtract a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define MUL     0x12    // Multiple a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define DIV     0x13    // Divide a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define MOD     0x14    // Compute the integer remainder when dividing a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)

// ACC-MEM Logical Instructions
#define AND     0x20    // Compute the binary AND of a specific memory location and the accumulator (leave the result in the accumulator)
#define ORR     0x21    // Compute the binary OR of a specific memory location and the accumulator (leave the result in the accumulator)
#define NOT     0x22    // Compute the logical NOT of the accumulator (leave the result in the accumulator)
#define XOR     0x23    // Compute the binary XOR of a specific memory location and the accumulator (leave the result in the accumulator)
#define LSR     0x24    // Compute the binary logical shift right (1-bit) of the accumulator (leave the result in the accumulator)
#define ASR     0x25    // Compute the binary arithmetic shift right (1-bit) of the accumulator (leave the result in the accumulator)
#define LSL     0x26    // Compute the binary logical shift left of the accumulator (leave the result in the accumulator)

// Branch Instructions
#define B       0X30    // Branch to a specific location in memory
#define BNEG    0X31    // Branch to a specific location in memory if the accumulator is negative
#define BPOS    0X32    // Branch to a specific location in memory if the accumulator is positive
#define BZRO    0X33    // Branch to a specific location in memory if the accumulator is zero

// ACC-MEM Load/Store Instructions
#define LOAD    0X40    // Load a word from a specific location in memory into the accumulator
#define STOR    0X41    // Store a word from the accumulator into a specific memory location

// I/O (Standard) Instructions
#define READ    0X50    // Read a word from the terminal into a specific location in memory
#define WRTE    0X51    // Write a word from a specific location in memory to the terminal

// HALT Instruction
#define HALT    0XFF    // Halt, i.e. the program has completed its task

const int MEMSIZE = 256;
 

/*
*Method declarations
*/
bool insertIntoSym(TableEntry *table, TableEntry symbol);
bool lookUpSymbol(int symbol, TableEntry *table);
TableEntry getSymbol(int symbol, TableEntry *table);
void printSymTab(TableEntry* tab, int size);
int* ifConverter(char *inst);
int* letConverter(char *inst);
int* printConverter(PeepCompiler *comp, int var);
int* gotoConverter(PeepCompiler *comp, int lineNumber);
int* endConverter(char* inst);
int* inputConverter(PeepCompiler *comp, int var);

/*
*Method Implementations
*/
bool lookUpSymbol(int symbol, TableEntry *table)
{
    for(int i = 0; i<MEMSIZE; ++i)
    {
        if(table[i].symbol == symbol)
        {
            return true;
        }
    }
    return false;
}

TableEntry getSymbol(int symbol, TableEntry *table)
{
    for(int i = 0; i<MEMSIZE; ++i)
    {
        if(table[i].symbol == symbol)
        {
            return table[i];
        }
    }
}


void printSymTab(TableEntry* tab, int size)
{
    printf("Symbol  Table  Location \n");
    for (int i = 0; i < size; ++i)
    {
        printf("  %d       %c      0x%02hX \n", tab[i].symbol, tab[i].type, tab[i].location);
    }
}

int* gotoConverter(PeepCompiler *compiler, int lineNumer)
{
    int result[1];
    if(lookUpSymbol(lineNumer, compiler->symTab))
    {
        result[0] = B * MEMSIZE + getSymbol(lineNumer, compiler->symTab).location;
    }
    else
    {
        compiler->flag[compiler->inscount] = lineNumer;
        result[0] = B * MEMSIZE + 0;
    }
    ++compiler->inscount;
    return result;
}

int* inputConverter(PeepCompiler *compiler, int var)
{
    int* result[1];
    TableEntry symbol;
    if(lookUpSymbol(var, compiler->symTab))
    {
        symbol = getSymbol(var, compiler->symTab);
    }
    else
    {
        
        char type;
        unsigned short int loc = 255 - compiler->datacount;
        if (var >= 97 && var <= 122)
        {
            type = 'V';
        }
        else
        {
            type = 'C';
        }
        /*
        * Creating a new symbol struct AKA TableEntry
        */
        symbol.location = loc;
        symbol.symbol = var;
        symbol.type = type;
        compiler->symTab[compiler->inscount + 1] = symbol; 
    }
    if(MEMSIZE < compiler->datacount)
    {
        fprintf(stderr, "%s", "Memory limit for HML reached!\n");
        return 0;
    }
    result[0] =  READ * MEMSIZE + symbol.location; // add hml instruction to the result array 
    ++compiler->inscount; // increment the instructions count
    ++compiler->datacount; // increment the data count
    return result;
}

int main(int argc, char *argv[]){
    FILE *fp;
    char *filename;
    /*
    * Memory for the Peep compiler.
    */
    char mem[256][256];
    
    // Check if a filename has been specified in the command
    if (argc < 2)
    {
        printf("Missing Filename\n");
        //return(1);
    }
    else
    {
        filename = argv[1];
        printf("Filename : %s\n", filename);
    }

    // Open file in read-only mode
    fp = fopen(filename,"r");

    // If file opened successfully, then print the contents
    if ( fp )
    {
        int i = 0;
        char word[256];
        while ((fgets(word, 256, fp)))
        {
            printf("%s",word);
            strcpy(mem[i], word);
            i++;
        }
        printf("\n");
    }
    else
    {
        printf("Failed to open the file\n");
    }
    
    /*
    * A symbol table to store consts, variables, and commands and their respective locations in memory.
    */
    TableEntry symTable[256];
    unsigned short int symSize;
    

    

    return(0);
}#include <stdlib.h>
#include <stdio.h>
#include "infToPstf.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "peep.h"


#define ADD     0x10    // Add a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define SUB     0x11    // Subtract a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define MUL     0x12    // Multiple a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define DIV     0x13    // Divide a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define MOD     0x14    // Compute the integer remainder when dividing a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)

// ACC-MEM Logical Instructions
#define AND     0x20    // Compute the binary AND of a specific memory location and the accumulator (leave the result in the accumulator)
#define ORR     0x21    // Compute the binary OR of a specific memory location and the accumulator (leave the result in the accumulator)
#define NOT     0x22    // Compute the logical NOT of the accumulator (leave the result in the accumulator)
#define XOR     0x23    // Compute the binary XOR of a specific memory location and the accumulator (leave the result in the accumulator)
#define LSR     0x24    // Compute the binary logical shift right (1-bit) of the accumulator (leave the result in the accumulator)
#define ASR     0x25    // Compute the binary arithmetic shift right (1-bit) of the accumulator (leave the result in the accumulator)
#define LSL     0x26    // Compute the binary logical shift left of the accumulator (leave the result in the accumulator)

// Branch Instructions
#define B       0X30    // Branch to a specific location in memory
#define BNEG    0X31    // Branch to a specific location in memory if the accumulator is negative
#define BPOS    0X32    // Branch to a specific location in memory if the accumulator is positive
#define BZRO    0X33    // Branch to a specific location in memory if the accumulator is zero

// ACC-MEM Load/Store Instructions
#define LOAD    0X40    // Load a word from a specific location in memory into the accumulator
#define STOR    0X41    // Store a word from the accumulator into a specific memory location

// I/O (Standard) Instructions
#define READ    0X50    // Read a word from the terminal into a specific location in memory
#define WRTE    0X51    // Write a word from a specific location in memory to the terminal

// HALT Instruction
#define HALT    0XFF    // Halt, i.e. the program has completed its task

const int MEMSIZE = 256;
 

/*
*Method declarations
*/
bool insertIntoSym(TableEntry *table, TableEntry symbol);
bool lookUpSymbol(int symbol, TableEntry *table);
TableEntry getSymbol(int symbol, TableEntry *table);
void printSymTab(TableEntry* tab, int size);
int* ifConverter(char *inst);
int* letConverter(char *inst);
int* printConverter(PeepCompiler *comp, int var);
int* gotoConverter(PeepCompiler *comp, int lineNumber);
int* endConverter(char* inst);
int* inputConverter(PeepCompiler *comp, int var);

/*
*Method Implementations
*/
bool lookUpSymbol(int symbol, TableEntry *table)
{
    for(int i = 0; i<MEMSIZE; ++i)
    {
        if(table[i].symbol == symbol)
        {
            return true;
        }
    }
    return false;
}

TableEntry getSymbol(int symbol, TableEntry *table)
{
    for(int i = 0; i<MEMSIZE; ++i)
    {
        if(table[i].symbol == symbol)
        {
            return table[i];
        }
    }
}


void printSymTab(TableEntry* tab, int size)
{
    printf("Symbol  Table  Location \n");
    for (int i = 0; i < size; ++i)
    {
        printf("  %d       %c      0x%02hX \n", tab[i].symbol, tab[i].type, tab[i].location);
    }
}

int* gotoConverter(PeepCompiler *compiler, int lineNumer)
{
    int result[1];
    if(lookUpSymbol(lineNumer, compiler->symTab))
    {
        result[0] = B * MEMSIZE + getSymbol(lineNumer, compiler->symTab).location;
    }
    else
    {
        compiler->flag[compiler->inscount] = lineNumer;
        result[0] = B * MEMSIZE + 0;
    }
    ++compiler->inscount;
    return result;
}

int* inputConverter(PeepCompiler *compiler, int var)
{
    int* result[1];
    TableEntry symbol;
    if(lookUpSymbol(var, compiler->symTab))
    {
        symbol = getSymbol(var, compiler->symTab);
    }
    else
    {
        
        char type;
        unsigned short int loc = 255 - compiler->datacount;
        if (var >= 97 && var <= 122)
        {
            type = 'V';
        }
        else
        {
            type = 'C';
        }
        /*
        * Creating a new symbol struct AKA TableEntry
        */
        symbol.location = loc;
        symbol.symbol = var;
        symbol.type = type;
        compiler->symTab[compiler->inscount + 1] = symbol; 
    }
    if(MEMSIZE < compiler->datacount)
    {
        fprintf(stderr, "%s", "Memory limit for HML reached!\n");
        return 0;
    }
    result[0] =  READ * MEMSIZE + symbol.location; // add hml instruction to the result array 
    ++compiler->inscount; // increment the instructions count
    ++compiler->datacount; // increment the data count
    return result;
}

int main(int argc, char *argv[]){
    FILE *fp;
    char *filename;
    /*
    * Memory for the Peep compiler.
    */
    char mem[256][256];
    
    // Check if a filename has been specified in the command
    if (argc < 2)
    {
        printf("Missing Filename\n");
        //return(1);
    }
    else
    {
        filename = argv[1];
        printf("Filename : %s\n", filename);
    }

    // Open file in read-only mode
    fp = fopen(filename,"r");

    // If file opened successfully, then print the contents
    if ( fp )
    {
        int i = 0;
        char word[256];
        while ((fgets(word, 256, fp)))
        {
            printf("%s",word);
            strcpy(mem[i], word);
            i++;
        }
        printf("\n");
    }
    else
    {
        printf("Failed to open the file\n");
    }
    
    /*
    * A symbol table to store consts, variables, and commands and their respective locations in memory.
    */
    TableEntry symTable[256];
    unsigned short int symSize;
    

    

    return(0);
}
