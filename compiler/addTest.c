/*======================================================================*\

  addTest.c

  Unit tests for ADD node in the Alan compiler

\*======================================================================*/

#include "add.c"

#include "unit.h"

#include "cla_x.h"
#include "sym.h"


static void testMultipleAddAttribute()
{
  IdNode *theId = newId(nulsrcp, "aClassId");
  Class *theClass;
  Attribute *theFirstAttribute = newBooleanAttribute(nulsrcp, newId(nulsrcp, "firstAttribute"), FALSE);
  Attribute *theSecondAttribute = newBooleanAttribute(nulsrcp, newId(nulsrcp, "secondAttribute"), FALSE);
  Properties *theFirstAttributeProps = newProps(NULL, NULL,
						nulsrcp, NULL,
						concat(NULL, theFirstAttribute,
						       ATTRIBUTE_LIST),
						NULL, NULL,
						nulsrcp, NULL, NULL, NULL, NULL,
						NULL, NULL,
						nulsrcp, NULL,
						NULL, NULL);
  Properties *theSecondAttributeProps = newProps(NULL, NULL,
						 nulsrcp, NULL,
						 concat(NULL, theSecondAttribute,
							ATTRIBUTE_LIST),
						 NULL, NULL,
						 nulsrcp, NULL, NULL, NULL, NULL,
						 NULL, NULL,
						 nulsrcp, NULL,
						 NULL, NULL);
  AddNode *add1 = newAdd(nulsrcp, theId, NULL, theFirstAttributeProps);
  AddNode *add2 = newAdd(nulsrcp, theId, NULL, theSecondAttributeProps);
  Symbol *aSymbol;

  theClass = newClass(&nulsrcp, theId, NULL, NULL);
  aSymbol = lookup("aClassId");
  ASSERT(aSymbol != NULL);
  ASSERT(length(aSymbol->fields.entity.props->attributes) == 0);

  addAttributes(add1, aSymbol);
  ASSERT(length(aSymbol->fields.entity.props->attributes) == 1);
  
  addAttributes(add2, aSymbol);
  ASSERT(length(aSymbol->fields.entity.props->attributes) == 2);
}


void testAddDescription() {
  Description *addedDescription = newDescription(nulsrcp, NULL, nulsrcp, NULL);
  Properties *addProps = newProps(NULL, NULL,
				  nulsrcp, NULL,
				  NULL, NULL, /*description*/ NULL,
				  nulsrcp, NULL, NULL, NULL, NULL,
				  NULL, NULL,
				  nulsrcp, NULL,
				  NULL, NULL);
  AddNode *add = newAdd(nulsrcp, NULL, NULL, addProps);

  Properties *originalProps = newProps(NULL, NULL,
				       nulsrcp, NULL,
				       NULL, NULL, /*description*/ NULL,
				       nulsrcp, NULL, NULL, NULL, NULL,
				       NULL, NULL,
				       nulsrcp, NULL,
				       NULL, NULL);
  Symbol *symbol = newClassSymbol(newId(nulsrcp, "testAddId"), originalProps, NULL);
  List list;			/* Dummy list */

  /* Start by testing when the target symbols props are NULL */
  /* Added description is NULL */
  addDescriptionCheck(add, symbol);
  ASSERT(symbol->fields.entity.props->description == NULL);

  /* Added description exists but empty */
  addProps->description = addedDescription;
  addDescriptionCheck(add, symbol);
  ASSERT(symbol->fields.entity.props->description == NULL);

  addedDescription->checks = &list;
  addDescriptionCheck(add, symbol);
  ASSERT(symbol->fields.entity.props->description->checks == &list);


  /* Now test descriptionDoes */
  /* No description */
  originalProps->description = NULL; /* Reset to empty */
  addProps->description = NULL; /* Reset to empty */
  addDescription(add, symbol);
  ASSERT(symbol->fields.entity.props->description == NULL);

  /* Empty description */
  addProps->description = addedDescription;
  addDescription(add, symbol);
  ASSERT(symbol->fields.entity.props->description == NULL);

  /* Description with does */
  originalProps->description = NULL; /* Reset to empty */
  addedDescription->does = &list;
  addDescription(add, symbol);
  ASSERT(symbol->fields.entity.props->description->does == &list);
}

void addUnitTests()
{
  registerUnitTest(testMultipleAddAttribute);
  registerUnitTest(testAddDescription);
}

