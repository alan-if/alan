#include "Attribute.h"

#include "main.h"
#include "syserr.h"
#include "state.h"


/*======================================================================*/
Attribute *findAttribute(Attribute *attributeTable, int attributeCode)
{
  Attribute *attribute = attributeTable;
  while (attribute->code != attributeCode) {
    attribute++;
    if (isEndOfList(attribute))
      syserr("Attribute not found.");
  }
  return attribute;
}


/*======================================================================*/
Aword getAttribute(Attribute *attributeTable, int attributeCode)
{
  Attribute *attribute = findAttribute(attributeTable, attributeCode);

  return attribute->value;
}


/*======================================================================*/
void setAttribute(Attribute *attributeTable, int attributeCode, Aword newValue)
{
  Attribute *attribute = findAttribute(attributeTable, attributeCode);

  attribute->value = newValue;
  gameStateChanged = TRUE;
}
