#include <stdlib.h>
#include <stdio.h>
#include "infToPstf.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "peepcc.h"


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


/*
*Method declarations
*/


/*
*Method Implementations
*/

void secondPass(PeepCompiler *compiler)
{
    for(int i = 0; i<MEMSIZE; ++i)
    {
        if(compiler->flag[i] != -1)
        {
            compiler->hml[i] = compiler->hml[i] + getSymbol(compiler->flag[i], compiler->symTab).location;
        }    
    }
    
}

void writeToFile(PeepCompiler* compiler, char fileName[])
{
    FILE *fp;

   fp = fopen(fileName, "w+");
   for(int i = 0; i<compiler->inscount; ++i)
   {
        if(compiler->hml[i] != 0)
        {
            // "%04hX "
            fprintf(fp, "%02hX %04hX\n", i, compiler->hml[i]);
        }
        
   }
   for(int i = 0; i<compiler->symSize; ++i)
   {
        if(compiler->symTab[i].type == 'C' || compiler->symTab[i].type == 'V')
        {
            fprintf(fp, "%02hX %04hX\n", compiler->symTab[i].location, compiler->hml[compiler->symTab[i].location]);
        }
   }
   
   fclose(fp);
}

int letConverter(PeepCompiler *comp, char res, char expr[])
{
    TableEntry entry;
    if(lookUpSymbol(res, comp))
    {
        entry = getSymbol(res, comp->symTab);
    }
    else
    {
        entry.symbol = res;
        entry.location = comp->datacount--;
        entry.type = 'V';
        comp->symTab[comp->symSize++] = entry;
    }
    
    char post[256];
    infixToPostfix(expr, post);
    
    int resMem = evaluatePostfixExpression(comp, post);
    comp->hml[comp->inscount++] = LOAD * MEMSIZE + resMem;
	comp->hml[comp->inscount++] = STOR * MEMSIZE + entry.location;
    return 0;
}


