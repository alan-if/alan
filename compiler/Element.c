/*----------------------------------------------------------------------*\

				Element.c

			      Element Nodes

\*----------------------------------------------------------------------*/


#include "Element.h"

#include "dump.h"




/*======================================================================

  newElement()

  Allocates and initialises a Element.

 */
#ifdef _PROTOTYPES_
Element *newElement(Srcp *srcp,	/* IN - Source position */
		    ElementKind kind,	/* IN - Kind of Element */
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

  return(new);
}


/*======================================================================

  analyseElements()

  Analyze a list of Elements.

 */
#ifdef _PROTOTYPES_
List *analyseElements(List *elements, List *restrictions, Syntax *syntax)
#else
List *analyseElement(elements, restrictions, syntax)
     List *elements;
     List *restrictions;
     Syntax *syntax;
#endif
{
  /* 4f - analyse elements */
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
  put("kind: "); dumpElementKind(element->kind); nl();
  put("restrictions: "); dumpList(element->restrictions, RESTRICTION_NODE); out();
}


