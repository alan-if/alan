#ifndef _TYPE_H_
#define _TYPE_H_
/*----------------------------------------------------------------------*\

				TYPE.H
			      Type Nodes

\*----------------------------------------------------------------------*/


/* USE other definitions */


/* TYPES: */

typedef enum TypeKind {		/* TYPE kinds */
  UNINITIALIZED_TYPE = 0,
  ERROR_TYPE,
  INTEGER_TYPE,
  BOOLEAN_TYPE,
  STRING_TYPE,
  INSTANCE_TYPE
} TypeKind;

#endif
