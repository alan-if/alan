/*======================================================================*\

  expTest.c

  Unit tests for EXP node in the Alan compiler

\*======================================================================*/

#include "exp.c"
#include "ins_x.h"

void testAttributeToThis()
{
  InsNod *theInstance = newInstance(&nulsrcp, newId(&nulsrcp, "ins"),
				     NULL, newEmptySlots());
  IdNode *theAttributeId = newId(&nulsrcp, "Atr");
  Attribute *theAttribute = newAttribute(&nulsrcp, BOOLEAN_TYPE,
					 theAttributeId, 0, 0, 0);
  Context *theContext = newContext(INSTANCE_CONTEXT);
  What *theWhat = newWhat(&nulsrcp, WHAT_THIS, NULL);
  ExpNod *theWhatExp = newexp(&nulsrcp, WHAT_EXPRESSION);
  ExpNod *theExp = newexp(&nulsrcp, ATTRIBUTE_EXPRESSION);

  theInstance->slots->attributes = concat(NULL, theAttribute, ATTRIBUTE_LIST);
  theContext->instance = theInstance;

  theExp->fields.atr.atr = theAttributeId;
  theWhatExp->fields.wht.wht = theWhat;
  theExp->fields.atr.wht = theWhatExp;

  anexpatr(theExp, theContext);
  unitAssert(theExp->type == BOOLEAN_TYPE);
}


void registerExpUnitTests()
{
  registerUnitTest(testAttributeToThis);
}

