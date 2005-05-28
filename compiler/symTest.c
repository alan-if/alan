/*======================================================================*\

  symTest.c

  A unit test module for SYM nodes in the Alan compiler

\*======================================================================*/

#include "sym.c"
#include "elm_x.h"
#include "prop_x.h"

/*======================================================================

  Symbol table tests

*/

static char symbolName1[] = "n-is-in-the-middle";
static char symbolName2[] = "b-is-lower";
static char symbolName3[] = "p-is-higher";
static IdNode *symbolId1;
static IdNode *symbolId2;
static IdNode *symbolId3;


void testContentOfSymbol() {
  IdNode *contentClassId = newId(nulsrcp, "contentClassId");
  Symbol *contentSymbol = newClassSymbol(contentClassId, NULL, NULL);
  IdNode *content = newId(nulsrcp, "content");
  Container *container = newContainer(newContainerBody(nulsrcp, FALSE, content, NULL, NULL, NULL, NULL, NULL));

  IdNode *id = newId(nulsrcp, "id");
  Properties *props = newProps(NULL, NULL, nulsrcp, NULL, NULL, 0, container,
			       NULL,
			       nulsrcp, NULL,
			       nulsrcp, NULL,
			       nulsrcp, NULL, FALSE,
			       nulsrcp, NULL, FALSE,
			       NULL, NULL, NULL);
  Symbol *classTakingContent = newClassSymbol(id, props, NULL);
  IdNode *instanceId = newId(nulsrcp, "instance");
  Symbol *instance = newInstanceSymbol(instanceId, newEmptyProps(), classTakingContent);
  content->symbol = contentSymbol;

  ASSERT(contentOfSymbol(classTakingContent) == contentSymbol);
  ASSERT(contentOfSymbol(instance) == contentSymbol);

}


void testSymCheck()
{
  Srcp srcp = {14, 12, 333};
  IdNode *unknownId = newId(srcp, "unknownId");
  IdNode *aClassId = newId(srcp, "aClassId");
  Symbol *aClassSymbol = newSymbol(aClassId, CLASS_SYMBOL);
  IdNode *anInstanceId = newId(srcp, "anInstanceId");
  Symbol *anInstanceSymbol = newSymbol(anInstanceId, INSTANCE_SYMBOL);
  Symbol *foundSymbol;


  foundSymbol = symcheck(unknownId, CLASS_SYMBOL, NULL);
  ASSERT(foundSymbol == NULL);
  ASSERT(readEcode() == 310 && readSev() == sevERR);

  foundSymbol = symcheck(aClassId, CLASS_SYMBOL, NULL);
  ASSERT(foundSymbol == aClassSymbol);

  foundSymbol = symcheck(aClassId, INSTANCE_SYMBOL, NULL);
  ASSERT(foundSymbol == NULL);
  ASSERT(readEcode() == 319 && readSev() == sevERR);

  foundSymbol = symcheck(anInstanceId, INSTANCE_SYMBOL, NULL);
  ASSERT(foundSymbol == anInstanceSymbol);
}


static List *createOneParameter(char *id)
{
  return concat(NULL, newParameterElement(nulsrcp,
			     newId(nulsrcp, id), 0), ELEMENT_LIST);
}

void testVerbSymbols()
{
  IdNode *v1Id = newId(nulsrcp, "v1");
  Symbol *v1Symbol = newSymbol(v1Id, VERB_SYMBOL);
  Symbol *foundSymbol;
  List *parameters, *l, *p;
  Context context;

  foundSymbol = lookup("v1");
  ASSERT(foundSymbol == v1Symbol);

  parameters = createOneParameter("p1");
  setParameters(v1Symbol, parameters);
  ASSERT(v1Symbol->fields.verb.parameterSymbols != NULL);
  for (l = v1Symbol->fields.verb.parameterSymbols,
	 p = parameters;
       l && p;
       l = l->next, p = p->next)
    ASSERT(l->element.sym->fields.parameter.element == p->element.elm);

  foundSymbol = lookupInParameterList("p1", v1Symbol->fields.verb.parameterSymbols);
  ASSERT(foundSymbol == v1Symbol->fields.verb.parameterSymbols->element.sym);

  context.kind = VERB_CONTEXT;
  context.verb = v1Symbol;
  foundSymbol = lookupInContext("p1", &context);
  ASSERT(foundSymbol == v1Symbol->fields.verb.parameterSymbols->element.sym);

  
}



