/*----------------------------------------------------------------------*\

                                TYPE.C
			     Type Handling

\*----------------------------------------------------------------------*/

#include "type_x.h"
#include "dump_x.h"


/*======================================================================*/
Bool isComplexType(TypeKind type) {
  return type == SET_TYPE || type == INSTANCE_TYPE;
}

/*======================================================================*/
char *typeToString(TypeKind type)
{
  switch (type) {
  case BOOLEAN_TYPE: return "Boolean"; break;
  case INTEGER_TYPE: return "Integer"; break;
  case STRING_TYPE: return "String"; break;
  case REFERENCE_TYPE: return "Reference"; break;
  case INSTANCE_TYPE: return "Instance"; break;
  case EVENT_TYPE: return "Event"; break;
  case SET_TYPE: return "Set"; break;
  case ERROR_TYPE: return "ERROR"; break;
  case UNINITIALIZED_TYPE: return "UNINITIALIZED"; break;
  }
  return "***ERROR: Unexpected type kind***";
}

/*======================================================================*/
void dumpType(TypeKind type)
{
  put(typeToString(type));
}


