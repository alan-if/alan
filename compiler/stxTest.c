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

void registerStxUnitTests()
{
  registerUnitTest(testCountParameters);
  registerUnitTest(testCompatibleParameterLists);
}

