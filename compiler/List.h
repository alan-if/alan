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
  union {
    struct Id *id;
    struct Attribute *attribute;
    struct Class *class;
    struct Instance *instance;
    struct Statement *statement;
    char *string;
    void *anyType;
  } element;			/* Pointer to any type of element */
} List;


/* Data: */



/* Functions: */

#ifdef _PROTOTYPES_

/* Append an element to a list */
extern List *append(List *lst, void *elem);

/* Combine two generic lists */
extern List *combine(List *lst1, List *lst2);

/* Dump a list of nodes */
extern void dumpList(List *lst, NodeKind nod);

#else
extern List *append();
extern List *combine();
extern void dumpList();
#endif

#endif
