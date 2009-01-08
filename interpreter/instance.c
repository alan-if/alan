#include "instance.h"

/* IMPORTS */
#include "memory.h"
#include "parse.h"
#include "exe.h"


/* PUBLIC DATA */

InstanceEntry *instances;   /* Instance table pointer */
AdminEntry *admin;      /* Administrative data about instances */
AttributeEntry *attributes; /* Dynamic attribute values */

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* Instance query methods */

Bool isObject(int instance)
{
  return isA(instance, OBJECT);
}

Bool isContainer(int instance)
{
  return instance != 0 && instances[instance].container != 0;
}

Bool isActor(int instance)
{
  return isA(instance, ACTOR);
}

Bool isLocation(int instance)
{
  return isA(instance, LOCATION);
}


Bool isLiteral(int instance)
{
  return instance > header->instanceMax;
}

Bool isNumeric(int instance)
{
  return isLiteral(instance) && literal[literalFromInstance(instance)].type == NUMERIC_LITERAL;
}

Bool isString(int instance)
{
  return isLiteral(instance) && literal[literalFromInstance(instance)].type == STRING_LITERAL;
}
