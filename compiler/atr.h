#ifndef _ATR_H_
#define _ATR_H_

/* USE other definitions */
#include "srcp.h"
#include "sym.h"
#include "id.h"
#include "exp.h"


/* Types: */

typedef struct AtrNod {		/* ATTRIBUTE */
  Srcp srcp;			/* Source position of the attribute */
  TypeKind typ;			/* Type of this attribute */
  IdNode *id;			/* Id of the attribute */
  Aword stradr;			/* Acode address to the attribute name */
  Aword adr;			/* Acode address to the attribute value */
  int val;			/* The initial value */
  Bool encoded;			/* For string attributes... */
  long fpos;
  int len;
} AtrNod;

#endif
