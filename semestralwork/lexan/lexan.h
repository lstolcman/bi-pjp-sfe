/* lexan.h */

typedef enum {IDENT, NUMB, PLUS, MINUS, LESS, LESS_OR_EQ, ASSIGN,
              kwIF, kwTHEN, kwELSE, EOI, ERR} LexSymbolType;

extern const char *symbTable[12];

#define MAX_IDENT_LEN 32

typedef struct LexicalSymbol {
  LexSymbolType type;
  char      ident[MAX_IDENT_LEN];  /* atribute of IDENT token */
  int       number;                 /* atribute of NUMB token */
} LexicalSymbol;

LexicalSymbol readLexem(void);
int initLexan(char*);
