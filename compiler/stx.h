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
  List *clas;			/* List of class definitions */
  List *pars;			/* AN - List of parameters */
  Boolean generated;		/* AN - Already generated? */
  Aaddr elmsadr;		/* GE - Address to element table for */
				/* all stxs with the same first word */
  Aaddr claadr;			/* GE - Address to class checks */
} StxNod;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Syntax node */
extern StxNod *newstx(Srcp *srcp,
		      NamNod *verb,
		      List *comps,
		      List *classes);

/* Create a deafult syntax node */
extern StxNod *defaultStx(char vrbstr[]);

/* Compare parameter lists of two syntaxes */
extern Boolean eqparams(StxNod *stx1,
			StxNod *stx2);

/* Analyze a list of Syntaxs */
extern void anstxs(void);

/* Generate code for a list of Syntaxs */
extern Aaddr gestxs(void);

/* Dump a Syntax node */
extern void dustx(StxNod *stx);

#else
extern StxNod *newstx();
extern StxNod *defaultStx();
extern Boolean eqparams();
extern void anstxs();
extern Aaddr gestxs();
extern void dustx();
#endif

#endif
