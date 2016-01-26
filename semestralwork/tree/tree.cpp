/* tree.cpp */

#include "tree.h"

IntConst::IntConst(int c)
{
   val = c;
}

int IntConst::Value()
{
   return val;
}

BinOp::BinOp(char o, Node *l, Node *r)
{
   op = o; left = l; right = r;
}

int BinOp::Value()
{
   int l = left->Value();
   int r = right->Value();
   switch (op) {
   case '+':
      return l+r;
   case '-':
      return l-r;
   case '*':
      return l*r;
   case '/':
      return l/r;
   }
}
