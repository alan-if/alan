#ifndef _EXIT_H_
#define _EXIT_H_
/*----------------------------------------------------------------------*\

				Exit.h

			      Exit Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "List.h"
#include "Id.h"
#include "Does.h"

#include "acode.h"


/* Types: */

typedef struct Exit {		/* EXIT */
  Srcp srcp;			/* Source position of this exit */
  List *directions;		/* Directions of the exits */
  Id *to;			/* Name of the location to exit to */
  List *checks;			/* List of checks */
  Aaddr chkadr;			/* ACODE address of check code */
  Does *does;			/* The does part */
  Aaddr stmadr;			/* ACODE address of statements code */
} Exit;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Exit node */
extern Exit *newExit(Srcp *srcp,
		     List *directions,
		     Id *to,
		     List *checks,
		     List *statements);

/* Analyze a list of Exits */
extern void analyseExits(List *exits);

/* Generate code for a list of Exits */
extern Aaddr generateExits(List *exits);

/* Dump an Exit node */
extern void dumpExit(Exit *exit);

#else
extern Exit *newExit();
extern void anExits();
extern Aaddr geExits();
extern void duExit();
#endif

#endif
