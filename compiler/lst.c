/*----------------------------------------------------------------------*\

				LST.C
			Generic lists handling

\*----------------------------------------------------------------------*/

#include "lst_x.h"


/* IMPORT */
#include "dump_x.h"

#include "util.h"


/* PUBLIC DATA */

void (*(dumpNodeTable[LAST_LIST_KIND]))();


/* Import of dump functions to be used in dumpNodeTable */
extern void dumpAlternative();
extern void duchk();
extern void dumpElement();
extern void dumpExit();
extern void dumpAdd();
extern void dumpAttribute();
extern void dumpClass();
extern void dumpId();
extern void dumpInstance();
extern void dumpRestriction();
extern void dumpScript();
extern void dumpStatement();
extern void dumpSyntax();
extern void dumpStep();
extern void dumpVerb();
extern void dumpExpression();
extern void dumpLimit();


/*======================================================================

  initDumpNodeList()

*/
void initDumpNodeList()
{
  dumpNodeTable[ADD_LIST] = &dumpAdd;
  dumpNodeTable[ALTERNATIVE_LIST] = &dumpAlternative;
  dumpNodeTable[ATTRIBUTE_LIST] = &dumpAttribute;
  dumpNodeTable[CHECK_LIST] = &duchk;
  dumpNodeTable[CLASS_LIST] = &dumpClass;
  dumpNodeTable[CONTAINER_LIST] = &dumpPointer;
  dumpNodeTable[ELEMENT_LIST] = &dumpElement;
  dumpNodeTable[EXIT_LIST] = &dumpExit;
  dumpNodeTable[ID_LIST] = &dumpId;
  dumpNodeTable[INSTANCE_LIST] = &dumpInstance;
  dumpNodeTable[RESTRICTION_LIST] = &dumpRestriction;
  dumpNodeTable[STATEMENT_LIST] = &dumpStatement;
  dumpNodeTable[SYNTAX_LIST] = &dumpSyntax;
  dumpNodeTable[VERB_LIST] = &dumpVerb;
  dumpNodeTable[SCRIPT_LIST] = &dumpScript;
  dumpNodeTable[STEP_LIST] = &dumpStep;
  dumpNodeTable[EXPRESSION_LIST] = &dumpExpression;  
  dumpNodeTable[NAME_LIST] = &dumpId;
  dumpNodeTable[LIMIT_LIST] = &dumpLimit;
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
    SYSERR("Inserting an element in a NULL list");
  if (kind != thePoint->kind)
    SYSERR("Inserting wrong kind of element in list");

  /* Move the first element to a new list node */
  newListNode = concat(NULL, thePoint->element.atr, kind);

  newListNode->next = thePoint->next;
  thePoint->element.atr = element;
  thePoint->next = newListNode;
}



List *tailOf(List *aList)
{
  List *tail;

  if (aList == NULL)
    return NULL;

  for (tail = aList; tail->next != NULL; tail = tail->next)
    ;
  return tail;
}


/*======================================================================*/
List *concat(List *list /*@null@*/, void *element, ListKind kind)	
{
  List *new;			/* The newly created list node */
  List *tail;			/* Traversal pointer to find the tail */

  if (element == NULL) return(list);

  new = NEW(List);

  new->element.cla = (struct Class *) element;
  new->kind = kind;

  new->next = NULL;
  if (list == NULL) {
    return(new);
  } else {
    tail = tailOf(list);
    tail->next = new;	/* Concat at end of list */
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
  List *tail = tailOf(list1);

  if (list1 == NULL) return(list2);
  if (list2 == NULL) return(list1);

  tail->next = list2;	/* Combine at end of list1 */
  return(list1);
}


/*======================================================================*/
int length(List *aList)
{
  int count = 0;
  List *thePoint;

  for (thePoint = aList; thePoint != NULL; thePoint = thePoint->next)
    count ++;
  return count;
}


/*----------------------------------------------------------------------*/
static List *removeFromList(List *theList, List *theElement)
{
  if (theList == NULL)		/* No list */
    return NULL;
  else if (theList == theElement) { /* First element */
    List *theRest = theElement->next;
    theElement->next = NULL;
    return theRest;
  } else {
    List *sentinel = theList;
    while (sentinel->next != theElement && sentinel->next != NULL)
      sentinel = sentinel->next;
    if (sentinel->next != NULL) {
      List *foundElement = sentinel->next;
      sentinel->next = sentinel->next->next;
      foundElement->next = NULL;
    }
  }
  return theList;
}    


/*======================================================================*/
List *sortList(List *theList, int compare(List *element1, List *element2))
{
  List *unsorted = theList;
  List *sorted = NULL;
  List *candidate;

  if (!compare) return theList;

  while (unsorted) {
    List *current = unsorted;
    candidate = unsorted;
    while (current) {
      if (compare(current, candidate) < 0)
	candidate = current;
      current = current->next;
    }
    unsorted = removeFromList(unsorted, candidate);
    if (sorted == NULL)
      sorted = candidate;
    else {
      List *tail = sorted;
      while (tail->next) tail = tail->next;
      tail->next = candidate;
      candidate->next = NULL;
    }
  }
  return sorted;
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
  
  put("LST: "); dumpPointer(theList); indent();
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
  
  put("LST: "); dumpPointer(listOfList); indent();
  while (listOfList != NULL) {
    dumpList((void *)listOfList->element.lst, listKind);
    listOfList = listOfList->next;
    if (listOfList != NULL) nl();
  }
  out();
}
