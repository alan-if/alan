/*----------------------------------------------------------------------*\

                               ELM.C
                            Element Nodes

\*----------------------------------------------------------------------*/

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
#include "elm.h"                /* ELM-nodes */

#include "emit.h"
#include "../interpreter/acode.h"


/* PUBLIC: */


/* PRIVATE: */

static int level = 0;


/*----------------------------------------------------------------------*/
static Element *newElement(Srcp *srcp,	/* IN - Source Position */
			   ElementKind kind, /* IN - Kind of element (parm or word) */
			   IdNode *id,	/* IN - The name */
			   int flags)	/* IN - Flags for omni/multiple... */
{
  Element *new;                                  /* The newly created node */

  showProgress();

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
Element *newWordElement(Srcp srcp,	/* IN - Source Position */
			IdNode *word)	/* IN - Flags for omni/multiple... */
{
  Element *new;			/* The newly created node */

  new = newElement(&srcp, WORD_ELEMENT, word, 0);

  return new;
}


/*======================================================================*/
Element *newParameterElement(Srcp srcp,
			     IdNode *word, int flags)
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
  showProgress();

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


/*======================================================================*/
List *analyzeElements(List *elms,        /* IN - List to analyze */
		      List *ress,        /* IN - The class restrictions */
		      Syntax *stx        /* IN - The stx we're in */
)
{
  Element *elm = elms->element.elm; /* Set to be the first (yes, there is always at least one!) */
  List *lst, *pars = NULL;
  List *resLst;
  int paramNo = 1;
  Bool multiple = FALSE;

  if (elm->kind != WORD_ELEMENT)
    /* First element must be a player word */
    lmLog(&elm->srcp, 209, sevERR, "");
  else
    elm->id->code = newVerbWord(elm->id->string, stx);

  /* Analyze the elements, number parameters and find the restriction */
  /* Start with the second since the first is analyzed above */
  for (lst = elms->next; lst != NULL; lst = lst->next) {
    if (lst->element.elm->kind == PARAMETER_ELEMENT) {
      lst->element.elm->id->code = paramNo++;
      if ((lst->element.elm->flags & MULTIPLEBIT) != 0) {
        if (multiple)
          lmLog(&lst->element.elm->srcp, 217, sevWAR, "");
        else
          multiple = TRUE;
      }
      pars = concat(pars, lst->element.elm, ELEMENT_LIST);

      /* Find first class restrictions */
      for (resLst = ress; resLst; resLst = resLst->next) {
        if (equalId(resLst->element.res->parameterId, lst->element.elm->id)) {
	  lst->element.elm->res = resLst->element.res;
	  resLst->element.res->parameterId->code = lst->element.elm->id->code;
        }
      }
    }
    analyzeElement(lst->element.elm);
  }

  /* Check for multiple definition of parameter names */
  for (lst = pars; lst != NULL; lst = lst->next)
    for (elms = lst->next; elms != NULL; elms = elms->next) {
      if (equalId(lst->element.elm->id, elms->element.elm->id))
        lmLog(&elms->element.elm->id->srcp, 216, sevERR, elms->element.elm->id->string);
    }
  return pars;
}



/*----------------------------------------------------------------------*/
static Bool equalElements(List *element1, List *element2)
{
  if (element1 == NULL)
    return (element2 == NULL);
  else if (element2 == NULL)
    return FALSE;
  else
    return (element1->element.elm->kind == element2->element.elm->kind &&
	    (element1->element.elm->kind == END_OF_SYNTAX ||
	     element1->element.elm->kind == PARAMETER_ELEMENT ||
	     (element1->element.elm->kind == WORD_ELEMENT &&
              equalId(element1->element.elm->id, element2->element.elm->id))));
}


/*----------------------------------------------------------------------

  advance()

  Advances a whole list of elmList pointers parallell to their next elm.
  Returns the address to the generated class restrictions for any syntax that
  was terminated here.

  */
static Aaddr advance(List *elmsList) /* IN - The list to advance */
{
  List *elms;
  Aaddr resadr = 0;             /* Saved address to class restriction */

  for (elms = elmsList; elms != NULL; elms = elms->next) {
    elms->element.lst = elms->element.lst->next;
    if (elms->element.lst->element.elm->kind == END_OF_SYNTAX)
      resadr = elms->element.lst->element.elm->stx->restrictionsAddress;
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

  if (*elmsListP == NULL || (*elmsListP)->element.elm->kind == END_OF_SYNTAX)
    return NULL;

  /* Remove the first element from the list to form the partition */
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


/*======================================================================*/
Aaddr generateElements(List *elms, Syntax *stx)
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
  */

  List *lst;                    /* Traversal list */
  List *part;                   /* The current partion */
  Aaddr elmadr, restrictionTableAddress;
  List *entries = NULL;         /* List of next level entries */
  ElementEntry *entry;              /* One entry in the list */

  if (elms == NULL)
    return 0;           /* End of chain */

  showProgress();

  /* Move all to their next elm */
  restrictionTableAddress = advance(elms);

  level++;
  for (part = partition(&elms); part != NULL; part = partition(&elms)) {
    /* Make one entry for this partition */
    entry = NEW(ElementEntry);
    entry->flags = 0;
    entries = concat(entries, entry, LIST_EENT);
    switch (part->element.lst->element.elm->kind) {

    case END_OF_SYNTAX:		/* This partition was at end of syntax */
      if (part->next != NULL) { /* More than one element in this partition? */
        /* That means that two syntax's are the same */
	for (lst = part; lst != NULL; lst = lst->next)
	  lmLog(&lst->element.lst->element.elm->stx->srcp, 334, sevWAR, "");
      }
      entry->code = EOS;        /* End Of Syntax */
      entry->flags = part->element.lst->element.elm->stx->number; /* Syntax number */
      /* Point to the generated class restriction table */
      entry->next = restrictionTableAddress;
      break;

    case PARAMETER_ELEMENT:
      entry->code = 0;
      entry->flags = part->element.lst->element.elm->flags;
      entry->next = generateElements(part, stx);
      break;

    case WORD_ELEMENT:
      entry->code = part->element.lst->element.elm->id->code;
      entry->flags = 0;
      entry->next = generateElements(part, stx);
      break;
    }
  }
  
  /* Finally, generate this level */
  elmadr = nextEmitAddress();
  for (lst = entries; lst; lst = lst->next)
    emitEntry(lst->element.eent, sizeof(*entry));
  emit(EOF);

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

  put("ELM: "); dumpPointer(elm); dumpSrcp(&elm->srcp); indent();
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
