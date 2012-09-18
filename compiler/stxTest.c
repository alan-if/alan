/*======================================================================*\

  stxTest.c

\*======================================================================*/

#include "stx.c"

#include <cgreen/cgreen.h>

#include "unit.h"


/*----------------------------------------------------------------------*/
Ensure(canCountParameters) {
  List *elementList;

  elementList = concat(NULL, newParameterElement(nulsrcp, NULL, 0), ELEMENT_LIST);
  assert_true(countParameters(elementList) == 1);
  elementList = concat(elementList, newWordElement(nulsrcp, NULL), ELEMENT_LIST);
  assert_true(countParameters(elementList) == 1);
  elementList = concat(elementList, newParameterElement(nulsrcp, NULL, 0), ELEMENT_LIST);
  assert_true(countParameters(elementList) == 2);
}


/*----------------------------------------------------------------------*/
Ensure(parameterListsShouldBeCompatibleIfTheyHaveTheSameNumberOfParameters) {
  Syntax s1, s2;

  s1.elements = concat(NULL, newParameterElement(nulsrcp, newId(nulsrcp, "a"), 0), ELEMENT_LIST);
  s2.elements = concat(NULL, newParameterElement(nulsrcp, newId(nulsrcp, "a"), 0), ELEMENT_LIST);
	     

  s2.elements = concat(s2.elements, newWordElement(nulsrcp, newId(nulsrcp, "x")), ELEMENT_LIST);
  assert_true(compatibleParameterLists(&s1, &s2));

  s1.elements = concat(s1.elements, newParameterElement(nulsrcp, newId(nulsrcp, "b"), 0), ELEMENT_LIST);
  assert_true(!compatibleParameterLists(&s1, &s2));
}



// TODO Refactor handling of the Element lists to:
// newElementList()
// getFirstElement(list)
// getLastElement(list)
// getElement(list, n)
/*----------------------------------------------------------------------*/
Ensure(canCreateNewSyntaxWithEOS) {
  Syntax *syntax = newSyntaxWithEOS(nulsrcp, NULL, NULL, nulsrcp);
  assert_true(syntax->elements->member.elm->kind == END_OF_SYNTAX);
}


/*----------------------------------------------------------------------*/
Ensure(canAddElementBeforeEOS) {
  Syntax *syntax = newSyntaxWithEOS(nulsrcp, NULL, NULL, nulsrcp);
  Element *firstElement = newParameterElement(nulsrcp, NULL, 0);
  addElement(syntax, firstElement);
  assert_true(length(syntax->elements) == 2);
  assert_true(((Element *)getLastMember(syntax->elements))->kind == END_OF_SYNTAX);

  Element *secondElement = newParameterElement(nulsrcp, NULL, 0);
  addElement(syntax, secondElement);
  assert_true(length(syntax->elements) == 3);
  assert_true(getMember(syntax->elements, 1) == firstElement);
  assert_true(getMember(syntax->elements, 2) == secondElement);
  assert_true(((Element *)getLastMember(syntax->elements))->kind == END_OF_SYNTAX);
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
Ensure(connectSyntaxesConnectsVerbsForSameVerb) {
  Syntax *s1 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  Syntax *s2 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  Syntax *s3 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  Syntax *s4 = givenASyntax("verb", givenAnElementListWithOneParameterElement("parameter"));
  List *stxs = givenAListOfFourSyntaxes(s1, s2, s3, s4);

  connectSyntaxesForSameVerb(stxs);

  assert_true(s1->nextSyntaxForSameVerb == s2);
  assert_true(s1->firstSyntax);
  assert_true(s2->nextSyntaxForSameVerb == s3);
  assert_true(!s2->firstSyntax);
  assert_true(s3->nextSyntaxForSameVerb == s4);
  assert_true(!s3->firstSyntax);
  assert_true(s4->nextSyntaxForSameVerb == NULL);
  assert_true(!s4->firstSyntax);
}


/*----------------------------------------------------------------------*/
Ensure(analyzeSyntaxWillAddSyntaxesStartingWithInstance) {
    newVerbSymbol(newId(nulsrcp, "verb"));
    List *elms = givenAnElementListWithOneParameterElement("parameter");
    Syntax *stx = givenASyntax("verb", elms);
    
    adv.stxsStartingWithInstanceReference = NULL;
    
    analyzeSyntax(stx);
    
    assert_true(adv.stxsStartingWithInstanceReference != NULL);
}


TestSuite *stxTests()
{
    TestSuite *suite = create_test_suite(); 

    add_test(suite, canCountParameters);
    add_test(suite, parameterListsShouldBeCompatibleIfTheyHaveTheSameNumberOfParameters);
    add_test(suite, canCreateNewSyntaxWithEOS);
    add_test(suite, canAddElementBeforeEOS);
    add_test(suite, connectSyntaxesConnectsVerbsForSameVerb);
    add_test(suite, analyzeSyntaxWillAddSyntaxesStartingWithInstance);

    return suite;
}

