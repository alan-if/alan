#ifndef _EXT_H_
#define _EXT_H_
/*----------------------------------------------------------------------*\

				EXT.H
			      Exit Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "id.h"


/* Types: */

typedef struct ExtNod {		/* EXIT */
  Srcp srcp;			/* Source position of this exit */
  List *dirs;			/* Directions of the exits */
  IdNode *to;			/* Id of the location to exit to */
  List *chks;			/* List of checks */
  Aaddr chkadr;			/* ACODE address of check code */
  List *stms;			/* List of statements */
  Aaddr stmadr;			/* ACODE address of statements code */
} ExtNod;



/* Data: */

extern int dirmin, dirmax;
extern int dircount;


/* Functions: */

/* Create a new Exit node */
extern ExtNod *newext(Srcp *srcp,
		      List *dirs,
		      IdNode *to,
		      List *chks,
		      List *stms);

/* Analyze a list of Exits */
extern void anexts(List *exts);

/* Generate code for a list of Exits */
extern Aaddr geexts(List *exts);

/* Dump an Exit node */
extern void duext(ExtNod *ext);


#endif
