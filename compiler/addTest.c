/*======================================================================*\

  addTest.c

  Unit tests for ADD node in the Alan compiler

\*======================================================================*/

#include "add.c"

static void testMultipleAddAttribute()
{
  IdNode *theId = newId(&nulsrcp, "aClassId");
  ClaNod *theClass = newClass(&nulsrcp, theId, NULL, NULL);
  Attribute *theFirstAttribute = newAttribute(&nulsrcp,
					   BOOLEAN_TYPE,
					   newId(&nulsrcp, "firstAttribute"),
					   0, 0, 0);
  Attribute *theSecondAttribute = newAttribute(&nulsrcp,
					    BOOLEAN_TYPE,
					    newId(&nulsrcp, "secondAttribute"),
					    0, 0, 0);
  Slots *theFirstAttributeSlots = newSlots(NULL, NULL,
					   concat(NULL, theFirstAttribute,
						  ATTRIBUTE_LIST),
					   NULL, NULL,
					   &nulsrcp, NULL, &nulsrcp, NULL,
					   &nulsrcp, NULL, NULL, NULL, NULL);
  Slots *theSecondAttributeSlots = newSlots(NULL, NULL,
					    concat(NULL, theSecondAttribute,
						   ATTRIBUTE_LIST),
					    NULL, NULL,
					    &nulsrcp, NULL, &nulsrcp, NULL,
					    &nulsrcp, NULL, NULL, NULL, NULL);
  AddNode *add1 = newAdd(&nulsrcp, theId, NULL, theFirstAttributeSlots);
  AddNode *add2 = newAdd(&nulsrcp, theId, NULL, theSecondAttributeSlots);

  Symbol *aSymbol = lookup("aClassId");
  unitAssert(length(aSymbol->fields.claOrIns.slots->attributes) == 0);
  (theClass);			/* Dummy to avoid "unused" warning */

  addAttributes(add1, aSymbol);
  unitAssert(length(aSymbol->fields.claOrIns.slots->attributes) == 1);
  
  addAttributes(add2, aSymbol);
  unitAssert(length(aSymbol->fields.claOrIns.slots->attributes) == 2);
  
}

void registerAddUnitTests()
{
  registerUnitTest(testMultipleAddAttribute);
}


