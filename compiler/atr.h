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
  int code;
  Aword stringAddress;		/* Acode address to the attribute name */
  Aword address;		/* Acode address to the attribute value */
  int value;			/* The initial value */
  Bool encoded;			/* For string attributes... */
  long fpos;
  int len;
} AtrNod;

#endif
