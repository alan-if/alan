/*======================================================================*\

  whrTest.c

  Unit tests for WHR node in the Alan compiler

\*======================================================================*/

#include "whr.c"

void testClassOfContent()
{
  IdNode *id = newId(&nulsrcp, "inCont");
  Where *whereInLocation = newwhr(&nulsrcp, WHR_IN, newWhat(&nulsrcp, WHAT_LOCATION, id));
  Where *whereInActor = newwhr(&nulsrcp, WHR_IN, newWhat(&nulsrcp, WHAT_ACTOR, id));
  Where *whereInId = newwhr(&nulsrcp, WHR_IN, newWhat(&nulsrcp, WHAT_ID, id));
  IdNode *takesId = newId(&nulsrcp, "location");
  ContainerBody *containerBody = newContainerBody(&nulsrcp, FALSE, takesId, NULL, NULL, NULL, NULL, NULL);
  Container *container = newContainer(containerBody);
  Properties *properties = newProps(NULL, NULL, NULL, container,
				    nulsrcp, NULL,
				    nulsrcp, NULL,
				    nulsrcp, NULL,
				    nulsrcp, NULL,
				    nulsrcp, NULL, FALSE,
				    nulsrcp, NULL, FALSE,
				    NULL, NULL,NULL);
  Instance *containerInstance = newInstance(&nulsrcp, id, NULL, properties);

  ASSERT(classOfContent(whereInLocation) == NULL);

  symbolizeId(takesId);
  ASSERT(classOfContent(whereInId) == locationSymbol);
}

void testInitialLocation()
{
  IdNode *id = newId(&nulsrcp, "atLoc");
  Where *whr = newwhr(&nulsrcp, WHERE_AT, newWhat(&nulsrcp, WHAT_ID, id));
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

