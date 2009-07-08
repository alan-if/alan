/*----------------------------------------------------------------------*\

                               ELM.C
                            Element Nodes

\*----------------------------------------------------------------------*/

#include "elm_x.h"

#include "util.h"

#include "srcp_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "wrd_x.h"
#include "dump_x.h"
#include "lmList.h"

#include "stx.h"
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */

#include "emit.h"
#include "acode.h"


/* PUBLIC: */


/* PRIVATE: */

static int level = 0;


/*----------------------------------------------------------------------*/
static Element *newElement(Srcp *srcp,
			   ElementKind kind,
			   IdNode *id,
			   int flags)
{
  Element *new;                                  /* The newly created node */

  progressCounter();

  new = NEW(Element);

  new->srcp = *srcp;
  new->kind = kind;
  new->id = id;
  new->flags = flags;
  new->res = NULL;
  new->stx = NULL;

  return(new);
}


/*======================================================================*/
Element *newWordElement(Srcp srcp, IdNode *word)
{
  Element *new;			/* The newly created node */

  new = newElement(&srcp, WORD_ELEMENT, word, 0);

  return new;
}


/*======================================================================*/
Element *newParameterElement(Srcp srcp, IdNode *word, int flags)
{
  Element *new;			/* The newly created node */

  new = newElement(&srcp, PARAMETER_ELEMENT, word, flags);

  return new;
}


/*======================================================================*/
Element *newEndOfSyntax()
{
  Element *new;			/* The newly created node */

  new = newElement(&nulsrcp, END_OF_SYNTAX, NULL, 0);

  return new;
}


/*----------------------------------------------------------------------*/
static void analyzeElement(Element *elm)
{
  progressCounter();

  switch (elm->kind) {
  case WORD_ELEMENT:
    elm->id->code = newPrepositionWord(elm->id->string);
    break;
  case PARAMETER_ELEMENT:
  case END_OF_SYNTAX:
    break;
  default:
    SYSERR("Unknown element node kind");
    break;
  }
}

/*----------------------------------------------------------------------*/
static void checkForDuplicatedParameterNames(List *parameters) {
  List *elements, *list;

  for (list = parameters; list != NULL; list = list->next) {
    Element *outerElement = list->element.elm;
    for (elements = list->next; elements != NULL; elements = elements->next) {
      Element *innerElement = elements->element.elm;
      if (equalId(outerElement->id, innerElement->id))
        lmLog(&innerElement->id->srcp, 216, sevERR, innerElement->id->string);
    }
  }
}


/*======================================================================*/
List *analyzeElements(List *elements,        /* IN - List to analyze */
		      List *restrictions,        /* IN - The class restrictions */
		      Syntax *syntax        /* IN - The stx we're in */
)
{
  Element *firstElement = elements->element.elm; /* Set to be the first (yes, there is always at least one!) */
  List *list, *parameters = NULL;
  List *restrictionList;
  int parameterCount = 1;
  Bool multiple = FALSE;

  if (firstElement->kind != WORD_ELEMENT)
    /* First element must be a player word */
    lmLog(&firstElement->srcp, 209, sevERR, "");
  else
    firstElement->id->code = newVerbWord(firstElement->id->string, syntax);

  /* Analyze the elements, number parameters and find the restriction */
  /* Start with the second since the first is analyzed above */
  for (list = elements->next; list != NULL; list = list->next) {
    Element *element = list->element.elm;
    if (element->kind == PARAMETER_ELEMENT) {
      element->id->code = parameterCount++;
      if ((element->flags & MULTIPLEBIT) != 0) {
        if (multiple)
          lmLog(&element->srcp, 217, sevWAR, "");
        else
          multiple = TRUE;
      }
      parameters = concat(parameters, element, ELEMENT_LIST);

      /* Find first class restrictions */
      for (restrictionList = restrictions; restrictionList; restrictionList = restrictionList->next) {
        if (equalId(restrictionList->element.res->parameterId, element->id)) {
	  element->res = restrictionList->element.res;
	  restrictionList->element.res->parameterId->code = element->id->code;
        }
      }
    }
    analyzeElement(element);
  }

  checkForDuplicatedParameterNames(parameters);
  return parameters;
}



/*----------------------------------------------------------------------*/
static Bool equalElements(List *element1, List *element2)
{
  if (element1 == NULL || element2 == NULL)
    return element2 == element1;
  else if (element1->element.elm->kind == element2->element.elm->kind)
    switch (element1->element.elm->kind) {
    case END_OF_SYNTAX:
    case PARAMETER_ELEMENT:
      return TRUE;
    case WORD_ELEMENT:
      return equalId(element1->element.elm->id, element2->element.elm->id);
    }
  else
    return FALSE;
  SYSERR("Unexpected case");
  return FALSE;
}


/*----------------------------------------------------------------------
  Advances a whole list of elmList pointers parallell to their next elm.
  Returns the address to the generated class restrictions for any syntax that
  was terminated here.
  */
static Aaddr advance(List *elmsList) /* IN - The list to advance */
{
  List *list;
  Aaddr resadr = 0;             /* Saved address to class restriction */

  for (list = elmsList; list != NULL; list = list->next) {
    list->element.lst = list->element.lst->next;
    if (list->element.lst->element.elm->kind == END_OF_SYNTAX)
      resadr = list->element.lst->element.elm->stx->restrictionsAddress;
  }
  return resadr;
}


/*----------------------------------------------------------------------*/
static List *first(List **listP)
{
  List *theFirst = *listP;

  *listP = theFirst->next;	/* Set list to point to second element */
  theFirst->next = NULL;	/* Remove first element */
  return theFirst;
}


