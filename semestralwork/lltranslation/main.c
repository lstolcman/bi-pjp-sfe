/* main.c */
/* syntax analyzer */

#include "lexan.h"
#include "parser.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *fileName;
  printf("Evaualte math expression.\n");
  if (argc == 1) {
    printf("Input from keyboard, type the input text ended by EOF.\n");
    fileName = NULL;
  } else {
    fileName = argv[1];
    printf("Input from file %s.\n", fileName);
  }
  if(!initParser(fileName)) {
    printf("Error creating the syntx parser.\n");
    return 0;
  }
  Start();
  printf("End\n");
  return 0;
}
