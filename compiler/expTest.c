/*======================================================================*\

  expTest.c

  Unit tests for EXP node in the Alan compiler

\*======================================================================*/

#include "exp.c"
#include "ins_x.h"

void testVerifySetMember() {
  Expression *theSet = newWhatExpression(nulsrcp, NULL);
  Expression *theMember = newWhatExpression(nulsrcp, NULL);

  theSet->type = INTEGER_TYPE;
  theMember->type = INSTANCE_TYPE;
  theMember->class = locationSymbol;
  verifySetMember(theSet, theMember, "Set member test");
  ASSERT(readEcode() == 410);

  theSet->class = locationSymbol;
  theSet->type = INSTANCE_TYPE;
  verifySetMember(theSet, theMember, "Set member test");
  ASSERT(readEcode() == 0);
}

void testSymbolOf() {
  Context *context = newNullContext();
  initSymbols();
  ASSERT(symbolOfExpression(NULL, context) == NULL);

  Instance *theInstance = newInstance(&nulsrcp, newId(nulsrcp, "ins"),
				     NULL, newEmptyProps());
  What *theWhat = newWhat(&nulsrcp, WHAT_ID, theInstance->props->id);
  Expression *theWhatExp = newWhatExpression(nulsrcp, theWhat);
  ASSERT(symbolOfExpression(theWhatExp, context) != NULL);
  ASSERT(symbolOfExpression(theWhatExp, context) == theInstance->props->id->symbol);

  Expression *theAttributeExpression = newAttributeExpression(nulsrcp,
							      newId(nulsrcp, "atr"), FALSE,
							      theWhatExp);
  ASSERT(symbolOfExpression(theAttributeExpression, context) == NULL);
}
  


void testAttributeToThis()
{
  Instance *theInstance = newInstance(&nulsrcp, newId(nulsrcp, "ins"),
				     NULL, newEmptyProps());
  IdNode *theAttributeId = newId(nulsrcp, "Atr");
  Attribute *theAttribute = newBooleanAttribute(nulsrcp, theAttributeId, FALSE);
  Context *theContext = newInstanceContext(theInstance);
  What *theWhat = newWhat(&nulsrcp, WHAT_THIS, NULL);
  Expression *theWhatExp = newExpression(nulsrcp, WHAT_EXPRESSION);
  Expression *theExp = newExpression(nulsrcp, ATTRIBUTE_EXPRESSION);

  theInstance->props->attributes = concat(NULL, theAttribute, ATTRIBUTE_LIST);
  theContext->instance = theInstance;

  theExp->fields.atr.id = theAttributeId;
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

static void testIsConstant()
{
  Expression *integer = newIntegerExpression(nulsrcp, 4);
  IdNode *instanceId = newId(nulsrcp, "instanceId");
  Symbol *instanceSymbol = newInstanceSymbol(instanceId, NULL, NULL);
  Expression *instanceExp = newWhatExpression(nulsrcp, newWhat(&nulsrcp, WHAT_ID, instanceId));
  IdNode *parameterId = newId(nulsrcp, "parameterId");
  Symbol *parameterSymbol = newInstanceSymbol(parameterId, NULL, NULL);
  Expression *parameterExp = newWhatExpression(nulsrcp, newWhat(&nulsrcp, WHAT_ID, parameterId));
  List *members = concat(NULL, instanceExp, EXPRESSION_LIST);
  Expression *setExp = newSetExpression(nulsrcp, members);

  ASSERT(isConstantExpression(integer));

  instanceId->symbol = instanceSymbol;
  ASSERT(isConstantExpression(instanceExp));

  parameterSymbol->kind = PARAMETER_SYMBOL;
  parameterId->symbol = parameterSymbol;
  ASSERT(!isConstantExpression(parameterExp));

  ASSERT(isConstantExpression(setExp));
  concat(members, parameterExp, EXPRESSION_LIST);
  ASSERT(!isConstantExpression(setExp));
}

void registerExpUnitTests()
{
  registerUnitTest(testSymbolOf);
  registerUnitTest(testAttributeToThis);
  registerUnitTest(testIsConstantIdentifier);
  registerUnitTest(testAnalyzeIsaExpression);
  registerUnitTest(testVerifySetMember);
  registerUnitTest(testIsConstant);
}

