/*----------------------------------------------------------------------*\

	syntax

\*----------------------------------------------------------------------*/
#include "syntax.h"

/* IMPORTS */
#include "word.h"
#include "msg.h"
#include "lists.h"


/* CONSTANTS */


/* PUBLIC DATA */
SyntaxEntry *stxs;      /* Syntax table pointer */


/* PRIVATE TYPES & DATA */


/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*======================================================================*/
ElementEntry *elementTreeOf(SyntaxEntry *stx) {
    return (ElementEntry *) pointerTo(stx->elms);
}

/*======================================================================*/
char *parameterNameInSyntax(int parameterNumber) {
    SyntaxEntry *syntax = findSyntaxTreeForVerb(verbWordCode);
    Aaddr *parameterNameTable = (Aaddr *)pointerTo(syntax->parameterNameTable);
    if (syntax->parameterNameTable != 0)
		return stringAt(parameterNameTable[parameterNumber-1]);
	else
		return NULL;
}

/*======================================================================*/
SyntaxEntry *findSyntaxTreeForVerb(int verbCode) {
    SyntaxEntry *stx;
    for (stx = stxs; !isEndOfArray(stx); stx++)
        if (stx->code == verbCode)
            return stx;
    /* No matching syntax */
    error(M_WHAT);
    return NULL;
}


