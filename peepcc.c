#include <stdlib.h>
#include <stdio.h>
#include "infToPstf.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "peep.h"

/*
*Method declarations
*/
void printSymTab(TableEntry* tab, int size);
void ifConverter(char *inst);
void letConverter(char *inst);

/*
*Method Implementations
*/
void printSymTab(TableEntry* tab, int size)
{
    printf("Symbol  Table  Location \n");
    for (int i = 0; i < size; ++i)
    {
        printf("  %d       %c      0x%02hX \n", tab[i].symbol, tab[i].type, tab[i].location);
    }
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
