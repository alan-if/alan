#ifndef _SYMBOL_H_
#define _SYMBOL_H_
/*----------------------------------------------------------------------*\

				Symbol.h
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "Id.h"


/* Types: */

typedef enum SymbolKind {
  CLASS_SYMBOL,
  INSTANCE_SYMBOL,
  VERB_SYMBOL,
  DIRECTION_SYMBOL,
  SCRIPT_SYMBOL,
  UNKNOWN_SYMBOL
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

/* Lookup a symbol */
extern Symbol *lookup(char symbol[]);

#else
extern Symbol *newSymbol();
extern Symbol *lookup();
#endif

#endif
