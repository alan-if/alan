#ifndef _SYMBOL_H_
#define _SYMBOL_H_
/*----------------------------------------------------------------------*\

				Symbol.h
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "Id.h"
#include "List.h"


/* Types: */

typedef enum SymbolKind {
  CLASS_SYMBOL,
  INSTANCE_SYMBOL,
  VERB_SYMBOL,
  DIRECTION_SYMBOL,
  ERROR_SYMBOL
} SymbolKind;

typedef struct Symbol {		/* SYMBOL TABLE ENTRY */
  SymbolKind kind;		/* Which kind of symbol */
  char *string;			/* Name of this entry */
  int code;			/* Internal code for this class */
  struct Symbol *lower, *higher; /* Links */
  union {
    struct Class *class;	/* For CLASS symbols */
    struct Instance *instance;	/* For INSTANCE symbols */
  } info;
} Symbol;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new symbol node */
extern Symbol *newSymbol(Id *id,
			 SymbolKind kind);

/* Convert a symbol type to a character string */
extern char *symbolKindString(SymbolKind kind);

/* Lookup a symbol */
extern Symbol *lookup(char symbol[]);

/* Check that a symbol is of a particular kind */
extern Symbol *symbolCheck(Id *id, SymbolKind kind);

/* Check that a symbol (class or instance) inherits a particular class */
extern void classCheck(Id *id, char className[]);

/* Does the identifier inherit the indicate class? */
extern Bool isA(Id *id, char className[]);

/* Does any of the identifiers inherit the indicate class? */
extern Bool anyIsA(List *ids, char className[]);

#else
extern Symbol *newSymbol();
extern char *symbolKindString();
extern Symbol *lookup();
extern Symbol *symbolCheck();
extern void classCheck();
extern Bool isA();
extern Bool anyIsA();
#endif

#endif
