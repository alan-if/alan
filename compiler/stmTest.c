/*======================================================================*\

  stmTest.c

  Unit tests for STM node in the Alan compiler

\*======================================================================*/

#include "stm.c"

void testVerifySetAssignment() {
  Expression *exp = newSetExpression(nulsrcp, NULL);
  Expression *wht = newWhatExpression(nulsrcp, newWhat(&nulsrcp, WHAT_ID, newId(nulsrcp, "setAttribute")));

  wht->type = SET_TYPE;
  exp->type = SET_TYPE;

  exp->class = locationSymbol;
  wht->class = objectSymbol;

  /* Set a set attribute to an empty set (of a different member class) */
  (void)readEcode();
  verifySetAssignment(exp, wht);
  ASSERT(readEcode() == 0);

  /* Set a set attribute to a non-empty set of different type */
  /* Dummy element to make length > 0 */
  exp->fields.set.members = concat(NULL, exp, EXPRESSION_LIST);
  verifySetAssignment(exp, wht);
  ASSERT(readEcode() == 431);

  /* Set a set attribute to a non-empty set the same type */
  exp->class = wht->class;
  verifySetAssignment(exp, wht);
  ASSERT(readEcode() == 0);

  /* Assign a set attribute from another set attribute of the same type */
  exp->kind = ATTRIBUTE_EXPRESSION;
  verifySetAssignment(exp, wht);
  ASSERT(readEcode() == 0);

  /* Assign a set attribute from another set attribute of a different type */
  exp->class = locationSymbol;
  verifySetAssignment(exp, wht);
  ASSERT(readEcode() == 431);
}

void registerStmUnitTests()
{
  registerUnitTest(testVerifySetAssignment);
}

