/*======================================================================*\

  unit.c

  A unit test main program for the Alan compiler
  It will include the complete module source and then define some tests
  and test cases which will be run through the unitTest() runner.

\*======================================================================*/

#include <stdio.h>

#include "sym.c"


static void (*(cases[]))();

#include "unitTest.h"


int main()
{

  lmLiInit("Alan Compiler Unit Test", "<no file>", lm_ENGLISH_Messages);

  unitTest();

  return 0;
}
  

/*======================================================================

  Symbol table tests

*/

static char symbolName1[] = "n-is-in-the-middle";
static char symbolName2[] = "b-is-lower";
static char symbolName3[] = "p-is-higher";

/* Test symbol table by inserting a symbol with an initial name */
void testBuildSymbol1()
{
  int x;
  SymNod *sym1 = newsym(symbolName1, CLASS_SYMBOL, &x);
  SymNod *sym2 = lookup(symbolName1);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName1) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
  unitAssert(sym2->ref.cla == (struct ClaNod *)&x);
}


/* Test symbol table by inserting a symbol with a higher name */
void testBuildSymbolHigher()
{
  int x;
  SymNod *sym1 = newsym(symbolName2, CLASS_SYMBOL, &x);
  SymNod *sym2 = lookup(symbolName2);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName2) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
  unitAssert(sym2->ref.cla == (struct ClaNod *)&x);
}

/* Test symbol table by inserting a symbol with a lower name */
void testBuildSymbolLower()
{
  int x;
  SymNod *sym1 = newsym(symbolName3, CLASS_SYMBOL, &x);
  SymNod *sym2 = lookup(symbolName3);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName3) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
  unitAssert(sym2->ref.cla == (struct ClaNod *)&x);
}

/* Test inheritance by setting it and retrieving it */
void testInherit1()
{
  /* Insert inheritance in alphabetical order */
  SymNod *sym1 = lookup(symbolName1);
  SymNod *sym2 = lookup(symbolName2);
  SymNod *sym3 = lookup(symbolName3);

  setParent(sym1, sym2);
  setParent(sym2, sym3);

  unitAssert(parentOf(sym1) == sym2);
  unitAssert(parentOf(sym2) == sym3);
  unitAssert(parentOf(sym3) == NULL);
}


/* Test symbol table by verifying inheritance */
void testInherit2()
{
  SymNod *sym1 = lookup(symbolName1);
  SymNod *sym2 = lookup(symbolName2);
  SymNod *sym3 = lookup(symbolName3);

  unitAssert(inheritsFrom(sym1, sym2));
  unitAssert(inheritsFrom(sym2, sym3));
  unitAssert(inheritsFrom(sym1, sym3));

  unitAssert(!inheritsFrom(sym3, sym1));
  unitAssert(!inheritsFrom(sym3, sym3));
  unitAssert(!inheritsFrom(sym3, sym2));
}


/* Test symbol table initialisation */
void testInitTest()
{
  SymNod *thing;
  SymNod *object;
  SymNod *actor;
  SymNod *location;

  SymNod *sym1 = lookup(symbolName1);
  SymNod *sym2 = lookup(symbolName2);
  SymNod *sym3 = lookup(symbolName3);
  
  initSymbols();

  thing = lookup("thing");
  object = lookup("object");
  actor = lookup("actor");
  location = lookup("location");

  unitAssert(inheritsFrom(object, thing));
  unitAssert(inheritsFrom(location, thing));
  unitAssert(inheritsFrom(actor, thing));

  unitAssert(!inheritsFrom(thing, thing));

  setParent(sym1, object);
  setParent(sym2, actor);
  setParent(sym3, location);

  unitAssert(inheritsFrom(sym1, object));
  unitAssert(inheritsFrom(sym2, actor));
  unitAssert(inheritsFrom(sym3, location));

  unitAssert(inheritsFrom(sym1, thing));
  unitAssert(inheritsFrom(sym2, thing));
  unitAssert(inheritsFrom(sym3, thing));

}


/* Create a new CLAss node */
void testCreateCla()
{
  Srcp srcp = {12,3,45};
  IdNod *id = newid(&srcp, "cla");
  IdNod *heritage = newid(&nulsrcp, "object");
  ClaNod *cla = newcla(&srcp, id, heritage, NULL);
  SymNod *sym, *obj = lookup("object");

  symbolizeClasses();

  sym = lookup("cla");

  unitAssert(sym && sym->ref.cla == cla);
  unitAssert(inheritsFrom(sym, obj));
}


static void (*(cases[]))() = {
  &testBuildSymbol1,
  &testBuildSymbolLower,
  &testBuildSymbolHigher,
  &testInherit1,
  &testInherit2,
  &testInitTest,
  &testCreateCla,
  NULL
};

