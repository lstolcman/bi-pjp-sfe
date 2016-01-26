/* parser.c */

#include "parser.h"
#include "lexan.h"
#include "output.h"
#include <stdio.h>
#include <stdlib.h>

void Start(void);
void Expression(void);
void ExpressionPrime(void);
void Term(void);
void TermPrime(void);
void Factor(void);

LexicalSymbol Symb;

void ChybaSrovnani(LexSymbolType s) {
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
      ChybaSrovnani(s);
}

int Compare_NUMB() {
   if (Symb.type == NUMB) {
      int shod = Symb.number;
      Symb = readLexem();
      return shod;
   } else
      ChybaSrovnani(NUMB);
   
   return 0;
}

void Start(void)
{
   /* S -> E Stop */
   Expression();
   Gener(Stop, 0);
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
      /* E' -> + T BinOp E' */
      Symb = readLexem();
      Term();
      Gener(BinOp, '+');   /* BinOp.dop = '+' */
      ExpressionPrime();
      break;
   case MINUS:
      /* E' -> - T BinOp E' */
      Symb = readLexem();
      Term();
      Gener(BinOp, '-');   /* BinOp.dop = '-' */
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
      /* T' -> * F BinOp T' */
      Symb = readLexem();
      Factor();
      Gener(BinOp, '*');      /* BinOp.dop = '*' */
      TermPrime();
      break;
   case DIVIDE:
      /* T' -> / F BinOp T' */
      Symb = readLexem();
      Factor();
      Gener(BinOp, '/');      /* BinOp.dop = '/' */
      TermPrime();
      break;
   case PLUS:
   case MINUS:
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
   case NUMB: {
      /* F -> c IntKonst */
      int cshod = Compare_NUMB(); 
      Gener(IntConst, cshod);
      break;
   }
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
