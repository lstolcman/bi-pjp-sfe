/* parser.c */

#include "parser.h"
#include "lexan.h"
#include <stdio.h>
#include <stdlib.h>

void Expression(void);
void ExpressionPrime(void);
void Term(void);
void TermPrime(void);
void Factor(void);

LexicalSymbol Symb;

void CompareError(LexSymbolType s) {
   printf("Error while comparing, expected %s.\n", symbTable[s]);
   exit(1);
}

void ExpansionError(char* nonterminal, LexSymbolType s) {
   printf("Error while expanding nonterminal %s, unexpected token %s.\n", nonterminal, symbTable[s]);
   exit(1);
}

void Compare(LexSymbolType s) {
   if (Symb.type == s)
      Symb = readLexem();
   else
      CompareError(s);
}

void Expression(void) {
   switch (Symb.type) {
   case IDENT:
   case LPAR:
      printf("(1) E -> T E'\n");
      Term();
      ExpressionPrime();
      break;
   default:
      ExpansionError("E", Symb.type);
   }
}

void ExpressionPrime(void) {
   switch (Symb.type) {
   case PLUS:
      printf("(2) E' -> + T E'\n");
      Compare(PLUS);
      Term();
      ExpressionPrime();
      break;
   case MINUS:
      printf("(2) E' -> - T E'\n");
      Compare(MINUS);
      Term();
      ExpressionPrime();
      break;
   case RPAR:
   case EOI:
      printf("(3) E' -> e\n");
      break;
   default:
      ExpansionError("E'", Symb.type);
   }
}

void Term(void) {
   switch (Symb.type) {
   case IDENT:
   case LPAR:
      printf("(4) T -> F T'\n");
      Factor();
      TermPrime();
      break;
   default:
      ExpansionError("T", Symb.type);
   }
}

void TermPrime(void) {
   switch (Symb.type) {
   case TIMES:
      printf("(5) T' -> * F T'\n");
      Compare(TIMES);
      Factor();
      TermPrime();
      break;
   case DIVIDE:
      printf("(5) T' -> / F T'\n");
      Compare(DIVIDE);
      Factor();
      TermPrime();
      break;
   case PLUS:
   case MINUS:
   case RPAR:
   case EOI:
      printf("(6) T' -> e\n");
      break;
   default:
      ExpansionError("T'", Symb.type);
   }
}

void Factor(void) {
   switch (Symb.type) {
   case IDENT:
      printf("(7) F -> a\n");
      Compare(IDENT);
      break;
   case LPAR:
      printf("(8) F -> ( E )\n");
      Compare(LPAR);
      Expression();
      Compare(RPAR);
      break;
   default:
      ExpansionError("F", Symb.type);
   }
}

int initParser(char *fileName) {
  if(!initLexan(fileName)) return 0;
  Symb = readLexem();
  return 1;
}
