/*----------------------------------------------------------------------*\

				List.c

			Generic lists handling

\*----------------------------------------------------------------------*/

#include "types.h"
#include "alan.h"
#include "List.h"

#include "dump.h"




/*======================================================================

  append()

  Append an element to a list.

  */
#ifdef _PROTOTYPES_
List *append(List *list,	/* IN - List to append to */
	     void *element)	/* IN - Pointer to any element type */
#else
List *append(list, element)
     List *list;
     void *element;
#endif
{
  List *new;			/* The newly created list node */
  List *l;

  if (element == NULL)
    syserr("Appending a NULL element.");

  new = NEW(List);

  new->element.anyType = element;

  new->next = NULL;
  if (list == NULL) {
    return(new);
  } else {
    for (l = list; l->next; l = l->next); /* Go to end */
    l->next = new;		/* Append at end of list */
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
  List *l;

  if (list1 == NULL) return(list2);
  if (list2 == NULL) return(list1);
 
  for (l = list1; l->next; l = l->next); /* Go to end */
  l->next = list2;	/* Combine at end of list1 */
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


