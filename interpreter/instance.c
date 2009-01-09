#include "instance.h"

/* IMPORTS */
#include "memory.h"
#include "parse.h"
#include "exe.h"
#include "instance.h"
#include "syserr.h"
#include "AttributeEntry.h"


/* PUBLIC DATA */

InstanceEntry *instances;   /* Instance table pointer */
static LiteralEntry literalTable[100];
LiteralEntry *literals = literalTable;

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
  return isLiteral(instance) && literals[literalFromInstance(instance)].type == NUMERIC_LITERAL;
}

Bool isString(int instance)
{
  return isLiteral(instance) && literals[literalFromInstance(instance)].type == STRING_LITERAL;
}


/*======================================================================*/
void setInstanceAttribute(int instance, int attribute, Aword value)
{
    char str[80];

    if (instance > 0 && instance <= header->instanceMax) {
        setAttribute(admin[instance].attributes, attribute, value);
        if (isLocation(instance))   /* May have changed so describe next time */
            admin[instance].visitsCount = 0;
    } else {
        sprintf(str, "Can't SET/MAKE instance (%d).", instance);
        syserr(str);
    }
}


/*======================================================================*/
void setInstanceStringAttribute(int instance, int attribute, char *string)
{
    free((char *)getInstanceAttribute(instance, attribute));
    setInstanceAttribute(instance, attribute, (Aword)string);
}


/*======================================================================*/
void setInstanceSetAttribute(int instance, int attribute, Aword set)
{
    freeSet((Set *)getInstanceAttribute(instance, attribute));
    setInstanceAttribute(instance, attribute, (Aword)set);
}


/*----------------------------------------------------------------------*/
static Aword literalAttribute(int literal, int attribute)
{
    char str[80];

    if (attribute == 1)
        return literals[literalFromInstance(literal)].value;
    else {
        sprintf(str, "Unknown attribute for literal (%d).", attribute);
        syserr(str);
    }
    return(EOF);
}


/*======================================================================*/
Aword getInstanceAttribute(int instance, int attribute)
{
    char str[80];

    if (isLiteral(instance))
        return literalAttribute(instance, attribute);
    else {
        if (instance > 0 && instance <= header->instanceMax)
            return getAttribute(admin[instance].attributes, attribute);
        else {
            sprintf(str, "Can't ATTRIBUTE item (%d).", instance);
            syserr(str);
        }
    }
    return(EOF);
}


/*======================================================================*/
char *getInstanceStringAttribute(int instance, int attribute)
{
    return strdup((char *)getInstanceAttribute(instance, attribute));
}


/*======================================================================*/
Set *getInstanceSetAttribute(int instance, int attribute)
{
    return copySet((Set *)getInstanceAttribute(instance, attribute));
}
