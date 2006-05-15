/*======================================================================*\

  propTest.c

  Unit tests for PROP node in the Alan compiler

\*======================================================================*/

#include "prop.c"


static void testAddOpaqueAttribute()
{
  Properties *prop = newProps(NULL, NULL,
			      nulsrcp, NULL,
			      NULL, NULL, NULL,
			      nulsrcp, NULL, NULL,
			      NULL, NULL,
			      NULL, NULL,
			      nulsrcp, NULL,
			      NULL, NULL);
  IdNode *opaque = newId(nulsrcp, "opaque");
  Attribute *attribute;

  addOpaqueAttribute(prop, TRUE);
  ASSERT((attribute = findAttribute(prop->attributes, opaque)) != NULL);
  ASSERT(attribute && attribute->value);

}

static Where *newWhereIdString(char id[]) {
  return newWhere(&nulsrcp, FALSE, WHERE_AT,
		  newWhatExpression(nulsrcp,
				    newWhatId(nulsrcp,
					      newId(nulsrcp, id))));
}

static void testCircularLocation() {
  Where *whr1 = newWhereIdString("loc2");
  Properties *props1 = newProps(NULL, NULL,
				nulsrcp, NULL,
				NULL, NULL, NULL,
				nulsrcp, NULL, NULL, NULL, NULL,
				NULL, NULL,
				nulsrcp, NULL,
				NULL, NULL);
  Instance *loc1;
  Where *whr2 = newWhereIdString("loc1");
  Properties *props2 = newProps(whr2, NULL,
				nulsrcp, NULL,
				NULL, NULL, NULL,
				nulsrcp, NULL, NULL, NULL, NULL,
				NULL, NULL,
				nulsrcp, NULL,
				NULL, NULL);
  Instance *loc2;

  initAdventure();
  loc1 = newInstance(&nulsrcp, newId(nulsrcp, "loc1"), NULL, props1);
  loc2 = newInstance(&nulsrcp, newId(nulsrcp, "loc2"), NULL, props2);
  symbolizeAdventure();

  analyzeCircularLocations(props1);
  ASSERT(readEcode() == 0);

  props1->circularInspection = VISITED;
  analyzeCircularLocations(props1);
  ASSERT(readEcode() == 802);

  props1->circularInspection = REPORTED;
  analyzeCircularLocations(props1);
  ASSERT(readEcode() == 0);

  props1->circularInspection = VIRGIN;
  props1->whr = whr1;
  analyzeCircularLocations(props1);
  ASSERT(readEcode() == 0);

  props1->circularInspection = VIRGIN;
  analyzeCircularLocations(props2);
  ASSERT(readEcode() == 0);

  props1->circularInspection = VIRGIN;
  props2->circularInspection = VIRGIN;
  props1->whr = whr1;
  symbolizeWhere(whr1);
  analyzeCircularLocations(props1);
  ASSERT(readEcode() == 802);

  loc1 = loc2;			/* Do make use of variables... */
}



void registerPropUnitTests()
{
  registerUnitTest(testCircularLocation);
  registerUnitTest(testAddOpaqueAttribute);
}

