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
  ID_RESTRICTION,
  INTEGER_RESTRICTION,
  STRING_RESTRICTION,
  OBJECT_RESTRICTION,
  ACTOR_RESTRICTION,
  CONTAINER_RESTRICTION,
  CONTAINEROBJECT_RESTRICTION,
  CONTAINERACTOR_RESTRICTION
} ResKind;


typedef struct ResNod {		/* RESTRICTION */
  Srcp srcp;			/* Source position of this element */
  NamNod *nam;			/* Name */
  Bool single;			/* TRUE if a single class */
  ResKind resKd;		/* The restriction type the parameter must follow */
  List *classes;		/* Which named class must it be if ID_RESTRICTION */
  List *stms;			/* Statements to execute if not */
  Aaddr stmadr;			/* GE - Address to generated statements */
} ResNod;



/* Data: */


/* Functions: */

/* Create a new element Restriction node */
extern ResNod *newres(Srcp *srcp,
		      NamNod *nam,
		      Bool single,
		      ResKind resKd,
		      List *classes,
		      List *stms);

/* Analyze a list of Restriction nodes */
extern void anress(List *ress, List *params);

/* Generate code for a list of Restriction nodes */
extern Aaddr geress(List *ress, StxNod *stx);

/* Dump a Restriction node */
extern void dures(ResNod *res);


#endif
