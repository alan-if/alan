#ifndef _ID_X_H_
#define _ID_X_H_
/*----------------------------------------------------------------------*\

				ID_X.H
		       Identifier Nodes Export

\*----------------------------------------------------------------------*/

#include "id.h"

/* USE: */
#include "types.h"
#include "sym.h"


/* DATA: */


/* METHODS: */
extern char *generateIdName(void);
extern Bool isGeneratedId(IdNode *id);
extern IdNode *newId(Srcp srcp, char str[]);
extern List *newIdList(List *list, char *str);
extern Bool equalId(IdNode *id1, IdNode *id2);
extern void symbolizeId(IdNode *id);
extern IdNode *findIdInList(IdNode *theId, List *theList);
extern void generateId(IdNode *id);
extern void dumpId(IdNode *id);
#endif
