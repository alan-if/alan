#ifndef _SYM_H_
#define _SYM_H_
/*----------------------------------------------------------------------*\

				SYM.H
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "types.h"

#include "elm.h"
#include "lst.h"


/* Types: */

typedef enum SymbolKind {
  CLASS_SYMBOL,
  INSTANCE_SYMBOL,
  VERB_SYMBOL
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

/* Check if an Id exists and if so if of an allowed kind in this context */
extern SymNod *symcheck(	/* OUT - Found symbol */
    ElmNod **elm,		/* OUT - Found parameter  */
    char *string,		/* IN - The Id to check */
    SymbolKind kind,		/* IN - Allowed identifier kind */
    List *pars			/* IN - Possible parameters valid in this context */
    );

/* Lookup a symbol */
extern SymNod *lookup(char symbol[]);

/* Inheritance of a class */
extern void setParent(SymNod *child, SymNod *parent);
extern SymNod *parentOf(SymNod *child);
extern Bool inheritsFrom(SymNod *child, SymNod *ancestor);
#endif
