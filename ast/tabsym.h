/* tabsym.h */







#include "../sfe-lang.h" //for tree type



enum TypeId {Undef, VarId, ConstId};




struct TabElement
{
	char *ident;
	TypeId type;
	tree node;
	TabElement *next;
	TabElement(char *i, TypeId d, tree nd, TabElement *n);
};





void declConst(char*, int);
void declVar(char*);
tree varAddr(char*);
TypeId varConstId(char*, tree*);
void freeSymbTab();


TabElement* retTabSym(void);
