#ifndef _STX_H_
#define _STX_H_
/*----------------------------------------------------------------------*\

				STX.H
			      Syntax Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "id.h"

#include "../interpreter/acode.h"


/* TYPES: */

typedef struct Syntax {
  Srcp srcp;			/* Source position of this syntax */
  struct IdNode *id;		/* Name of the verb */
  int number;			/* Syntax number */
  List *elements;
  List *restrictionLists;
  List *parameters;
  Bool generated;
  Bool muldef;			/* AN - Multiply defined verb syntax? */
  Aaddr elementsAddress;	/* GE - Address to element table for */
				/* all stxs with the same first word */
  Aaddr restrictionsAddress;	/* GE - Address to class restriction checks */
} Syntax;


#endif
