/*----------------------------------------------------------------------*\

                               STX.C
                            Syntax Nodes

\*----------------------------------------------------------------------*/

#include "stx_x.h"


#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "id_x.h"
#include "sym_x.h"
#include "res_x.h"
#include "elm_x.h"
#include "lst_x.h"
#include "wrd_x.h"
#include "cla_x.h"


#include "lmList.h"

#include "adv.h"                /* ADV-nodes */
#include "elm.h"                /* ELM-nodes */

#include "emit.h"

#include "../interpreter/acode.h"

#include "dump.h"


/* PUBLIC: */


/*======================================================================

  newstx()

  Allocates and initialises a syntax node.

 */
StxNod *newstx(Srcp *srcp,
               IdNode *id,
               List *elements,
               List *restrictionLists)
{
  StxNod *new;                  /* The newly created node */

  showProgress();

  new = NEW(StxNod);

  new->srcp = *srcp;
  new->id = id;
  new->elements = elements;
  new->restrictionLists = restrictionLists;

  new->generated = FALSE;

  return(new);
}


/*----------------------------------------------------------------------

  setDefaultRestriction()

  Sets the restictions class symbol to the objectSymbol for the
  restrictions that has no explicit declared.

 */
static void setDefaultRestriction(List *parameters)
{
  List *p;

  if (parameters != NULL && parameters->kind != SYMBOL_LIST)
    syserr("Not a symbol list in setDefaultRestriction()");

  for (p = parameters; p != NULL; p = p->next)
    if (p->element.sym->fields.parameter.element->res == NULL) {
      p->element.sym->fields.parameter.class = objectSymbol;
      p->element.sym->fields.parameter.type = INSTANCE_TYPE;
    }
}




/*----------------------------------------------------------------------

  anstx()

  Analyzes one syntax node.

 */
static void anstx(StxNod *stx)  /* IN - Syntax node to analyze */
{
  Symbol *verbSymbol;

  showProgress();

  /* Find which verb it defines */
  verbSymbol = lookup(stx->id->string);
  if (verbSymbol == NULL) {
    if (stx->id->string[0] != '$') /* generated id? */
      lmLog(&stx->id->srcp, 207, sevERR, stx->id->string);
  } else {
    if (verbSymbol->kind != VERB_SYMBOL)
      lmLog(&stx->id->srcp, 208, sevERR, stx->id->string);
    else {
      stx->id->symbol = verbSymbol;
      stx->id->code = verbSymbol->code;
    }
    
    stx->parameters = anelms(stx->elements, stx->restrictionLists, stx);
    setParameters(verbSymbol, stx->parameters);
    analyzeRestrictions(stx->restrictionLists, verbSymbol);
    setDefaultRestriction(verbSymbol->fields.verb.parameterSymbols);
    
    /* Link last syntax element to this stx to prepare for code generation */
    (tailOf(stx->elements))->element.elm->stx = stx;
  }
}



/*======================================================================

  anstxs()

  Analyzes all syntax nodes in the adventure by calling the analyzer for
  each one.

 */
void anstxs(void)
{
  List *lst, *other;

  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    anstx(lst->element.stx);

  /* Check for multiple definitions of the syntax for a verb */
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    for (other = lst->next; other != NULL; other = other->next) {
      if (equalId(other->element.stx->id, lst->element.stx->id)) {
	if (!lst->element.stx->muldef){
	  lmLog(&lst->element.stx->id->srcp, 206, sevWAR,
		lst->element.stx->id->string);
	  lst->element.stx->muldef = TRUE;
	}
	if (!other->element.stx->muldef){
	  lmLog(&other->element.stx->id->srcp, 206, sevWAR,
		other->element.stx->id->string);
	  other->element.stx->muldef = TRUE;
	}
	break;
      }
    }
}


/*======================================================================

  defaultStx()

  Returns the address a default syntax node which is used for verbs
  without any defined syntax:

  Syntax x = x (object).

 */
