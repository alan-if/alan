#ifndef _CLA_H_
#define _CLA_H_
/*----------------------------------------------------------------------*\

				CLA.H
			     Class Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "nam.h"
#include "lst.h"
#include "whr.h"
#include "cnt.h"


/* Types */

typedef struct ClaNod {		/* CLASS */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Name of the class */
  List *heritage;		/* The list of inherited classes */
  List *nams;			/* Name Slot: List of adjectives and a noun */
  WhrNod *whr;			/* Where Slot: Initial location */
  List *atrs;			/* Attributes Slot: */
  CntNod *props;		/* Container Slot: limits, header, empty */
  List *surr;			/* Surroundings Slot: inside description */
  List *descr;			/* Description Slot: outside description */
  List *mentioned;		/* Mentioned Slot: short form */
  List *does;			/* Does Slot: when entered */
  List *exits;			/* Exits Slot: how to exit */
  List *verbs;			/* Verbs Slot: local verbs */
  List *script;			/* Script Slot: what it does */
} ClaNod;


/* Data: */

/* Methods */

#ifdef _PROTOTYPES_

/* Create a new Class node */
extern ClaNod *newcla(
		      Srcp srcp,
		      NamNod *nam,
		      List *heritage,
		      List *nams,
		      WhrNod *whr,
		      List *atrs,
		      CntNod *props,
		      List *surr,
		      List *descr,
		      List *mentioned,
		      List *does,
		      List *exits,
		      List *verbs,
		      List *script);

/* Analyze all Classes */
extern void anclas(void);

/* Dump a Class node */
extern void ducla(ClaNod *cla);

#else
extern ClaNod *newcla();
extern void anclas();
extern void ducla();
#endif

#endif
