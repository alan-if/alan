/*----------------------------------------------------------------------*\

				List.c

			Generic lists handling

\*----------------------------------------------------------------------*/

#include "types.h"
#include "List.h"

#include "dump.h"




/*======================================================================

  concat()

  Generic list concatenation.

  */
#ifdef _PROTOTYPES_
List *concat(List *list, void *element)
                		/* IN - List to concat to */
                   		/* IN - Pointer to any element type */
#else
List *concat(list, element)
     List *list;		/* IN - List to concat to */
     void *element;		/* IN - Pointer to any element type */
#endif
{
  List *new;			/* The newly created list node */

  if (element == NULL) return(list);

  new = NEW(List);

  new->element.anyType = (void *) element;

  new->next = NULL;
  if (list == NULL) {
    new->tail = new;		/* This node is tail */
    return(new);
  } else {
    list->tail->next = new;	/* Concat at end of list */
    list->tail = new;		/* New node is tail */
    return(list);
  }
}



/*======================================================================

  combine()

  Generic list combination.

  */
#ifdef _PROTOTYPES_
List *combine(List *list1, List *list2)
                         	/* IN - Lists to combine */
#else
List *combine(list1, list2)
     List *list1, *list2;	/* IN - Lists to combine */
#endif
{
  if (list1 == NULL) return(list2);
  if (list2 == NULL) return(list1);

  list1->tail->next = list2;	/* Combine at end of list1 */
  list1->tail = list2->tail;	/* Tail of list2 is tail */
  return(list1);
}



/*----------------------------------------------------------------------

  dumpNode()

  Dump a particular node.

  */
#ifdef _PROTOTYPES_
static void dumpNode(void *nod, NodeKind class)
#else
static void dumpNode(nod, class)
    void *nod;
    NodeKind class;
#endif
{
  switch (class) {
  default: put("Not implemented in DUMP."); nl(); break;
  }
}



/*======================================================================

  dumpList()

  Dump a list of nodes

  */
#ifdef _PROTOTYPES_
void dumpList(List *list,	/* IN - The list */
	      NodeKind kind)	/* IN - Kind of the elements */
#else
void dumpList(list, kind)
     List *list;
     NodeKind kind;
#endif
{
  if (list == NULL) {
    put("NULL");
    return;
  }
  
  put("LIST: "); dumpAddress(list); in();
  while (list != NULL) {
    dumpNode((void *)list->element.anyType, kind);
    list = list->next;
    if (list != NULL) nl();
  }
  out();
}


