/*======================================================================*\

  vrbTest.c

  Unit tests for VRB node in the Alan compiler

\*======================================================================*/

#include "vrb.c"

void testMultipleVerbs()
{
  IdNode *v1 = newId(nulsrcp, "v1");
  IdNode *v2 = newId(nulsrcp, "v2");
  IdNode *v3 = newId(nulsrcp, "v3");
  IdNode *v4 = newId(nulsrcp, "v4");
  IdNode *v5 = newId(nulsrcp, "v5");
  IdNode *v6 = newId(nulsrcp, "v6");
  List *verbIds1 = concat(NULL, v1, ID_LIST);
  List *verbIds2 = concat(concat(NULL, v2, ID_LIST), v3, ID_LIST);
  List *verbIds3 = concat(concat(NULL, v1, ID_LIST), v5, ID_LIST);
  List *verbs = concat(concat(concat(NULL,
				     newVerb(&nulsrcp, verbIds1, NULL),
				     VERB_LIST),
			      newVerb(&nulsrcp, verbIds2, NULL),
			      VERB_LIST),
		       newVerb(&nulsrcp, verbIds3, NULL),
		       VERB_LIST);

  checkMultipleVerbDeclarations(verbs);
  ASSERT(readEcode() != 0);
  ASSERT(readSev() != 0);

  ASSERT(verbIdFound(v1, verbs));
  ASSERT(verbIdFound(v2, verbs));
  ASSERT(verbIdFound(v3, verbs));
  ASSERT(!verbIdFound(v4, verbs));
  ASSERT(verbIdFound(v5, verbs));
  ASSERT(!verbIdFound(v6, verbs));
}


void registerVrbUnitTests()
{
  registerUnitTest(testMultipleVerbs);
}

