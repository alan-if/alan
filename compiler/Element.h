#ifndef _ELEMENT_H_
#define _ELEMENT_H_
/*----------------------------------------------------------------------*\

			      Element.h

			 Syntax Element Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"

#include "Restriction.h"
#include "Syntax.h"

#include "List.h"
#include "Id.h"

#include "acode.h"


/* Types: */

typedef enum ElementKind {
  PARAMETER_ELEMENT,
  WORD_ELEMENT
} ElementKind;

typedef struct Element {	/* ELEMENT */
  Srcp srcp;			/* Source position of this element */
  ElementKind kind;		/* Kind of Parameter */
  Id *id;			/* Name */
  Bool multiple;		/* Multiple indicator */
  int number;			/* AN - parameter number */
  List *restrictions;		/* AN - link to the class restriction check */
  Syntax *syntax;		/* AN - pointer back to syntax node */
} Element;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Syntax Element node */
extern Element *newElement(Srcp *srcp,
			   ElementKind kind,
			   Id *id,
			   Bool multiple);

/* Analyze a list of Syntax elements and return a list of the parameters */
extern List *analyseElements(List *elements,
			     List *restrictions,
			     Syntax *syntax);

/* Generate code for a list of Syntax elements */
extern Aaddr generateElements(List *elements);

/* Dump a Syntax node */
extern void dumpElement(Element *element);

#else
extern Element *newElement();
extern List *analyseElements();
extern Aaddr generateElements();
extern void dumpElement();
#endif

#endif
