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
extern Symbol *messageVerbSymbol;


/* FUNCTIONS: */

extern void redefined(IdNode *id, Symbol *sym);
extern void initSymbols(void);
extern void createMessageVerb();

extern Symbol *newSymbol(IdNode *id, SymbolKind kind);
extern Symbol *newInstanceSymbol(IdNode *id, Properties *props, Symbol *parent);
extern Symbol *newClassSymbol(IdNode *id, Properties *props, Symbol *parent);
extern Symbol *newVerbSymbol(IdNode *id);
extern Symbol *symcheck(IdNode *id, SymbolKind kind, Context *context);

/* Lookup a symbol */
extern Symbol *lookup(char idString[]);
extern Script *lookupScript(Symbol *aSymbol, IdNode *scriptName);
extern Symbol *lookupParameter(IdNode *parameterId, List *parameterSymbols);
extern Bool isClass(Symbol *symbol);
extern Bool symbolIsContainer(Symbol *symbol);
extern Symbol *contentOfSymbol(Symbol *symbol);
extern void newFrame(void);
extern void deleteFrame(void);

/* Inheritance of a class */
extern void setParent(Symbol *child, Symbol *parent);
extern Symbol *parentOf(Symbol *child);
extern Bool inheritsFrom(Symbol *child, Symbol *ancestor);
extern void inheritCheck(IdNode *id, char *what, char classOrInstance[], char className[]);

extern void setParameters(Symbol *verb, List *parameters);

extern TypeKind classToType(Symbol *symbol);
extern Symbol *definingSymbolOfAttribute(Symbol *symbol, IdNode *id);
extern Attribute *findInheritedAttribute(Symbol *symbol, IdNode *id);
extern void numberAllAttributes(void);
extern void replicateInherited(void);
extern void dumpSymbols(void);

#endif
