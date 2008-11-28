#include "AttributeEntryArray.h"

#include "main.h"
#include "syserr.h"
#include "state.h"


/*======================================================================*/
AttributeEntry *findAttribute(AttributeEntry *attributeTable,
			      Aint attributeCode)
{
  AttributeEntry *attribute = attributeTable;
  while (attribute->code != attributeCode) {
    attribute++;
    if (endOfTable(attribute))
      syserr("Attribute not found.");
  }
  return attribute;
}


/*======================================================================*/
Aword getAttribute(AttributeEntry *attributeTable, Aint attributeCode)
{
  AttributeEntry *attribute = findAttribute(attributeTable, attributeCode);

  return attribute->value;
}


/*======================================================================*/
void setAttribute(AttributeEntry *attributeTable,
		  Aint attributeCode,
		  Aword newValue)
{
  AttributeEntry *attribute = findAttribute(attributeTable, attributeCode);

  attribute->value = newValue;
  gameStateChanged = TRUE;
}
