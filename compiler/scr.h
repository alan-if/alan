#ifndef _SCR_H_
#define _SCR_H_
/*----------------------------------------------------------------------*\

				SCR.H
			     Script Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "srcp.h"
#include "lst.h"
#include "act.h"


/* Types: */

typedef struct ScrNod {		/* SCRIPT */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Name for this script (if any) */
  int code;			/* and its code */
  List *descr;			/* List of optional description statements */
  Aaddr stmadr;			/* ACODE address description statements */
  List *stps;			/* List of step-blocks */
  Aaddr stpadr;			/* ACODE address to step table */
} ScrNod;


/* Data: */


/* Functions: */

/* Create a new Script node */
extern ScrNod *newscr(Srcp *srcp,
		      NamNod *nam,
		      int code,
		      List *descr,
		      List *stps);

/* Analyse a list of Scripts */
extern void anscrs(List *scrs,
		   ActNod *act);

/* Generate script routines for an actor */
extern Aword gescrs(ActNod *act);

/* Dump a Script node */
extern void duscr(ScrNod *scr);


#endif
