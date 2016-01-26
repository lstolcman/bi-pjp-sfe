/* output.h */

typedef enum {IntConst, BinOp, Stop} InstructionCode;

void Gener(InstructionCode, int);
void Execute(void);
