/*----------------------------------------------------------------------*\

                               ELM.C
			    Element Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "sym.h"				/* SYM-nodes */
#include "lst.h"				/* LST-nodes */
#include "stx.h"				/* STX-nodes */
#include "nam.h"				/* NAM-nodes */
#include "elm.h"                /* ELM-nodes */
#include "wrd.h"                /* WRD-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"


/* PUBLIC: */


/* PRIVATE: */

typedef struct ElmEntry {
  int code;			/* Word code, if 0 means parameter */
  Aaddr adr;			/* Address to next level for this */
				/* entry */
  Aword flags;			/* Multiple indicator */
} ElmEntry;


/*======================================================================

  newelm()

  Allocates and initialises a syntax element node.

 */
ElmNod *newelm(Srcp *srcp,	/* IN - Source Position */
	       ElmKind kind,	/* IN - Kind of element (parm or word) */
	       NamNod *nam,	/* IN - The name */
	       int flags)	/* IN - Flags for omni/multiple... */
{
  ElmNod *new;					/* The newly created node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ElmNod);

  new->srcp = *srcp;
  new->kind = kind;
  new->nam = nam;
  new->flags = flags;
  new->res = NULL;

  return(new);
}



/*----------------------------------------------------------------------

  anelm()

  Analyzes one syntax element node.

 */
static void anelm(ElmNod *elm)	/* IN - Syntax element to analyze */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  switch (elm->kind) {
  case ELMPAR:
    elm->nam->kind = NAMPAR;	/* It is a parameter */
    elm->nam->code = elm->no;
    break;
  case ELMWRD:
    elm->nam->kind = NAMWRD;	/* It is a word */
    elm->nam->code = newwrd(elm->nam->str, WRD_PREP, 0, NULL);
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
List *anelms(
	     List *elms,	/* IN - List to analyze */
	     List *ress,	/* IN - The class restrictions */
	     StxNod *stx	/* IN - The stx we're in */
)
{
  ElmNod *elm = elms->element.elm; /* Set to be the first */
  List *lst, *resLst, *pars = NULL;
  int paramNo = 1;
  Bool multiple = FALSE;
  Bool found;

  if (elm->kind != ELMWRD)
    /* First element must be a player word */
    lmLog(&elm->srcp, 209, sevERR, "");
  else {
    elm->nam->kind = NAMWRD;	/* It is a word */
    elm->nam->code = newwrd(elm->nam->str, WRD_VRB, 0, (void *)stx);
  }

  /* Analyze the elements, number the parameters and find the class res */
  /* Start with the second since the first is analyzed above */
  for (lst = elms->next; lst != NULL; lst = lst->next) {
    if (lst->element.elm->kind == ELMPAR) {
      lst->element.elm->no = paramNo++;
      if ((lst->element.elm->flags & MULTIPLEBIT) != 0) {
	if (multiple)
	  lmLog(&lst->element.elm->srcp, 217, sevWAR, "");
	else
	  multiple = TRUE;
      }
      pars = concat(pars, lst->element.elm);

      /* Find any class restrictions and warn for multiple for same parameter */
      found = FALSE;
      for (resLst = ress; resLst; resLst = resLst->next) {
	if (eqnams(resLst->element.res->nam, lst->element.elm->nam)) {
	  if (found)
	    lmLog(&resLst->element.res->nam->srcp, 221, sevERR, resLst->element.res->nam->str);
	  else {
	    found = TRUE;
	    lst->element.elm->res = resLst->element.res;
	    resLst->element.res->nam->kind = NAMPAR;
	    resLst->element.res->nam->code = lst->element.elm->no;
	  }
	}
      }
    }
    anelm(lst->element.elm);
  }

  /* Check for multiple definition of parameter names */
  for (lst = pars; lst != NULL; lst = lst->next)
    for (elms = lst->next; elms != NULL; elms = elms->next) {
      if (eqnams(lst->element.elm->nam, elms->element.elm->nam))
	lmLog(&elms->element.elm->nam->srcp, 216, sevERR, elms->element.elm->nam->str);
    }
  return pars;
}



/*----------------------------------------------------------------------

  eqElms()

  Return true if two pointers to elms lists have their first element
  the same.

  */
static Bool eqElms(List *elm1,	/* IN - One list pointer */
		   List *elm2)	/* IN - The other */
{
  if (elm1 == NULL)
    return (elm2 == NULL);
  else if (elm2 == NULL)
    return FALSE;
  else
    return (elm1->element.elm->kind == elm2->element.elm->kind &&
	    (elm1->element.elm->kind == ELMPAR ||
	     (elm1->element.elm->kind == ELMWRD &&
	      eqnams(elm1->element.elm->nam, elm2->element.elm->nam))));
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
  Aaddr resadr = 0;		/* Saved address to class restriction */

  for (elms = elmsList; elms != NULL; elms = elms->next) {
    if (elms->element.lst->next == NULL)
      resadr = elms->element.lst->element.elm->stx->resadr;
    elms->element.lst = elms->element.lst->next;
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
  theFirst->tail = theFirst;

  return theFirst;
}


/*----------------------------------------------------------------------

  partition()

  Partitions a list of elmLists into one list containing all elms
  equal to the first one, and one list containing the rest of the list.
  If there is a empty element in the list make sure we take that one
  first since the address to its class restrictions is already saved up in
  geelms().

  */
static List *partition(List **elmsListP) /* INOUT - Address to pointer to the list */
{
  List *part, *rest, *elms, *this, *p;
  Bool emptyFound = FALSE;	/* Have we already found an empty element? */

  if (*elmsListP == NULL)
    return NULL;

  /* Remove the first element from the list to form the partition */
  rest = *elmsListP;
  part = first(&rest);
  emptyFound = part->element.lst == NULL;

  elms = rest;
  while (elms != NULL) {
    if (elms->element.lst == NULL && !emptyFound) {
      /* Swap this for the part we already found to take empty elms first */
      this = first(&elms);
      if (rest == this)
	rest = combine(part, elms);
      else
	rest = combine(part, rest);
      part = this;
      emptyFound = TRUE;
    } else if (eqElms(part->element.lst, elms->element.lst)) {
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
  List *lst;			/* Traversal list */
  List *part;			/* The current partion */
  Aaddr elmadr, resadr;
  List *entries = NULL;		/* List of next level entries */
  ElmEntry *entry;		/* One entry in the list */

  if (elms == NULL)
    return 0;		/* End of chain */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* Move all to their next elm */
  resadr = advance(elms);

  for (part = partition(&elms); part != NULL; part = partition(&elms)) {
    /* Make one entry for this partition */
    entry = NEW(ElmEntry);
    entry->flags = 0;
    entries = concat(entries, entry);
    if (part->element.lst == NULL) {
      /* This partition was at end of syntax */
      if (part->next != NULL) /* More than one element in this partition? */
	/* That means that two syntax's are the same */
	lmLog(&stx->srcp, 334, sevWAR, "");
      entry->code = EOS;	/* End Of Syntax */
      /* Point to the generated class restriction table */
      entry->adr = resadr;
    } else {
      if (part->element.lst->element.elm->kind == ELMPAR) {
	/* A parameter! */
	entry->code = 0;
	entry->flags = part->element.lst->element.elm->flags;
      } else {
	entry->code = part->element.lst->element.elm->nam->code;
	entry->flags = FALSE;
      }
      entry->adr = geelms(part, stx);
    }
  }
  
  /* Finally, generate this level */
  elmadr = emadr();
  for (lst = entries; lst; lst = lst->next) {
    entry = (ElmEntry *) lst->element.elm;
    emit(entry->code);
    emit(entry->flags);
    emit(entry->adr);
  }
  emit(EOF);

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

  put("ELM: "); duptr(elm); dusrcp(&elm->srcp); in();
  put("kind: ");
  switch (elm->kind) {
  case ELMPAR: {
    char buf[80];
    sprintf(buf, "PARAMETER (Flags: 0x%x)", elm->flags);
    put(buf);
    nl();
    break;
  }
  case ELMWRD:
    put("WORD"); nl();
    break;
  default:
    put("*** ERROR ***"); nl();
    break;
  }
  put("no: "); duint(elm->no); nl();
  put("nam: "); dunam(elm->nam); out();
}
