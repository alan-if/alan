/*======================================================================*\

  unit.c

  A unit test main program for the Alan compiler
  It will include the complete module source and then define some tests
  and test cases which will be run through the unitTest() runner.

\*======================================================================*/

#include <stdio.h>

#include "sym.c"
#include "id_x.h"


static void (*(cases[]))();

#include "unitTest.h"

/* From unitList.c */  
extern int lastEcode;
extern lmSev lastSev;


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


void testSymCheck()
{
  Srcp srcp = {14, 12, 333};
  ElmNod *elm;
  IdNode *unknownId = newId(&srcp, "unknownId");
  IdNode *aClassId = newId(&srcp, "aClassId");
  SymNod *aClassSymbol = newsym("aClassId", CLASS_SYMBOL);
  SymNod *foundSymbol;

  foundSymbol = symcheck(&elm, unknownId, CLASS_SYMBOL, NULL);
  unitAssert(foundSymbol == NULL);
  unitAssert(lastEcode == 310); /* Unknown id */
  unitAssert(lastSev == sevERR);

  foundSymbol = symcheck(&elm, aClassId, INSTANCE_SYMBOL, NULL);
  unitAssert(foundSymbol == aClassSymbol);
  unitAssert(lastEcode == 319);
  unitAssert(lastSev == sevERR);
}

/* Test symbol table by inserting a symbol with an initial name */
void testBuildSymbol1()
{
  SymNod *sym1 = newsym(symbolName1, CLASS_SYMBOL);
  SymNod *sym2 = lookup(symbolName1);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName1) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
}


/* Test symbol table by inserting a symbol with a higher name */
void testBuildSymbolHigher()
{
  SymNod *sym1 = newsym(symbolName2, CLASS_SYMBOL);
  SymNod *sym2 = lookup(symbolName2);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName2) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
}

/* Test symbol table by inserting a symbol with a lower name */
void testBuildSymbolLower()
{
  SymNod *sym1 = newsym(symbolName3, CLASS_SYMBOL);
  SymNod *sym2 = lookup(symbolName3);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName3) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
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
  IdNode *id = newId(&srcp, "cla");
  IdNode *heritage = newId(&nulsrcp, "object");
  SymNod *sym, *obj;

  (void) newcla(&srcp, id, heritage, NULL);

  symbolizeClasses();

  sym = lookup("cla");
  obj = lookup("object");

  unitAssert(sym != NULL);
  unitAssert(obj != NULL);
  unitAssert(strcmp(sym->string, "cla") == 0);
  unitAssert(strcmp(obj->string, "object") == 0);
  unitAssert(inheritsFrom(sym, obj));
}


static void (*(cases[]))() = {
  &testSymCheck,
  &testBuildSymbol1,
  &testBuildSymbolLower,
  &testBuildSymbolHigher,
  &testInherit1,
  &testInherit2,
  &testInitTest,
  &testCreateCla,
  NULL
};

