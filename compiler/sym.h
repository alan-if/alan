#ifndef _SYM_H_
#define _SYM_H_
/*----------------------------------------------------------------------*\

				SYM.H
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "types.h"


/* Types: */

typedef enum SymbolKind {
  CLASS_SYMBOL,
  INSTANCE_SYMBOL
} SymbolKind;


typedef struct SymNod {		/* SYMBOL TABLE ENTRY */
  SymbolKind kind;		/* What kind of symbol? */
  char *string;			/* Name of this entry */
  int code;			/* Internal code for this symbol in its kind */
  struct SymNod *low, *high;	/* Links to build a binary search tree */
  union {
    struct ClaNod *cla;
    struct InsNod *ins;
  } ref;
  union {
    struct {
      struct SymNod *parent;
    } cla;
    struct {
      struct SymNod *parent;
    } ins;
  } fields;
} SymNod;


/* Data: */
extern int classCount;
extern int instanceCount;


/* Functions: */

/* List a message indicating a redefined symbol */
extern void redefined(Srcp *srcp,
		      SymNod *sym,
		      char str[]);

/* Create a new symbol node */
extern SymNod *newsym(char str[],
		      SymbolKind kind,
		      void *ref);

/* Initialise the symbol table with predefined classes etc. */
extern void initSymbols();


/* Lookup a symbol */
extern SymNod *lookup(char symbol[]);

/* Inheritance of a class */
extern void setParent(SymNod *child, SymNod *parent);
extern SymNod *parentOf(SymNod *child);
extern Bool inheritsFrom(SymNod *child, SymNod *ancestor);
#endif
