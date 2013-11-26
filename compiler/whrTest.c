/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

#include <cgreen/cgreen.h>

#include "ins_x.h"
#include "cla_x.h"
#include "prop_x.h"


Describe(Where);
BeforeEach(Where) {}
AfterEach(Where) {}


Ensure(Where, testClassOfContent) {
  initSymbols();
  initClasses();
  IdNode *id = newId(nulsrcp, "inCont");
  Expression *whatLocation = newWhatExpression(nulsrcp,
						 newWhatId(nulsrcp, id));
  Expression *whatId = newWhatExpression(nulsrcp,
					 newWhatId(nulsrcp, id));
  IdNode *takesId = newId(nulsrcp, "location");
  ContainerBody *containerBody = newContainerBody(nulsrcp, FALSE, takesId,
						  NULL, NULL, NULL, NULL, NULL);
  Container *container = newContainer(containerBody);
  Properties *properties = newProps(NULL, NULL,
				    nulsrcp, NULL,
				    NULL, NULL, NULL,
				    nulsrcp, NULL, NULL,
				    NULL, NULL,
				    container, NULL, 
				    nulsrcp, NULL,
				    NULL,NULL);

  Instance *containerInstance = newInstance(&nulsrcp, id, NULL, properties);

  (void)containerInstance;
  assert_true(contentOf(whatLocation, NULL) == NULL);

  symbolizeId(takesId);
  assert_true(contentOf(whatId, NULL) == locationSymbol);
}

Ensure(Where, InitialLocationOfObjectIsNowhere) {
  IdNode *locId = newId(nulsrcp, "atLoc");
  Where *whr = newWhere(&nulsrcp, FALSE, WHERE_AT,
			newWhatExpression(nulsrcp, newWhatId(nulsrcp, locId)));
  IdNode *id = newId(nulsrcp, "id");
  Instance *atLoc = newInstance(&nulsrcp, locId, NULL, NULL);
  Properties *properties = newProps(NULL, NULL, nulsrcp,
				    NULL, NULL, NULL, NULL, nulsrcp,
				    NULL, NULL, NULL, NULL, NULL,
				    NULL, nulsrcp, NULL, NULL, NULL);

  symbolizeWhere(whr);
  properties->id = id;

  symbolizeProps(properties, FALSE);
  assert_true(generateInitialLocation(properties) == 1); /* #nowhere */

  properties->whr = whr;
  symbolizeProps(properties, FALSE);
  assert_true(generateInitialLocation(properties) == atLoc->props->id->symbol->code);
}

Ensure(Where, InitialLocationOfLocationIsNull) {
  IdNode *locId = newId(nulsrcp, "atLoc");
  Where *whr = newWhere(&nulsrcp, FALSE, WHERE_AT,
			newWhatExpression(nulsrcp, newWhatId(nulsrcp, locId)));
  Instance *atLoc = newInstance(&nulsrcp, locId, NULL, NULL);
  IdNode *id = newId(nulsrcp, "id");
  Properties *properties = newProps(NULL, NULL, nulsrcp,
				    NULL, NULL, NULL, NULL, nulsrcp,
				    NULL, NULL, NULL, NULL, NULL,
				    NULL, nulsrcp, NULL, NULL, NULL);

  initClasses();

  symbolizeWhere(whr);
  id->symbol = newSymbol(id, INSTANCE_SYMBOL);
  properties->id = id;
  properties->parentId = newId(nulsrcp, "location");

  symbolizeProps(properties, FALSE);
  assert_that(generateInitialLocation(properties), is_equal_to(0));

  properties->whr = whr;
  assert_that(generateInitialLocation(properties), is_equal_to(atLoc->props->id->symbol->code));
}

TestSuite *whrTests()
{
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, Where, testClassOfContent);
    add_test_with_context(suite, Where, InitialLocationOfObjectIsNowhere);
    add_test_with_context(suite, Where, InitialLocationOfLocationIsNull);

    return suite;
}

