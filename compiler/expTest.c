/*======================================================================*\

  expTest.c

  Unit tests for EXP node in the Alan compiler

\*======================================================================*/

#include "exp.c"

#include <cgreen/cgreen.h>

#include "unitmock.h"
#include "unitList.h"

#include "ins_x.h"
#include "prop_x.h"
#include "cla_x.h"


Ensure(SetMembersAreVerifiedAccordingToClass) { 
  Expression *theSet = newWhatExpression(nulsrcp, NULL);
  Expression *theMember = newWhatExpression(nulsrcp, NULL);

  initClasses();

  theSet->type = INTEGER_TYPE;
  theMember->type = INSTANCE_TYPE;
  theMember->class = locationSymbol;
  verifySetMember(theSet, theMember, "Set member test");
  assert_that(readEcode(), is_equal_to(410));

  theSet->class = locationSymbol;
  theSet->type = INSTANCE_TYPE;
  verifySetMember(theSet, theMember, "Set member test");
  assert_that(readEcode(), is_equal_to(0));
}

Ensure(testSymbolOf) { 
  Context *context = newNullContext();
  initSymbols();
  assert_true(symbolOfExpression(NULL, context) == NULL);

  Instance *theInstance = newInstance(&nulsrcp, newId(nulsrcp, "ins"),
				     NULL, newEmptyProps());
  What *theWhat = newWhatId(nulsrcp, theInstance->props->id);
  Expression *theWhatExp = newWhatExpression(nulsrcp, theWhat);
  assert_true(symbolOfExpression(theWhatExp, context) != NULL);
  assert_true(symbolOfExpression(theWhatExp, context) == theInstance->props->id->symbol);

  Expression *theAttributeExpression = newAttributeExpression(nulsrcp,
							      newId(nulsrcp, "atr"), FALSE,
							      theWhatExp);
  assert_true(symbolOfExpression(theAttributeExpression, context) == NULL);
}
  


Ensure(testAttributeToThis) {
  Instance *theInstance = newInstance(&nulsrcp, newId(nulsrcp, "ins"),
				     NULL, newEmptyProps());
  IdNode *theAttributeId = newId(nulsrcp, "Atr");
  Attribute *theAttribute = newBooleanAttribute(nulsrcp, theAttributeId, FALSE);
  Context *theContext = newInstanceContext(theInstance);
  What *theWhat = newWhatThis(nulsrcp);
  Expression *theWhatExp = newExpression(nulsrcp, WHAT_EXPRESSION);
  Expression *theExp = newExpression(nulsrcp, ATTRIBUTE_EXPRESSION);

  theInstance->props->attributes = newList(theAttribute, ATTRIBUTE_LIST);
  theContext->instance = theInstance;

  theExp->fields.atr.id = theAttributeId;
  theWhatExp->fields.wht.wht = theWhat;
  theExp->fields.atr.wht = theWhatExp;

  analyzeAttributeExpression(theExp, theContext);
  assert_true(theExp->type == BOOLEAN_TYPE);
}

Ensure(testIsConstantIdentifier) {
  Symbol symbol;
  IdNode id;
  SymbolKind kind;

  id.symbol = &symbol;

  for (kind = CLASS_SYMBOL; kind <= LOCAL_SYMBOL; kind++) {
    symbol.kind = kind;
    if (kind == PARAMETER_SYMBOL || kind == LOCAL_SYMBOL)
      assert_true(!isConstantIdentifier(&id));
    else
      assert_true(isConstantIdentifier(&id));
  }
}

Ensure(testAnalyzeClassingFilter) {
  Expression *btw = newBetweenExpression(nulsrcp, NULL, FALSE, newIntegerExpression(nulsrcp, 1), newIntegerExpression(nulsrcp, 2));

  analyzeClassingFilter("", NULL, btw);
  assert_true(btw->class == integerSymbol);
  assert_true(readEcode() == 0);
}

Ensure(testIsConstant) {
  Expression *integer = newIntegerExpression(nulsrcp, 4);
  IdNode *instanceId = newId(nulsrcp, "instanceId");
  Symbol *instanceSymbol = newInstanceSymbol(instanceId, NULL, NULL);
  Expression *instanceExp = newWhatExpression(nulsrcp, newWhatId(nulsrcp, instanceId));
  IdNode *parameterId = newId(nulsrcp, "parameterId");
  Symbol *parameterSymbol = newInstanceSymbol(parameterId, NULL, NULL);
  Expression *parameterExp = newWhatExpression(nulsrcp, newWhatId(nulsrcp, parameterId));
  List *members = newList(instanceExp, EXPRESSION_LIST);
  Expression *setExp = newSetExpression(nulsrcp, members);

  assert_true(isConstantExpression(integer));

  instanceId->symbol = instanceSymbol;
  assert_true(isConstantExpression(instanceExp));

  parameterSymbol->kind = PARAMETER_SYMBOL;
  parameterId->symbol = parameterSymbol;
  assert_true(!isConstantExpression(parameterExp));

  assert_true(isConstantExpression(setExp));
  concat(members, parameterExp, EXPRESSION_LIST);
  assert_true(!isConstantExpression(setExp));
}

Ensure(testCombineFilterClasses) {
  initSymbols();    
  initClasses();
  symbolizeClasses();
  assert_true(combineFilterClasses(NULL, NULL, nulsrcp) == NULL);
  assert_true(combineFilterClasses(NULL, integerSymbol, nulsrcp) == integerSymbol);
  assert_true(combineFilterClasses(integerSymbol, NULL, nulsrcp) == integerSymbol);
  assert_true(combineFilterClasses(integerSymbol, integerSymbol, nulsrcp) == integerSymbol);
  assert_true(combineFilterClasses(integerSymbol, literalSymbol, nulsrcp) == integerSymbol);
  assert_true(combineFilterClasses(literalSymbol, integerSymbol, nulsrcp) == integerSymbol);

  /* Not compatible types should generate an error and return original */
  assert_true(combineFilterClasses(integerSymbol, stringSymbol, nulsrcp) == integerSymbol);
  assert_true(readEcode() == 441);
}

TestSuite *expTests()
{
    TestSuite *suite = create_test_suite();

    add_test(suite, testSymbolOf);
    add_test(suite, testAttributeToThis);
    add_test(suite, testIsConstantIdentifier);
    add_test(suite, SetMembersAreVerifiedAccordingToClass);
    add_test(suite, testIsConstant);
    add_test(suite, testAnalyzeClassingFilter);
    add_test(suite, testCombineFilterClasses);

    return suite;

}
