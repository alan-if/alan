/*----------------------------------------------------------------------*\

				Element.c

			      Element Nodes

\*----------------------------------------------------------------------*/


#include "Element.h"
#include "Restriction.h"
#include "Word.h"
#include "lmList.h"
#include "dump.h"
#include "alan.h"




/*======================================================================

  newElement()

  Allocates and initialises a Element.

 */
#ifdef _PROTOTYPES_
Element *newElement(Srcp *srcp,	/* IN - Source position */
		    ElementKind kind, /* IN - Kind of Element */
		    Id *id,	/* IN - The identifier */
		    Bool multiple) /* IN - Multiple marking? */
#else
Element *newElement(srcp, kind, id, multiple)
     Srcp *srcp;
     ElementKind kind;
     Id *id;
     Bool multiple;
#endif
{
  Element *new;			/* The newly allocated area */

  new = NEW(Element);

  new->srcp = *srcp;
  new->kind = kind;
  new->id = id;
  new->multiple = multiple;
  new->restrictions = NULL;

  return(new);
}


/*----------------------------------------------------------------------

  analyseElement()

  Analyze a syntax Element.

 */
#ifdef _PROTOTYPES_
static void analyseElement(Element *element)
#else
List *analyseElement(element)
     Element *element;
#endif
{
  switch (element->kind) {
  case PARAMETER_ELEMENT:
    break;
  case WORD_ELEMENT:
    newWord(element->id->string, PREPOSITION_WORD, NULL);
    break;
  default:
    syserr("Unknown syntax element node kind in analyseElement()");
    break;
  }
}



/*======================================================================

  analyseElements()

  Analyze a list of Elements.

 */
#ifdef _PROTOTYPES_
List *analyseElements(Syntax *syntax)
#else
List *analyseElement(syntax)
     Syntax *syntax;
#endif
{
  Element *element = syntax->elements->the.element;	/* First syntax element */
  List *list;
  List *parameters = NULL;
  List *restrictions;
  List *elements;
  Bool multiple = FALSE;
  int parameterNumber = 1;

  if (element->kind != WORD_ELEMENT)
    /* First element must be a player word */
    lmLog(&element->srcp, 209, sevERR, "");
  else
    /* Enter word in dictionary as a verb */
    newWord(element->id->string, VERB_WORD, (void *)syntax);

  /* Analyze the elements, number the parameters and find class restriction */
  /* Start with the second since the first is analyzed above */
  for (list = syntax->elements->next; list != NULL; list = list->next) {
    if (list->the.element->kind == PARAMETER_ELEMENT) {
      list->the.element->code = parameterNumber++;
      if (list->the.element->multiple) {
	if (multiple)
	  lmLog(&list->the.element->srcp, 217, sevWAR, "");
	else
	  multiple = TRUE;
      }
      parameters = append(parameters, list->the.element);

      /* Find all class restrictions for this parameter */
      for (restrictions = syntax->restrictions; restrictions;
	   restrictions = restrictions->next) {
	if (equalIds(restrictions->the.restriction->id,
		     list->the.element->id)) {
	  list->the.element->restrictions =
	    append(list->the.element->restrictions,
		   restrictions->the.restriction);
	  restrictions->the.restriction->code = list->the.element->code;
	}
      }
    }
    analyseElement(list->the.element);
  }

  /* Check for multiple definition of parameter names */
  for (list = parameters; list != NULL; list = list->next)
    for (elements = list->next; elements != NULL; elements = elements->next) {
      if (equalIds(list->the.element->id, elements->the.element->id))
	lmLog(&elements->the.element->id->srcp, 216, sevERR,
	      elements->the.element->id->string);
    }
  return parameters;


  return NULL;
}



/*----------------------------------------------------------------------

  dumpElementKind()

  Dump a Element kind.

  */
#ifdef _PROTOTYPES_
static void dumpElementKind(ElementKind kind)
#else
static void dumpElementKind(kind)
     ElementKind kind;
#endif
{
  switch (kind) {
  case PARAMETER_ELEMENT: put("PARAMETER"); break;
  case WORD_ELEMENT: put("WORD"); break;
  default: put("*** ERROR unknown element kind ***");
  }
}



/*======================================================================

  dumpElement()

  Dump a Element node.

 */
#ifdef _PROTOTYPES_
void dumpElement(Element *element)
#else
void dumpElement(element)
     Element *element;
#endif
{
  if (element == NULL) {
    put("NULL");
    return;
  }

  put("ELEMENT: "); dumpSrcp(&element->srcp); in();
  put("id: "); dumpId(element->id); nl();
  put("kind: "); dumpElementKind(element->kind); nl();
  put("restrictions: "); dumpList(element->restrictions, RESTRICTION_NODE); out();
}


