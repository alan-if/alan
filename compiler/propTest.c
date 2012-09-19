/*======================================================================*\

  propTest.c

  Unit tests for PROP node in the Alan compiler

\*======================================================================*/

#include "prop.c"

#include <cgreen/cgreen.h>

#include "unitList.h"

#include "exp_x.h"
#include "wht_x.h"
#include "adv_x.h"
#include "ins_x.h"


Ensure(testAddOpaqueAttribute) {
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
  assert_true((attribute = findAttribute(prop->attributes, opaque)) != NULL);
  assert_true(attribute && attribute->value);

}

static Where *newWhereIdString(char id[]) {
  return newWhere(&nulsrcp, FALSE, WHERE_AT,
		  newWhatExpression(nulsrcp,
				    newWhatId(nulsrcp,
					      newId(nulsrcp, id))));
}

Ensure(testCircularLocation) {
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

  readEcode();
  analyzeCircularLocations(props1);
  assert_true(readEcode() == 0);

  props1->circularInspection = VISITED;
  analyzeCircularLocations(props1);
  assert_true(readEcode() == 802);

  props1->circularInspection = REPORTED;
  analyzeCircularLocations(props1);
  assert_true(readEcode() == 0);

  props1->circularInspection = VIRGIN;
  props1->whr = whr1;
  analyzeCircularLocations(props1);
  assert_true(readEcode() == 0);

  props1->circularInspection = VIRGIN;
  analyzeCircularLocations(props2);
  assert_true(readEcode() == 0);

  props1->circularInspection = VIRGIN;
  props2->circularInspection = VIRGIN;
  props1->whr = whr1;
  symbolizeWhere(whr1);
  analyzeCircularLocations(props1);
  assert_true(readEcode() == 802);

  loc1 = loc2;			/* Do make use of variables... */
}



TestSuite *propTests() {
    TestSuite *suite = create_test_suite(); 

    add_test(suite, testCircularLocation);
    add_test(suite, testAddOpaqueAttribute);

    return suite;
}
