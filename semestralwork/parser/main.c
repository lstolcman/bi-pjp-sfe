/* main.c */
/* syntactic analyzer */

#include "lexan.h"
#include "parser.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *fileName;
  printf("Syntactic analyzer.\n");
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
  Expression();
  printf("End.\n");
  return 0;
}
