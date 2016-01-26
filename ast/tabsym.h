/* tabsym.h */

enum TypeId {Undef, VarId, ConstId};

void declConst(char*, int);
void declVar(char*);
int varAddr(char*);
TypeId varConstId(char*, int*);
void freeSymbTab();
