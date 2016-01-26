/* main.c */
/* pushdown computer */

#include "lexan.h"
#include "parser.h"
#include "output.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
   char *fileName;
   printf("Expression evaliation using pushdown computer.\n");
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
   Start();
   Execute();
   printf("End.\n");
   return 0;
}
