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
#include "scr.h"


/* DATA: */

extern int classCount;
extern int instanceCount;
extern int eventCount;
extern int verbCount;

extern Symbol *thingSymbol, *objectSymbol, *locationSymbol, *actorSymbol, *theHero;


/* FUNCTIONS: */

/* List a message indicating a redefined symbol */
extern void redefined(IdNode *id, Symbol *sym);

/* Initialise the symbol table with predefined classes etc. */
extern void initSymbols(void);

/* Create a new symbol node */
extern Symbol *newSymbol(IdNode *id,
			 SymbolKind kind);

/* Check if an Id exists in this context and if so if of an allowed kind */
extern Symbol *symcheck(
    IdNode *id,
    SymbolKind kind,
    Context *context
    );

/* Lookup a symbol, check in parameters first */
extern Symbol *lookup(char idString[]);

extern Script *lookupScript(Symbol *aSymbol, IdNode *scriptName);

/* Inheritance of a class */
extern void setParent(Symbol *child, Symbol *parent);
extern Symbol *parentOf(Symbol *child);
extern Bool inheritsFrom(Symbol *child, Symbol *ancestor);
extern void inheritCheck(IdNode *id, char classOrInstance[], char className[]);
extern Symbol *lookupParameter(IdNode *parameterId, List *parameterSymbols);
extern void setParameters(Symbol *verb, List *parameters);
extern void numberAllAttributes(void);
extern void replicateInherited(void);
extern void dumpSymbols(void);

#endif
