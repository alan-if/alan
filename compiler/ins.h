#ifndef _INS_H_
#define _INS_H_
/*----------------------------------------------------------------------*\

				INS.H
			     Instance Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "id.h"
#include "sym.h"
#include "slt.h"


/* TYPES: */

typedef struct InsNod {		/* INSTANCE */
  Srcp srcp;			/* Source position */
  struct IdNode *id;		/* Name of the instance */
  struct SymNod *symbol;	/* Pointer to symbol table node */
  struct IdNode *heritage;	/* The instances heritage */
  struct Slots *slt;		/* Slots */
} InsNod;


#endif
