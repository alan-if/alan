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
#include "context.h"


/* DATA: */

extern int classCount;
extern int instanceCount;
extern SymNod *thingSymbol, *objectSymbol, *locationSymbol, *actorSymbol;


/* FUNCTIONS: */

/* List a message indicating a redefined symbol */
extern void redefined(Srcp *srcp,
		      SymNod *sym,
		      char str[]);

/* Initialise the symbol table with predefined classes etc. */
extern void initSymbols();

/* Create a new symbol node */
extern SymNod *newSymbol(char str[],
			 SymbolKind kind);

/* Check if an Id exists in this contest and if so if of an allowed kind */
extern SymNod *symcheck(
    IdNode *id,
    SymbolKind kind,
    Context *context
    );

/* Lookup a symbol, check in parameters first */
extern SymNod *lookup(char idString[]);

/* Inheritance of a class */
extern void setParent(SymNod *child, SymNod *parent);
extern SymNod *parentOf(SymNod *child);
extern Bool inheritsFrom(SymNod *child, SymNod *ancestor);
extern void inheritCheck(IdNode *id, char classOrInstance[], char className[]);
extern SymNod *findParameter(IdNode *parameterId, List *parameterSymbols);
extern void setParameters(SymNod *verb, List *parameters);
extern void numberAllAttributes(void);
extern void replicateInheritedAttributes(void);

#endif
