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
    struct List *list;
    struct Statement *statement;
    char *string;
    void *anyType;
  } element;			/* Pointer to any type of element */
} List;


/* Data: */



/* Functions: */

#ifdef _PROTOTYPES_

/* Prepend an element to a list */
extern List *prepend(void *element, List *list);

/* Append an element to a list */
extern List *append(List *list, void *element);

/* Return number of elements in a list */
extern int listLength(List *list);

/* Combine two generic lists */
extern List *combine(List *list1, List *list2);

/* Dump a list of nodes */
extern void dumpList(List *list, NodeKind node);

#else
extern List *prepend();
extern List *append();
extern int listLength();
extern List *combine();
extern void dumpList();
#endif

#endif
