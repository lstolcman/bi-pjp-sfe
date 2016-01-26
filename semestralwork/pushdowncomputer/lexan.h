/* lexan.h */

typedef enum {IDENT, NUMB, PLUS, MINUS, TIMES, DIVIDE, LPAR, RPAR, EOI, ERR} LexSymbolType;

extern const char *symbTable[10];

#define MAX_IDENT_LEN 32

typedef struct LexicalSymbol {
  LexSymbolType type;
  char      ident[MAX_IDENT_LEN];  /* atribute of IDENT token */
  int       number;                 /* atribute of NUMB token */
} LexicalSymbol;

LexicalSymbol readLexem(void);
int initLexan(char*);
