/* parser.c */

#include "parser.h"
#include "lexan.h"
#include <stdio.h>
#include <stdlib.h>

void Start(void);
void Expression(int*);
void ExpressionPrime(int, int*);
void Term(int*);
void TermPrime(int, int*);
void Factor(int*);

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

void Compare_NUMB(int *sval) {
   if (Symb.type == NUMB) {
      *sval = Symb.number;
      Symb = readLexem();
   } else
      CompareError(NUMB);
}

void Output(int dval) {
   printf("%d\n", dval);
}

void Start(void)
{
   /* S -> E v */
   int Esval;                 /* variable for E.sval */
   Expression(&Esval);
   Output(Esval);             /* v.dval = E.sval */
}

void Expression(int *sval)
{
   /* E -> T E' */
   int Tsval, ECsval;         /* variables for T.sval and E'.sval */
   Term(&Tsval);
   ExpressionPrime(Tsval, &ECsval);  /* E'.dval = T.sval */
   *sval = ECsval;            /* E.sval = E'.sval */
}

void ExpressionPrime(int dval, int *sval)
{
   switch (Symb.type) {
   case PLUS: {
      /* E' -> + T E' */
      int Tsval, ECsval;      /* variables for T.sval and E'.sval */
      Symb = readLexem();
      Term(&Tsval);
      ExpressionPrime(dval+Tsval, &ECsval);   /* E'1.dval = E'0.dval + T.sval */
      *sval = ECsval;         /* E'0.sval = E'1.sval */ 
      break;
   }
   case MINUS: {
      /* E' -> - T E' */
      int Tsval, ECsval;      /* variable for T.sval and E'.sval */
      Symb = readLexem();
      Term(&Tsval);
      ExpressionPrime(dval-Tsval, &ECsval);   /* E'1.dval = E'0.dval - T.sval */
      *sval = ECsval;         /* E'0.sval = E'1.sval */
      break;
   }
   case RPAR:
   case EOI:
      /* E' -> e */
      *sval = dval;           /* E'.sval = E'.dval */
      break;
   default:
      ExpansionError("E'", Symb.type);
   }
}

void Term(int *sval)
{
   /* T -> F T' */
   int Fsval, TCval;
   Factor(&Fsval);
   TermPrime(Fsval, &TCval);   /* T'.dval = F.sval */
   *sval = TCval;            /* T.sval = T'.sval */
}

void TermPrime(int dval, int *sval)
{
   switch (Symb.type) {
   case TIMES: {
      /* T' -> * F T' */
      int Fsval, TCval;
      Symb = readLexem();
      Factor(&Fsval);
      TermPrime(dval*Fsval, &TCval); /* T'1.dval = T'0.dval * F.sval */
      *sval = TCval;         /* T'0.sval = T'1.sval */
      break;
   }
   case DIVIDE: {
      /* T' -> / F T' */
      int Fsval, TCval;
      Symb = readLexem();
      Factor(&Fsval);
      TermPrime(dval/Fsval, &TCval); /* T'1.dval = T'0.dval / F.sval */
      *sval = TCval;         /* T'0.sval = T'1.sval */
      break;
   }
   case PLUS:
   case MINUS:
   case RPAR:
   case EOI:
      /* T' -> e */
      *sval = dval;           /* TC.sval = TC.dval */
      break;
   default:
      ExpansionError("T'", Symb.type);
   }
}

void Factor(int *sval)
{
   switch (Symb.type) {
   case NUMB: {
      /* F -> c */
      int csval;
      Compare_NUMB(&csval); 
      *sval = csval;          /* F.sval = c.sval */
      break;
   }
   case LPAR: {
      /* F -> ( E ) */
      int Esval;
      Symb = readLexem();
      Expression(&Esval);
      Compare(RPAR);
      *sval = Esval;          /* F.sval = E.sval */
      break;
   }
   default:
      ExpansionError("F", Symb.type);
   }
}

int initParser(char *fileName) {
  if(!initLexan(fileName)) return 0;
  Symb = readLexem();
  return 1;
}
