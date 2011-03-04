/*======================================================================*\

  stxTest.c

\*======================================================================*/

#include "stx.c"

#include "unit.h"


/*----------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------*/
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



// TODO Refactor handling of the Element lists to:
// newElementList()
// getFirstElement(list)
// getLastElement(list)
// getElement(list, n)
/*----------------------------------------------------------------------*/
static void canCreateNewSyntaxWithEOS() {
  Syntax *syntax = newSyntaxWithEOS(nulsrcp, NULL, NULL, nulsrcp);
  ASSERT(syntax->elements->member.elm->kind == END_OF_SYNTAX);
}


/*----------------------------------------------------------------------*/
static void canAddElementBeforeEOS() {
  Syntax *syntax = newSyntaxWithEOS(nulsrcp, NULL, NULL, nulsrcp);
  Element *firstElement = newParameterElement(nulsrcp, NULL, 0);
  addElement(syntax, firstElement);
  ASSERT(length(syntax->elements) == 2);
  ASSERT(((Element *)getLastMember(syntax->elements))->kind == END_OF_SYNTAX);

  Element *secondElement = newParameterElement(nulsrcp, NULL, 0);
  addElement(syntax, secondElement);
  ASSERT(length(syntax->elements) == 3);
  ASSERT(getMember(syntax->elements, 1) == firstElement);
  ASSERT(getMember(syntax->elements, 2) == secondElement);
  ASSERT(((Element *)getLastMember(syntax->elements))->kind == END_OF_SYNTAX);
}


static List *givenAnElementListWithOneParameterElement(char *parameterName) {
	return concat(NULL, newParameterElement(nulsrcp, newId(nulsrcp, parameterName), 0), ELEMENT_LIST);
}


static Syntax *givenASyntax(char *id, List *elements) {
	return newSyntax(nulsrcp, newId(nulsrcp, id), elements, NULL, nulsrcp);
}


static List *givenAListOfFourSyntaxes(Syntax *stx1, Syntax *stx2, Syntax *stx3, Syntax *stx4) {
	return concat(concat(concat(concat(NULL, stx1, SYNTAX_LIST), stx2, SYNTAX_LIST), stx3, SYNTAX_LIST), stx4, SYNTAX_LIST);
}


/*----------------------------------------------------------------------*/
static void connectSyntaxesConnectsVerbsForSameVerb()
{
  Syntax *s1 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  Syntax *s2 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  Syntax *s3 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  Syntax *s4 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  List *stxs = givenAListOfFourSyntaxes(s1, s2, s3, s4);

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


/*----------------------------------------------------------------------*/
static void analyzeSyntaxWillAddSyntaxesStartingWithInstance() {
	newVerbSymbol(newId(nulsrcp, "verb"));
	List *elms = givenAnElementListWithOneParameterElement("parameter");
	Syntax *stx = givenASyntax("verb", elms);

	adv.stxsStartingWithInstanceReference = NULL;

	analyzeSyntax(stx);

	ASSERT(adv.stxsStartingWithInstanceReference != NULL);
}


void stxUnitTests()
{
	registerUnitTest(canCountParameters);
	registerUnitTest(parameterListsShouldBeCompatibleIfTheyHaveTheSameNumberOfParameters);
	registerUnitTest(canCreateNewSyntaxWithEOS);
	registerUnitTest(canAddElementBeforeEOS);
	registerUnitTest(connectSyntaxesConnectsVerbsForSameVerb);
	registerUnitTest(analyzeSyntaxWillAddSyntaxesStartingWithInstance);
}