/*----------------------------------------------------------------------*/
static List *partition(List **elmsListP) /* INOUT - Address to pointer to the list */
{
  /*
    Partitions a list of elmLists into one list containing all elms
    equal to the first one, and one list containing the rest of the
    list.
  */

  List *part, *rest, *elms, *this, *p;

  if (*elmsListP == NULL)
    return NULL;

  /* Remove the first element from the list to form the base for the new partition */
  rest = *elmsListP;
  part = first(&rest);

  elms = rest;
  while (elms != NULL) {
    if (equalElements(part->element.lst, elms->element.lst)) {
      this = first(&elms);
      part = combine(part, this);
      if (rest == this)
        rest = elms;
      else {
        for (p = rest; p->next != this; p = p->next)
          ;
        p->next = elms;
      }
    } else {
      elms = elms->next;
    }
  }
  *elmsListP = rest;
  return part;
}


/*----------------------------------------------------------------------*/
static ElementEntry *newEntryForPartition(List **entries) {
  ElementEntry *entry;

  entry = NEW(ElementEntry);
  entry->flags = 0;
  *entries = concat(*entries, entry, ELEMENT_ENTRIES_LIST);
  return(entry);
}


/*----------------------------------------------------------------------*/
static void entryForEOS(ElementEntry *entry, List *part, Aaddr restrictionTableAddress) {
  List *lst;
  if (part->next != NULL) { /* More than one element in this partition? */
    /* That means that two syntax's are the same */
    for (lst = part; lst != NULL; lst = lst->next)
      lmLog(&lst->element.lst->element.elm->stx->srcp, 334, sevWAR, "");
  }
  entry->code = EOS;        /* End Of Syntax */
  entry->flags = part->element.lst->element.elm->stx->number; /* Syntax number */
  /* Point to the generated class restriction table */
  entry->next = restrictionTableAddress;
}


/*----------------------------------------------------------------------*/
static void entryForParameter(ElementEntry *entry, List *part, Syntax *stx) {
  List *element;

  entry->code = 0;
  entry->flags = part->element.lst->element.elm->flags;
  TRAVERSE(element, part->next) {
    entry->flags |= element->element.lst->element.elm->flags;
  }
  entry->next = generateElements(part, stx);
}


/*----------------------------------------------------------------------*/
static void entryForWord(ElementEntry *entry, Syntax *stx, List *part) {
  entry->code = part->element.lst->element.elm->id->code;
  entry->flags = 0;
  entry->next = generateElements(part, stx);
}


/*----------------------------------------------------------------------*/
static Aaddr generateEntries(List *entries, ElementEntry *entry) {
  List *lst;
  Aaddr elmadr;
  elmadr = nextEmitAddress();
  for (lst = entries; lst; lst = lst->next)
    emitEntry(lst->element.eent, sizeof(*entry));
  emit(EOF);
  return(elmadr);
}



/*======================================================================*/
Aaddr generateElements(List *elementLists, Syntax *stx)
{
  /*
    Generate the data structure for the syntax elements.  NOTE that
    the list is not the list of words as specified in the syntax
    statement.  Instead this list contains all identical elms for
    *all* syntax structures. Also note that the list links not elms
    but list nodes where the first element is the elm to consider (an
    extra level of lists!).

    This function is recursive in pre-order by grouping equal elements
    in the next level and generating each group first, then a table
    for this group pointing to the next level for each group, a.s.o.

    Currently this is a bit vasteful in that it always creates a new
    list. Also it should really request a partitioned list from
    partition() instead and then traverse that list. This requires
    rewriting partition() to deliver a list of partitions instead.

    TODO This code would be much clearer if it used Collection instead
    of Lists for the partitions.

  */
  List *elms = copyList(elementLists);
  List *part;                   /* The current partion */
  Aaddr elmadr, restrictionTableAddress;
  List *entries = NULL;         /* List of next level entries */
  ElementEntry *entry;		/* One entry in the list */

  if (elms == NULL)
    return 0;			/* End of chain */

  progressCounter();

  /* Move all to their next elm */
  restrictionTableAddress = advance(elms);

  level++;
  for (part = partition(&elms); part != NULL; part = partition(&elms)) {
    /* Make one entry for this partition */
    entry = newEntryForPartition(&entries);

    switch (part->element.lst->element.elm->kind) {

    case END_OF_SYNTAX:		/* This partition was at end of syntax */
      entryForEOS(entry, part, restrictionTableAddress);
      break;

    case PARAMETER_ELEMENT:
      entryForParameter(entry, part, stx);
      break;

    case WORD_ELEMENT:
      entryForWord(entry, stx, part);
      break;
    }
  }

  /* Finally, generate this level */
  elmadr = generateEntries(entries, entry);

  level--;
  return(elmadr);
}



/*======================================================================*/
void dumpElement(Element *elm)
{
  if (elm == NULL) {
    put("NULL");
    return;
  }

  put("ELM: "); dumpPointer(elm); dumpSrcp(elm->srcp); indent();
  put("kind: ");
  switch (elm->kind) {
  case PARAMETER_ELEMENT: {
    char buf[80];
    sprintf(buf, "PARAMETER (Flags: 0x%x)", elm->flags);
    put(buf);
    nl();
    break;
  }
  case WORD_ELEMENT:
    put("WORD"); nl();
    break;
  case END_OF_SYNTAX:
    put("EOS"); nl();
    break;
  default:
    put("*** ERROR ***"); nl();
    break;
  }
  put("id: "); dumpId(elm->id); out();
}
