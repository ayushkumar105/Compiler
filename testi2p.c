#include "peepcc.h"
#include "infToPstf.h"

int main()
{
    PeepCompiler comp;
    initCompiler(&comp);
    comp.file = "ex1.peep";
    firstPass(&comp, comp.file);
    secondPass(&comp);
    printMem(comp.hml);
    printSymTab(comp.symTab, comp.symSize);
    return (0);
}