int ifConverter(PeepCompiler *comp, char *var1, char *op, char *var2, int lineNumber)
{
    TableEntry entry;
    if(isdigit(var1[0]))
    {
        if (lookUpSymbol(atoi(var1), comp))
        {
            entry = getSymbol(atoi(var1), comp->symTab);
        }
        else
        {
            entry.location = comp->datacount;
            entry.symbol = atoi(var1);
            entry.type = 'C';
            comp->symTab[comp->symSize++] = entry;
            comp->hml[comp->datacount--] = atoi(var1);
        }
        
    }
    else
    {
        if (lookUpSymbol(var1[0], comp))
        {
            
            entry = getSymbol(var1[0], comp->symTab);
        }
        else
        {
            
            entry.location = comp->datacount--;
            entry.symbol = var1[0];
            entry.type = 'V';
            comp->symTab[comp->symSize++] = entry;
            
        }
    }
    TableEntry entry2;
    if(isdigit(var2[0]))
    {
        if (lookUpSymbol(atoi(var2), comp))
        {
            entry2 = getSymbol(atoi(var2), comp->symTab);
        }
        else
        {
            
            entry2.location = comp->datacount;
            entry2.symbol = atoi(var2);
            entry2.type = 'C';
            comp->symTab[comp->symSize++] = entry2;
            comp->hml[comp->datacount--] = atoi(var2);
        }
        
    }
    else
    {
        if (lookUpSymbol(var2[0], comp))
        {
            entry2 = getSymbol(var2[0], comp->symTab);
        }
        else
        {
            
            entry2.location = comp->datacount--;
            entry2.symbol = var2[0];
            entry2.type = 'V';
            comp->symTab[comp->symSize++] = entry2;
            
        }
    }

    int lineAddr = 0;
    if(lookUpSymbol(lineNumber, comp))
    {
        TableEntry line = getSymbol(lineNumber, comp->symTab);
        lineAddr = line.location;
    }
    else
    {
        lineAddr = -1;
    }
    
    if(strcmp(op, "==") == 0)
    {
        comp->hml[comp->inscount++] = LOAD * MEMSIZE + entry.location;
        comp->hml[comp->inscount++] = SUB * MEMSIZE + entry2.location;
        if(lineAddr != -1)
        {
            comp->hml[comp->inscount++] = BZRO * MEMSIZE + lineAddr;
        }
        else
        {
            comp->hml[comp->inscount++] = BZRO * MEMSIZE + 0;
            comp->flag[comp->inscount - 1] = lineNumber;
        }
        
    }
    else if (strcmp(op, "!=") == 0)
    {
        comp->hml[comp->inscount++] = LOAD * MEMSIZE + entry.location;
        comp->hml[comp->inscount++] = SUB * MEMSIZE + entry2.location;
        comp->hml[comp->inscount++] = BZRO * MEMSIZE + 2;
        if(lineAddr != -1)
        {
            comp->hml[comp->inscount++] = B * MEMSIZE + lineAddr;
        }
        else
        {
            comp->hml[comp->inscount++] = B * MEMSIZE + 0;
            comp->flag[comp->inscount - 1] = lineNumber;
        }
    }
    else if (strcmp(op, ">") == 0)
    {
        comp->hml[comp->inscount++] = LOAD * MEMSIZE + entry2.location;
        comp->hml[comp->inscount++] = SUB * MEMSIZE + entry.location;
        
        if(lineAddr != -1)
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + lineAddr;
        }
        else
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + 0;
            comp->flag[comp->inscount - 1] = lineNumber;
        }
    }
    else if (strcmp(op, "<") == 0)
    {
        comp->hml[comp->inscount++] = LOAD * MEMSIZE + entry.location;
        comp->hml[comp->inscount++] = SUB * MEMSIZE + entry2.location;
        
        if(lineAddr != -1)
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + lineAddr;
        }
        else
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + 0;
            comp->flag[comp->inscount - 1] = lineNumber;
        }
    }
    else if (strcmp(op, ">=") == 0)
    {
        comp->hml[comp->inscount++] = LOAD * MEMSIZE + entry2.location;
		comp->hml[comp->inscount++] = SUB * MEMSIZE + entry.location;
        if (lineAddr != -1)
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + lineAddr;
		    comp->hml[comp->inscount++] = BZRO * MEMSIZE + lineAddr;
        }
        else
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + 0;
		    comp->hml[comp->inscount++] = BZRO * MEMSIZE + 0;
            comp->flag[comp->inscount - 1] = lineNumber;
            comp->flag[comp->inscount - 2] = lineNumber;
        }	
    }
    else if (strcmp(op, "<=") == 0)
    {
        comp->hml[comp->inscount++] = LOAD * MEMSIZE + entry.location;
		comp->hml[comp->inscount++] = SUB * MEMSIZE + entry2.location;
        if (lineAddr != -1)
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + lineAddr;
		    comp->hml[comp->inscount++] = BZRO * MEMSIZE + lineAddr;
        }
        else
        {
            comp->hml[comp->inscount++] = BNEG * MEMSIZE + 0;
		    comp->hml[comp->inscount++] = BZRO * MEMSIZE + 0;
            comp->flag[comp->inscount - 1] = lineNumber;
            comp->flag[comp->inscount - 2] = lineNumber;
        }		
    }
    
    
    
    return 0;
    



}


void initCompiler(PeepCompiler *compiler)
{
    TableEntry symbols[256];
    unsigned short int hml[256] = {0};
    compiler->datacount = 255;
    compiler->hml = hml;
    compiler->inscount = 0;
    compiler->ln = 0;
    compiler->symSize = 0;
    compiler->symTab = symbols;
    for(int i = 0; i<MEMSIZE; ++i)
    {
        compiler->flag[i] = -1;
    }
    compiler->memsize = 0;
}


bool lookUpSymbol(int symbol, PeepCompiler *comp)
{
    for(int i = 0; i<comp->symSize; ++i)
    {
        
        if(comp->symTab[i].symbol == symbol)
        {
            return true;
        }
    }
    
    return false;
}

