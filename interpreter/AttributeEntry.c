#include "AttributeEntry.h"

#include "syserr.h"
#include "state.h"
#include "lists.h"


/*======================================================================*/
AttributeEntry *findAttribute(AttributeEntry *attributeTable, int attributeCode)
{
  AttributeEntry *attribute = attributeTable;
  while (attribute->code != attributeCode) {
    attribute++;
    if (isEndOfList(attribute))
      syserr("Attribute not found.");
  }
  return attribute;
}


/*======================================================================*/
Aword getAttribute(AttributeEntry *attributeTable, int attributeCode)
{
  AttributeEntry *attribute = findAttribute(attributeTable, attributeCode);

  return attribute->value;
}


/*======================================================================*/
void setAttribute(AttributeEntry *attributeTable, int attributeCode, Aword newValue)
{
  AttributeEntry *attribute = findAttribute(attributeTable, attributeCode);

  attribute->value = newValue;
  gameStateChanged = TRUE;
}
