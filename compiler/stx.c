/*----------------------------------------------------------------------*\

                               STX.C
                            Syntax Nodes

\*----------------------------------------------------------------------*/

#include "stx_x.h"


#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "id_x.h"
#include "adv_x.h"
#include "sym_x.h"
#include "res_x.h"
#include "elm_x.h"
#include "lst_x.h"
#include "wrd_x.h"
#include "cla_x.h"


#include "lmList.h"

#include "elm.h"                /* ELM-nodes */

#include "emit.h"

#include "../interpreter/acode.h"

#include "dump.h"


/* PUBLIC: */


/*======================================================================*/
Syntax *newSyntax(Srcp *srcp,
		  IdNode *id,
		  List *elements,
		  List *restrictionLists)
{
  Syntax *new;                  /* The newly created node */
  static int number = 1;

  showProgress();

  new = NEW(Syntax);

  new->srcp = *srcp;
  new->id = id;
  new->number = number++;
  new->elements = elements;
  new->restrictionLists = restrictionLists;

  new->generated = FALSE;

  return(new);
}


/*----------------------------------------------------------------------*/
static void setDefaultRestriction(List *parameters)
{
  /*
      Sets the restrictions class symbol to the objectSymbol for the
      restrictions that has no explicit declared.
  */

  List *p;

  if (parameters != NULL && parameters->kind != SYMBOL_LIST)
    syserr("Not a symbol list in '%s()'", __FUNCTION__);

  for (p = parameters; p != NULL; p = p->next)
    if (p->element.sym->fields.parameter.element->res == NULL) {
      p->element.sym->fields.parameter.class = objectSymbol;
      p->element.sym->fields.parameter.type = INSTANCE_TYPE;
    }
}




/*----------------------------------------------------------------------*/
static void analyzeSyntax(Syntax *stx)  /* IN - Syntax node to analyze */
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
    
    stx->parameters = analyzeElements(stx->elements, stx->restrictionLists, stx);
    setParameters(verbSymbol, stx->parameters);
    analyzeRestrictions(stx->restrictionLists, verbSymbol);
    setDefaultRestriction(verbSymbol->fields.verb.parameterSymbols);
    
    /* Link last syntax element to this stx to prepare for code generation */
    (tailOf(stx->elements))->element.elm->stx = stx;
  }
}