/* Test symbol table by inserting a symbol with an initial name */
void testBuildSymbol1()
{
  Symbol *sym1;
  Symbol *sym2;

  symbolId1 = newId(nulsrcp, symbolName1);
  symbolId2 = newId(nulsrcp, symbolName2);
  symbolId3 = newId(nulsrcp, symbolName3);

  sym1 = newSymbol(symbolId1, CLASS_SYMBOL);
  sym2 = lookup(symbolName1);

  ASSERT(sym1 == sym2);
  ASSERT(strcmp(sym2->string, symbolName1) == 0);
  ASSERT(sym2->kind == CLASS_SYMBOL);
}


/* Test symbol table by inserting a symbol with a higher name */
void testBuildSymbolHigher()
{
  Symbol *sym1 = newSymbol(symbolId2, CLASS_SYMBOL);
  Symbol *sym2 = lookup(symbolName2);

  ASSERT(sym1 == sym2);
  ASSERT(strcmp(sym2->string, symbolName2) == 0);
  ASSERT(sym2->kind == CLASS_SYMBOL);
}

/* Test symbol table by inserting a symbol with a lower name */
void testBuildSymbolLower()
{
  Symbol *sym1 = newSymbol(symbolId3, CLASS_SYMBOL);
  Symbol *sym2 = lookup(symbolName3);

  ASSERT(sym1 == sym2);
  ASSERT(strcmp(sym2->string, symbolName3) == 0);
  ASSERT(sym2->kind == CLASS_SYMBOL);
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

  ASSERT(parentOf(sym1) == sym2);
  ASSERT(parentOf(sym2) == sym3);
  ASSERT(parentOf(sym3) == NULL);
}


/* Test symbol table by verifying inheritance */
void testInherit2()
{
  Symbol *sym1 = lookup(symbolName1);
  Symbol *sym2 = lookup(symbolName2);
  Symbol *sym3 = lookup(symbolName3);

  ASSERT(!inheritsFrom(NULL, NULL));

  ASSERT(inheritsFrom(sym1, sym2));
  ASSERT(inheritsFrom(sym2, sym3));
  ASSERT(inheritsFrom(sym1, sym3));
  ASSERT(inheritsFrom(sym3, sym3));

  ASSERT(!inheritsFrom(sym3, sym1));
  ASSERT(!inheritsFrom(sym3, sym2));
}


/* Test symbol table by verifying inheritance */
void testInheritErrorSymbol()
{
  Symbol *sym1 = lookup(symbolName1);

  Symbol *err = newSymbol(newId(nulsrcp, "ErrorSymbol"), ERROR_SYMBOL);

  ASSERT(inheritsFrom(err, sym1));
  ASSERT(inheritsFrom(sym1, err));
  ASSERT(inheritsFrom(err, err));
}


/* Test symbol table initialisation */
void testSymbolTableInit()
{
  Symbol *entitySymbol;
  Symbol *thingSymbol;
  Symbol *objectSymbol;
  Symbol *actorSymbol;
  Symbol *locationSymbol;

  Symbol *sym1 = lookup(symbolName1);
  Symbol *sym2 = lookup(symbolName2);
  Symbol *sym3 = lookup(symbolName3);
  
  initAdventure();
  adv.whr = NULL;
  ASSERT(classCount == 8);	/* Standard classes */
  ASSERT(instanceCount == 0);
  addHero();
  ASSERT(instanceCount == 1);

  symbolizeAdventure();
  thingSymbol = lookup("thing");
  entitySymbol = lookup("entity");
  objectSymbol = lookup("object");
  actorSymbol = lookup("actor");
  locationSymbol = lookup("location");

  ASSERT(entity->props->id->symbol == entitySymbol);
  ASSERT(thing->props->id->symbol == thingSymbol);
  ASSERT(object->props->id->symbol == objectSymbol);
  ASSERT(location->props->id->symbol == locationSymbol);
  ASSERT(actor->props->id->symbol == actorSymbol);
  
  ASSERT(inheritsFrom(thingSymbol, entitySymbol));
  ASSERT(inheritsFrom(thingSymbol, thingSymbol));
  ASSERT(inheritsFrom(locationSymbol, entitySymbol));
  ASSERT(inheritsFrom(objectSymbol, thingSymbol));
  ASSERT(inheritsFrom(actorSymbol, thingSymbol));

  setParent(sym1, objectSymbol);
  setParent(sym2, actorSymbol);
  setParent(sym3, locationSymbol);

  ASSERT(inheritsFrom(sym1, objectSymbol));
  ASSERT(inheritsFrom(sym2, actorSymbol));
  ASSERT(inheritsFrom(sym3, locationSymbol));

  ASSERT(inheritsFrom(sym1, thingSymbol));
  ASSERT(inheritsFrom(sym2, thingSymbol));
  ASSERT(!inheritsFrom(sym3, thingSymbol));
  ASSERT(inheritsFrom(sym3, entitySymbol));

}


