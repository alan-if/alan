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

extern int frameLevel;

extern int classCount;
extern int instanceCount;
extern int eventCount;
extern int verbCount;

extern Symbol *entitySymbol;
extern Symbol *thingSymbol;
extern Symbol *objectSymbol;
extern Symbol *locationSymbol;
extern Symbol *actorSymbol;
extern Symbol *literalSymbol;
extern Symbol *stringSymbol;
extern Symbol *integerSymbol;
extern Symbol *theHero;


/* FUNCTIONS: */

/* List a message indicating a redefined symbol */
extern void redefined(IdNode *id, Symbol *sym);

/* Initialise the symbol table with predefined classes etc. */
extern void initSymbols(void);

/* Create a new symbol node */
extern Symbol *newSymbol(IdNode *id, SymbolKind kind);

/* Check if an Id exists in this context and if so if of an allowed kind */
extern Symbol *symcheck(
    IdNode *id,
    SymbolKind kind,
    Context *context
    );

/* Lookup a symbol */
extern Symbol *lookup(char idString[]);
extern Script *lookupScript(Symbol *aSymbol, IdNode *scriptName);
extern Symbol *lookupParameter(IdNode *parameterId, List *parameterSymbols);
extern Bool isClass(Symbol *symbol);

extern void newFrame(void);
extern void deleteFrame(void);

/* Inheritance of a class */
extern void setParent(Symbol *child, Symbol *parent);
extern Symbol *parentOf(Symbol *child);
extern Bool inheritsFrom(Symbol *child, Symbol *ancestor);
extern void inheritCheck(IdNode *id, char classOrInstance[], char className[]);

extern void setParameters(Symbol *verb, List *parameters);

extern void numberAllAttributes(void);
extern void replicateInherited(void);
extern void dumpSymbols(void);

#endif
