#ifndef _WHAT_H_
#define _WHAT_H_
/*----------------------------------------------------------------------*\

				WHAT.H
			      What Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "Id.h"


/* Types: */

typedef enum WhatKind {
  WHAT_LOCATION,
  WHAT_ACTOR,
  WHAT_THIS,
  WHAT_ID
} WhatKind;

typedef struct What {		/* WHAT nodes */
  Srcp srcp;			/* Source position */
  WhatKind kind;		/* What kind */
  Id *id;			/* for WHAT id */
} What;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new What node */
extern What *newWhat(Srcp *srcp,
		     WhatKind kind,
		     Id *id);

/* Generate a reference to a What */
extern void generateWhat(What *what);

/* Dump a What node */
extern void dumpWhat(What *what);

#else
extern What *newWhat();
extern void generateWhat();
extern void dumpWhat();
#endif

#endif
