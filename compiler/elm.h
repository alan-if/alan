#ifndef _ELM_H_
#define _ELM_H_
/*----------------------------------------------------------------------*\

				ELM.H
			      Syntax Element Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "stx.h"
#include "res.h"
#include "id.h"


/* TYPES: */

typedef enum ElmKind {
  PARAMETER_ELEMENT,
  WORD_ELEMENT,
  END_OF_SYNTAX
} ElmKind;

typedef struct ElmNod {		/* ELEMENT */
  Srcp srcp;			/* Source position of this element */
  ElmKind kind;			/* Kind of Parameter */
  IdNode *id;			/* Identifier */
  int flags;			/* Multiple/omni etc. flags */
  ResNod *res;			/* AN - link to the class restriction check */
  StxNod *stx;			/* AN - pointer back to syntax node */
} ElmNod;


#endif
