/* tree.h */

#ifndef _TREE_
#define _TREE_

#include "../sfe-lang.h" //for tree type

#include "output.h"

class Node
{
public:
	virtual Node* Optimize()
	{
		return this;
	}
	virtual tree Translate() = 0;
	virtual ~Node()
	{
	}
	tree build_string_constant(const char*, int);
	tree build_print_integer_expr(location_t, tree);
};

class Expr : public Node
{
};

class Statm : public Node
{
};

class Var : public Expr
{
	int addr;
	bool rvalue;

public:
	Var(int, bool);
	virtual tree Translate();
};

class Numb : public Expr
{
	int value;

public:
	Numb(int);
	virtual tree Translate();
	int Value();
};

class Bop : public Expr
{
	Operator op;
	Expr *left, *right;

public:
	Bop(Operator, Expr*, Expr*);
	virtual ~Bop();
	virtual Node* Optimize();
	virtual tree Translate();
};

class UnMinus : public Expr
{
	Expr *expr;

public:
	UnMinus(Expr *e);
	virtual ~UnMinus();
	virtual Node* Optimize();
	virtual tree Translate();
};

class Assign : public Statm
{
	Var *var;
	Expr *expr;

public:
	Assign(Var*, Expr*);
	virtual ~Assign();
	virtual Node* Optimize();
	virtual tree Translate();
};

class Write : public Statm
{
	Expr *expr;

public:
	Write(Expr*);
	virtual ~Write();
	virtual Node* Optimize();
	virtual tree Translate();
};

class Read : public Statm
{
	Var *var;

public:
	Read(Var*);
	virtual ~Read();
	virtual Node* Optimize();
	virtual tree Translate();
};

class If : public Statm
{
	Expr *cond;
	Statm *thenstm;
	Statm *elsestm;

public:
	If(Expr*, Statm*, Statm*);
	virtual ~If();
	virtual Node* Optimize();
	virtual tree Translate();
};

class While : public Statm
{
	Expr *cond;
	Statm *body;

public:
	While(Expr*, Statm*);
	virtual ~While();
	virtual Node* Optimize();
	virtual tree Translate();
};

class StatmList : public Statm
{
	Statm *statm;
	StatmList *next;

public:
	StatmList(Statm*, StatmList*);
	virtual ~StatmList();
	virtual Node* Optimize();
	virtual tree Translate();
};

class Empty : public Statm
{
	virtual tree Translate()
	{
		tree empty_stmt = alloc_stmt_list();
		return empty_stmt;
	}
};

class Prog : public Node
{
	StatmList *stm;

public:
	Prog(StatmList*);
	virtual ~Prog();
	virtual Node* Optimize();
	virtual tree Translate();
};

Expr* VarOrConst(char*);

#endif
