/*======================================================================*\

  paramTest.c

  Unit tests for PARAM node in the Alan compiler

\*======================================================================*/



void testParameterReference()
{
  List *parameters;
  IdNode *p1Id = newId(nulsrcp, "p1");
  Symbol *foundSymbol;
  IdNode *v1Id = newId(nulsrcp, "v1");
  Symbol *v1Symbol;
  Context *context;

  initAdventure();

  v1Symbol = newSymbol(v1Id, VERB_SYMBOL);
  context = newContext(VERB_CONTEXT, v1Symbol);
  parameters = createOneParameter("p1");
  setParameters(v1Symbol, parameters);

  /* Parameter not found if not in verb context */
  foundSymbol = symcheck(p1Id, INSTANCE_SYMBOL, NULL);
  ASSERT(foundSymbol == NULL);
  ASSERT(readEcode() == 310 && readSev() == sevERR); /* Not found! */

  foundSymbol = symcheck(p1Id, INSTANCE_SYMBOL, context);
  ASSERT(foundSymbol != NULL);
  ASSERT(foundSymbol->kind == PARAMETER_SYMBOL);
  ASSERT(foundSymbol->fields.parameter.element == parameters->element.elm);
}


void registerParamUnitTests()
{
  registerUnitTest(testParameterReference);
}

