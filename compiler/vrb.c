/*----------------------------------------------------------------------*\

				VRB.C
			      Verb Nodes

\*----------------------------------------------------------------------*/

#include "vrb_x.h"

/* USE: */
#include "srcp_x.h"
#include "sym_x.h"
#include "lst_x.h"
#include "id_x.h"
#include "stx_x.h"
#include "context_x.h"

#include "lmList.h"
#include "util.h"
#include "adv.h"		/* ADV-nodes */
#include "alt.h"		/* ALT-nodes */
#include "ins.h"		/* INS-nodes */

#include "../interpreter/acode.h"

#include "emit.h"
#include "dump.h"


/* PUBLIC: */



/*======================================================================*/
Verb *newVerb(Srcp *srcp, List *ids, List *alts)
{
  Verb *new;			/* The newly allocated area */
  Symbol *sym;
  List *lst;			/* Traversal pointer */

  progressMeter();

  new = NEW(Verb);

  new->srcp = *srcp;
  new->ids = ids;
  new->alternatives = alts;

  for (lst = ids; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.id->string); /* Find earlier definition */
    if (sym == NULL) {
      lst->element.id->symbol = newSymbol(lst->element.id, VERB_SYMBOL);
      lst->element.id->code = lst->element.id->symbol->code;
    } else if (sym->kind == VERB_SYMBOL) {
      lst->element.id->symbol = sym;
      lst->element.id->code = sym->code;
    } else
      redefined(lst->element.id, sym);
  }

  /* Use first verb symbol as context symbol */
  new->symbol = ids->element.id->symbol;

  return(new);
}


/*----------------------------------------------------------------------*/
static void analyzeVerb(Verb *theVerb, Context *previousContext)
{
  List *lst, *ids, *syntaxList = NULL;
  StxNod *stx;
  Context *context = copyContext(previousContext);

  progressMeter();

  /* First find the syntax definitions for all verbs */
  for (ids = theVerb->ids; ids; ids = ids->next) {
    stx = NULL;
    for (lst = adv.stxs; lst; lst = lst->next) {
      if (lst->element.stx->id->symbol != NULL)
	if (lst->element.stx->id->symbol->code == ids->element.id->symbol->code) {
	  stx = lst->element.stx;
	  break;
	}
    }
    if (stx == NULL) {
      lmLog(&ids->element.id->srcp, 230, sevINF, ids->element.id->string);
      stx = defaultStx(ids->element.id->string);
    }
    syntaxList = concat(syntaxList, stx, LIST_STX);
  }
  stx = syntaxList->element.stx;	/* Use first syntax */
  theVerb->stx = stx;
    
  /* Check compatible parameter lists for all the ids? */
  ids = theVerb->ids->next;
  for (lst = syntaxList->next; lst != NULL; lst = lst->next) {
    if (!eqparams(stx, lst->element.stx))
      lmLog(&ids->element.id->srcp, 215, sevERR,
	    theVerb->ids->element.id->string);
    ids = ids->next;
  }

  /* No alternatives allowed in global verb definition */
  if (context->instance == NULL && theVerb->alternatives->element.alt->id != NULL)
    lmLog(&theVerb->alternatives->element.alt->srcp, 213, sevERR, "");

  /* FIXME - Warn if no ALT for every parameter in the defined syntax */

  context->kind = VERB_CONTEXT;
  if (stx != NULL) {
    context->verb = theVerb->symbol;
    analyzeAlternatives(theVerb->alternatives, context);
  } else
    analyzeAlternatives(theVerb->alternatives, context);
}


/*----------------------------------------------------------------------*/
static IdNode *findIdInList(IdNode *theId, List *theList) {
  List *here;

  for (here = theList; here != NULL; here = here->next)
    if (equalId(here->element.id, theId))
      return here->element.id;
  return NULL;
}


/*----------------------------------------------------------------------*/
static void checkMultipleVerbs(List *verbs)
{
  List *thisVerbDeclaration, *otherVerbs;
  List *firstId, *otherId;
  IdNode *foundId;

  for (thisVerbDeclaration = verbs; thisVerbDeclaration != NULL; thisVerbDeclaration = thisVerbDeclaration->next) {
    for (firstId = thisVerbDeclaration->element.vrb->ids; firstId != NULL; firstId = firstId->next) {
      if ((foundId = findIdInList(firstId->element.id, firstId->next)) != NULL)
	lmLog(&foundId->srcp, 205, sevWAR, foundId->string);
      /* Then the names in the other VERBs */
      for (otherVerbs = thisVerbDeclaration->next; otherVerbs != NULL; otherVerbs = otherVerbs->next) {
	if ((foundId = findIdInList(firstId->element.id, otherVerbs->element.vrb->ids)) != NULL)
	  lmLog(&foundId->srcp, 220, sevWAR, foundId->string);
      }
    }
  }
}



/*======================================================================  */
void analyzeVerbs(List *verbs, Context *context)
{
  List *verb;

  for (verb = verbs; verb != NULL; verb = verb->next)
    analyzeVerb(verb->element.vrb, context);

  checkMultipleVerbs(verbs);
}



/*----------------------------------------------------------------------*/
static void generateVerb(Verb *vrb, int currentInstance)
{
  progressMeter();

  if (vrb->alternatives == NULL)
    vrb->altAddress = 0;
  else
    vrb->altAddress = gealts(vrb->alternatives, currentInstance);
}



/*----------------------------------------------------------------------*/
static void generateVerbEntry(Verb *vrb)
{
  List *ids;

  for (ids = vrb->ids; ids != NULL; ids = ids->next) {
    generateId(ids->element.id);
    emit(vrb->altAddress);
  }
}



/*======================================================================*/
Aaddr generateVerbs(List *vrbs, int currentInstance)
{
  List *lst;			/* Save the list of verbs */
  Aaddr vrbadr;			/* Address to alt-table */

  if (vrbs == NULL)
    return 0;

  /* First generate action procedures for all verbs */
  for (lst = vrbs; lst != NULL; lst = lst->next)
    generateVerb(lst->element.vrb, currentInstance);
  
  /* and then the verb table */
  vrbadr = emadr();
  for (lst = vrbs; lst != NULL; lst = lst->next)
    generateVerbEntry(lst->element.vrb);
  emit(EOF);

  return(vrbadr);
}



/*======================================================================*/
void dumpVerb (Verb *vrb)
{
  if (vrb == NULL) {
    put("NULL");
    return;
  }

  put("VRB: "); dumpSrcp(&vrb ->srcp); in();
  put("ids: "); dumpList(vrb->ids, LIST_ID); nl();
  put("altadr: "); dumpAddress(vrb->altAddress); nl();
  put("alts: "); dumpList(vrb->alternatives, LIST_ALT); out();
}


