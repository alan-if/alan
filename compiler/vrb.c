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
#include "adv_x.h"
#include "stx_x.h"
#include "context_x.h"
#include "dump_x.h"

#include "lmList.h"
#include "util.h"
#include "alt.h"
#include "ins.h"

#include "../interpreter/acode.h"
#include "emit.h"


/* PUBLIC: */



/*======================================================================*/
Verb *newVerb(Srcp *srcp, List *ids, List *alts)
{
  Verb *new;			/* The newly allocated area */
  Symbol *sym;
  List *lst;			/* Traversal pointer */

  showProgress();

  new = NEW(Verb);

  new->srcp = *srcp;
  new->ids = ids;
  new->alternatives = alts;

  for (lst = ids; lst != NULL; lst = lst->next) {
    sym = lookup(lst->element.id->string); /* Find earlier definition */
    if (sym == NULL) {
      lst->element.id->symbol = newVerbSymbol(lst->element.id);
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
  List *lst, *ids, *syntaxLists = NULL;
  Syntax *stx;
  Context *context = pushContext(previousContext);

  showProgress();

  /* First find the syntax definitions for all verbs */
  for (ids = theVerb->ids; ids; ids = ids->next) {
    stx = NULL;
    for (lst = adv.stxs; lst; lst = lst->next) {
      if (lst->element.stx->id->symbol != NULL && ids->element.id->symbol != NULL)
	if (lst->element.stx->id->symbol->code == ids->element.id->symbol->code) {
	  stx = lst->element.stx;
	  break;
	}
    }
    if (stx == NULL) {
      /* Define a default syntax for the verb */
      if (!inEntityContext(context)) {
	/* A global, no parameter, verb */
	lmLog(&ids->element.id->srcp, 230, sevINF, ids->element.id->string);
	stx = defaultSyntax0(ids->element.id->string);
      } else {
	IdNode *className = classIdInContext(context);
	if (className == NULL)
	  className = newId(nulsrcp, "object");
	lmLogv(&ids->element.id->srcp, 231, sevINF, ids->element.id->string,
	       className->string, NULL);
	stx = defaultSyntax1(ids->element.id, context);
      }
    }
    syntaxLists = concat(syntaxLists, stx, SYNTAX_LIST);
  }
  stx = syntaxLists->element.stx;	/* Use first syntax */
  theVerb->stx = stx;
    
  /* Check compatible parameter lists for all the verbs? */
  ids = theVerb->ids->next;
  for (lst = syntaxLists->next; lst != NULL; lst = lst->next) {
    if (!equalParameterLists(stx, lst->element.stx))
      lmLog(&ids->element.id->srcp, 215, sevERR,
	    theVerb->ids->element.id->string);
    ids = ids->next;
  }

  if (!inEntityContext(context)) {
    /* No alternatives allowed in global verb definition */
    if (theVerb->alternatives->element.alt->id != NULL)
      lmLog(&theVerb->alternatives->element.alt->srcp, 213, sevERR, "");
    /* No parameters allowed in global verb definition */
    if (syntaxLists->element.stx->parameters != NULL)
      lmLog(&theVerb->srcp, 219, sevERR, "");
  }

  /* TODO - Warn if no ALT for every parameter in the defined syntax */

  context->kind = VERB_CONTEXT;
  if (stx != NULL) {
    context->verb = theVerb->symbol;
    analyzeAlternatives(theVerb->alternatives, context);
  } else
    analyzeAlternatives(theVerb->alternatives, context);
}


/*======================================================================*/
Bool verbIdFound(IdNode *targetId, List *verbs)
{
  List *theVerb;
  List *theIdInList;

  for (theVerb = verbs; theVerb != NULL; theVerb = theVerb->next) {
    for (theIdInList = theVerb->element.vrb->ids; theIdInList != NULL; theIdInList = theIdInList->next)
      if (findIdInList(targetId, theIdInList) != NULL)
	return TRUE;
  }
  return FALSE;
}


/*----------------------------------------------------------------------*/
static void checkMultipleVerbs(List *verbs)
{
  List *thisVerbDeclaration, *otherVerbs;
  List *firstId;
  IdNode *foundId;

  for (thisVerbDeclaration = verbs; thisVerbDeclaration != NULL; thisVerbDeclaration = thisVerbDeclaration->next) {
    for (firstId = thisVerbDeclaration->element.vrb->ids; firstId != NULL; firstId = firstId->next) {
      if ((foundId = findIdInList(firstId->element.id, firstId->next)) != NULL)
	lmLogv(&foundId->srcp, 201, sevWAR, "verb", foundId->string, "in this VERB declaration", NULL);
      /* Then the names in the other VERBs */
      for (otherVerbs = thisVerbDeclaration->next; otherVerbs != NULL; otherVerbs = otherVerbs->next) {
	if ((foundId = findIdInList(firstId->element.id, otherVerbs->element.vrb->ids)) != NULL)
	  lmLogv(&foundId->srcp, 201, sevWAR, "verb", foundId->string, "in this class/instance. Duplicate in a previous VERB clause", NULL);
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
static void generateVerb(Verb *vrb)
{
  showProgress();

  if (vrb->alternatives == NULL)
    vrb->altAddress = 0;
  else
    vrb->altAddress = gealts(vrb->alternatives);
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
Aaddr generateVerbs(List *vrbs)
{
  List *lst;			/* Save the list of verbs */
  Aaddr vrbadr;			/* Address to alt-table */

  if (vrbs == NULL)
    return 0;

  /* First generate action procedures for all verbs */
  for (lst = vrbs; lst != NULL; lst = lst->next)
    generateVerb(lst->element.vrb);
  
  /* and then the verb table */
  vrbadr = nextEmitAddress();
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

  put("VRB: "); dumpSrcp(vrb ->srcp); indent();
  put("ids: "); dumpList(vrb->ids, ID_LIST); nl();
  put("altadr: "); dumpAddress(vrb->altAddress); nl();
  put("alts: "); dumpList(vrb->alternatives, ALTERNATIVE_LIST); out();
}