/* Create a new CLAss symbol */
void testCreateClassSymbol()
{
  Srcp srcp = {12,3,45};
  IdNode *id = newId(srcp, "cla");
  IdNode *heritage = newId(nulsrcp, "object");
  Symbol *sym, *obj;

  (void) newClass(&srcp, heritage, NULL, NULL);
  (void) newClass(&srcp, id, heritage, NULL);

  symbolizeClasses();

  sym = lookup("cla");
  obj = lookup("object");

  ASSERT(sym != NULL);
  ASSERT(obj != NULL);
  ASSERT(strcmp(sym->string, "cla") == 0);
  ASSERT(strcmp(obj->string, "object") == 0);
  ASSERT(inheritsFrom(sym, obj));
}

static void testLookupScript()
{
  Symbol *classSymbol;
  Symbol *instanceSymbol;
  IdNode *notAScriptId = newId(nulsrcp, "notAScript");
  IdNode *script1Id = newId(nulsrcp, "script1");
  IdNode *script2Id = newId(nulsrcp, "script2");
  IdNode *script3Id = newId(nulsrcp, "script3");
  IdNode *script4Id = newId(nulsrcp, "script4");
  Script script1 = {{0,0,0}, script1Id};
  Script script2 = {{0,0,0}, script2Id};
  Script script3 = {{0,0,0}, script3Id};
  Script script4 = {{0,0,0}, script4Id};
  List *classScripts;
  List *instanceScripts;

  initAdventure();
  classSymbol = newSymbol(newId(nulsrcp, "aClass"), CLASS_SYMBOL);
  instanceSymbol = newSymbol(newId(nulsrcp, "anInstance"), INSTANCE_SYMBOL);
  setParent(instanceSymbol, classSymbol);
  classScripts = concat(NULL, &script1, SCRIPT_LIST);
  classScripts = concat(classScripts, &script2, SCRIPT_LIST);
  instanceScripts = concat(NULL, &script3, SCRIPT_LIST);
  instanceScripts = concat(instanceScripts, &script4, SCRIPT_LIST);

  classSymbol->fields.entity.props = NEW(Properties);
  classSymbol->fields.entity.props->scripts = classScripts;
  instanceSymbol->fields.entity.props = NEW(Properties);
  instanceSymbol->fields.entity.props->scripts = instanceScripts;

  ASSERT(lookupScript(classSymbol, notAScriptId) == NULL);
  ASSERT(lookupScript(classSymbol, script1Id) == &script1);
  ASSERT(lookupScript(classSymbol, script2Id) == &script2);

  ASSERT(lookupScript(instanceSymbol, notAScriptId) == NULL);
  ASSERT(lookupScript(instanceSymbol, script1Id) == &script1);
  ASSERT(lookupScript(instanceSymbol, script2Id) == &script2);
  ASSERT(lookupScript(instanceSymbol, script3Id) == &script3);
  ASSERT(lookupScript(instanceSymbol, script4Id) == &script4);
}


void testNewFrame()
{
  Symbol *verbSymbol;
  Element *element;
  Symbol *parameterSymbol;
  Symbol *localSymbol1;
  Symbol *localSymbol2;
  IdNode *parameterId = newId(nulsrcp, "p");
  IdNode *localId = newId(nulsrcp, "p");
  Context context;

  initAdventure();
  /* Create a verb v with a parameter p */
  verbSymbol = newSymbol(newId(nulsrcp, "v"), VERB_SYMBOL);
  element = newParameterElement(nulsrcp, parameterId, 0);
  parameterSymbol = newParameterSymbol("p", element);

  verbSymbol->fields.verb.parameterSymbols = concat(NULL, parameterSymbol, SYMBOL_LIST);
  context.kind = VERB_CONTEXT;
  context.verb = verbSymbol;
  context.previous = NULL;
  context.class = NULL;
  context.instance = NULL;

  ASSERT(lookupInContext("p", &context) == parameterSymbol);

  /* Now create a new frame with a local variable "p" */
  newFrame();
  localSymbol1 = newSymbol(localId, LOCAL_SYMBOL);
  ASSERT(lookupInContext("p", &context) == localSymbol1);
  newFrame();
  localSymbol2 = newSymbol(localId, LOCAL_SYMBOL);
  ASSERT(lookupInContext("p", &context) == localSymbol2);
  ASSERT(localSymbol2->fields.local.level == 2);
  deleteFrame();
  ASSERT(lookupInContext("p", &context) == localSymbol1);
  ASSERT(localSymbol1->fields.local.level == 1);
  deleteFrame();
  ASSERT(lookupInContext("p", &context) == parameterSymbol);
}


