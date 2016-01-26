/* output.c */

#include "output.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct {
   InstructionCode code;
   int          operand;
} Instruction;

#define MAX_PROGRAM_SIZE 1000

static Instruction Program[MAX_PROGRAM_SIZE];

void Gener(InstructionCode code, int op)
{
   static int ci;     /* instruction counter */
   if (ci == MAX_PROGRAM_SIZE) {
      printf("Program to long.\n");
      exit(1);
   }
   Program[ci].code = code;
   Program[ci].operand = op;
   ci++;
}

void Execute(void)
{
   int z[100];    /* pushdown store */
   int v = -1;    /* index of top of the pushdown store */
   int ci = 0;    /* instruction counter */
   
   for (;;) {
      switch (Program[ci].code) {
      case IntConst:
         z[++v] = Program[ci].operand;
         break;
      case BinOp: {
         int p = z[v--];
         int l = z[v--];
         switch (Program[ci].operand) {
         case '+':
            z[++v] = l + p;
            break;
         case '-':
            z[++v] = l - p;
            break;
         case '*':
            z[++v] = l * p;
            break;
         case '/':
            z[++v] = l / p;
            break;
         }
         break;
      }
      case Stop:
         printf("\n%d\n", z[v]);
         return;
      }
      ci++;
   }
}
