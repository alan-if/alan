#ifndef _TYPE_H_
#define _TYPE_H_
/*----------------------------------------------------------------------*\

				TYPE.H
			      Type Nodes

\*----------------------------------------------------------------------*/


/* USE other definitions */


/* TYPES: */

typedef enum TypeKind {		/* TYPE kinds */
  ERROR_TYPE = 0,
  UNKNOWN_TYPE,
  INTEGER_TYPE,
  BOOLEAN_TYPE,
  STRING_TYPE,
  INSTANCE_TYPE
} TypeKind;

#endif
