/* parser.cpp */

#include "parser.h"
#include "lexan.h"
#include "tabsym.h"
#include "output.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Program();
void Decl();
void DeclConst();
void DeclConstPrime();
void DeclProm();
void DeclPromPrime();
void CompoundStatement();
void CompoundStatementPrime();
void Statement();
void ElsePart(int);
void Condition();
Operator RelOp();
void Expression();
void ExpressionPrimed();
void Term();
void TermPrime();
void Factor();

LexicalSymbol Symb;

void CompareError(LexSymbolType s, int line) {
   printf("Error while comparing, expected %s, got %s on line %d.\n", symbTable[s], symbTable[Symb.type], line);
   exit(1);
}

void ExpansionError(const char* nonterminal, LexSymbolType s, int line) {
   printf("Error while expanding nonterminal %s, unexpected token %s, got %s on line %d.\n", nonterminal, symbTable[s], symbTable[Symb.type], line);
   exit(1);
}

void Compare(LexSymbolType s, int line) {
   if (Symb.type == s)
      Symb = readLexem();
   else
      CompareError(s, line);
}

void Compare_IDENT(char *id, int line)
{
   if (Symb.type == IDENT) {
      strcpy(id, Symb.ident);
      Symb = readLexem();
   } else
      CompareError(IDENT, line);
}

void Compare_NUMB(int *h, int line)
{
   if (Symb.type == NUMB) {
      *h = Symb.number;
      Symb = readLexem();
   } else
      CompareError(NUMB, line);
}

void Program()
{
   Decl();
   CompoundStatement();
   Gener(STOP);
}

void Decl()
{
   switch (Symb.type) {
   case kwVAR:
      DeclProm();
      Decl();
      break;
   case kwCONST:
      DeclConst();
      Decl();
      break;
   default:
      ;
   }
}

void DeclConst()
{
   char id[MAX_IDENT_LEN];
   int hod;
   Symb = readLexem();
   Compare_IDENT(id, __LINE__);
   Compare(EQ, __LINE__);
   Compare_NUMB(&hod, __LINE__);
   declConst(id, hod);
   DeclConstPrime();
   Compare(SEMICOLON, __LINE__);
}

void DeclConstPrime()
{
   if (Symb.type == COMMA) {
      char id[MAX_IDENT_LEN];
      int hod;
      Symb = readLexem();
      Compare_IDENT(id, __LINE__);
      Compare(EQ, __LINE__);
      Compare_NUMB(&hod, __LINE__);
      declConst(id, hod);
      DeclConstPrime();
   }
}

void DeclProm()
{
   char id[MAX_IDENT_LEN];
   Symb = readLexem();
   Compare_IDENT(id, __LINE__);
   declVar(id);
   DeclPromPrime();
   Compare(SEMICOLON, __LINE__);
}

void DeclPromPrime()
{
   if (Symb.type == COMMA) {
      char id[MAX_IDENT_LEN];
      Symb = readLexem();
      Compare_IDENT(id, __LINE__);
      declVar(id);
      DeclPromPrime();
   }
}

void CompoundStatement()
{
   Compare(kwBEGIN, __LINE__);
   Statement();
   CompoundStatementPrime();
   Compare(kwEND, __LINE__);
}

void CompoundStatementPrime()
{
   if (Symb.type == SEMICOLON) {
      Symb = readLexem();
      Statement();
      CompoundStatementPrime();
   }
}

void Statement()
{
   switch (Symb.type) {
   case IDENT: {
      Gener(TA, varAddr(Symb.ident));
      Symb = readLexem();
      Compare(ASSIGN, __LINE__);
      Expression();
      Gener(ST);
      break;
   }
   case kwWRITE:
      Symb = readLexem();
      Expression();
      Gener(WRT);
      break;
   case kwREAD:
      Symb = readLexem();
      char id[MAX_IDENT_LEN];
      Compare_IDENT(id, __LINE__);
      Gener(TA, varAddr(id));
      Gener(RD);
      Gener(ST);
      break;
   case kwIF: {
      Symb = readLexem();
      Condition();
      int adrIFJ = Gener(IFJ);
      Compare(kwTHEN, __LINE__);
      Statement();
      ElsePart(adrIFJ);
      break;
   }
   case kwWHILE: {
      int a1 = GetIC();
      Symb = readLexem();
      Condition();
      int aIFJ = Gener(IFJ);
      Compare(kwDO, __LINE__);
      Statement();
      Gener(JU, a1);
      PutIC(aIFJ);
      break;
   }
   case kwBEGIN:
      CompoundStatement();
      break;
   default:
      break;
   }
}

void ElsePart(int adrIFJ)
{
   if (Symb.type == kwELSE) {
      Symb = readLexem();
      int adrJU = Gener(JU);
      PutIC(adrIFJ);
      Statement();
      PutIC(adrJU);
   } else {
      PutIC(adrIFJ);
   }
}

void Condition()
{
   Expression();
   Operator op = RelOp();
   Expression();
   Gener(BOP, op);
}

Operator RelOp()
{
   switch (Symb.type) {
   case EQ:
      Symb = readLexem();
      return Eq;
   case NEQ:
      Symb = readLexem();
      return NotEq;
   case LT:
      Symb = readLexem();
      return Less;
   case GT:
      Symb = readLexem();
      return Greater;
   case LTE:
      Symb = readLexem();
      return LessOrEq;
   case GTE:
      Symb = readLexem();
      return GreaterOrEq;
   default:
      ExpansionError("RelOp", Symb.type, __LINE__);
      return Error;
   }
}

void Expression()
{
   if (Symb.type == MINUS) {
      Symb = readLexem();
      Term();
      Gener(UNM);
      ExpressionPrimed();
   } else {
      Term();
      ExpressionPrimed();
   }
}

void ExpressionPrimed()
{
   switch (Symb.type) {
   case PLUS:
      Symb = readLexem();
      Term();
      Gener(BOP, Plus);
      ExpressionPrimed();
      break;
   case MINUS:
      Symb = readLexem();
      Term();
      Gener(BOP, Minus);
      ExpressionPrimed();
      break;
   default:
      break;
   }
}

void Term()
{
   Factor();
   TermPrime();
}

void TermPrime()
{
   switch (Symb.type) {
   case TIMES:
      Symb = readLexem();
      Factor();
      Gener(BOP, Times);
      TermPrime();
      break;
   case DIVIDE:
      Symb = readLexem();
      Factor();
      Gener(BOP, Divide);
      TermPrime();
      break;
   default:
      break;
   }
}

void Factor()
{
   switch (Symb.type) {
   case IDENT:
      char id[MAX_IDENT_LEN];
      Compare_IDENT(id, __LINE__);
      GenTR(id);
      break;
   case NUMB:
      int hodn;
      Compare_NUMB(&hodn, __LINE__);
      Gener(TC, hodn);
      break;
   case LPAR: {
      Symb = readLexem();
      Expression();
      Compare(RPAR, __LINE__);
      break;
   }
   default:
      ExpansionError("Factor", Symb.type, __LINE__);
   }
}

int initParser(char *fileName) {
  if(!initLexan(fileName)) return 0;
  Symb = readLexem();
  return 1;
}
