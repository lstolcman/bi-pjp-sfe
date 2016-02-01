/* parser.cpp */

#include "tree.h"
#include "parser.h"
#include "lexan.h"
#include "tabsym.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Prog* Program();
void InitDecl();
void InitDecl();
void Decl();
void DeclProgram();
void DeclConst();
void DeclConstPrimed();
void DeclVar();
void DeclVarPrimed();
StatmList* CompoundStatement();
StatmList* CompoundStatementPrimed();
Statm* Statement();
Statm* ElsePart();
Expr* Condition();
Operator RelOp();
Expr* Expression();
Expr* ExpressionPrimed(Expr*);
Expr* Term();
Expr* TermPrimed(Expr*);
Expr* Factor();

LexicalSymbol Symb;

void CompareError(LexSymbolType s, int line)
{
	printf("Error while comparing, expected %s, got %s on line %d.\n", symbTable[s], symbTable[Symb.type], line);
	exit(1);
}

void ExpansionError(const char *nonterminal, LexSymbolType s, int line)
{
	printf("Error while expanding nonterminal %s, unexpected token %s, got %s on line %d.\n", nonterminal, symbTable[s], symbTable[Symb.type], line);
	exit(1);
}

void Compare(LexSymbolType s, int line)
{
	if (Symb.type == s)
	{
		Symb = readLexem();
	}
	else
	{
		CompareError(s, line);
	}
}

void Compare_IDENT(char *id, int line)
{
	if (Symb.type == IDENT)
	{
		strcpy(id, Symb.ident);
		Symb = readLexem();
	}
	else
	{
		CompareError(IDENT, line);
	}
}

void Compare_NUMB(int *h, int line)
{
	if (Symb.type == NUMB)
	{
		*h = Symb.number;
		Symb = readLexem();
	}
	else
	{
		CompareError(NUMB, line);
	}
}

Prog* Program()
{
	InitDecl();
	Decl();
	return new Prog(CompoundStatement());
}

void InitDecl()
{
	switch(Symb.type)
	{
	case kwPROGRAM:
		DeclProgram();
		break;

	default:
		CompareError(Symb.type, __LINE__);
	}
	Decl();
}

