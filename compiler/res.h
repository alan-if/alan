#ifndef _RES_H_
#define _RES_H_
/*----------------------------------------------------------------------*\

				RES.H
		Syntax Element Class Restriction Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "id.h"

#include "acode.h"


/* Types: */

typedef enum ResKind {
  ID_RESTRICTION,		/* An identifier */
  INTEGER_RESTRICTION,		/* An integer */
  STRING_RESTRICTION,		/* A string */
  OBJECT_RESTRICTION,
  ACTOR_RESTRICTION,
  CONTAINER_RESTRICTION,
  CONTAINEROBJECT_RESTRICTION,	/* compatibility with 2.x */
  CONTAINERACTOR_RESTRICTION	/* compatibility with 2.x */
} ResKind;


typedef struct ResNod {		/* RESTRICTION */
  Srcp srcp;			/* SY - Source position of this element */
  IdNode *id;			/* SY - Parameter name to apply restriction to */
  Bool single;			/* AN - A single class in the list? */
  List *classes;		/* SY - Which named class or classes must it be */
  int classbits;		/* AN - Bits for the classes the parameter must be */
  List *stms;			/* SY - Statements to execute if not */
  Aaddr stmadr;			/* GE - Address to generated statements */
} ResNod;

#endif