TableEntry getSymbol(int symbol, TableEntry *table)
{
    TableEntry entry;
    entry.symbol = '0';
    entry.location = 0;
    entry.type = 'N';
    for(int i = 0; i<MEMSIZE; ++i)
    {
        if(table[i].symbol == symbol)
        {
            return table[i];
        }
    }
    return entry;
}


void printSymTab(TableEntry* tab, int size)
{
    printf("Symbol  Table  Location \n");
    for (int i = 0; i < size; ++i)
    {
        if(tab[i].type == 'V')
        {
            printf("  %c        %c      0x%02hX \n", tab[i].symbol, tab[i].type, tab[i].location);
        }
        else
        {
            printf("  %d        %c      0x%02hX \n", tab[i].symbol, tab[i].type, tab[i].location);
        }
        
    }
}

int endConverter(PeepCompiler *compiler)
{
    compiler->hml[compiler->inscount++] = HALT * MEMSIZE + 0;
    return HALT * MEMSIZE + 0;
}

int gotoConverter(PeepCompiler *compiler, int lineNumber)
{
    int result[1];
    if(lookUpSymbol(lineNumber, compiler))
    {
        printf("%d \n", lineNumber);
        result[0] = B * MEMSIZE + getSymbol(lineNumber, compiler->symTab).location;
    }
    else
    {
        compiler->flag[compiler->inscount] = lineNumber;
        result[0] = B * MEMSIZE + 0;
    }
    compiler->hml[compiler->inscount++] = result[0]; //add hml instr to the hml memory
    return result[0];
}

int printConverter(PeepCompiler *compiler, int var)
{
    TableEntry symbol;
    
    if(lookUpSymbol(var, compiler))
    {
        symbol = getSymbol(var, compiler->symTab);
    }
    
    compiler->hml[compiler->inscount++] = WRTE * MEMSIZE + symbol.location; //add hml instr to the hml memory
    
    return WRTE * MEMSIZE + symbol.location;
}


int inputConverter(PeepCompiler *compiler, int var)
{
    
    TableEntry symbol;
    
    if(lookUpSymbol(var, compiler))
    {
        symbol = getSymbol(var, compiler->symTab);
        
    }
    
    compiler->hml[compiler->inscount++] = READ * MEMSIZE + symbol.location; //add hml instr to the hml memory
    
    return READ * MEMSIZE + symbol.location;
}


void printMem(unsigned short int mem[256])
{
    printf("\n");
    printf("      ");
    for (short int i = 0; i < 16; ++i)
    {
        printf("%4hX ", i);
    }
    printf("\n");
    printf("    0");
    printf(" %04hX ", mem[0]);
    short int j = 1;
    for (int i = 1; i < 256; ++i)
    {
        if (i % 16 == 0)
        {
            printf("\n");
            printf("%4hX0 ", j);
            ++j;
        }
        printf("%04hX ", mem[i]);
    }
    printf("\n");
}

void generateInstructions(char *command, char vars[], PeepCompiler *compiler)
{
    
    if(strcmp(command, "print") == 0)
    {
        printConverter(compiler, vars[0]);
    }
    else if(strcmp(command, "goto") == 0)
    {
        gotoConverter(compiler, atoi(vars));
    }
    else if(strcmp(command, "input") == 0)
    {
        inputConverter(compiler, vars[0]);
    }
    else if(strcmp(command, "end") == 0)
    {
        endConverter(compiler);
    }
    
}


