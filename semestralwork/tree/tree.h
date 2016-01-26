/* tree.h */

#ifndef _NODE_
#define _NODE_

class Node {
public:
   virtual int Value() = 0;
};

class IntConst : public Node {
   int val;
public:
   IntConst(int c);
   virtual int Value();
};

class BinOp : public Node {
   char  op;
   Node  *left, *right;
public:
   BinOp(char, Node*, Node*);
   virtual int Value();
};

#endif