void testReplicateContainer()
{
  Symbol *child = newSymbol(newId(nulsrcp, "child"), CLASS_SYMBOL);
  Symbol *parent = newSymbol(newId(nulsrcp, "parent"), CLASS_SYMBOL);
  Container *container = newContainer(newContainerBody(nulsrcp, FALSE, NULL, (void *)1, (void *)2, (void *)3, (void *)4, (void *)5));

  child->fields.entity.props = NEW(Properties);
  parent->fields.entity.props = NEW(Properties);
  setParent(child, parent);
  parent->fields.entity.props->container = container;

  replicateContainer(child);

  ASSERT(child->fields.entity.props->container->body->limits == (void *)1);
  ASSERT(child->fields.entity.props->container->body->hstms == (void *)2);
  ASSERT(child->fields.entity.props->container->body->estms == (void *)3);
}

static void testCreateMessageVerbs()
{
  Symbol *v, *p;
  Symbol *typeSymbol = newClassSymbol(newId(nulsrcp, "type"), NULL, NULL);

  v = createMessageVerb(0, typeSymbol);
  ASSERT(v->kind == VERB_SYMBOL);
  ASSERT(length(v->fields.verb.parameterSymbols) == 0);

  v = createMessageVerb(2, typeSymbol);
  ASSERT(v->kind == VERB_SYMBOL);
  ASSERT(length(v->fields.verb.parameterSymbols) == 2);

  p = v->fields.verb.parameterSymbols->element.sym;
  ASSERT(p->kind == PARAMETER_SYMBOL);
  ASSERT(p->fields.parameter.type == INSTANCE_TYPE);
  ASSERT(p->fields.parameter.class == typeSymbol);
  p = v->fields.verb.parameterSymbols->next->element.sym;
  ASSERT(p->kind == PARAMETER_SYMBOL);
  ASSERT(p->fields.parameter.type == INSTANCE_TYPE);
  ASSERT(p->fields.parameter.class == typeSymbol);
}

static void testInheritOpaqueAttribute() {
  /* Set up a parent class with container properties */
  Bool opaqueState = TRUE;
  ContainerBody *pBody = newContainerBody(nulsrcp, opaqueState, NULL, NULL,
					  NULL, NULL, NULL, NULL);
  Container *pCont = newContainer(pBody);
  Properties *pProps = newProps(NULL, NULL, nulsrcp,
				NULL, NULL, 0, pCont,
				NULL, nulsrcp,
				NULL, nulsrcp,
				NULL, nulsrcp,
				NULL, FALSE, nulsrcp,
				NULL, FALSE, NULL, NULL, NULL);
  Symbol *parent = newClassSymbol(newId(nulsrcp, "p"), pProps, NULL);

  /* Setup a child */
  List *attributes = concat(NULL, newBooleanAttribute(nulsrcp,
						      newId(nulsrcp, "b"),
						      FALSE), ATTRIBUTE_LIST);
						      
  Properties *cProps = newProps(NULL, NULL, nulsrcp,
				NULL, attributes, 0, NULL,
				NULL, nulsrcp,
				NULL, nulsrcp,
				NULL, nulsrcp,
				NULL, FALSE, nulsrcp,
				NULL, FALSE, NULL, NULL, NULL);
  Symbol *child = newInstanceSymbol(newId(nulsrcp, "c"), cProps, parent);

  replicateContainer(child);

  ASSERT(length(cProps->attributes) == 2);
  ASSERT(cProps->attributes->element.atr->id->code == OPAQUEATTRIBUTE); /* Predefined OPAQUE */
  ASSERT(cProps->attributes->element.atr->value == opaqueState);
}

/*----------------------------------------------------------------------*/
static void testMultipleSymbolKinds() {
  ASSERT(multipleSymbolKinds(0) == FALSE);
  ASSERT(multipleSymbolKinds(INSTANCE_SYMBOL) == FALSE);
  ASSERT(multipleSymbolKinds(INSTANCE_SYMBOL|CLASS_SYMBOL) == TRUE);
}

void registerSymUnitTests()
{
  registerUnitTest(testMultipleSymbolKinds);
  registerUnitTest(testContentOfSymbol);
  registerUnitTest(testSymCheck);
  registerUnitTest(testBuildSymbol1);
  registerUnitTest(testBuildSymbolHigher);
  registerUnitTest(testBuildSymbolLower);
  registerUnitTest(testInherit1);
  registerUnitTest(testInherit2);
  registerUnitTest(testInheritErrorSymbol);
  registerUnitTest(testSymbolTableInit);
  registerUnitTest(testCreateClassSymbol);
  registerUnitTest(testVerbSymbols);
  registerUnitTest(testLookupScript);
  registerUnitTest(testNewFrame);
  registerUnitTest(testReplicateContainer);
  registerUnitTest(testCreateMessageVerbs);
  registerUnitTest(testInheritOpaqueAttribute);
}

