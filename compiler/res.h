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
  IdNode *classId;		/* SY - Which named class must it be */
  List *stms;			/* SY - Statements to execute if not */
  Aaddr stmadr;			/* GE - Address to generated statements */
} ResNod;

#endif
