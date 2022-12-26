#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "peepcc.h"
//================================================
//
// Ayush Kumar
//
// Hatchling Simulator
// Computer System Fundamentals
//
//================================================

//================================================
// includes and defines
//================================================
#include <stdio.h>
// ACC-MEM Arithmetic Instructions
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


//================================================
// global type statements
//================================================
struct hatchling {
    short int accumulator;
    unsigned short int instructReg;
    unsigned char instructCntr;
    unsigned char opCode;
    unsigned char operand;
    short int mem[256];
    int insCount;

};
typedef struct hatchling Hatchling;

//================================================
// function prototypes
//================================================
void printMem(short int mem[256]);

// LOGICAL AND ARITHMETIC OPERATIONS
short int add(short int acc,short int val);

short int sub(short int acc, short int val);
short int divd(short int acc, short int val);
short int mul(short int acc,short int val);

short int mod(short int acc,short int val);

short int and (short int acc,short int val);

short int or (short int acc,short int val);

short int not (short int acc);

short int xor (short int acc,short int val);

short int lsr (short int acc);

short int asr (short int acc);

short int lsl (short int acc);

//LOADING, STORING, WRITING AND READING OPERATIONS
short int load(Hatchling* memory, short int location);
void store(Hatchling* hatch, short int acc, short int location);
void read(Hatchling* hatch, short int location);
void write(Hatchling* hatch, short int acc, short int location);
void load_array(Hatchling* h, FILE* infp);
int check_error(short int* result, short int a, short b);
int check_error_mult(short int a, short b);


void sim(char file[])
{
    PeepCompiler comp;
    initCompiler(&comp);
    firstPass(&comp, file);
    secondPass(&comp);
    
    Hatchling hatch = {
        .accumulator = 0,
        .instructReg = 0,
        .instructCntr = 0,
        .mem = {0},
        .opCode = 0,
        .operand = 0,
        .insCount = 0
    };
    
    for (int i = 0; i < 256; i++) {     
        hatch.mem[i] = comp.hml[i];     
    }   

    
    //STARTING TO EXECUTE THE PROGRAM
    printf("\n");
    printf("*** PROGRAM LOADING COMPLETED *** \n");
    printf("*** PROGRAM EXECUTION BEGINS  *** \n");
    int j = 1;
    for (short int i = 0; i < 256; i += j)
    {
        //INITIALIZING ALL THE VARIABLES
        j = 1;
        short int inst = hatch.mem[i];
        hatch.instructCntr = i & 0xFF;
        hatch.instructReg = hatch.mem[hatch.instructCntr];
        hatch.opCode = inst >> 8;
        hatch.operand = inst & 0xFF;
        
        if (hatch.opCode == LOAD)
        {
            
            hatch.accumulator = load(&hatch, hatch.operand);
        }
        else if (hatch.opCode == STOR)
        {
            store(&hatch, hatch.accumulator, hatch.operand);
        }
        else if (hatch.opCode == READ)
        {
            read(&hatch, hatch.operand);
        }
        else if (hatch.opCode == WRTE)
        {
            write(&hatch, hatch.accumulator, hatch.operand);
        }
        else if (hatch.opCode == B)
        {
            //BRANCH TO THE GIVEN LINE OF CODE
            j = hatch.operand - i;
        }
        else if (hatch.opCode == BNEG)
        {
            
            if (hatch.accumulator < 0)
            {
                //BRANCH TO THE GIVEN LINE OF CODE
                j = hatch.operand - i;
            }
        }
        else if (hatch.opCode == BPOS)
        {
            if (hatch.accumulator > 0)
            {
                //BRANCH TO THE GIVEN LINE OF CODE
                j = hatch.operand - i;
            }
        }
        else if (hatch.opCode == BZRO)
        {
            if (hatch.accumulator == 0)
            {
                //BRANCH TO THE GIVEN LINE OF CODE
                j = hatch.operand - i;
            }
        }
        else if (hatch.opCode == ADD)
        {
            //CHECKING FOR ACCUMULATOR OVERFLOW
            short int temp;
            int res = check_error(&temp, hatch.accumulator, hatch.mem[hatch.operand]);
            if (res == -1)
            {
                printf("*** ACCUMULATOR OVERFLOW ***");
                printf("*** HATCHLING EXECUTION ABNORMALLY TERMINATED ***");
                break;
            }
            //ADDING TO THE ACCUMULATOR
            temp = hatch.accumulator;
            hatch.accumulator = add(temp, hatch.mem[hatch.operand]);
        }
        else if (hatch.opCode == MUL)
        {
            //CHECKING FOR ACCUMULATOR OVERFLOW
            short int temp;
            int res = check_error_mult(hatch.accumulator, hatch.mem[hatch.operand]);
            if (res == -1)
            {
                printf("*** ACCUMULATOR OVERFLOW ***\n");
                printf("*** HATCHLING EXECUTION ABNORMALLY TERMINATED ***");
                break;
            }
            //MULTIPLYING TO THE ACCUMULATOR
            temp = hatch.accumulator;
            hatch.accumulator = mul(temp, hatch.mem[hatch.operand]);
        }
        else if (hatch.opCode == SUB)
        {
            //SUBTRACTING FROM THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = sub(temp, hatch.mem[hatch.operand]);
            
        }
        else if (hatch.opCode == DIV)
        {
            //CHECKING FOR DIVIDE BY ZERO ERROR
            if (hatch.mem[hatch.operand] == 0)
            {
                printf("*** ATTEMPT TO DIVIDE BY ZERO ***\n");
                printf("*** HATCHLING EXECUTION ABNORMALLY TERMINATED ***");
                break;
            }
            //DIVING FROM THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = divd(temp, hatch.mem[hatch.operand]);
        }
        else if (hatch.opCode == MOD)
        {
            //MODING FROm THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = mod(temp, hatch.mem[hatch.operand]);
        }
        else if (hatch.opCode == AND)
        {
            //ANDING THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = and(temp, hatch.mem[hatch.operand]);
        }
        else if (hatch.opCode == ORR)
        {
            //OR-ING FROM THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = or(temp, hatch.mem[hatch.operand]);
        }
        else if (hatch.opCode == XOR)
        {
            //XOR-RING FROM THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = xor(temp, hatch.mem[hatch.operand]);
        }
        else if (hatch.opCode == NOT)
        {
            //NOTING FROM THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = not(temp);
        }
        else if (hatch.opCode == LSL)
        {
            //LEFT SHIFTING THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = lsl(temp);
        }
        else if (hatch.opCode == LSR)
        {
            //RIGHT SHIFTING THE ACCUMULATOR
            short int temp = hatch.accumulator;
            hatch.accumulator = lsr(temp);
        }
        else if (hatch.opCode == ASR)
        {
            //ARITHMETIC SHIFT RIGHT 
            short int temp = hatch.accumulator;
            hatch.accumulator = asr(temp);
        }
        else if (hatch.opCode == HALT)
        {
            printf("*** PROGRAM EXECUTION TERMINATED *** \n");
            break;
        }
        else
        {
            //IF OP CODE IS NOT IN THE LIST
            printf("*** BAD OPERATION CODE ***\n");
            printf("*** HATCHLING EXECUTION ABNORMALLY TERMINATED ***");
        }
        
    }
    
    printf("\n");
    printf("REGISTERS \n");
    printf("ACC     %04hX \n", hatch.accumulator);
    printf("InstCtr   %02hX \n", hatch.instructCntr);
    printf("InstReg %04hX \n", hatch.instructReg);
    printf("OpCode    %02hX \n", hatch.opCode);
    printf("Operand   %02hX \n", hatch.operand);
    printf("\n");
    printf("MEMORY: \n");
    printMem(hatch.mem);
}


