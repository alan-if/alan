/*----------------------------------------------------------------------*\

                               ELM.C
                            Element Nodes

\*----------------------------------------------------------------------*/

#include "util.h"

#include "srcp_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "wrd_x.h"
#include "lmList.h"

#include "stx.h"
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "elm.h"                /* ELM-nodes */

#include "emit.h"
#include "../interpreter/acode.h"
#include "dump.h"


/* PUBLIC: */


/* PRIVATE: */

static int level = 0;


/*======================================================================

  newelm()

  Allocates and initialises a syntax element node.

 */
ElmNod *newelm(Srcp *srcp,      /* IN - Source Position */
               ElementKind kind, /* IN - Kind of element (parm or word) */
               IdNode *id,	/* IN - The name */
               int flags)       /* IN - Flags for omni/multiple... */
{
  ElmNod *new;                                  /* The newly created node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ElmNod);

  new->srcp = *srcp;
  new->kind = kind;
  new->id = id;
  new->flags = flags;
  new->res = NULL;
  new->stx = NULL;

  return(new);
}



/*----------------------------------------------------------------------

  anelm()

  Analyzes one syntax element node.

 */
static void anelm(ElmNod *elm)  /* IN - Syntax element to analyze */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  switch (elm->kind) {
  case WORD_ELEMENT:
    elm->id->code = newwrd(elm->id->string, WRD_PREP, 0, NULL);
    break;
  case PARAMETER_ELEMENT:
  case END_OF_SYNTAX:
    break;
  default:
    syserr("Unknown element node kind in anelm()");
    break;
  }
}



/*======================================================================

  anelms()

  Analyzes all elements in a list by calling the analyzer for all.
 */
List *anelms(List *elms,        /* IN - List to analyze */
             List *ress,        /* IN - The class restrictions */
             StxNod *stx        /* IN - The stx we're in */
)
{
  ElmNod *elm = elms->element.elm; /* Set to be the first (yes, there is always at least one!) */
  List *lst, *pars = NULL;
  List *resLst;
  int paramNo = 1;
  Bool multiple = FALSE;

  if (elm->kind != WORD_ELEMENT)
    /* First element must be a player word */
    lmLog(&elm->srcp, 209, sevERR, "");
  else
    elm->id->code = newwrd(elm->id->string, WRD_VRB, 0, (void *)stx);

  /* Analyze the elements, number the parameters and find the class restriction */
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
      pars = concat(pars, lst->element.elm, LIST_ELM);

      /* Find first class restrictions */
      for (resLst = ress; resLst; resLst = resLst->next) {
        if (equalId(resLst->element.res->parameterId, lst->element.elm->id)) {
	  lst->element.elm->res = resLst->element.res;
	  resLst->element.res->parameterId->code = lst->element.elm->id->code;
        }
      }
    }
    anelm(lst->element.elm);
  }

  /* Check for multiple definition of parameter names */
  for (lst = pars; lst != NULL; lst = lst->next)
    for (elms = lst->next; elms != NULL; elms = elms->next) {
      if (equalId(lst->element.elm->id, elms->element.elm->id))
        lmLog(&elms->element.elm->id->srcp, 216, sevERR, elms->element.elm->id->string);
    }
  return pars;
}



/*----------------------------------------------------------------------

  eqElms()

  Return true if two pointers to elms lists have their first element
  the same.

  */
static Bool eqElms(List *elm1,  /* IN - One list pointer */
                   List *elm2)  /* IN - The other */
{
  if (elm1 == NULL)
    return (elm2 == NULL);
  else if (elm2 == NULL)
    return FALSE;
  else
    return (elm1->element.elm->kind == elm2->element.elm->kind &&
	    (elm1->element.elm->kind == END_OF_SYNTAX ||
	     elm1->element.elm->kind == PARAMETER_ELEMENT ||
	     (elm1->element.elm->kind == WORD_ELEMENT &&
              equalId(elm1->element.elm->id, elm2->element.elm->id))));
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
      resadr = elms->element.lst->element.elm->stx->resadr;
  }
  return resadr;
}


/*----------------------------------------------------------------------

  first()

  Remove the first element from a list and return that.

  */
static List *first(List **listP) /* IN OUT - Address of pointer to list */
{
  List *theFirst = *listP;

  *listP = theFirst->next;

  theFirst->next = NULL;

  return theFirst;
}


/*----------------------------------------------------------------------

  partition()

  Partitions a list of elmLists into one list containing all elms
  equal to the first one, and one list containing the rest of the list.

  */
static List *partition(List **elmsListP) /* INOUT - Address to pointer to the list */
{
  List *part, *rest, *elms, *this, *p;

  if (*elmsListP == NULL || (*elmsListP)->element.elm->kind == END_OF_SYNTAX)
    return NULL;

  /* Remove the first element from the list to form the partition */
  rest = *elmsListP;
  part = first(&rest);

  elms = rest;
  while (elms != NULL) {
    if (eqElms(part->element.lst, elms->element.lst)) {
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


/*======================================================================

  geelms()

  Generate the data structure for the syntax elements.  NOTE that the
  list is not the list of words as specified in the syntax statement.
  Instead this list contains all identical elms for *all* syntax
  structures. Also note that the list links not elms but list nodes
  where the first element is the elm to consider (an extra level of
  lists!).

  This function is recursive in pre-order by grouping equal elements
  in the next level and generating each group first, then a table for
  this group pointing to the next level for each group, a.s.o.

 */
Aaddr geelms(List *elms, StxNod *stx) /* IN - The elements */
{
  List *lst;                    /* Traversal list */
  List *part;                   /* The current partion */
  Aaddr elmadr, resadr;
  List *entries = NULL;         /* List of next level entries */
  ElementEntry *entry;              /* One entry in the list */

  if (elms == NULL)
    return 0;           /* End of chain */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* Move all to their next elm */
  resadr = advance(elms);

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
      /* Point to the generated class restriction table */
      entry->next = resadr;
      break;

    case PARAMETER_ELEMENT:
      entry->code = 0;
      entry->flags = part->element.lst->element.elm->flags;
      entry->next = geelms(part, stx);
      break;

    case WORD_ELEMENT:
      entry->code = part->element.lst->element.elm->id->code;
      entry->flags = 0;
      entry->next = geelms(part, stx);
      break;
    }
  }
  
  /* Finally, generate this level */
  elmadr = emadr();
  for (lst = entries; lst; lst = lst->next)
    emitEntry(lst->element.eent, sizeof(*entry));
  emit(EOF);

  level--;
  return(elmadr);
}



/*======================================================================

  duelm()

  Dump a Syntax element node.

 */
void duelm(ElmNod *elm)
{
  if (elm == NULL) {
    put("NULL");
    return;
  }

  put("ELM: "); dumpPointer(elm); dumpSrcp(&elm->srcp); in();
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
