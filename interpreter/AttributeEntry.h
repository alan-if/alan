#ifndef ATTRIBUTEENTRY_H_
#define ATTRIBUTEENTRY_H_

#include "acode.h"

/* CONSTANTS */

/* TYPES */

/* DATA */

/* FUNCTIONS */
extern AttributeEntry *findAttribute(AttributeEntry *attributeTable, int attributeCode);
extern Aword getAttribute(AttributeEntry *attributeTable, int attributeCode);
extern void setAttribute(AttributeEntry *attributeTable, int attributeCode, Aword newValue);

#endif /* ATTRIBUTE_H_ */
