#ifndef _RES_H_
#define _RES_H_
/*----------------------------------------------------------------------*\

				RES.H
		Syntax Element Class Restriction Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "lst.h"
#include "id.h"

#include "../interpreter/acode.h"


/* Types: */

typedef enum RestrictionKind {
  ID_RESTRICTION,
  INTEGER_RESTRICTION,
  STRING_RESTRICTION,
  CONTAINER_RESTRICTION
} RestrictionKind;


typedef struct ResNod {		/* RESTRICTION */
  Srcp srcp;
  RestrictionKind kind;
  IdNode *parameterId;
  IdNode *classId;
  List *stms;
  Aaddr stmadr;
} ResNod;

#endif
