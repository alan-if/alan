#ifndef _CLA_H_
#define _CLA_H_
/*----------------------------------------------------------------------*\

				CLA.H
			      Syntax Element Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "stx.h"
#include "lst.h"
#include "nam.h"

#include "acode.h"


/* Types: */

typedef struct ClaNod {		/* CLASS */
  Srcp srcp;			/* Source position of this element */
  NamNod *nam;			/* Name */
  Boolean single;		/* TRUE if a single class */
  NamKind classes;		/* The classes the id may have */
  List *stms;			/* Statements to execute if not */
  Aaddr stmadr;			/* GE - Address to generated statements */
} ClaNod;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new element Class node */
extern ClaNod *newcla(Srcp *srcp,
		      NamNod *nam,
		      Boolean single,
		      NamKind classes,
		      List *stms);

/* Analyze a list of Class nodes */
extern void anclas(List *clas, List *params);

/* Generate code for a list of Class nodes */
extern Aaddr geclas(List *clas, StxNod *stx);

/* Dump a Class node */
extern void ducla(ClaNod *cla);

#else
extern ClaNod *newcla();
extern void anclas();
extern Aaddr geclas();
extern void ducla();
#endif

#endif
