#ifndef _CLA_H_
#define _CLA_H_
/*----------------------------------------------------------------------*\

				CLASS.H
			     Class Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "types.h"
#include "id.h"
#include "slt.h"


/* Types */

typedef struct ClaNod {		/* CLASS */
  Srcp srcp;			/* Source position */
  IdNode *id;			/* Programmer Name of the class */
  SymNod *symbol;		/* Pointer to symbol table node */
  IdNode *heritage;		/* The class' heritage (parent) */
  struct Slots *slt;		/* Slots */
} ClaNod;


/* Data: */
extern int classCount;

/* Methods: */

/* Create a new Claect node */
extern ClaNod *newcla(Srcp *srcp,
		      IdNode *id,
		      IdNode *heritage,
		      Slots *slt);

/* Symbolize all Classes */
extern void symbolizeClasses(void);

/* Analyze all Classes */
extern void analyzeClasses(void);

/* Generate code for all Classes */
extern Aaddr generateClasses(void);

/* Dump all Class node */
extern void dumpClasses(void);


#endif
