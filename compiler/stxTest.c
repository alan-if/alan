/*======================================================================*\

  stxTest.c

\*======================================================================*/

#include "stx.c"


void testCountParameters()
{
  List *elementList;

  elementList = concat(NULL, newElement(&nulsrcp, PARAMETER_ELEMENT, NULL, 0), ELEMENT_LIST);
  ASSERT(countParameters(elementList) == 1);
  elementList = concat(elementList, newElement(&nulsrcp, WORD_ELEMENT, NULL, 0), ELEMENT_LIST);
  ASSERT(countParameters(elementList) == 1);
  elementList = concat(elementList, newElement(&nulsrcp, PARAMETER_ELEMENT, NULL, 0), ELEMENT_LIST);
  ASSERT(countParameters(elementList) == 2);
}


void testCompatibleParameterLists()
{
  Syntax s1, s2;

  s1.elements = concat(NULL, newElement(&nulsrcp, PARAMETER_ELEMENT, newId(&nulsrcp, "a"), 0), ELEMENT_LIST);
  s2.elements = concat(NULL, newElement(&nulsrcp, PARAMETER_ELEMENT, newId(&nulsrcp, "a"), 0), ELEMENT_LIST);
	     

  s2.elements = concat(s2.elements, newElement(&nulsrcp, WORD_ELEMENT, newId(&nulsrcp, "x"), 0), ELEMENT_LIST);
  ASSERT(compatibleParameterLists(&s1, &s2));

  s1.elements = concat(s1.elements, newElement(&nulsrcp, PARAMETER_ELEMENT, newId(&nulsrcp, "b"), 0), ELEMENT_LIST);
  ASSERT(!compatibleParameterLists(&s1, &s2));
}



void testConnectSyntaxForSameVerb()
{
  List *elements = concat(NULL,
			  newElement(&nulsrcp, PARAMETER_ELEMENT, 
				     newId(&nulsrcp, "a"), 0),
			  ELEMENT_LIST);
  IdNode *verbId1 = newId(&nulsrcp, "verb");
  IdNode *verbId2 = newId(&nulsrcp, "verb");
  IdNode *verbId3 = newId(&nulsrcp, "verb");
  IdNode *verbId4 = newId(&nulsrcp, "verb");

  Syntax *s1 = newSyntax(nulsrcp, verbId1, elements, NULL, nulsrcp);
  Syntax *s2 = newSyntax(nulsrcp, verbId2, elements, NULL, nulsrcp);
  Syntax *s3 = newSyntax(nulsrcp, verbId3, elements, NULL, nulsrcp);
  Syntax *s4 = newSyntax(nulsrcp, verbId4, elements, NULL, nulsrcp);
  List *stxs;

  stxs = concat(NULL, s1, SYNTAX_LIST);
  stxs = concat(stxs, s2, SYNTAX_LIST);
  stxs = concat(stxs, s3, SYNTAX_LIST);
  stxs = concat(stxs, s4, SYNTAX_LIST);

  connectSyntaxesForSameVerb(stxs);

  ASSERT(s1->nextSyntaxForSameVerb == s2);
  ASSERT(s1->firstSyntax);
  ASSERT(s2->nextSyntaxForSameVerb == s3);
  ASSERT(!s2->firstSyntax);
  ASSERT(s3->nextSyntaxForSameVerb == s4);
  ASSERT(!s3->firstSyntax);
  ASSERT(s4->nextSyntaxForSameVerb == NULL);
  ASSERT(!s4->firstSyntax);
}



void registerStxUnitTests()
{
  registerUnitTest(testCountParameters);
  registerUnitTest(testCompatibleParameterLists);
  registerUnitTest(testConnectSyntaxForSameVerb);
}

