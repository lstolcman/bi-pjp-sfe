/* lexan.c */

#include "lexan.h"
#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {LETTER, NUMBER, WHITE_SPACE, END, NO_TYPE} InputCharType;

const char *symbTable[12] = {
   "IDENT", "NUMB", "PLUS", "MINUS", "LESS", "LESS_OR_EQ", "ASSIGN",
   "kwIF", "kwTHEN", "kwELSE", "EOI", "ERR"
}; //symbol names in the same order as in LexSymbolType

static int character;   // input symbol
static InputCharType input; // input symbol type

void readInput(void) {
   character = getChar();
   if ((character>='A' && character<='Z') || (character>='a' && character<='z'))
      input = LETTER;
   else if (character>='0' && character<='9')
      input = NUMBER;
   else if (character == EOF)
      input = END;
   else if (character <= ' ')
      input = WHITE_SPACE;
   else
      input = NO_TYPE;
}

const struct {char* slovo; LexSymbolType symb;} keyWordTable[] = {
   {"if", kwIF},
   {"then", kwTHEN},
   {"else", kwELSE},
   {NULL, (LexSymbolType) 0}
};

LexSymbolType keyWord(char* id) {
   int i = 0;
   while (keyWordTable[i].slovo) 
      if (strcmp(id, keyWordTable[i].slovo)==0) 
         return keyWordTable[i].symb;
      else
         i++;
   return IDENT;
}

void error(char* text) {
   printf("\n%s\n", text);
   exit(1);
}

LexicalSymbol readLexem(void) {
  LexicalSymbol data;
  int identLen;
q0:
  switch (character) {
    case '{':
      readInput();
      goto q1;
    case '+':
      data.type = PLUS;
      readInput();

      return data;
    case '-':
      data.type = MINUS;
      readInput();

      return data;
    case '<':
      readInput();
      goto q4;
    case ':':
      readInput();
      goto q5;
    default:;
  }
  switch (input) {
    case WHITE_SPACE:
      readInput();
      goto q0;
    case END:
      data.type = EOI;
      return data;
    case LETTER:
      identLen = 1;
      data.ident[0] = character;
      readInput();
      goto q2;
    case NUMBER:
      data.number = character - '0';
      data.type = NUMB;
      readInput();
      goto q3;
    default:
      data.type = ERR;
      error("Invalid symbol.");
      return data;
  }

q1:
  switch(character) {
    case '}':
      readInput();
      goto q0;
    default:;
  }
  switch(input) {
    case END:
      data.type = ERR;
      error("Uneexpected end of file in a comment.");
      return data;
    default:
      readInput();
      goto q1;
  }

q2:
  switch(input) {
    case LETTER:
    case NUMBER:
      data.ident[identLen] = character;
      identLen++;
      readInput();
      goto q2;
    default:
      data.ident[identLen] = 0;
      data.type = keyWord(data.ident);
      return data;
  }

q3:
  switch(input) {
    case NUMBER:
      data.number = 10 * data.number + (character - '0');
      readInput();
      goto q3;
    default:
      return data;
  }

q4:
  switch(character) {
    case '=':
      data.type = LESS_OR_EQ;
      readInput();
      return data;
    default:;
  }
  switch(input) {
    default:
      data.type = LESS;
      return data;
  }

q5:
  switch(character) {
    case '=':
      readInput();
      data.type = ASSIGN;
      return data;
    default:;
  }
  switch(input) {
    default:
      data.type = ERR;
      error("Expected \'=\'.");
      return data;
  }

}

int initLexan(char *fileName) {
  if(!initInput(fileName)) return 0;
  readInput();
  return 1;
}
