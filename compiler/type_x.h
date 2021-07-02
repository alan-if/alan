#ifndef _TYPE_X_H_
#define _TYPE_X_H_
/*----------------------------------------------------------------------*\

                TYPE_X.H
                 Type handling

\*----------------------------------------------------------------------*/

#include "type.h"
#include "types.h"

extern bool isComplexType(TypeKind type);
extern char *typeToString(TypeKind type);
extern bool equalTypes(TypeKind typ1, TypeKind typ2);
extern void dumpType(TypeKind type);

#endif
