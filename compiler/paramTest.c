/*======================================================================*\

  paramTest.c

  Unit tests for PARAM node in the Alan compiler

\*======================================================================*/



void testParameterReference()
{
  Context context;
  List *parameters;
  IdNode *p1Id = newId(&nulsrcp, "p1");
  SymNod *foundSymbol;
  IdNode *v1Id = newId(&nulsrcp, "v1");
  SymNod *v1Symbol;

  initadv();

  v1Symbol = newSymbol(v1Id, VERB_SYMBOL);
  context.kind = VERB_CONTEXT;
  context.verb = v1Symbol;
  parameters = createOneParameter("p1");
  setParameters(v1Symbol, parameters);

  /* Parameter not found if not in verb context */
  foundSymbol = symcheck(p1Id, INSTANCE_SYMBOL, NULL);
  unitAssert(foundSymbol == NULL);
  unitAssert(readEcode() == 310 && readSev() == sevERR); /* Not found! */

  foundSymbol = symcheck(p1Id, INSTANCE_SYMBOL, &context);
  unitAssert(foundSymbol != NULL);
  unitAssert(foundSymbol->kind == PARAMETER_SYMBOL);
  unitAssert(foundSymbol->fields.parameter.element == parameters->element.elm);
}


void registerParamUnitTests()
{
  registerUnitTest(testParameterReference);
}

