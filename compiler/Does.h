#ifndef _DOES_H_
#define _DOES_H_
/*----------------------------------------------------------------------*\

				DOES.H
			      Does Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "Id.h"


/* Types: */

typedef enum DoesKind {
  DOES_BEFORE,
  DOES_AFTER,
  DOES_ONLY,
  DOES_DEFAULT
} DoesKind;

typedef struct Does {		/* DOES nodes */
  Srcp srcp;			/* Source position */
  DoesKind kind;		/* Does kind */
  List *statements;		/* The statements */
} Does;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Does node */
extern Does *newDoes(Srcp *srcp,
		     DoesKind kind,
		     List *statements);

/* Generate a reference to a Does */
extern void generateDoes(Does *does);

/* Dump a Does node */
extern void dumpDoes(Does *does);

#else
extern Does *newDoes();
extern void generateDoes();
extern void dumpDoes();
#endif

#endif
