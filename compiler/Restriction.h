#ifndef _RESTRICTION_H_
#define _RESTRICTION_H_
/*----------------------------------------------------------------------*\

			    Restriction.h

		Syntax Element Class Restriction Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "Syntax.h"
#include "List.h"
#include "Id.h"

#include "acode.h"


/* Types: */

typedef enum RestrictionKind {
  ID_RESTRICTION,
  INTEGER_RESTRICTION,
  STRING_RESTRICTION
} RestrictionKind;


typedef struct Restriction {	/* RESTRICTION */
  Srcp srcp;			/* Source position of this element */
  Id *id;			/* Name */
  Bool single;			/* TRUE if a single class */
  RestrictionKind kind;		/* Kind of class (id, string, integer) */
  Id *class;			/* The classes the id may have */
  List *stms;			/* Statements to execute if not */
  Aaddr stmadr;			/* GE - Address to generated statements */
} Restriction;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new element Restrictiontriction node */
extern Restriction *newRestriction(Srcp *srcp,
				   Id *id,
				   Bool single,
				   RestrictionKind kind,
				   Id *class,
				   List *stms);

/* Analyze a list of Restriction nodes */
extern void analyseRestrictions(List *restrictions, List *parameters);

/* Generate code for a list of Restriction nodes */
extern Aaddr generateRestrictions(List *restrictions, Syntax *syntax);

/* Dump a Restrictiontriction node */
extern void dumpRestriction(Restriction *restriction);

#else
extern Restriction *newRestriction();
extern void ananlyseRestrictions();
extern Aaddr generateRestrictions();
extern void dumpRestriction();
#endif

#endif
