/* lexan.cpp */

#include "lexan.h"
#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Symbol types
 */
typedef enum {LETTER, NUMBER, WHITE_SPACE, END, NO_TYPE} InputCharType;

/**
 * Array of string constants for each lexical elements.
 * Symbol string representation is in the same order as in LexSymbolType enum
 */
const char *symbTable[30] = {
   "IDENT", "NUMB", "PLUS", "MINUS", "TIMES", "DIVIDE", 
   "EQ", "NEQ", "LT", "GT", "LTE", "GTE", "LPAR", "RPAR", "ASSIGN",
   "COMMA", "SEMICOLON",
   "kwVAR", "kwCONST", "kwBEGIN", "kwEND", "kwIF", "kwTHEN", "kwELSE", 
   "kwWHILE", "kwDO", "kwWRITE", "kwREAD", 
   "EOI", "ERR"
};

/**
 * the next character from input is stored here
 */
static int character;
/**
 * the type of the next character from input is stored here
 */
static InputCharType input;

/**
 * reads next symbol from input and determines its type
 */
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

/**
 * list of available keywords with appropriate lexical element
 */
const struct {const char *slovo; LexSymbolType symb;} keyWordTable[] = {
   {"var", kwVAR},
   {"const", kwCONST},
   {"begin", kwBEGIN},
   {"end", kwEND},
   {"if", kwIF},
   {"then", kwTHEN},
   {"else", kwELSE},
   {"while", kwWHILE},
   {"do", kwDO},
   {"write", kwWRITE},
   {"read", kwREAD},
   {NULL, (LexSymbolType) 0}
};

/**
 * identifier id is tested if it is a keyword in this function
 */
LexSymbolType keyWord(char* id) {
   int i = 0;
   while (keyWordTable[i].slovo) 
      if (strcmp(id, keyWordTable[i].slovo)==0) 
         return keyWordTable[i].symb;
      else
         i++;
   return IDENT;
}

/**
 * error signaling. Terminates execution
 */
void error(const char *text) {
   printf("\n%s\n", text);
   exit(1);
}

/**
 * implementation of lexical analyzator
 */
LexicalSymbol readLexem(void) {
  LexicalSymbol data;
  int idLength;
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
    case '*':
      data.type = TIMES;
      readInput();
      return data;
   case '/':
      data.type = DIVIDE;
      readInput();
      return data;
   case '(':
      data.type = LPAR;
      readInput();
      return data;
   case ')':
      data.type = RPAR;
      readInput();
      return data;
   case '=':
      data.type = EQ;
      readInput();
      return data;
    case '<':
      readInput();
      goto q4;
    case '>':
      readInput();
      goto q5;
    case ':':
      readInput();
      goto q6;
    case ',':
      data.type = COMMA;
      readInput();
      return data;
    case ';':
      data.type = SEMICOLON;
      readInput();

      return data;
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
      idLength = 1;
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
      data.ident[idLength] = character;
      idLength++;
      readInput();
      goto q2;
    default:
      data.ident[idLength] = 0;
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
      data.type = LTE;
      readInput();
      return data;
    case '>': 
      data.type = NEQ;
      readInput();
      return data;
    default:;
  }
  switch(input) {
    default:
      data.type = LT;
      return data;
  }

q5:
  switch(character) {
    case '=':
      data.type = GTE;
      readInput();
      return data;
    default:;
  }
  switch(input) {
    default:
      data.type = GT;
      return data;
  }

q6:
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

/**
 * lexan initialization function
 */
int initLexan(char *fileName) {
  if(!initInput(fileName)) return 0;
  readInput();
  return 1;
}
