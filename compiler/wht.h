#ifndef _WHT_H_
#define _WHT_H_
/*----------------------------------------------------------------------*\

				WHT.H
			      What Nodes
                                                                     
\*----------------------------------------------------------------------*/

/* USE: */
#include "id.h"


/* TYPES: */

typedef enum WhatKind {
  WHAT_LOCATION,
  WHAT_ACTOR,
  WHAT_ID,
  WHAT_THIS
} WhatKind;

typedef struct What {
  Srcp srcp;			/* Source position */
  WhatKind kind;		/* What kind */
  IdNode *id;			/* Identifier if WHAT_ID */
} What;

#endif
