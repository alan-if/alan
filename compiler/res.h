#ifndef _RES_H_
#define _RES_H_
/*----------------------------------------------------------------------*\

				RES.H
		Syntax Element Class Restriction Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "stx.h"
#include "lst.h"
#include "nam.h"

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
  struct IdNod *id;		/* SY - Parameter name to apply restriction to */
  Bool single;			/* AN - A single class in the list? */
  List *classes;		/* SY - Which named class or classes must it be */
  int classbits;		/* AN - Bits for the classes the parameter must be */
  List *stms;			/* SY - Statements to execute if not */
  Aaddr stmadr;			/* GE - Address to generated statements */
} ResNod;



/* Data: */


/* Functions: */

/* Create a new element Restriction node */
extern ResNod *newres(Srcp *srcp,
		      struct IdNod *id,
		      Bool single,
		      List *classes,
		      List *stms);

/* Analyze a list of Restriction nodes */
extern void anress(List *ress, List *params);

/* Generate code for a list of Restriction nodes */
extern Aaddr geress(List *ress, StxNod *stx);

/* Dump a Restriction node */
extern void dures(ResNod *res);


#endif
