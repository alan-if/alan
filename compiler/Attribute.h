#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_
/*----------------------------------------------------------------------*\

			     Attribute.h

			   Attribute Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "alan.h"

#include "Srcp.h"
#include "List.h"
#include "Id.h"
#include "Expression.h"

#include "emit.h"


/* Types: */

typedef struct Attribute {		/* ATTRIBUTE */
  Srcp srcp;			/* Source position of the attribute */
  TypeKind type;		/* Type of this attribute */
  Id *id;			/* Name of the attribute */
  int code;			/* The attribute code (within class tree) */
  Aword stradr;			/* Acode address to the attribute name (debug) */
  Aword adr;			/* Acode address to the attribute value */
  Aword value;			/* The initial value */
  Bool encoded;			/* For string attributes... */
  long fpos;
  int len;
} Attribute;


/* Data: */


/* Functions: */

#ifdef _PROTOTYPES_

/* Create a new Attribute node */
extern Attribute *newAttribute(Srcp *srcp,
			       TypeKind type,
			       Id *id,
			       int value,
			       long fpos,
			       int len);

/* Find a particular attribute */
extern Attribute *findAttribute(Id *id,
				List *attributes);

/* Analyze a list of attribute nodes */
extern void analyseAttributes(List *attributes);

/* Generate an attribute table */
extern Aaddr generateAttributes(List *attributes);

/* Generate the string initialisation table */
extern Aaddr generateStringInit(void);

/* Dump an Attribute node */
extern void dumpAttribute(Attribute *attribute);

#else
extern Attribute *newAttribute();
extern Attribute *findAttribute();
extern void sortattribute();
extern void anattributes();
extern void prepattributes();
extern Aaddr geattributes();
extern Aaddr geinit();
extern void duattribute();
#endif

#endif
