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
#include "context_x.h"

#include "lmList.h"

#include "elm.h"                /* ELM-nodes */

#include "emit.h"

#include "../interpreter/acode.h"

#include "dump.h"


/* PUBLIC: */


/*======================================================================*/
Syntax *newSyntax(Srcp srcp,
		  IdNode *id,
		  List *elements,
		  List *restrictionLists,
		  Srcp restrictionSrcp)
{
  Syntax *new;                  /* The newly created node */
  static int number = 1;

  showProgress();

  new = NEW(Syntax);

  new->srcp = srcp;
  new->id = id;
  new->number = number++;
  new->elements = elements;
  new->firstSyntax = TRUE;	/* Assume first and only so far */
  new->nextSyntaxForSameVerb = NULL;
  new->restrictionLists = restrictionLists;
  new->restrictionSrcp = restrictionSrcp;

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




/*======================================================================*/
Bool equalParameterLists(Syntax *stx1, Syntax *stx2)
{
  /*
    Compare two syntax nodes and return true if their parameter lists
    are equal (same ordering with same parameter names).
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
static int countParameters(List *elms)
{
  /*
    Count number of PARAMETER_ELEMENTs in a ELEMENT_LIST
  */
  List *lst;
  int count = 0;

  TRAVERSE(lst, elms) {
    if (lst->element.elm->kind == PARAMETER_ELEMENT)
      count++;
  }
  return count;
}


/*----------------------------------------------------------------------*/
static Bool compatibleParameterLists(Syntax *stx1, Syntax *stx2)
{
  /*
    Compare two syntax nodes and return true if their parameter lists
    are compatible (same ordering with same parameter names).
  */

  List *elm1, *elm2;
  int foundInOther = 0;
  Bool found;

  TRAVERSE(elm1, stx1->elements) {
    if (elm1->element.elm->kind == PARAMETER_ELEMENT) {
      found = FALSE;
      TRAVERSE(elm2, stx2->elements) {
	if (elm2->element.elm->kind == PARAMETER_ELEMENT)
	  if (equalId(elm1->element.elm->id, elm2->element.elm->id)){
	    found = TRUE;
	    break;
	  }
      }
      if (!found) return FALSE;
      else foundInOther++;
    }
  }
  return foundInOther == countParameters(stx2->elements);
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
    /* Register the parameters, use last syntax if multiple */
    if (stx->firstSyntax)
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
  Bool error;

  /* Check for multiple definitions of the syntax for a verb */
  TRAVERSE(lst, adv.stxs) {
    error = FALSE;
    for (other = lst->next; other != NULL; other = other->next) {
      if (equalId(other->element.stx->id, lst->element.stx->id)) {
	lst->element.stx->nextSyntaxForSameVerb = other->element.stx;
	other->element.stx->firstSyntax = FALSE;
	if (!compatibleParameterLists(lst->element.stx, other->element.stx)) {
	  lmLog(&other->element.stx->id->srcp, 206, sevERR,
		lst->element.stx->id->string);
	  error = TRUE;
	}
	if (other->element.stx->restrictionLists != NULL) {
	  lmLog(&other->element.stx->restrictionSrcp, 250, sevERR,
		lst->element.stx->id->string);
	  error = TRUE;
	}
      }
    }
    if (error) {
      char insertString[1000];
      sprintf(insertString, "syntax for '%s'", lst->element.stx->id->string);
      lmLog(&lst->element.stx->id->srcp, 205, sevWAR, insertString);
    }
  }
  /* Now do the analysis */
  for (lst = adv.stxs; lst != NULL; lst = lst->next)
    analyzeSyntax(lst->element.stx);

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
  stx = newSyntax(nulsrcp, newId(&nulsrcp, verbName), elements, NULL, nulsrcp);

  adv.stxs = concat(adv.stxs, stx, SYNTAX_LIST);
  analyzeSyntax(stx);                   /* Make sure the syntax is analysed */
  return stx;
}


/*======================================================================*/
Syntax *defaultSyntax1(char *vrbstr, Context *context)
{
  /*
    Returns the address a default syntax node which is used for verbs
    in instances (taking one parameter) without any defined syntax:

    Syntax x = x (<id>).

    Where <id> is the class identifier of the class or inherited class
    in which the declaration occured.
  */

  Syntax *stx;
  List *elements;
  IdNode *classId;

  classId = classNameIn(context);
  if (classId == NULL)		/* No class, so use any */
    classId = newId(&nulsrcp, "object");

  elements = concat(concat(concat(NULL,
			      newElement(&nulsrcp, WORD_ELEMENT, newId(&nulsrcp,
							     vrbstr),
				     FALSE),
			      ELEMENT_LIST),
		       newElement(&nulsrcp, PARAMETER_ELEMENT, newId(&nulsrcp, classId->string), FALSE),
		       ELEMENT_LIST),
		newElement(&nulsrcp, END_OF_SYNTAX, NULL, FALSE), ELEMENT_LIST);
  stx = newSyntax(nulsrcp, newId(&nulsrcp, vrbstr), elements, NULL, nulsrcp);

  adv.stxs = concat(adv.stxs, stx, SYNTAX_LIST);
  analyzeSyntax(stx);                   /* Make sure the syntax is analysed */
  return stx;
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
    if (wrd->classbits&(1L<<PREPOSITION_WORD))
      lst = wrd->ref[PREPOSITION_WORD];
    if (wrd->classbits&(1L<<VERB_WORD))
      lst = wrd->ref[VERB_WORD];
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
static void generateRestrictionTable(void) {
  List *lst;

  /* Generate all syntax parameter restriction checks */
  TRAVERSE(lst, adv.stxs) {
    Syntax *stx = lst->element.stx;
    Syntax *nextSyntax;
    if (stx->firstSyntax) {
      stx->restrictionsAddress = generateRestrictions(stx->restrictionLists, stx);
      for (nextSyntax = stx->nextSyntaxForSameVerb; nextSyntax;
	   nextSyntax = nextSyntax->nextSyntaxForSameVerb)
	nextSyntax->restrictionsAddress = stx->restrictionsAddress;
    }
  }
}


/*======================================================================*/
Aaddr generateParseTable(void) {
  List *lst;
  Aaddr parseTableAddress;

  generateRestrictionTable();

  TRAVERSE(lst, adv.stxs)
    generateParseTree(lst->element.stx);

  parseTableAddress = nextEmitAddress();
  TRAVERSE(lst, adv.stxs)
    generateParseEntry(lst->element.stx);
  emit(EOF);

  return(parseTableAddress);
}


/*----------------------------------------------------------------------*/
static void generateParameterMapping(Syntax *syntax)
{
  List *list;
  Aaddr parameterMappingTableAddress = nextEmitAddress();
  List *originalParameters = syntax->id->symbol->fields.verb.parameterSymbols;
  List *originalPosition;
  Bool found;

  TRAVERSE(list, syntax->parameters) {
    /* Generate a parameter mapping entry */
    TRAVERSE(originalPosition, originalParameters) {
      /* Find its original position */
      if (strcmp(list->element.elm->id->string, originalPosition->element.sym->string) == 0) {
	emit(originalPosition->element.sym->code);
	found = TRUE;
	break;
      }
    }
    if (!found) syserr("Could not find parameter in '%s()'", __FUNCTION__);
  }
  emit(EOF);
  syntax->parameterMappingAddress = parameterMappingTableAddress;
}


/*======================================================================*/
Aaddr generateSyntaxMapping(void)
{
  List *list;
  Aaddr syntaxMappingTableAddress;
  SyntaxEntry entry;

  TRAVERSE(list, adv.stxs)
    generateParameterMapping(list->element.stx);

  syntaxMappingTableAddress = nextEmitAddress();
  TRAVERSE(list, adv.stxs) {
    entry.syntaxNumber = list->element.stx->number;
    entry.parameterMapping = list->element.stx->parameterMappingAddress;
    entry.verbCode = list->element.stx->id->code;
    emitEntry(&entry, sizeof(entry));
  }
  emit(EOF);

  return syntaxMappingTableAddress;
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
