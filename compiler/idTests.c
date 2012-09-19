/*======================================================================*\

  idTest.c

  A unit test module for ID nodes in the Alan compiler

\*======================================================================*/

#include "id.c"

#include <cgreen/cgreen.h>


Ensure(testGeneratedId) {
  IdNode *id1 = NEW(IdNode);
  IdNode *id2 = NEW(IdNode);
  id1->string = generateIdName();
  id2->string = generateIdName();

  assert_true(isGeneratedId(id1));
  assert_true(isGeneratedId(id2));
  assert_true(!equalId(id1, id2));
}

TestSuite *idTests()
{
    TestSuite *suite = create_test_suite(); 

    add_test(suite, testGeneratedId);

    return suite;
}

