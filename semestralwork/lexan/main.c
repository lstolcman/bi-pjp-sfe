/* main.c */
/* lexical analyzer test */

#include "lexan.h"
#include <stdio.h>

void printSymb(LexicalSymbol lexSymbol) {
   printf("<%s", symbTable[lexSymbol.type]);
   switch (lexSymbol.type) {
   case IDENT:
      printf(", %s", lexSymbol.ident);
      break;
   case NUMB:
      printf(", %d", lexSymbol.number);
      break;
   default:
     break;
   }
   printf(">\n");
}

int main(int argc, char *argv[]) {
   char *fileName;
   printf("Lexical analyzer test.\n");
   if (argc == 1) {
      printf("Keyboard input, write the source code.\n");
      fileName = NULL;
   } else {
      fileName = argv[1];
      printf("Input file %s.\n", fileName);
   }
   if(!initLexan(fileName)) {
     printf("Error creating lexical analyzer.\n");
     return 0;
   }
   LexicalSymbol lexSymbol;
   do {
      lexSymbol = readLexem();
      printSymb(lexSymbol);
   } while (lexSymbol.type != EOI);
   printf("End.\n");
   return 0;
}
