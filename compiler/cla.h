#ifndef _CLA_H_
#define _CLA_H_
/*----------------------------------------------------------------------*\

				CLASS.H
			     Class Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "nam.h"
#include "lst.h"
#include "whr.h"
#include "slt.h"


/* Types */

typedef struct ClaNod {		/* CLASS */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Programmer Name of the class */
  List *heritage;		/* The class' heritage */
  Slots *slt;			/* Slots */
} ClaNod;


/* Data: */


/* Methods: */

/* Create a new Claect node */
extern ClaNod *newcla(Srcp *srcp,
		      NamNod *nam,
		      List *heritage,
		      Slots *slt);

/* Analyze all Classes */
extern void anclas(void);

/* Prepare all Classes before analysis */
extern void prepclas(void);

/* Generate code for all Classes */
extern Aaddr geclas(void);

/* Dump an Class node */
extern void ducla(ClaNod *cla);


#endif
