#include "peepcc.h"
#include "infToPstf.h"

void test()
{
    PeepCompiler comp;
    initCompiler(&comp);
    comp.file = "myFile.peep";
    firstPass(&comp, comp.file);
    secondPass(&comp);
    writeToFile(&comp, "test.hml");
    printf("The HML instructions have been written to a file called test.hml \n");
    printf("Below is the HML memory layout!");
    printMem(comp.hml);
    printf("Below is the symbol table created during compilation!\n");
    printSymTab(comp.symTab, comp.symSize);
}
