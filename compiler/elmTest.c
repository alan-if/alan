/*======================================================================*\

  elmTest.c

  Unit tests for ELM node in the Alan compiler

\*======================================================================*/

#include "elm.c"

#include "stx_x.h"
#include "lst_x.h"


void testPartition()
{
  List *p = NULL;
  Element *e = newEndOfSyntax();
  List *ep = concat(NULL, e, ELEMENT_LIST);
  List *epp = concat(NULL, ep, LIST_LIST);

  ASSERT(partition(&p) == NULL);
  ASSERT(partition(&epp) != NULL);
}

void entryForParameterShouldMergeFlagsFromAllElements() {
  // Create three parameter elements
  Element *firstElement = newParameterElement(nulsrcp, NULL, 0);
  Element *secondElement = newParameterElement(nulsrcp, NULL, 0);
  Element *thirdElement = newParameterElement(nulsrcp, NULL, 0);

  // And a END_OF_SYNTAX
  Element *firstEOS = newEndOfSyntax();
  Element *secondEOS = newEndOfSyntax();
  Element *thirdEOS = newEndOfSyntax();

  //  Which must be lists pointing to Elements
  List *firstParameterList = concat(NULL, firstElement, ELEMENT_LIST);
  List *secondParameterList = concat(NULL, secondElement, ELEMENT_LIST);
  List *thirdParameterList = concat(NULL, thirdElement, ELEMENT_LIST);

  // Terminate all of them
  firstParameterList = concat(firstParameterList, firstEOS, ELEMENT_LIST);
  secondParameterList = concat(secondParameterList, secondEOS, ELEMENT_LIST);
  thirdParameterList = concat(thirdParameterList, thirdEOS, ELEMENT_LIST);

  // Hook them into three syntaxes
  Syntax *firstSyntax = newSyntax(nulsrcp, NULL, firstParameterList, NULL, nulsrcp);
  Syntax *secondSyntax = newSyntax(nulsrcp, NULL, secondParameterList, NULL, nulsrcp);
  Syntax *thirdSyntax = newSyntax(nulsrcp, NULL, thirdParameterList, NULL, nulsrcp);

  // Back point the EOS elements to the syntaxes
  firstEOS->stx = firstSyntax;
  secondEOS->stx = secondSyntax;
  thirdEOS->stx = thirdSyntax;

  // Put them in a list/collection as one partition
  List *partition = NULL;
  partition = concat(partition, firstParameterList, LIST_LIST);
  partition = concat(partition, secondParameterList, LIST_LIST);
  partition = concat(partition, thirdParameterList, LIST_LIST);

  ElementEntry entry;

  firstElement->flags = MULTIPLEBIT;
  secondElement->flags = 0;
  thirdElement->flags = 0;

  entryForParameter(&entry, partition, NULL);
  ASSERT(entry.flags == MULTIPLEBIT);

  firstElement->flags = 0;
  secondElement->flags = MULTIPLEBIT;
  thirdElement->flags = 0;

  entryForParameter(&entry, partition, NULL);
  ASSERT(entry.flags == MULTIPLEBIT);


  firstElement->flags = 0;
  secondElement->flags = 0;
  thirdElement->flags = MULTIPLEBIT;

  entryForParameter(&entry, partition, NULL);
  ASSERT(entry.flags == MULTIPLEBIT);

}

void registerElmUnitTests()
{
  registerUnitTest(testPartition);
  registerUnitTest(entryForParameterShouldMergeFlagsFromAllElements);
}

