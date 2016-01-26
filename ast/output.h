/* output.h */

#ifndef _OUTPUT_
#define _OUTPUT_

enum InstrType {
	TA, TC, BOP, UNM, DR, ST, IFJ, JU, WRT, RD, DUP, SWAP, STOP
};
enum Operator {
	Plus, Minus, Times, Divide,
	Eq, NotEq, Less, Greater, LessOrEq, GreaterOrEq,
	Error
};

int Gener(InstrType, int = 0);
void GenTR(char*);
void PutIC(int);
int GetIC();
void Print();
void Run();

#endif