void  firstPass(PeepCompiler *compiler, char *fileName)
{

    char mem[256][256];

    FILE* f; 
        
    f = fopen(fileName,"r");
    int ii = 0;
    // If file opened successfully, then add it to the memory array of peep.
    if ( f )
    {
        
        char word[256];
        while ((fgets(word, 256, f)))
        {
            
            strcpy(mem[ii], word);
            ii++;
        }
        printf("\n");
    }
    else
    {
        printf("Failed to open the file\n");
    }

    for(int i = 0; i < ii; ++i)
    {
        
        // Declaration of delimiter
        const char s[4] = " ";
        char* token;

        // Use of strtok
        // get first token
        
        token = strtok(mem[i], s);
        
        TableEntry entry;
        

        //Inserting line number in symbol table---------
        entry.type = 'L';
        entry.location = compiler->inscount;
        entry.symbol = atoi(token);
        compiler->symTab[compiler->symSize++] = entry;
        //-----------------------------------------------

        //Getting command token--------------------------
        token = strtok(NULL, s);
        char *command;
        command = token;
        //-----------------------------------------------
        
        token = strtok(NULL, s);
        
        //Control flow based on the command of the line
        if (strcmp(command,"print") == 0 || strcmp(command, "input") == 0 )
        {
                //Table entry struct incase a symbol needs to be inserted in the symbol table
                TableEntry entry;
                

                //If token is a variable
                if(isalpha((unsigned int) token[0]))
                {
                    //If symbol is in the symbol table
                    if(lookUpSymbol(token[0], compiler))
                    {
                        entry = getSymbol(token[0], compiler->symTab);
                    }
                    //If symbol is not in the symbol table then insert it
                    else
                    {
                        entry.type = 'V';
                        entry.location = compiler->datacount--;
                        entry.symbol = token[0];
                        compiler->symTab[compiler->symSize++] = entry;
                    }
                }

                //If token is a constant 
                else
                {
                    //If symbol is in the symbol table
                    if(lookUpSymbol(atoi(token), compiler))
                    {
                        entry = getSymbol(atoi(token), compiler->symTab);
                    }
                    //If symbol is not in the symbol table then insert it
                    else
                    {
                        entry.type = 'C';
                        entry.location = compiler->datacount--;
                        entry.symbol = atoi(token);
                        compiler->symTab[compiler->symSize++] = entry;
                    }
                }
            generateInstructions(command, token, compiler);
        }
        else if (strcmp(command, "end") == 0)
        {
            generateInstructions(command, "", compiler);
        }
        else if (strcmp(command, "goto") == 0)
        {
            generateInstructions(command, token, compiler);
        }
        else if (strcmp(command, "if") == 0)
        {
            char *op1;
            char *op2;
            char *relOp;

            op1 = token;
            token = strtok(NULL, s);
            relOp = token;
            token = strtok(NULL, s);
            op2 = token;
            token = strtok(NULL, s);
            token = strtok(NULL, s);
            char *lineNumber = token;
            ifConverter(compiler, op1, relOp, op2, atoi(lineNumber));
        }
        else if (strcmp(command, "let") == 0)
        {
            
            char expr[256];
            char res;
            res = token[0];
            
            token = strtok(NULL, s);
            token = strtok(NULL, s);
            int jj = 0;
            
            while (token != NULL)
            {
                for(int kk = 0; kk<strlen(token); ++kk)
                {
                    expr[jj] = token[kk];
                    
                    jj++;
                }
                expr[jj] = ' ';
                jj++;
                token = strtok(NULL, s);
            }
            expr[jj - 1] = '\0';
            
            letConverter(compiler, res, expr);
        }
        
        
    }
}

// int main(int argc, char *argv[]){

//     /*
//     *Do not delete the commented code below. Might need it incase we need to read file from the terminal.
//     */
//     // FILE *fp;
//     // char *filename;
//     // Check if a filename has been specified in the command
//     // if (argc < 2)
//     // {
//     //     printf("Missing Filename\n");
//     //     //return(1);
//     // }
//     // else
//     // {
//     //     filename = argv[1];
//     //     printf("Filename : %s\n", filename);
//     // }
//     // TableEntry entry;
//     PeepCompiler compiler;
//     initCompiler(&compiler);
//     firstPass(&compiler, "myFile.peep");
//     secondPass(&compiler);
//     printMem(compiler.hml);
//     printSymTab(compiler.symTab, compiler.symSize);
//     writeToFile(&compiler, "test.peep");
//     return(0);
// }
int main()
{
	return(0);
}
