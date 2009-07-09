/*======================================================================*\

  stxTest.c

\*======================================================================*/

#include "stx.c"


static void canCountParameters()
{
  List *elementList;

  elementList = concat(NULL, newParameterElement(nulsrcp, NULL, 0), ELEMENT_LIST);
  ASSERT(countParameters(elementList) == 1);
  elementList = concat(elementList, newWordElement(nulsrcp, NULL), ELEMENT_LIST);
  ASSERT(countParameters(elementList) == 1);
  elementList = concat(elementList, newParameterElement(nulsrcp, NULL, 0), ELEMENT_LIST);
  ASSERT(countParameters(elementList) == 2);
}


static void parameterListsShouldBeCompatibleIfTheyHaveTheSameNumberOfParameters()
{
  Syntax s1, s2;

  s1.elements = concat(NULL, newParameterElement(nulsrcp, newId(nulsrcp, "a"), 0), ELEMENT_LIST);
  s2.elements = concat(NULL, newParameterElement(nulsrcp, newId(nulsrcp, "a"), 0), ELEMENT_LIST);
	     

  s2.elements = concat(s2.elements, newWordElement(nulsrcp, newId(nulsrcp, "x")), ELEMENT_LIST);
  ASSERT(compatibleParameterLists(&s1, &s2));

  s1.elements = concat(s1.elements, newParameterElement(nulsrcp, newId(nulsrcp, "b"), 0), ELEMENT_LIST);
  ASSERT(!compatibleParameterLists(&s1, &s2));
}



static void syntaxForSameVerbCanBeConnected()
{
  List *elements = concat(NULL,
			  newParameterElement(nulsrcp, newId(nulsrcp, "a"), 0),
			  ELEMENT_LIST);
  IdNode *verbId1 = newId(nulsrcp, "verb");
  IdNode *verbId2 = newId(nulsrcp, "verb");
  IdNode *verbId3 = newId(nulsrcp, "verb");
  IdNode *verbId4 = newId(nulsrcp, "verb");

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


// TODO Refactor handling of the Element lists to:
// newElementList()
// getFirstElement(list)
// getLastElement(list)
// getElement(list, n)
static void canCreateNewSyntaxWithEOS() {
  Syntax *syntax = newSyntaxWithEOS(nulsrcp, NULL, NULL, nulsrcp);
  ASSERT(syntax->elements->element.elm->kind == END_OF_SYNTAX);
}


static void canAddElementBeforeEOS() {
  Syntax *syntax = newSyntaxWithEOS(nulsrcp, NULL, NULL, nulsrcp);
  Element *firstElement = newParameterElement(nulsrcp, NULL, 0);
  addElement(syntax, firstElement);
  ASSERT(length(syntax->elements) == 2);
  // TODO Refactor to use getLastElement()
  ASSERT(((Element *)getLastMember(syntax->elements))->kind == END_OF_SYNTAX);

  Element *secondElement = newParameterElement(nulsrcp, NULL, 0);
  addElement(syntax, secondElement);
  ASSERT(length(syntax->elements) == 3);
  ASSERT(getMember(syntax->elements, 1) == firstElement);
  ASSERT(getMember(syntax->elements, 2) == secondElement);
  ASSERT(((Element *)getLastMember(syntax->elements))->kind == END_OF_SYNTAX);
}

void registerStxUnitTests()
{
  registerUnitTest(canCountParameters);
  registerUnitTest(parameterListsShouldBeCompatibleIfTheyHaveTheSameNumberOfParameters);
  registerUnitTest(syntaxForSameVerbCanBeConnected);
  registerUnitTest(canCreateNewSyntaxWithEOS);
  registerUnitTest(canAddElementBeforeEOS);
}

