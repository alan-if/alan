/*----------------------------------------------------------------------*\

				VRB.C
			      Verb Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "adv.h"		/* ADV-nodes */
#include "alt.h"		/* ALT-nodes */
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stx.h"		/* STX-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "vrb.h"		/* VRB-nodes */

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
	       List *nams,	/* IN - The verb names */
	       List *alts)	/* IN - List of alternatives  */
{
  VrbNod *new;			/* The newly allocated area */
  SymNod *sym;
  List *lst;			/* Traversal pointer */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(VrbNod);

  new->srcp = *srcp;
  new->nams = nams;
  new->alts = alts;

  for (lst = nams; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.nam->str); /* Find earlier definition */
    if (sym == NULL) {
      lst->element.nam->code = newsym(lst->element.nam->str, NAMVRB, new);
      lst->element.nam->kind = NAMVRB;
    } else if (sym->class == NAMVRB) {
      lst->element.nam->code = sym->code;
      lst->element.nam->kind = NAMVRB;
    } else
      redefined(&lst->element.nam->srcp, sym, lst->element.nam->str);
  }

  return(new);
}


/*----------------------------------------------------------------------

  anvrb()

  Analyze one verb.

  */
static void anvrb(VrbNod *vrb,	/* IN - The verb to analyze */
		  ObjNod *obj,                 
		  ActNod *act)
{
  List *lst, *nams, *stxs = NULL;
  StxNod *stx;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* First find the syntax definitions for all verbs */
  for (nams = vrb->nams; nams; nams = nams->next) {
    stx = NULL;
    for (lst = adv.stxs; lst; lst = lst->next) {
      if (lst->element.stx->nam->code == nams->element.nam->code) {
	stx = lst->element.stx;
	break;
      }
    }
    if (stx == NULL) {
      lmLog(&nams->element.nam->srcp, 230, sevINF, nams->element.nam->str);
      stx = defaultStx(nams->element.nam->str);
    }
    stxs = concat(stxs, stx, STXNOD);
  }
  stx = stxs->element.stx;	/* Use first syntax */
  vrb->stx = stx;
    
  /* Check compatible parameter lists for all the nams? */
  nams = vrb->nams->next;
  for (lst = stxs->next; lst != NULL; lst = lst->next) {
    if (!eqparams(stx, lst->element.stx))
      lmLog(&nams->element.nam->srcp, 215, sevERR,
	    vrb->nams->element.nam->str);
    nams = nams->next;
  }

  if (obj == NULL && act == NULL)
    if (vrb->alts->element.alt->nam != NULL)
      lmLog(&vrb->alts->element.alt->srcp, 213, sevERR, "");

  /* 4f_ni - Warn if no ALT for every parameter in the defined syntax */
  if (stx != NULL)
    analts(vrb->alts, act, stx->pars);
  else
    analts(vrb->alts, act, NULL);
}


/*======================================================================

  anvrbs()

  Analyze all verbs in a list.

  */
void anvrbs(List *vrbs,		/* IN - The verbs to analyze */
	    ObjNod *obj,                 
	    ActNod *act)
{
  List *vrb, *nam, *lst, *other;

  for (vrb = vrbs; vrb != NULL; vrb = vrb->next)
    anvrb(vrb->element.vrb, obj, act);

  /* Check for multiple definitions of a verb */
  for (vrb = vrbs; vrb != NULL; vrb = vrb->next) {
    nam = vrb->element.vrb->nams;
    /* First check other names in this VERB */
    for (other = nam->next; other != NULL; other = other->next) {
      if (other->element.nam->code == nam->element.nam->code)
	lmLog(&other->element.nam->srcp, 205, sevWAR,
	      other->element.nam->str);
    }
    /* Then the names in the other VERBs */
    for (lst = vrb->next; lst != NULL; lst = lst->next) {
      for (other = lst->element.vrb->nams; other != NULL; other = other->next)
	if (other->element.nam->code == nam->element.nam->code)
	  lmLog(&other->element.nam->srcp, 220, sevWAR, other->element.nam->str);
    }
  }
}



/*----------------------------------------------------------------------

  gevrb()

  Generate a procedure for the actions of a verb.

  */
static void gevrb(VrbNod *vrb,	/* IN - The verb to generate */
		  ActNod *act)	/* IN - Inside any actor */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  if (vrb->alts == NULL)
    vrb->altadr = 0;
  else
    vrb->altadr = gealts(vrb->alts, act);
}



/*----------------------------------------------------------------------

  gevrbent()

  Generate entries for one VERB.

  */
static void gevrbent(VrbNod *vrb) /* IN - Verb to generate entry for */
{
  List *nam;

  for (nam = vrb->nams; nam != NULL; nam = nam->next) {
    genam(nam->element.nam);
    emit(vrb->altadr);
  }
}



/*======================================================================

  gevrbs()

  Generate all verbs in a list.

  */
Aaddr gevrbs(List *vrbs,	/* IN - The list of verbs */
	     ActNod *act)	/* IN - Inside any actor */
{
  List *lst;			/* Save the list of verbs */
  Aaddr vrbadr;			/* Address to alt-table */

  if (vrbs == NULL)
    return 0;

  /* First generate action procedures for all verbs */
  for (lst = vrbs; lst != NULL; lst = lst->next)
    gevrb(lst->element.vrb, act);
  
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

  put("VRB: "); dusrcp(&vrb ->srcp); in();
  put("nams: "); dulst(vrb->nams,NAMNOD); nl();
  put("altadr: "); duadr(vrb->altadr); nl();
  put("alts: "); dulst(vrb->alts, ALTNOD); out();
}