//================================================
// main function
//================================================
int main(int argc, char** argv) {
    
    //If no file to read
    if (argc != 2) {
        printf("Please enter a file name to execute hml code");
    }
    //If there is a file to read
    else {
        sim(argv[1]);
    }
    
    // return to calling environment
    return(0);
}
void printMem(short int mem[256])
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

short int add(short int acc, short int val)
{
    return acc + val;
}
short int divd(short int acc, short int val)
{
    return acc / val;
}
short int sub(short int acc, short int val)
{
    return acc - val;
}
short int mul(short int acc, short int val)
{
    return acc * val;
}
short int mod(short int acc, short int val)
{
    return acc % val;
}
short int and (short int acc, short int val)
{
    return acc && val;
}
short int or (short int acc, short int val)
{
    return acc || val;
}
short int not(short int acc)
{
    acc = !acc;
    return acc;
}
short int xor (short int acc, short int val)
{
    return acc ^ val;
}
short int lsr(short int acc)
{
    //unsigned int to get the logical shift instead of the arithmetic
    return (short int)((unsigned int)acc >> 1);
}
short int asr(short int acc)
{
    return (short int)(acc >> 1);
}
short int lsl(short int acc)
{
    return (short int)(acc << 1);
}
short int load(Hatchling* hatch, short int location)
{
    return hatch->mem[location];
}

void store(Hatchling* hatch, short int acc, short int location)
{
    hatch->mem[location] = acc;
}

void read(Hatchling* hatch, short int location)
{
    char buf[6];
    fgets(buf, 6, stdin);
    short int temp = strtol(buf, NULL, 10);
    hatch->mem[location] = temp;
}
void write(Hatchling* hatch, short int acc, short int location)
{
    
    printf("\n");
    printf("TERMINAL OUTPUT: %d \n", hatch->mem[location]);
}

void load_array(Hatchling* h, FILE* infp)
{
    char word[256];
    int j = 0;

    while (fgets(word, 256, infp))
    {
        
        short int line = (short int)strtol(word, NULL, 16);

        h->mem[j] = line;
        j++;
    }
    return;
}
int check_error(short int* result, short int a, short b)
{
    *result = a + b;
    if (a > 0 && b > 0 && *result < 0)
        return -1;
    if (a < 0 && b < 0 && *result > 0)
        return -1;
    return 0;
}
int check_error_mult(short int a, short b)
{

    if (a == 0 || b == 0)
        return 0;
     
    short int result = a * b;
    if (a == result / b)
        return 0;
    else
        return -1;

    
}
