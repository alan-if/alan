/*======================================================================*\

  symTest.c

  A unit test module for SYM nodes in the Alan compiler

\*======================================================================*/

#include "sym.c"
#include "elm_x.h"
  

/*======================================================================

  Symbol table tests

*/

static char symbolName1[] = "n-is-in-the-middle";
static char symbolName2[] = "b-is-lower";
static char symbolName3[] = "p-is-higher";
static IdNode *symbolId1;
static IdNode *symbolId2;
static IdNode *symbolId3;


void testSymCheck()
{
  Srcp srcp = {14, 12, 333};
  IdNode *unknownId = newId(&srcp, "unknownId");
  IdNode *aClassId = newId(&srcp, "aClassId");
  Symbol *aClassSymbol = newSymbol(aClassId, CLASS_SYMBOL);
  IdNode *anInstanceId = newId(&srcp, "anInstanceId");
  Symbol *anInstanceSymbol = newSymbol(anInstanceId, INSTANCE_SYMBOL);
  Symbol *foundSymbol;


  foundSymbol = symcheck(unknownId, CLASS_SYMBOL, NULL);
  unitAssert(foundSymbol == NULL);
  unitAssert(readEcode() == 310 && readSev() == sevERR);

  foundSymbol = symcheck(aClassId, CLASS_SYMBOL, NULL);
  unitAssert(foundSymbol == aClassSymbol);

  foundSymbol = symcheck(aClassId, INSTANCE_SYMBOL, NULL);
  unitAssert(foundSymbol == NULL);
  unitAssert(readEcode() == 319 && readSev() == sevERR);

  foundSymbol = symcheck(anInstanceId, INSTANCE_SYMBOL, NULL);
  unitAssert(foundSymbol == anInstanceSymbol);
}


static List *createOneParameter(char *id)
{
  return concat(NULL, newelm(&nulsrcp, PARAMETER_ELEMENT,
			     newId(&nulsrcp, id), 0), ELEMENT_LIST);
}

void testVerbSymbols()
{
  IdNode *v1Id = newId(&nulsrcp, "v1");
  Symbol *v1Symbol = newSymbol(v1Id, VERB_SYMBOL);
  Symbol *foundSymbol;
  List *parameters, *l, *p;
  Context context;

  foundSymbol = lookup("v1");
  unitAssert(foundSymbol == v1Symbol);

  parameters = createOneParameter("p1");
  setParameters(v1Symbol, parameters);
  unitAssert(v1Symbol->fields.verb.parameterSymbols != NULL);
  for (l = v1Symbol->fields.verb.parameterSymbols,
	 p = parameters;
       l && p;
       l = l->next, p = p->next)
    unitAssert(l->element.sym->fields.parameter.element == p->element.elm);

  foundSymbol = lookupInParameterList("p1", v1Symbol->fields.verb.parameterSymbols);
  unitAssert(foundSymbol == v1Symbol->fields.verb.parameterSymbols->element.sym);

  context.kind = VERB_CONTEXT;
  context.verb = v1Symbol;
  foundSymbol = lookupInContext("p1", &context);
  unitAssert(foundSymbol == v1Symbol->fields.verb.parameterSymbols->element.sym);

  
}



/* Test symbol table by inserting a symbol with an initial name */
void testBuildSymbol1()
{
  Symbol *sym1;
  Symbol *sym2;

  symbolId1 = newId(&nulsrcp, symbolName1);
  symbolId2 = newId(&nulsrcp, symbolName2);
  symbolId3 = newId(&nulsrcp, symbolName3);

  sym1 = newSymbol(symbolId1, CLASS_SYMBOL);
  sym2 = lookup(symbolName1);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName1) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
}


/* Test symbol table by inserting a symbol with a higher name */
void testBuildSymbolHigher()
{
  Symbol *sym1 = newSymbol(symbolId2, CLASS_SYMBOL);
  Symbol *sym2 = lookup(symbolName2);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName2) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
}

/* Test symbol table by inserting a symbol with a lower name */
void testBuildSymbolLower()
{
  Symbol *sym1 = newSymbol(symbolId3, CLASS_SYMBOL);
  Symbol *sym2 = lookup(symbolName3);

  unitAssert(sym1 == sym2);
  unitAssert(strcmp(sym2->string, symbolName3) == 0);
  unitAssert(sym2->kind == CLASS_SYMBOL);
}

/* Test inheritance by setting it and retrieving it */
void testInherit1()
{
  /* Insert inheritance in alphabetical order */
  Symbol *sym1 = lookup(symbolName1);
  Symbol *sym2 = lookup(symbolName2);
  Symbol *sym3 = lookup(symbolName3);

  setParent(sym1, sym2);
  setParent(sym2, sym3);

  unitAssert(parentOf(sym1) == sym2);
  unitAssert(parentOf(sym2) == sym3);
  unitAssert(parentOf(sym3) == NULL);
}


/* Test symbol table by verifying inheritance */
void testInherit2()
{
  Symbol *sym1 = lookup(symbolName1);
  Symbol *sym2 = lookup(symbolName2);
  Symbol *sym3 = lookup(symbolName3);

  unitAssert(!inheritsFrom(NULL, NULL));

  unitAssert(inheritsFrom(sym1, sym2));
  unitAssert(inheritsFrom(sym2, sym3));
  unitAssert(inheritsFrom(sym1, sym3));
  unitAssert(inheritsFrom(sym3, sym3));

  unitAssert(!inheritsFrom(sym3, sym1));
  unitAssert(!inheritsFrom(sym3, sym2));
}


