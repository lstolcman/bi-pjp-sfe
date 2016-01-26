/* tabsym.cpp */

#include "tabsym.h"
#include <string.h>
#include <stdio.h>

struct TabElement {
   char *ident;
   TypeId type;
   int value;
   TabElement *next;
   TabElement(char *i, TypeId d, int h, TabElement *n);
};

TabElement::TabElement(char *i, TypeId d, int h, TabElement *n)
{
   ident = new char[strlen(i)+1];
   strcpy(ident, i);
   type = d; value = h; next = n;
}

static TabElement *TabSym;

static void error(char *id, const char *txt)
{
   printf("identifier %s: %s\n", id, txt);
}

void freeSymbTab() {
   TabElement *p = TabSym;
   while (p) {
      TabElement* tmp = p->next;
      delete[] p->ident;
      delete p;
      p = tmp;
   }
}

TabElement *searchId(char *id)
{
   TabElement *p = TabSym;
   while (p)
      if (strcmp(id,p->ident)==0) 
         return p;
      else
         p = p->next;
   return NULL;
}

void declConst(char *id, int val)
{
   TabElement *p = searchId(id);
   if (p) {
      error(id, "is declared again");
      return;
   }
   TabSym = new TabElement(id, ConstId, val, TabSym);
}

void declVar(char *id)
{
   static int free_address;
   TabElement *p = searchId(id);
   if (p) {
      error(id, "is declared again");
      return;
   }
   TabSym = new TabElement(id, VarId, free_address, TabSym);
   free_address++;
}

int varAddr(char *id)
{
   TabElement *p = searchId(id);
   if (!p) {
      error(id, "is not declared");
      return 0;
   } else if (p->type != VarId) {
      error(id, "is not a variable");
      return 0;
   } else
      return p->value;
}

TypeId varConstId(char *id, int *v)
{
   TabElement *p = searchId(id);
   if (p) {
      *v = p->value;
      return p->type;
   }
   error(id, "is not declared");
   return Undef;
}

