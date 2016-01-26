/* input.c */

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 257

/**
 * statically defined array of symbol to store individual lines
 */
char  line[MAX_LINE_LENGTH];
int   lineNumber = 0;
char *linePointer = line;
FILE *inputFile;
int extendedLine = 0;

/**
 * initialisation of input to a standard input or to a file input
 * standard input is selected if fileName is NULL
 */
int initInput(char* fileName) {
  if (!fileName) {
    inputFile = stdin;
  } else {
    inputFile = fopen(fileName, "rt");
    if (!inputFile) {
      printf("Input file %s not found.\n", fileName);
      return 0;
    }
  }
  return 1;
}

/**
 * will read single symbol from input
 */
int getChar() {
  if (!*linePointer) {
    if (!fgets(line, MAX_LINE_LENGTH, inputFile)) return EOF;

    linePointer = line;
    lineNumber++;

    int lineLength = strlen(line);
    if (extendedLine) {
      lineNumber--;
      printf("+    %s", line);
    } else {
      printf("%-4d %s", lineNumber, line);
    }

    extendedLine = line[lineLength - 1] != '\n';
  }
  return *linePointer++;
}
