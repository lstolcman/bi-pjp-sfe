/* lexan.h */

/**
 * Lexical elements that are recognised by Lexan
 */
typedef enum {
   IDENT, NUMB, PLUS, MINUS, TIMES, DIVIDE, 
   EQ, NEQ, LT, GT, LTE, GTE, LPAR, RPAR, ASSIGN,
   COMMA, SEMICOLON,
   kwVAR, kwCONST, kwBEGIN, kwEND, kwIF, kwTHEN, kwELSE, 
   kwWHILE, kwDO, kwWRITE, kwREAD, 
   EOI, ERR
} LexSymbolType;

extern const char *symbTable[30];

/**
 * maximal length of identifier
 */
#define MAX_IDENT_LEN 32

/**
 * Lexical element is represented by this structure. Basic type is stored and in case of IDENT and NUMB, fields ident and number contain some data
 */
typedef struct LexicalSymbol {
  LexSymbolType type;
  char      ident[MAX_IDENT_LEN];  /* atribute of IDENT token */
  int       number;                 /* atribute of NUMB token */
} LexicalSymbol;

LexicalSymbol readLexem(void);
int initLexan(char*);
