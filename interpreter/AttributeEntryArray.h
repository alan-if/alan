#ifndef ATTRIBUTEENTRYARRAY_H_
#define ATTRIBUTEENTRYARRAY_H_

#include "acode.h"

/* CONSTANTS */

/* TYPES */

/* DATA */

/* FUNCTIONS */
extern AttributeEntry *findAttribute(AttributeEntry *attributeTable, Aint attributeCode);
extern Aword getAttribute(AttributeEntry *attributeTable, Aint attributeCode);
extern void setAttribute(AttributeEntry *attributeTable, Aint attributeCode, Aword newValue);

#endif /* ATTRIBUTEENTRYARRAY_H_ */
