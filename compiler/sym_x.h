#ifndef _SYM_X_H_
#define _SYM_X_H_
/*----------------------------------------------------------------------*\

				SYM.H
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/

#include "sym.h"

/* USE: */
#include "types.h"
#include "elm.h"
#include "lst.h"
#include "id.h"
#include "atr.h"


/* DATA: */
extern int classCount;
extern int instanceCount;


/* FUNCTIONS: */

/* List a message indicating a redefined symbol */
extern void redefined(Srcp *srcp,
		      SymNod *sym,
		      char str[]);

/* Initialise the symbol table with predefined classes etc. */
extern void initSymbols();

/* Create a new symbol node */
extern SymNod *newsym(char str[],
		      SymbolKind kind);

/* Check if an Id exists and if so if of an allowed kind in this context */
extern SymNod *symcheck(	/* OUT - Found symbol */
    ElmNod **elm,		/* OUT - Found parameter  */
    IdNode *id,			/* IN - The id to check */
    SymbolKind kind,		/* IN - Allowed symbol kind */
    List *pars			/* IN - Possible parameters in this context */
    );

/* Lookup a symbol */
extern SymNod *lookup(char idString[]);

/* Inheritance of a class */
extern void setParent(SymNod *child, SymNod *parent);
extern SymNod *parentOf(SymNod *child);
extern Bool inheritsFrom(SymNod *child, SymNod *ancestor);
extern void inheritCheck(IdNode *id, char classOrInstance[], char className[]);

extern void numberAllAttributes(void);
extern void replicateInheritedAttributes(SymNod *sym);

#endif
