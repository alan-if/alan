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
extern void dumpIfid();


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
  dumpNodeTable[IFID_LIST] = &dumpIfid;
}


/*======================================================================*/
List *newEmptyList(ListKind kind) {
  List *new = NEW(List);

  new->kind = kind;

  return new;
}


List *newList(void *member, ListKind kind)	
{
  List *new = NEW(List);	/* The newly created list node */

  new->member.ptr = member;
  new->kind = kind;

  return(new);
}


/*======================================================================

  insert()

  Insert an member into a list at the point. To insert at the end
  use concat()

  */
void insert(List *thePoint, void *member, ListKind kind)
{
  List *newListNode;

  if (thePoint == NULL)
    SYSERR("Inserting an member in a NULL list");
  if (kind != thePoint->kind)
    SYSERR("Inserting wrong kind of member in list");
  if (member == NULL)
    SYSERR("Inserting a NULL member in a list");

  /* Move the first member to a new list node */
  newListNode = newList(thePoint->member.atr, kind);

  newListNode->next = thePoint->next;
  thePoint->member.atr = member;
  thePoint->next = newListNode;
}


/*======================================================================*/
List *copyList(List *aList)
{
  List *new = NULL;

  if (aList == NULL)
    return NULL;

  while (aList) {
    new = concat(new, aList->member.id, aList->kind);
    aList = aList->next;
  }
  return new;
}


/*======================================================================*/
extern void *getMember(List *theList, int number)
{
  int i = 1;

  if (number < 1) SYSERR("List member number must be > 0");

  while (theList) {
    if (i == number)
      return (void *)theList->member.id; /* All member pointers are overlayed */
    theList = theList->next;
    i++;
  }
  SYSERR("Not enough list members");
  return NULL;
}


/*======================================================================*/
extern List *getListNode(List *theList, int number)
{
  int i = 1;

  if (number < 1) SYSERR("List member number must be > 0");

  while (theList) {
    if (i == number)
      return theList;
    theList = theList->next;
    i++;
  }
  SYSERR("Not enough list members");
  return NULL;
}


/*======================================================================*/
List *getLastListNode(List *theList)
{
  List *last;

  if (theList == NULL)
    return NULL;

  for (last = theList; last->next != NULL; last = last->next)
    ;
  return last;
}


/*======================================================================*/
void *getLastMember(List *theList)
{
  List *tail;

  if (theList == NULL)
    return NULL;

  for (tail = theList; tail->next != NULL; tail = tail->next)
    ;
  return (void *)tail->member.id;
}



// TODO: Refactor to use newList(ListKind), remove kind from concat()
// and let it propagate it instead
// Probably should also disallow NULL as the list
/*======================================================================*/
List *concat(List *list /*@null@*/, void *member, ListKind kind)	
{
  List *new;			/* The newly created list node */
  List *tail;			/* Traversal pointer to find the tail */

  if (member == NULL) return(list);
  if (list != NULL && list->member.cla == NULL) {
    list->member.ptr = member;
    return list;
  }    

  new = NEW(List);

  new->member.ptr = member;
  new->kind = kind;

  new->next = NULL;
  if (list == NULL) {
    return(new);
  } else {
    tail = getLastListNode(list);
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
  List *tail = getLastListNode(list1);

  if (list1 == NULL) return(list2);
  if (list2 == NULL) return(list1);

  tail->next = list2;	/* Combine at end of list1 */
  return(list1);
}


/*======================================================================*/
int length(List *theList)
{
  int count = 0;
  List *thePoint;

  for (thePoint = theList; thePoint != NULL; thePoint = thePoint->next)
    count++;
  return count;
}


/*----------------------------------------------------------------------*/
static List *removeFromList(List *theList, List *theMember)
{
  if (theList == NULL)		/* No list */
    return NULL;
  else if (theList == theMember) { /* First member */
    List *theRest = theMember->next;
    theMember->next = NULL;
    return theRest;
  } else {
    List *sentinel = theList;
    while (sentinel->next != theMember && sentinel->next != NULL)
      sentinel = sentinel->next;
    if (sentinel->next != NULL) {
      List *foundMember = sentinel->next;
      sentinel->next = sentinel->next->next;
      foundMember->next = NULL;
    }
  }
  return theList;
}    


/*======================================================================*/
List *sortList(List *theList, int compare(List *member1, List *member2))
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
    put("*** Not implemented in dumpNode(). ***");
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
    dumpNode((void *)theList->member.atr, class);
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
    dumpList((void *)listOfList->member.lst, listKind);
    listOfList = listOfList->next;
    if (listOfList != NULL) nl();
  }
  out();
}
