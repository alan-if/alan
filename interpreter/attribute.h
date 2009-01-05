#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include "acode.h"

/* CONSTANTS */

/* TYPES */

/* DATA */

/* FUNCTIONS */
extern Attribute *findAttribute(Attribute *attributeTable, int attributeCode);
extern Aword getAttribute(Attribute *attributeTable, int attributeCode);
extern void setAttribute(Attribute *attributeTable, int attributeCode, Aword newValue);

#endif /* ATTRIBUTE_H_ */
