#ifndef _LIST_H_
#define _LIST_H_
/*----------------------------------------------------------------------*\

				List.h
			      List Nodes

\*----------------------------------------------------------------------*/

/* Use: */

#include "dump.h"


/* Types: */

typedef struct List {		/* GENERIC LISTS */
  struct List *next;		/* Pointer to next list node */
  struct List *tail;		/* Pointer to last list node */
  union {
    struct Id *id;
    char *string;
    void *anyType;
  } element;			/* Pointer to any type of element */
} List;


/* Data: */



/* Functions: */

#ifdef _PROTOTYPES_

/* Concat an element to a list */
extern List *concat(List *lst, void *elem);

/* Combine two generic lists */
extern List *combine(List *lst1, List *lst2);

/* Dump a list of nodes */
extern void dumpList(List *lst, NodeKind nod);

#else
extern List *concat();
extern List *combine();
extern void dumpList();
#endif

#endif
