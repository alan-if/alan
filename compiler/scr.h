#ifndef _SCR_H_
#define _SCR_H_
/*----------------------------------------------------------------------*\

				SCR.H
			     Script Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "srcp.h"
#include "lst.h"
#include "ins.h"


/* Types: */

typedef struct ScrNod {		/* SCRIPT */
  Srcp srcp;			/* Source position */
  IdNod *id;			/* Id for this script (if any) */
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
		      IdNod *id,
		      int code,
		      List *descr,
		      List *stps);

/* Prepare the scripts of an Instance for analysis */
extern void prepscrs(List *scrs, InsNod *ins);

/* Analyse a list of Scripts */
extern void anscrs(List *scrs,
		   InsNod *ins);

/* Generate script routines for an Instace */
extern Aword gescrs(InsNod *ins);

/* Dump a Script node */
extern void duscr(ScrNod *scr);


#endif
