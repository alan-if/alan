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
extern void dualt();
extern void duchk();
extern void duelm();
extern void duext();
extern void dumpAdd();
extern void dumpAttribute();
extern void dumpClass();
extern void dumpId();
extern void dumpInstance();
extern void dumpRestriction();
extern void dumpScript();
extern void dustm();
extern void dustx();
extern void dumpStep();
extern void dumpVerb();


/*======================================================================

  initDumpNodeList()

*/
void initDumpNodeList()
{
  dumpNodeTable[LIST_ADD] = &dumpAdd;
  dumpNodeTable[LIST_ALT] = &dualt;
  dumpNodeTable[LIST_ATR] = &dumpAttribute;
  dumpNodeTable[LIST_CHK] = &duchk;
  dumpNodeTable[LIST_CLA] = &dumpClass;
  dumpNodeTable[LIST_CNT] = &dumpPointer;
  dumpNodeTable[LIST_ELM] = &duelm;
  dumpNodeTable[LIST_EXT] = &duext;
  dumpNodeTable[LIST_ID] = &dumpId;
  dumpNodeTable[LIST_INS] = &dumpInstance;
  dumpNodeTable[LIST_RES] = &dumpRestriction;
  dumpNodeTable[LIST_STM] = &dustm;
  dumpNodeTable[LIST_STX] = &dustx;
  dumpNodeTable[LIST_VRB] = &dumpVerb;
  dumpNodeTable[LIST_SCR] = &dumpScript;
  dumpNodeTable[LIST_STP] = &dumpStep;
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


/*======================================================================*/
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




/*----------------------------------------------------------------------*/
static void dumpNode(void *theNode, ListKind kind)
{
  if (dumpNodeTable[kind] == NULL) {
    put("*** Not implemented in DUMP. ***");
  } else
    dumpNodeTable[kind](theNode);
}



/*======================================================================*/
void dumpList(List *theList, ListKind class)
{
  if (theList == NULL) {
    put("NULL");
    return;
  }
  
  put("LST: "); dumpPointer(theList); in();
  while (theList != NULL) {
    dumpNode((void *)theList->element.atr, class);
    theList = theList->next;
    if (theList != NULL) nl();
  }
  out();
}


/*======================================================================*/
void dumpListOfLists(List *listOfList, ListKind listKind)
{
  if (listOfList == NULL) {
    put("NULL");
    return;
  }
  
  put("LST: "); dumpPointer(listOfList); in();
  while (listOfList != NULL) {
    dumpList((void *)listOfList->element.lst, listKind);
    listOfList = listOfList->next;
    if (listOfList != NULL) nl();
  }
  out();
}


