#ifndef _STX_H_
#define _STX_H_
/*----------------------------------------------------------------------*\

				STX.H
			      Syntax Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "nam.h"

#include "acode.h"


/* Types: */

typedef struct StxNod {		/* SYNTAX */
  Srcp srcp;			/* Source position of this syntax */
  NamNod *nam;			/* Name of the verb */
  List *elms;			/* List of elements */
  List *ress;			/* List of class restrictions */
  List *pars;			/* AN - List of parameters */
  Bool generated;		/* AN - Already generated? */
  Aaddr elmsadr;		/* GE - Address to element table for */
				/* all stxs with the same first word */
  Aaddr resadr;			/* GE - Address to class restriction checks */
} StxNod;



/* Data: */


/* Functions: */

/* Create a new Syntax node */
extern StxNod *newstx(Srcp *srcp,
		      NamNod *verb,
		      List *comps,
		      List *ress);

/* Create a deafult syntax node */
extern StxNod *defaultStx(char vrbstr[]);

/* Compare parameter lists of two syntaxes */
extern Bool eqparams(StxNod *stx1,
			StxNod *stx2);

/* Analyze a list of Syntaxs */
extern void anstxs(void);

/* Generate code for a list of Syntaxs */
extern Aaddr gestxs(void);

/* Dump a Syntax node */
extern void dustx(StxNod *stx);


#endif
