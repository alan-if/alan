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

typedef struct ResNod {		/* RESTRICTION */
  Srcp srcp;			/* Source position of this element */
  NamNod *nam;			/* Name */
  Bool single;		/* TRUE if a single class */
  NamKind classes;		/* The classes the id may have */
  List *stms;			/* Statements to execute if not */
  Aaddr stmadr;			/* GE - Address to generated statements */
} ResNod;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new element Restriction node */
extern ResNod *newres(Srcp *srcp,
		      NamNod *nam,
		      Bool single,
		      NamKind classes,
		      List *stms);

/* Analyze a list of Restriction nodes */
extern void anress(List *ress, List *params);

/* Generate code for a list of Restriction nodes */
extern Aaddr geress(List *ress, StxNod *stx);

/* Dump a Restriction node */
extern void dures(ResNod *res);

#else
extern ResNod *newres();
extern void anress();
extern Aaddr geress();
extern void dures();
#endif

#endif
