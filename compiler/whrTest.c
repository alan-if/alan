/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

void testClassOfContent()
{
  IdNode *id = newId(nulsrcp, "inCont");
  Expression *whatLocation = newWhatExpression(nulsrcp,
						 newWhat(&nulsrcp,
							 WHAT_LOCATION,
							 id));
  Expression *whatId = newWhatExpression(nulsrcp,
					 newWhat(&nulsrcp,
						 WHAT_ID, id));
  IdNode *takesId = newId(nulsrcp, "location");
  ContainerBody *containerBody = newContainerBody(nulsrcp, FALSE, takesId,
						  NULL, NULL, NULL, NULL, NULL);
  Container *container = newContainer(containerBody);
  Properties *properties = newProps(NULL, NULL, nulsrcp, NULL, NULL, container,
				    nulsrcp, NULL,
				    nulsrcp, NULL,
				    nulsrcp, NULL,
				    nulsrcp, NULL,
				    nulsrcp, NULL, FALSE,
				    nulsrcp, NULL, FALSE,
				    NULL, NULL,NULL);

  Instance *containerInstance = newInstance(&nulsrcp, id, NULL, properties);

  ASSERT(contentOf(whatLocation, NULL) == NULL);

  symbolizeId(takesId);
  ASSERT(contentOf(whatId, NULL) == locationSymbol);
}

void testInitialLocation()
{
  IdNode *id = newId(nulsrcp, "atLoc");
  Where *whr = newWhere(&nulsrcp, WHERE_AT,
			newWhatExpression(nulsrcp, newWhat(&nulsrcp,
							   WHAT_ID, id)));
  Instance *atLoc = newInstance(&nulsrcp, id, NULL, NULL);

  symbolizeInstance(atLoc);
  symbolizeWhere(whr);
  ASSERT(generateInitialLocation(NULL) == 0);

  ASSERT(generateInitialLocation(whr) == atLoc->props->id->symbol->code);
}

void registerWhrUnitTests()
{
  registerUnitTest(testClassOfContent);
  registerUnitTest(testInitialLocation);
}

