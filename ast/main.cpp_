/* main.c */
/* syntakticky analyzator */

#include "lexan.h"
#include "parser.h"
#include "tree.h"
#include "output.h"
#include "tabsym.h"
#include "input.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
   char *fileName;
   printf("MILA interpreter\n");
   if (argc == 1) {
      printf("Input from keyboard, write the source code terminated by a dot.\n");
      fileName = NULL;
   } else {
      fileName = argv[1];
      printf("Input file %s.\n", fileName);
   }
   if(!initParser(fileName)) {
      printf("Error creating the syntax analyzer.\n");
      return 0;
   }
   Prog *prog = Program();
   closeInput();
   prog = (Prog*)(prog->Optimize());
   prog->Translate();
   delete prog;
   freeSymbTab();
   Print();
   Run();
   printf("End.\n");
   return 0;
}