StxNod *defaultStx(char *vrbstr) /* IN - The string for the verb */
{
  StxNod *stx;
  List *elements;

  elements = concat(concat(concat(NULL,
			      newelm(&nulsrcp, WORD_ELEMENT, newId(&nulsrcp,
							     vrbstr),
				     FALSE),
			      ELEMENT_LIST),
		       newelm(&nulsrcp, PARAMETER_ELEMENT, newId(&nulsrcp, "object"), FALSE),
		       ELEMENT_LIST),
		newelm(&nulsrcp, END_OF_SYNTAX, NULL, FALSE), ELEMENT_LIST);
  stx = newstx(&nulsrcp, newId(&nulsrcp, vrbstr), elements, NULL);

  adv.stxs = concat(adv.stxs, stx, SYNTAX_LIST);
  anstx(stx);                   /* Make sure the syntax is analysed */
  return stx;
}



/*======================================================================

  eqparams()

  Compare two syntax nodes and return true if their parameter lists are
  compatible (same ordering with same parameter names).

  */
Bool eqparams(StxNod *stx1,     /* IN - Syntax node to compare */
              StxNod *stx2)     /* IN - Syntax node to compare */
{
  List *elm1, *elm2;

  for (elm1 = stx1->parameters, elm2 = stx2->parameters;
       elm1 != NULL && elm2 != NULL;
       elm1 = elm1->next, elm2 = elm2->next) {
    if (!equalId(elm1->element.elm->id, elm2->element.elm->id))
      return FALSE;
    if (elm1->element.elm->flags != elm2->element.elm->flags)
      return FALSE;
  }
  return elm1 == elm2;          /* Both NULL => equal */
}


/*----------------------------------------------------------------------

  gestx()

  Generate one syntax node.

 */
static void gestx(StxNod *stx)  /* IN - Syntax node to generate for */
{
  WordNode *wrd;
  List *lst = NULL;
  List *elements = NULL;            /* A list of parallell elms-lists */


  showProgress();
  
  if (!stx->generated) {
    /* First word is a verb which points to all stxs starting with that word */
    wrd = findwrd(stx->elements->element.elm->id->string);
    /* Ignore words that are not verbs and prepositions */
    if (wrd->classbits&(1L<<WRD_PREP))
      lst = wrd->ref[WRD_PREP];
    if (wrd->classbits&(1L<<WRD_VRB))
      lst = wrd->ref[WRD_VRB];
    /* Create a list of all parallell elements */
    while (lst) {
      elements = concat(elements, lst->element.stx->elements, LIST_LIST);
      lst->element.stx->generated = TRUE;
      lst = lst->next;
    }
    stx->elmsadr = geelms(elements, stx);
  } else
    stx->elmsadr = 0;
}



/*----------------------------------------------------------------------

  gestxent()

  Generate a table entry for one syntax node.

 */
static void gestxent(StxNod *stx) /* IN - Syntax node to generate for */
{
  if (stx->elmsadr != 0) {
    /* The code for the verb word */
    emit(stx->elements->element.elm->id->code);
    /* Address to syntax element tables */
    emit(stx->elmsadr);
  }
}



/*======================================================================

  gestxs()

  Generate the data structure for all syntax definitions in the
  adventure.

 */
Aaddr gestxs(void)
{
  List *lst;
  Aaddr stxadr;

  /* First generate all class restriction checks */
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    lst->element.stx->resadr = generateRestrictions(lst->element.stx->restrictionLists, lst->element.stx);

  /* Then the actual stxs */
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    gestx(lst->element.stx);

  /* Then a table of entries */
  stxadr = emadr();
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    gestxent(lst->element.stx);
  emit(EOF);
  return(stxadr);
}



/*======================================================================

  dustx()

  Dump an Syntax node.

 */
void dustx(StxNod *stx)
{
  if (stx == NULL) {
    put("NULL");
    return;
  }

  put("STX: "); dumpPointer(stx); dumpSrcp(&stx->srcp); in();
  put("id: "); dumpId(stx->id); nl();
  put("generated: "); dumpBool(stx->generated); nl();
  put("elmsadr: "); dumpAddress(stx->elmsadr); nl();
  put("elements: "); dumpList(stx->elements, ELEMENT_LIST); nl();
  put("resadr: "); dumpAddress(stx->resadr); nl();
  put("restrictionLists: "); dumpList(stx->restrictionLists, RESTRICTION_LIST); nl();
  put("parameters: "); dumpList(stx->parameters, ELEMENT_LIST); out();
}
