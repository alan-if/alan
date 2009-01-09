#ifndef INSTANCES_H_
#define INSTANCES_H_
/*----------------------------------------------------------------------*\

  Instance

\*----------------------------------------------------------------------*/

/* Imports: */
#include "acode.h"
#include "types.h"
#include "set.h"

/* Constants: */


/* Types: */
typedef struct AdminEntry { /* Administrative data about instances */
  Aint location;
  AttributeEntry *attributes;
  Abool alreadyDescribed;
  Aint visitsCount;
  Aint script;
  Aint step;
  Aint waitCount;
} AdminEntry;


/* Data: */
extern InstanceEntry *instances; /* Instance table pointer */
extern LiteralEntry *literals;

extern AdminEntry *admin;   /* Administrative data about instances */
extern AttributeEntry *attributes; /* Dynamic attribute values */


/* Functions: */
extern Bool isObject(int instance);
extern Bool isContainer(int instance);
extern Bool isActor(int instance);
extern Bool isLocation(int instance);
extern Bool isLiteral(int instance);
extern Bool isNumeric(int instance);
extern Bool isString(int instance);

extern Aword getInstanceAttribute(int instance, int attribute);
extern char *getInstanceStringAttribute(int instane, int attribute);
extern Set *getInstanceSetAttribute(int instance, int attribute);

extern void setInstanceAttribute(int instance, int atr, Aword value);
extern void setInstanceStringAttribute(int instance, int attribute, char *string);
extern void setInstanceSetAttribute(int instance, int atr, Aword set);


#endif /* INSTANCES_H_ */
