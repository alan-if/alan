#ifndef _ATR_H_
#define _ATR_H_

/* USE other definitions */
#include "srcp.h"
#include "sym.h"
#include "id.h"
#include "exp.h"
#include "../interpreter/acode.h"


/* TYPES: */

typedef enum {
  UNKNOWN_INHERITANCE,
  LOCAL,
  INHERITED_REDEFINED,
  INHERITED
} AttributeInheritance;

typedef struct Attribute {
  Srcp srcp;			/* Source position of the attribute */
  TypeKind type;		/* Type of this attribute */
  IdNode *id;			/* Id of the attribute */
  AttributeInheritance inheritance;
  Aword stringAddress;		/* Acode address to the attribute name */
  Aword address;		/* Acode address to the attribute value */

  int value;			/* INTEGER - The initial value */

  long fpos;			/* STRING - initial value */
  long len;			/* STRING */
  Bool encoded;			/* STRING */

  IdNode *reference;		/* REFERENCE - initial value and class in the
				   symbol */
  Symbol *referenceClass;

  Expression *set;		/* SET - An expression for the initial set */
  TypeKind setType;		/* SET - Type of elements in SET */
  Aaddr setAddress;		/* Address to the attributes initial set */

  Symbol *setClass;		/* Class of instance elements in SET attributes
				   and REFERENCE attributes */
} Attribute;

#endif
