/*----------------------------------------------------------------------*\

				LST.C
			Generic lists handling

\*----------------------------------------------------------------------*/

#include "lst_x.h"


/* IMPORT */
#include "util.h"
#include "dump.h"


/* PUBLIC DATA */

void (*(dumpNodeTable[LIST_LAST_KIND]))();


/* Import of dump functions to be used in dumpNodeTable */
extern void dumpClass();
extern void dumpInstance();
extern void dumpId();
extern void dustm();
extern void duext();
extern void dustx();
extern void duvrb();
extern void dumpAdd();
extern void duelm();
extern void dumpAttribute();
extern void dumpRestriction();


/*======================================================================

  initDumpNodeList()

*/
void initDumpNodeList()
{
  dumpNodeTable[LIST_ADD] = &dumpAdd;
  dumpNodeTable[LIST_CLA] = &dumpClass;
  dumpNodeTable[LIST_INS] = &dumpInstance;
  dumpNodeTable[LIST_ATR] = &dumpAttribute;
  dumpNodeTable[LIST_ID] = &dumpId;
  dumpNodeTable[LIST_STM] = &dustm;
  dumpNodeTable[LIST_EXT] = &duext;
  dumpNodeTable[LIST_ELM] = &duelm;
  dumpNodeTable[LIST_STX] = &dustx;
  dumpNodeTable[LIST_VRB] = &duvrb;
  dumpNodeTable[LIST_CNT] = &dumpPointer;
  dumpNodeTable[LIST_RES] = &dumpRestriction;
}




/*======================================================================

  insert()

  Insert an element into a list at the point. Can not insert at end
  (i.e. on NULL lists)

  */
void insert(List *thePoint, void *element, ListKind kind)
{
  List *newListNode;

  if (thePoint == NULL)
    syserr("Inserting an element in a NULL list!");
  if (kind != thePoint->kind)
    syserr("Inserting wrong kind of element in list!");

  /* Move the first element to a new list node */
  newListNode = concat(NULL, thePoint->element.atr, kind);

  newListNode->next = thePoint->next;
  thePoint->element.atr = element;
  thePoint->next = newListNode;
}


/*======================================================================

  concat()

  Generic list concatenation.

  */
List *concat(List *list,	/* IN - List to concat to */
	     void *element,	/* IN - Pointer to any element type */
	     ListKind kind)	/* IN - Which kind of list? */
{
  List *new;			/* The newly created list node */

  if (element == NULL) return(list);

  new = NEW(List);

  new->element.cla = (struct ClaNod *) element;
  new->kind = kind;

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
List *combine(List *list1,	/* IN - Lists to combine */
	      List *list2)
{
  if (list1 == NULL) return(list2);
  if (list2 == NULL) return(list1);

  list1->tail->next = list2;	/* Combine at end of list1 */
  list1->tail = list2->tail;	/* Tail of list2 is tail */
  return(list1);
}


/*======================================================================

  length()

  Of a list.

*/
int length(List *aList)
{
  int count = 0;
  List *thePoint;

  for (thePoint = aList; thePoint != NULL; thePoint = thePoint->next)
    count ++;
  return count;
}




/*----------------------------------------------------------------------

  dunod()

  Dump a particular node kind.

  */
static void dunod(void *nod, ListKind kind)
{
  if (dumpNodeTable[kind] == NULL) {
    put("*** Not implemented in DUMP. ***");
  } else
    dumpNodeTable[kind](nod);
}



/*======================================================================

  dulst()

  Dump a list of nodes

  */
void dulst(List *lst,		/* IN - The list */
	   ListKind class)	/* IN - Class of the elements */
{
  if (lst == NULL) {
    put("NULL");
    return;
  }
  
  put("LST: "); dumpPointer(lst); in();
  while (lst != NULL) {
    dunod((void *)lst->element.atr, class);
    lst = lst->next;
    if (lst != NULL) nl();
  }
  out();
}


/*======================================================================

  dulst2()

  Dump a list of lists of nodes

  */
void dulst2(List *lstlst,	/* IN - The list of lists*/
	    ListKind class)	/* IN - Class of the elements */
{
  if (lstlst == NULL) {
    put("NULL");
    return;
  }
  
  put("LST: "); dumpPointer(lstlst); in();
  while (lstlst != NULL) {
    dulst((void *)lstlst->element.lst, class);
    lstlst = lstlst->next;
    if (lstlst != NULL) nl();
  }
  out();
}