/*======================================================================*/
void analyzeSyntaxes(void)
{
  List *lst, *other;

  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    analyzeSyntax(lst->element.stx);

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


/*======================================================================*/
Syntax *defaultSyntax0(char *verbName)
{
  /*
    Returns the address a default syntax node which is used for verbs
    without any defined syntax (global verbs, without a parameter):

    Syntax x = x.
  */

  Syntax *stx;
  List *elements;

  elements = concat(concat(NULL,
			   newElement(&nulsrcp, WORD_ELEMENT, newId(&nulsrcp,
								    verbName),
				      FALSE),
			   ELEMENT_LIST),
		    newElement(&nulsrcp, END_OF_SYNTAX, NULL, FALSE), ELEMENT_LIST);
  stx = newSyntax(&nulsrcp, newId(&nulsrcp, verbName), elements, NULL);

  adv.stxs = concat(adv.stxs, stx, SYNTAX_LIST);
  analyzeSyntax(stx);                   /* Make sure the syntax is analysed */
  return stx;
}



/*======================================================================*/
Syntax *defaultSyntax1(char *vrbstr) /* IN - The string for the verb */
{
  /*
    Returns the address a default syntax node which is used for verbs
    in instances (taking one parameter) without any defined syntax:

    Syntax x = x (object).
  */

  Syntax *stx;
  List *elements;

  elements = concat(concat(concat(NULL,
			      newElement(&nulsrcp, WORD_ELEMENT, newId(&nulsrcp,
							     vrbstr),
				     FALSE),
			      ELEMENT_LIST),
		       newElement(&nulsrcp, PARAMETER_ELEMENT, newId(&nulsrcp, "object"), FALSE),
		       ELEMENT_LIST),
		newElement(&nulsrcp, END_OF_SYNTAX, NULL, FALSE), ELEMENT_LIST);
  stx = newSyntax(&nulsrcp, newId(&nulsrcp, vrbstr), elements, NULL);

  adv.stxs = concat(adv.stxs, stx, SYNTAX_LIST);
  analyzeSyntax(stx);                   /* Make sure the syntax is analysed */
  return stx;
}



/*======================================================================*/
Bool eqparams(Syntax *stx1, Syntax *stx2)
{
  /*
    Compare two syntax nodes and return true if their parameter lists
    are compatible (same ordering with same parameter names).
  */

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


/*----------------------------------------------------------------------*/
static void generateParseTree(Syntax *stx)
{
  WordNode *wrd;
  List *lst = NULL;
  List *elements = NULL;            /* A list of parallell elms-lists */


  showProgress();
  
  if (!stx->generated) {
    /* First word is a verb which points to all stxs starting with that word */
    wrd = findWord(stx->elements->element.elm->id->string);
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
    stx->elementsAddress = generateElements(elements, stx);
  } else
    stx->elementsAddress = 0;
}



/*----------------------------------------------------------------------*/
static void generateParseEntry(Syntax *stx)
{
  ParseEntry entry;

  if (stx->elementsAddress != 0) {
    /* The code for the verb word */
    entry.code = stx->elements->element.elm->id->code;
    /* Address to syntax element tables */
    entry.elms = stx->elementsAddress;
    emitEntry(&entry, sizeof(entry));
  }
}


/*----------------------------------------------------------------------*/
static Aaddr generateParseTable(void) {
  List *lst;
  Aaddr parseTableAddress;

  TRAVERSE(lst, adv.stxs)
    generateParseTree(lst->element.stx);

  parseTableAddress = nextEmitAddress();
  TRAVERSE(lst, adv.stxs)
    generateParseEntry(lst->element.stx);
  emit(EOF);

  return(parseTableAddress);
}


/*----------------------------------------------------------------------*/
static void generateRestrictionTable(void) {
  List *lst;

  /* Generate all syntax parameter restriction checks */
  TRAVERSE(lst, adv.stxs)
    lst->element.stx->restrictionsAddress = generateRestrictions(lst->element.stx->restrictionLists,
								 lst->element.stx);
}


/*----------------------------------------------------------------------*/
static void generateParameterMapping(Syntax *syntax)
{
  List *list;
  Aaddr parameterMappingTableAddress = nextEmitAddress();

  TRAVERSE(list, syntax->parameters)
    /* Generate a parameter mapping entry */
    ;

  syntax->parameterMappingAddress = parameterMappingTableAddress;
}


/*----------------------------------------------------------------------*/
static void generateSyntaxMapping(void)
{
  List *list;
  Aaddr syntaxMappingTableAddress;
  SyntaxEntry entry;

  TRAVERSE(list, adv.stxs)
    generateParameterMapping(list->element.stx);

  syntaxMappingTableAddress = nextEmitAddress();
  TRAVERSE(list, adv.stxs) {
    entry.verbCode = list->element.stx->number;
    entry.parameterMapping = list->element.stx->parameterMappingAddress;
    entry.verbCode = list->element.stx->id->code;
    emitEntry(&entry, sizeof(entry));
  }
  emit(EOF);
}

  
/*======================================================================*/
Aaddr generateAllSyntaxes(void)
{
  generateSyntaxMapping();
  generateRestrictionTable();
  return generateParseTable();
}



/*======================================================================*/
void dumpSyntax(Syntax *stx)
{
  if (stx == NULL) {
    put("NULL");
    return;
  }

  put("STX: "); dumpPointer(stx); dumpSrcp(&stx->srcp); in();
  put("verbId: "); dumpId(stx->id); nl();
  put("number: "); dumpInt(stx->number); nl();
  put("generated: "); dumpBool(stx->generated); nl();
  put("elmsadr: "); dumpAddress(stx->elementsAddress); nl();
  put("elements: "); dumpList(stx->elements, ELEMENT_LIST); nl();
  put("resadr: "); dumpAddress(stx->restrictionsAddress); nl();
  put("restrictionLists: "); dumpList(stx->restrictionLists, RESTRICTION_LIST); nl();
  put("parameters: "); dumpList(stx->parameters, ELEMENT_LIST); out();
}
