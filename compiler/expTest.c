/*======================================================================*\

  expTest.c

  Unit tests for EXP node in the Alan compiler

\*======================================================================*/

#include "exp.c"
#include "ins_x.h"

void testSymbolOf() {
  Context *context = newContext(NULL_CONTEXT, NULL);
  initSymbols();
  ASSERT(symbolOfExpression(NULL, context) == NULL);

  Instance *theInstance = newInstance(&nulsrcp, newId(&nulsrcp, "ins"),
				     NULL, newEmptyProps());
  What *theWhat = newWhat(&nulsrcp, WHAT_ID, theInstance->props->id);
  Expression *theWhatExp = newWhatExpression(nulsrcp, theWhat);
  ASSERT(symbolOfExpression(theWhatExp, context) != NULL);
  ASSERT(symbolOfExpression(theWhatExp, context) == theInstance->props->id->symbol);

  Expression *theAttributeExpression = newAttributeExpression(nulsrcp,
							      newId(&nulsrcp, "atr"), FALSE,
							      theWhatExp);
  ASSERT(symbolOfExpression(theAttributeExpression, context) == NULL);
}
  


void testAttributeToThis()
{
  Instance *theInstance = newInstance(&nulsrcp, newId(&nulsrcp, "ins"),
				     NULL, newEmptyProps());
  IdNode *theAttributeId = newId(&nulsrcp, "Atr");
  Attribute *theAttribute = newAttribute(&nulsrcp, BOOLEAN_TYPE,
					 theAttributeId, 0, 0, 0, NULL);
  Context *theContext = newContext(INSTANCE_CONTEXT, theInstance);
  What *theWhat = newWhat(&nulsrcp, WHAT_THIS, NULL);
  Expression *theWhatExp = newExpression(&nulsrcp, WHAT_EXPRESSION);
  Expression *theExp = newExpression(&nulsrcp, ATTRIBUTE_EXPRESSION);

  theInstance->props->attributes = concat(NULL, theAttribute, ATTRIBUTE_LIST);
  theContext->instance = theInstance;

  theExp->fields.atr.atr = theAttributeId;
  theWhatExp->fields.wht.wht = theWhat;
  theExp->fields.atr.wht = theWhatExp;

  analyzeAttributeExpression(theExp, theContext);
  ASSERT(theExp->type == BOOLEAN_TYPE);
}

static void testIsConstantIdentifier()
{
  Symbol symbol;
  IdNode id;
  SymbolKind kind;

  id.symbol = &symbol;

  for (kind = CLASS_SYMBOL; kind <= LOCAL_SYMBOL; kind++) {
    symbol.kind = kind;
    if (kind == PARAMETER_SYMBOL || kind == LOCAL_SYMBOL)
      ASSERT(!isConstantIdentifier(&id));
    else
      ASSERT(isConstantIdentifier(&id));
  }
}

static void testAnalyzeIsaExpression()
{
  
}

void registerExpUnitTests()
{
  registerUnitTest(testSymbolOf);
  registerUnitTest(testAttributeToThis);
  registerUnitTest(testIsConstantIdentifier);
  registerUnitTest(testAnalyzeIsaExpression);
}

