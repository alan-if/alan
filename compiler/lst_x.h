#ifndef _LST_X_H_
#define _LST_X_H_
/*----------------------------------------------------------------------*\

				LST.H
			      List Nodes

\*----------------------------------------------------------------------*/

#include "lst.h"


/* USE: */

/* DATA: */

extern void (*(dumpNodeTable[]))();


/* FUNCTIONS: */

/* Init the list of node dumping functions */
extern void initDumpNodeList();

/* Concat an element to a list */
extern List *concat(List *lst, void *elem, ListKind kind);

/* Combine two generic lists */
extern List *combine(List *lst1, List *lst2);

/* Dump a list of nodes */
extern void dulst(List *lst, ListKind nod);

/* Dump a list of lists of nodes */
extern void dulst2(List *lstlst, ListKind nod);

#endif
