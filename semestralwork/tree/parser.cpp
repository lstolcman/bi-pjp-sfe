/* parser.c */

#include "parser.h"
#include "lexan.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

Node *Expression(void);
Node *ExpressionPrime(Node*);
Node *Term(void);
Node *TermPrime(Node*);
Node *Factor(void);

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

int Srovnani_NUMB() {
   if (Symb.type == NUMB) {
      int shod = Symb.number;
      Symb = readLexem();
      return shod;
   } else
      CompareError(NUMB);
}

Node *Expression(void)
{
   /* E -> T E' */
   return ExpressionPrime(Term());
}

Node *ExpressionPrime(Node *Dvalue)
{
   switch (Symb.type) {
   case PLUS:
      /* E' -> + T E' */
      Symb = readLexem();
      return ExpressionPrime(new BinOp('+', Dvalue, Term()));
   case MINUS:
      /* E' -> - T E' */
      Symb = readLexem();
      return ExpressionPrime(new BinOp('-', Dvalue, Term()));
   case RPAR: case EOI:
      /* E' -> e */
      return Dvalue;
   default:
      ExpansionError((char*) "E'", Symb.type);
   }
}

Node *Term(void)
{
   /* T -> F T' */
   return TermPrime(Factor());
}

Node *TermPrime(Node *Dvalue)
{
   switch (Symb.type) {
   case TIMES:
      /* T' -> * F T' */
      Symb = readLexem();
      return TermPrime(new BinOp('*', Dvalue, Factor()));
   case DIVIDE: 
      /* T' -> / F T' */
      Symb = readLexem();
      return TermPrime(new BinOp('/', Dvalue, Factor()));
   case PLUS:
   case MINUS:
   case RPAR:
   case EOI:
      /* T' -> e */
      return Dvalue;
   default:
      ExpansionError((char*) "T'", Symb.type);
   }
}

Node *Factor(void)
{
   switch (Symb.type) {
   case NUMB: {
      /* F -> c */
      return new IntConst(Srovnani_NUMB());
   }
   case LPAR: {
      /* F -> ( E ) */
      Node *Evalue;
      Symb = readLexem();
      Evalue = Expression();
      Compare(RPAR);
      return Evalue;
   }
   default:
      ExpansionError((char*) "F", Symb.type);
   }
}

int initParser(char *fileName) {
  if(!initLexan(fileName)) return 0;
  Symb = readLexem();
  return 1;
}
