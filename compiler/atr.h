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
  int value;			/* The initial value */
  Bool encoded;			/* For string attributes... */
  long fpos;
  long len;
  IdNode *instance;		/* For INSTANCE and SET type attributes */
  TypeKind setType;		/* Type of elements in SET attributes */
  Symbol *setClass;		/* Class of instance elements in SET attributes */
  List *set;			/* An expression list for the initial set */
} Attribute;

#endif
