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
  int code;			/* Code for this script */
  List *descr;			/* List of optional description statements */
  Aaddr stmadr;			/* ACODE address description statements */
  List *stps;			/* List of step-blocks */
  Aaddr stpadr;			/* ACODE address to step table */
} ScrNod;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Script node */
extern ScrNod *newscr(Srcp *srcp,
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

#else
extern ScrNod *newscr();
extern void anscrs();
extern Aword gescrs();
extern void duscr();
#endif

#endif
