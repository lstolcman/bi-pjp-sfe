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

void Output(char *s) {
   printf("%s ", s);
}

void Expression(void)
{
   /* E -> T E' */
   Term();
   ExpressionPrime();
}

void ExpressionPrime(void)
{
   switch (Symb.type) {
   case PLUS:
      /* E' -> + T <+> E' */
      Symb = readLexem();
      Term();
      Output("+");
      ExpressionPrime();
      break;
   case RPAR:
   case EOI:
      /* E' -> e */
      break;
   default:
      ExpansionError("E'", Symb.type);
   }
}

void Term(void)
{
   /* T -> F T' */
   Factor();
   TermPrime();
}

void TermPrime(void)
{
   switch (Symb.type) {
   case TIMES:
      /* T' -> * F <*> T' */
      Symb = readLexem();
      Factor();
      Output("*");
      TermPrime();
      break;
   case PLUS:
   case RPAR:
   case EOI:
      /* T' -> e */
      break;
   default:
      ExpansionError("T'", Symb.type);
   }
}

void Factor(void)
{
   switch (Symb.type) {
   case IDENT:
      /* F -> a <a> */
      Output(Symb.ident);
      Symb = readLexem();
      break;
   case LPAR:
      /* F -> ( E ) */
      Symb = readLexem();
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
