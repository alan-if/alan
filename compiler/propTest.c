/*======================================================================*\

  propTest.c

  Unit tests for PROP node in the Alan compiler

\*======================================================================*/

#include "prop.c"


static void testAddOpaqueAttribute()
{
  Properties *prop = newProps(NULL, NULL, nulsrcp, NULL, NULL, NULL,
			      nulsrcp, NULL, nulsrcp, NULL, nulsrcp, NULL, nulsrcp, NULL, nulsrcp,
			      NULL, FALSE, nulsrcp,
			      NULL, FALSE,
			      NULL, NULL, NULL);
  IdNode *opaque = newId(nulsrcp, "opaque");
  Attribute *attribute;

  addOpaqueAttribute(prop, TRUE);
  ASSERT((attribute = findAttribute(prop->attributes, opaque)) != NULL);
  ASSERT(attribute && attribute->value);

}

void registerPropUnitTests()
{
  registerUnitTest(testAddOpaqueAttribute);
}

