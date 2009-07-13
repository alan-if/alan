/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

#include "unit.h"

#include "ins_x.h"
#include "cla_x.h"
#include "prop_x.h"


void testClassOfContent()
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

void testInitialLocation()
{
  IdNode *id = newId(nulsrcp, "atLoc");
  Where *whr = newWhere(&nulsrcp, FALSE, WHERE_AT,
			newWhatExpression(nulsrcp, newWhatId(nulsrcp, id)));
  Instance *atLoc = newInstance(&nulsrcp, id, NULL, NULL);

  symbolizeProps(atLoc->props, FALSE);
  symbolizeWhere(whr);
  ASSERT(generateInitialLocation(NULL) == 0);

  ASSERT(generateInitialLocation(whr) == atLoc->props->id->symbol->code);
}

void whrUnitTests()
{
  registerUnitTest(testClassOfContent);
  registerUnitTest(testInitialLocation);
}