void Decl()
{
	switch (Symb.type)
	{
	case kwVAR:
		DeclVar();
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

void DeclProgram()
{
	char id[MAX_IDENT_LEN];
	Symb = readLexem();
	Compare_IDENT(id, __LINE__);
	Compare(SEMICOLON, __LINE__);
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
	DeclConstPrimed();
	Compare(SEMICOLON, __LINE__);
}

void DeclConstPrimed()
{
	if (Symb.type == COMMA)
	{
		char id[MAX_IDENT_LEN];
		int hod;
		Symb = readLexem();
		Compare_IDENT(id, __LINE__);
		Compare(EQ, __LINE__);
		Compare_NUMB(&hod, __LINE__);
		declConst(id, hod);
		DeclConstPrimed();
	}
}

void DeclVar()
{
	char id[MAX_IDENT_LEN];
	Symb = readLexem();
	Compare_IDENT(id, __LINE__);
	declVar(id);
	DeclVarPrimed();
	Compare(SEMICOLON, __LINE__);
}

void DeclVarPrimed()
{
	if (Symb.type == COMMA)
	{
		char id[MAX_IDENT_LEN];
		Symb = readLexem();
		Compare_IDENT(id, __LINE__);
		declVar(id);
		DeclVarPrimed();
	}
}

StatmList* CompoundStatement()
{
	Compare(kwBEGIN, __LINE__);
	Statm *p = Statement();
	StatmList *su = new StatmList(p, CompoundStatementPrimed());
	Compare(kwEND, __LINE__);
	return su;
}

StatmList* CompoundStatementPrimed()
{
	if (Symb.type == SEMICOLON)
	{
		Symb = readLexem();
		Statm *p = Statement();
		return new StatmList(p, CompoundStatementPrimed());
	}
	return 0;
}

Statm* Statement()
{
	switch (Symb.type)
	{
	case IDENT:
	{
		Var *var = new Var(varAddr(Symb.ident));
		Symb = readLexem();
		Compare(ASSIGN, __LINE__);
		return new Assign(var, Expression());
	}

	case kwWRITE:
		Symb = readLexem();
		return new Write(Expression());

	case kwREAD:
		Symb = readLexem();
		char id[MAX_IDENT_LEN];
		Compare_IDENT(id, __LINE__);
		return new Read(new Var(varAddr(id)));

	case kwIF:
	{
		Symb = readLexem();
		Expr *cond = Condition();
		Compare(kwTHEN, __LINE__);
		Statm *prikaz = Statement();
		return new If(cond, prikaz, ElsePart());
	}

	case kwWHILE:
	{
		Expr *cond;
		Symb = readLexem();
		cond = Condition();
		Compare(kwDO, __LINE__);
		return new While(cond, Statement());
	}

	case kwFOR:
	{
		Symb = readLexem();

		bool counting_up;
		Expr *value1;
		Expr *value2;

		Var *var = new Var(varAddr(Symb.ident));
		Symb = readLexem();
		Compare(ASSIGN, __LINE__);
		value1 = Expression();

		switch (Symb.type)
		{
		case kwTO:
			counting_up = true;
			break;

		case kwDOWNTO:
			counting_up = false;
			break;

		default:
			CompareError(Symb.type, __LINE__);
		}
		Symb = readLexem();
		value2 = Expression();
		Compare(kwDO, __LINE__);

		return new For(var, value1, value2, Statement(), counting_up);
	}

	case kwBEGIN:
		return CompoundStatement();

	default:
		return new Empty;
	}
}

Statm* ElsePart()
{
	if (Symb.type == kwELSE)
	{
		Symb = readLexem();
		return Statement();
	}
	return 0;
}

Expr* Condition()
{
	Expr *left = Expression();
	Operator op = RelOp();
	Expr *right = Expression();
	return new Bop(op, left, right);
}

Operator RelOp()
{
	switch (Symb.type)
	{
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

Expr* Expression()
{
	if (Symb.type == MINUS)
	{
		Symb = readLexem();
		return ExpressionPrimed(new UnMinus(Term()));
	}
	return ExpressionPrimed(Term());
}

Expr* ExpressionPrimed(Expr *du)
{
	switch (Symb.type)
	{
	case PLUS:
		Symb = readLexem();
		return ExpressionPrimed(new Bop(Plus, du, Term()));

	case MINUS:
		Symb = readLexem();
		return ExpressionPrimed(new Bop(Minus, du, Term()));

	default:
		return du;
	}
}

Expr* Term()
{
	return TermPrimed(Factor());
}

Expr* TermPrimed(Expr *du)
{
	switch (Symb.type)
	{
	case TIMES:
		Symb = readLexem();
		return TermPrimed(new Bop(Times, du, Factor()));

	case DIVIDE:
		Symb = readLexem();
		return ExpressionPrimed(new Bop(Divide, du, Factor()));

	default:
		return du;
	}
}

Expr* Factor()
{
	switch (Symb.type)
	{
	case IDENT:
		char id[MAX_IDENT_LEN];
		Compare_IDENT(id, __LINE__);
		return VarOrConst(id);

	case NUMB:
		int hodn;
		Compare_NUMB(&hodn, __LINE__);
		return new Numb(hodn);

	case LPAR:
	{
		Symb = readLexem();
		Expr *su = Expression();
		Compare(RPAR, __LINE__);
		return su;
	}

	default:
		ExpansionError("Factor", Symb.type, __LINE__);
		return 0;
	}
}

int initParser(char *fileName)
{
	if(!initLexan(fileName))
	{
		return 0;
	}
	Symb = readLexem();
	return 1;
}
