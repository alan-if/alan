/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

#include "unit.h"

#include "ins_x.h"
#include "cla_x.h"
#include "prop_x.h"


static void testClassOfContent()
{
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
  ASSERT(contentOf(whatLocation, NULL) == NULL);

  symbolizeId(takesId);
  ASSERT(contentOf(whatId, NULL) == locationSymbol);
}

static void testInitialLocationOfObject()
{
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
  ASSERT(generateInitialLocation(properties) == 1); /* #nowhere */

  properties->whr = whr;
  symbolizeProps(properties, FALSE);
  ASSERT(generateInitialLocation(properties) == atLoc->props->id->symbol->code);
}

static void testInitialLocationOfLocation()
{
  IdNode *locId = newId(nulsrcp, "atLoc");
  Where *whr = newWhere(&nulsrcp, FALSE, WHERE_AT,
			newWhatExpression(nulsrcp, newWhatId(nulsrcp, locId)));
  Instance *atLoc = newInstance(&nulsrcp, locId, NULL, NULL);
  IdNode *id = newId(nulsrcp, "id");
  Properties *properties = newProps(NULL, NULL, nulsrcp,
				    NULL, NULL, NULL, NULL, nulsrcp,
				    NULL, NULL, NULL, NULL, NULL,
				    NULL, nulsrcp, NULL, NULL, NULL);

  symbolizeWhere(whr);
  id->symbol = newSymbol(id, INSTANCE_SYMBOL);
  properties->id = id;
  properties->parentId = newId(nulsrcp, "location");

  symbolizeProps(properties, FALSE);
  ASSERT(generateInitialLocation(properties) == 0);

  properties->whr = whr;
  ASSERT(generateInitialLocation(properties) == atLoc->props->id->symbol->code);
}

void whrUnitTests()
{
  registerUnitTest(testClassOfContent);
  registerUnitTest(testInitialLocationOfObject);
  registerUnitTest(testInitialLocationOfLocation);
}

