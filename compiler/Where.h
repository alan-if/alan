#ifndef _WHERE_H_
#define _WHERE_H_
/*----------------------------------------------------------------------*\

			       Where.h
			     Where Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "What.h"


/* Types: */

typedef enum WhereKind {	/* WHERE kinds */
  WHERE_DEFAULT,		/* None */
  WHERE_HERE,			/* HERE */
  WHERE_NEAR,			/* NEARBY */
  WHERE_AT,			/* AT what */
  WHERE_IN			/* IN what */
} WhereKind;


typedef struct Where {		/* WHERE node */
  Srcp srcp;			/* Source position */
  WhereKind Where;		/* Where kind */
  What *what;			/* What */
} Where;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Allocate a new Where node */
extern Where *newWhere(Srcp *srcp,
		       WhereKind kind,
		       What *what);

/* Analyse a Where reference */
extern void analyseWhere(Where *Where);

/* Generate a Where reference */
extern void generateWhere(Where *Where);

/* Dump a Where node */
extern void dumpWhere(Where *Where);

#else
extern Where *newWhere();
extern void analyseWhere();
extern void generateWhere();
extern void dumpWhere();
#endif

#endif
