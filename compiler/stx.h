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

#include "acode.h"


/* TYPES: */

typedef struct StxNod {		/* SYNTAX */
  Srcp srcp;			/* Source position of this syntax */
  struct IdNode *id;		/* Name of the verb */
  List *elms;			/* List of elements */
  List *ress;			/* List of class restrictions */
  List *pars;			/* AN - List of parameters */
  Bool generated;		/* AN - Already generated? */
  Bool muldef;			/* AN - Multiply defined verb syntax? */
  Aaddr elmsadr;		/* GE - Address to element table for */
				/* all stxs with the same first word */
  Aaddr resadr;			/* GE - Address to class restriction checks */
} StxNod;


#endif
