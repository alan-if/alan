/*----------------------------------------------------------------------*\

				List.c

			Generic lists handling

\*----------------------------------------------------------------------*/

#include "types.h"
#include "alan.h"
#include "List.h"

#include "Alternative.h"
#include "Attribute.h"
#include "Check.h"
#include "Class.h"
#include "Instance.h"
#include "Statement.h"
#include "Verb.h"

#include "dump.h"




/*======================================================================

  prepend()

  Prepend an element to a list.

  */
#ifdef _PROTOTYPES_
List *prepend(void *element,	/* IN - Pointer to any element type */
	      List *list)	/* IN - List to prepend to */
#else
List *prepend(element, list)
     void *element;
     List *list;
#endif
{
  List *new;			/* The newly created list node */

  if (element == NULL)
    syserr("Prepending a NULL element.");

  new = NEW(List);

  new->element.anyType = element;

  new->next = list;
  return new;
}



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

  listLength()

  Return number of elements in a list.

  */
#ifdef _PROTOTYPES_
int listLength(List *list)	/* IN - List to count */
#else
int listLength(list)
     List *list;
#endif
{
  int i = 0;

  while (list != NULL) {
    i++;
    list = list->next;
  }
  return i;
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



/*======================================================================

  copyList()

  Generic list combination.

  */
#ifdef _PROTOTYPES_
List *copyList(List *list)	/* IN - Lists to copy */
#else
List *copyList(list)
     List *list;
#endif
{
  List *new, *oldList, *newList;

  if (list == NULL) return NULL;

  new, newList = NEW(List);
  for (oldList = list; oldList; oldList = oldList->next) {
    newList->element.anyType = oldList->element.anyType;
    if (oldList->next)
      newList->next = NEW(List);
    newList = newList->next;
  }
  return new;
}



/*----------------------------------------------------------------------

  dumpNode()

  Dump a particular node.

  */
#ifdef _PROTOTYPES_
static void dumpNode(void *node, NodeKind kind)
#else
static void dumpNode(node, kind)
    void *nod;
    NodeKind kind;
#endif
{
  switch (kind) {
  case ADDRESSLISTLIST_NODE: dumpList(node, ADDRESSLIST_NODE); break;
  case ADDRESSLIST_NODE: dumpList(node, ADDRESS_NODE); break;
  case ADDRESS_NODE: dumpAddress(node); break;
  case ALTERNATIVE_NODE: dumpAlternative(node); break;
  case ATTRIBUTE_NODE: dumpAttribute(node); break;
  case CHECK_NODE: dumpCheck(node); break;
  case CLASS_NODE: dumpClass(node); break;
  case ID_NODE: dumpId(node); break;
  case INSTANCE_NODE: dumpInstance(node); break;
  case STATEMENT_NODE: dumpStatement(node); break;
  case VERB_NODE: dumpVerb(node); break;
  default: put("Not implemented in dumpList()."); break;
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


