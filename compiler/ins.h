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
  SymNod *symbol;		/* Pointer to symbol table node */
  IdNode *parent;		/* The instances parent */
  SlotsNode *slots;
  Aword adr;
} InsNod;


#endif
