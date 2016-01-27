/* tabsym.cpp */

#include "tabsym.h"
#include <string.h>
#include <stdio.h>



TabElement::TabElement(char *i, TypeId d, tree nd, TabElement *n)
{
	ident = new char[strlen(i)+1];
	strcpy(ident, i);
	type = d;
	node = nd;
	next = n;
}

static TabElement *TabSym;

TabElement* retTabSym(void)
{
	return TabSym;
}

static void error(char *id, const char *txt)
{
	printf("identifier %s: %s\n", id, txt);
}

void freeSymbTab()
{
	TabElement *p = TabSym;

	while (p)
	{
		TabElement *tmp = p->next;
		delete[] p->ident;
		delete p;
		p = tmp;
	}
}

TabElement* searchId(char *id)
{
	TabElement *p = TabSym;

	while (p)
	{
		if (strcmp(id,p->ident)==0)
		{
			return p;
		}
		else
		{
			p = p->next;
		}
	}
	return NULL;
}

void declConst(char *id, int val)
{
	TabElement *p = searchId(id);

	if (p)
	{
		error(id, "is declared again");
		return;
	}

	TabSym = new TabElement(id, ConstId, build_int_cstu(integer_type_node, val), TabSym);
}

void declVar(char *id)
{
	static int free_address;
	TabElement *p = searchId(id);

	if (p)
	{
		error(id, "is declared again");
		return;
	}

	// build node GENERIC
	tree declaration = build_decl(UNKNOWN_LOCATION, VAR_DECL, get_identifier(id), integer_type_node);
	TREE_ADDRESSABLE(declaration) = true;
	TREE_USED(declaration) = true;
	DECL_INITIAL(declaration) = build_int_cstu(integer_type_node, 0);

	// set as global variable
	TREE_STATIC(declaration) = true;
	TREE_PUBLIC(declaration) = true;

	TabSym = new TabElement(id, VarId, declaration, TabSym);

	free_address++;
}

tree varAddr(char *id)
{
	TabElement *p = searchId(id);

	if (!p)
	{
		error(id, "is not declared");
		return 0;
	}
	else
	if (p->type != VarId)
	{
		error(id, "is not a variable");
		return 0;
	}
	else
	{
		return p->node;
	}
}

TypeId varConstId(char *id, tree *t)
{
	TabElement *p = searchId(id);

	if (p)
	{
		*t = p->node;
		return p->type;
	}
	error(id, "is not declared");
	return Undef;
}
