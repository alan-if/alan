/*----------------------------------------------------------------------*\

				VRB.C
			      Verb Nodes

\*----------------------------------------------------------------------*/

#include "vrb.h"		/* VRB-nodes */

#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "lst_x.h"
#include "id_x.h"

#include "lmList.h"

#include "adv.h"		/* ADV-nodes */
#include "alt.h"		/* ALT-nodes */
#include "stx.h"		/* STX-nodes */
#include "ins.h"		/* INS-nodes */

#include "acode.h"

#include "emit.h"
#include "dump.h"


/* PUBLIC: */

int vrbmin, vrbmax;
int vrbcount = 0;



/*======================================================================

  newvrb()

  Allocates and initialises a vrbnod.

  */
VrbNod *newvrb(Srcp *srcp,	/* IN - Source Position */
	       List *ids,	/* IN - List of verb ids */
	       List *alts)	/* IN - List of alternatives  */
{
  VrbNod *new;			/* The newly allocated area */
  SymNod *sym;
  List *lst;			/* Traversal pointer */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(VrbNod);

  new->srcp = *srcp;
  new->ids = ids;
  new->alts = alts;

  for (lst = ids; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.id->string); /* Find earlier definition */
    if (sym == NULL) {
      lst->element.id->symbol = newsym(lst->element.id->string, VERB_SYMBOL);
    } else if (sym->kind == VERB_SYMBOL) {
      lst->element.id->symbol->code = sym->code;
    } else
      redefined(&lst->element.id->srcp, sym, lst->element.id->string);
  }

  return(new);
}


/*----------------------------------------------------------------------

  anvrb()

  Analyze one verb.

  */
static void anvrb(VrbNod *vrb,	/* IN - The verb to analyze */
		  InsNod *ins)
{
  List *lst, *ids, *stxs = NULL;
  StxNod *stx;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* First find the syntax definitions for all verbs */
  for (ids = vrb->ids; ids; ids = ids->next) {
    stx = NULL;
    for (lst = adv.stxs; lst; lst = lst->next) {
      if (lst->element.stx->id->symbol->code == ids->element.id->symbol->code) {
	stx = lst->element.stx;
	break;
      }
    }
    if (stx == NULL) {
      lmLog(&ids->element.id->srcp, 230, sevINF, ids->element.id->string);
      stx = defaultStx(ids->element.id->string);
    }
    stxs = concat(stxs, stx, LIST_STX);
  }
  stx = stxs->element.stx;	/* Use first syntax */
  vrb->stx = stx;
    
  /* Check compatible parameter lists for all the ids? */
  ids = vrb->ids->next;
  for (lst = stxs->next; lst != NULL; lst = lst->next) {
    if (!eqparams(stx, lst->element.stx))
      lmLog(&ids->element.id->srcp, 215, sevERR,
	    vrb->ids->element.id->string);
    ids = ids->next;
  }

  if (ins == NULL)
    if (vrb->alts->element.alt->id != NULL)
      lmLog(&vrb->alts->element.alt->srcp, 213, sevERR, "");

  /* FIXME - Warn if no ALT for every parameter in the defined syntax */
  if (stx != NULL)
    analts(vrb->alts, ins, stx->pars);
  else
    analts(vrb->alts, ins, NULL);
}


/*======================================================================

  anvrbs()

  Analyze all verbs in a list.

  */
void anvrbs(List *vrbs,		/* IN - The verbs to analyze */
	    InsNod *ins)
{
  List *vrb, *nam, *lst, *other;

  for (vrb = vrbs; vrb != NULL; vrb = vrb->next)
    anvrb(vrb->element.vrb, ins);

  /* Check for multiple definitions of a verb */
  for (vrb = vrbs; vrb != NULL; vrb = vrb->next) {
    nam = vrb->element.vrb->ids;
    /* First check other names in this VERB */
    for (other = nam->next; other != NULL; other = other->next) {
      if (other->element.id->symbol->code == nam->element.id->symbol->code)
	lmLog(&other->element.id->srcp, 205, sevWAR,
	      other->element.id->string);
    }
    /* Then the names in the other VERBs */
    for (lst = vrb->next; lst != NULL; lst = lst->next) {
      for (other = lst->element.vrb->ids; other != NULL; other = other->next)
	if (other->element.id->symbol->code == nam->element.id->symbol->code)
	  lmLog(&other->element.id->srcp, 220, sevWAR, other->element.id->string);
    }
  }
}



/*----------------------------------------------------------------------

  gevrb()

  Generate a procedure for the actions of a verb.

  */
static void gevrb(VrbNod *vrb,	/* IN - The verb to generate */
		  InsNod *ins)	/* IN - Inside any Instance? */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  if (vrb->alts == NULL)
    vrb->altadr = 0;
  else
    vrb->altadr = gealts(vrb->alts, ins);
}



/*----------------------------------------------------------------------

  gevrbent()

  Generate entries for one VERB.

  */
static void gevrbent(VrbNod *vrb) /* IN - Verb to generate entry for */
{
  List *ids;

  for (ids = vrb->ids; ids != NULL; ids = ids->next) {
    geid(ids->element.id);
    emit(vrb->altadr);
  }
}



/*======================================================================

  gevrbs()

  Generate all verbs in a list.

  */
Aaddr gevrbs(List *vrbs,	/* IN - The list of verbs */
	     InsNod *ins)	/* IN - Inside any Instance? */
{
  List *lst;			/* Save the list of verbs */
  Aaddr vrbadr;			/* Address to alt-table */

  if (vrbs == NULL)
    return 0;

  /* First generate action procedures for all verbs */
  for (lst = vrbs; lst != NULL; lst = lst->next)
    gevrb(lst->element.vrb, ins);
  
  /* and then the verb table */
  vrbadr = emadr();
  for (lst = vrbs; lst != NULL; lst = lst->next)
    gevrbent(lst->element.vrb);
  emit(EOF);

  return(vrbadr);
}



/*======================================================================

  duvrb()

  Dump a verb node.

  */
void duvrb (VrbNod *vrb)
{
  if (vrb == NULL) {
    put("NULL");
    return;
  }

  put("VRB: "); dumpSrcp(&vrb ->srcp); in();
  put("ids: "); dulst(vrb->ids, LIST_ID); nl();
  put("altadr: "); duadr(vrb->altadr); nl();
  put("alts: "); dulst(vrb->alts, LIST_ALT); out();
}