/* Test symbol table initialisation */
void testSymbolTableInit()
{
  Symbol *thingSymbol;
  Symbol *objectSymbol;
  Symbol *actorSymbol;
  Symbol *locationSymbol;

  Symbol *sym1 = lookup(symbolName1);
  Symbol *sym2 = lookup(symbolName2);
  Symbol *sym3 = lookup(symbolName3);
  
  initAdventure();
  adv.whr = NULL;
  unitAssert(classCount == 4);	/* Standard classes */
  unitAssert(instanceCount == 0);
  addHero();
  unitAssert(instanceCount == 1);

  symbolizeAdventure();
  thingSymbol = lookup("thing");
  objectSymbol = lookup("object");
  actorSymbol = lookup("actor");
  locationSymbol = lookup("location");

  unitAssert(thing->props->id->symbol == thingSymbol);
  unitAssert(object->props->id->symbol == objectSymbol);
  unitAssert(location->props->id->symbol == locationSymbol);
  unitAssert(actor->props->id->symbol == actorSymbol);
  
  unitAssert(inheritsFrom(objectSymbol, thingSymbol));
  unitAssert(inheritsFrom(locationSymbol, thingSymbol));
  unitAssert(inheritsFrom(actorSymbol, thingSymbol));
  unitAssert(inheritsFrom(thingSymbol, thingSymbol));

  setParent(sym1, objectSymbol);
  setParent(sym2, actorSymbol);
  setParent(sym3, locationSymbol);

  unitAssert(inheritsFrom(sym1, objectSymbol));
  unitAssert(inheritsFrom(sym2, actorSymbol));
  unitAssert(inheritsFrom(sym3, locationSymbol));

  unitAssert(inheritsFrom(sym1, thingSymbol));
  unitAssert(inheritsFrom(sym2, thingSymbol));
  unitAssert(inheritsFrom(sym3, thingSymbol));

}


/* Create a new CLAss symbol */
void testCreateClassSymbol()
{
  Srcp srcp = {12,3,45};
  IdNode *id = newId(&srcp, "cla");
  IdNode *heritage = newId(&nulsrcp, "object");
  Symbol *sym, *obj;

  (void) newClass(&srcp, heritage, NULL, NULL);
  (void) newClass(&srcp, id, heritage, NULL);

  symbolizeClasses();

  sym = lookup("cla");
  obj = lookup("object");

  unitAssert(sym != NULL);
  unitAssert(obj != NULL);
  unitAssert(strcmp(sym->string, "cla") == 0);
  unitAssert(strcmp(obj->string, "object") == 0);
  unitAssert(inheritsFrom(sym, obj));
}

static void testLookupScript()
{
  Symbol *classSymbol;
  Symbol *instanceSymbol;
  IdNode *notAScriptId = newId(&nulsrcp, "notAScript");
  IdNode *script1Id = newId(&nulsrcp, "script1");
  IdNode *script2Id = newId(&nulsrcp, "script2");
  IdNode *script3Id = newId(&nulsrcp, "script3");
  IdNode *script4Id = newId(&nulsrcp, "script4");
  Script script1 = {{0,0,0}, script1Id};
  Script script2 = {{0,0,0}, script2Id};
  Script script3 = {{0,0,0}, script3Id};
  Script script4 = {{0,0,0}, script4Id};
  List *classScripts;
  List *instanceScripts;

  initAdventure();
  classSymbol = newSymbol(newId(&nulsrcp, "aClass"), CLASS_SYMBOL);
  instanceSymbol = newSymbol(newId(&nulsrcp, "anInstance"), INSTANCE_SYMBOL);
  setParent(instanceSymbol, classSymbol);
  classScripts = concat(NULL, &script1, SCRIPT_LIST);
  classScripts = concat(classScripts, &script2, SCRIPT_LIST);
  instanceScripts = concat(NULL, &script3, SCRIPT_LIST);
  instanceScripts = concat(instanceScripts, &script4, SCRIPT_LIST);

  classSymbol->fields.entity.props = NEW(Properties);
  classSymbol->fields.entity.props->scripts = classScripts;
  instanceSymbol->fields.entity.props = NEW(Properties);
  instanceSymbol->fields.entity.props->scripts = instanceScripts;

  unitAssert(lookupScript(classSymbol, notAScriptId) == NULL);
  unitAssert(lookupScript(classSymbol, script1Id) == &script1);
  unitAssert(lookupScript(classSymbol, script2Id) == &script2);

  unitAssert(lookupScript(instanceSymbol, notAScriptId) == NULL);
  unitAssert(lookupScript(instanceSymbol, script1Id) == &script1);
  unitAssert(lookupScript(instanceSymbol, script2Id) == &script2);
  unitAssert(lookupScript(instanceSymbol, script3Id) == &script3);
  unitAssert(lookupScript(instanceSymbol, script4Id) == &script4);
}


void registerSymUnitTests()
{
  registerUnitTest(testSymCheck);
  registerUnitTest(testBuildSymbol1);
  registerUnitTest(testBuildSymbolHigher);
  registerUnitTest(testBuildSymbolLower);
  registerUnitTest(testInherit1);
  registerUnitTest(testInherit2);
  registerUnitTest(testSymbolTableInit);
  registerUnitTest(testCreateClassSymbol);
  registerUnitTest(testVerbSymbols);
  registerUnitTest(testLookupScript);
}

