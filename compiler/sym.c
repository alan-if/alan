/*----------------------------------------------------------------------*\

                                SYM.C
                          Symbol Table Nodes

\*----------------------------------------------------------------------*/

#include "sym_x.h"

/* IMPORTS */
#include "sysdep.h"
#include "util.h"
#include "lmList.h"
#include "dump.h"

#include "srcp_x.h"
#include "cla_x.h"
#include "id_x.h"
#include "atr_x.h"
#include "exp_x.h"
#include "lst_x.h"


/* EXPORTS: */
int classCount = 0;
int instanceCount = 0;
int directionCount = 0;
int attributeCount = 0;
int verbCount = 0;
int eventCount = 0;

Symbol *thingSymbol, *objectSymbol, *locationSymbol, *actorSymbol, *theHero;



/* PRIVATE: */
static Symbol *symbolTree = NULL;
static Bool firstSymbolDumped = TRUE;


/*======================================================================

  redefined()

  Calls lmLog() with correct code according to the symnod sent.

  */
void redefined(IdNode *id,
               Symbol *sym)
{
  int code = 0;                     /* Error code */

  switch (sym->kind) {
  case DIRECTION_SYMBOL: code = 301; break;
  case VERB_SYMBOL: code = 303; break;
  case INSTANCE_SYMBOL: code = 304; break;
  case CLASS_SYMBOL: code = 305; break;
  case EVENT_SYMBOL: code = 307; break;
  default: syserr("Unrecognized switch in redefined()"); break;
  }

  lmLog(&id->srcp, code, sevERR, id->string);
}



/*----------------------------------------------------------------------------

  insertSymbol()

  Insert a new symbol in the symbol tree

*/
static void insertSymbol(Symbol *symbol)
{
  Symbol *s1,*s2;               /* Traversal pointers */
  int comp = 0;			/* Result of comparison */

  symbol->lower = NULL;
  symbol->higher = NULL;

  s1 = symbolTree;
  s2 = NULL;
  
  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(symbol->string, s1->string);
    if (comp < 0)
      s1 = s1->lower;
    else
      s1 = s1->higher;
  }
  
  if (s2 == NULL)
    symbolTree = symbol;
  else if(comp < 0)
    s2->lower = symbol;
  else
    s2->higher = symbol;
}


/*----------------------------------------------------------------------

  symbolKind()

  Return a pointer to a string representation of the SymbolKind

  */
static char *symbolKind(SymbolKind kind)
{
  switch (kind) {
  case CLASS_SYMBOL: return "a class";
  case INSTANCE_SYMBOL: return "an instance";
  case VERB_SYMBOL: return "a verb";
  default: syserr("Unimplemented case in symbolKind()");
  }
  return NULL;
}



/*----------------------------------------------------------------------

  newParameterSymbol()

  */
static Symbol *newParameterSymbol(char *string, ElmNod *element)
{
  Symbol *new;                  /* The newly created symnod */
  
  if (string == NULL)
    return (0);
  
  new = NEW(Symbol);
  
  new->kind = PARAMETER_SYMBOL;
  new->string = string;
  new->fields.parameter.element = element;
  element->id->symbol = new;
  element->id->symbol->code = element->id->code;

  return new;
}


/*======================================================================

  newSymbol()

  Creates a new symnod and links it in the symbolTree.

  */
Symbol *newSymbol(IdNode *id,	/* IN - Name of the new symbol */
		  SymbolKind kind) /* IN - What kind of symbol */
{
  Symbol *new;                  /* The newly created symnod */
  
  if (id == NULL)
    return NULL;
  
  new = lookup(id->string);
  if (new != NULL)
    redefined(id, new);

  new = NEW(Symbol);
  
  new->kind = kind;
  new->string = id->string;

  insertSymbol(new);

  switch (kind) {
  case CLASS_SYMBOL:
    new->code = ++classCount;
    new->fields.entity.parent = NULL;
    new->fields.entity.attributesNumbered = FALSE;
    new->fields.entity.replicated = FALSE;
    break;
  case INSTANCE_SYMBOL:
    new->code = ++instanceCount;
    new->fields.entity.parent = NULL;
    new->fields.entity.attributesNumbered = FALSE;
    new->fields.entity.replicated = FALSE;
    break;
  case DIRECTION_SYMBOL:
    new->code = ++directionCount;
    break;
  case VERB_SYMBOL:
    new->code = ++verbCount;
    break;
  case EVENT_SYMBOL:
    new->code = ++eventCount;
    break;
  default: syserr("Unexpected switch on SYMBOLKIND in newSymbol()"); break;
  }

  return new;
}



/*======================================================================

  initSymbols()

  Initialise the symbol table;

  */
void initSymbols()
{
  symbolTree = NULL;
  instanceCount = 0;
  classCount = 0;
  attributeCount = 0;
}


/*======================================================================

  lookupInParameterList()

  Look for a symbol. If found return a pointer to its symnod, else NULL.

  */
static Symbol *lookupInParameterList(char *idString, List *parameterSymbols)
{
  List *l;

  for (l = parameterSymbols; l != NULL; l = l->next)
    if (strcmp(idString, l->element.sym->fields.parameter.element->id->string) == 0)
      return l->element.sym;
  return NULL;
}

/*======================================================================

  findParameter()

*/
Symbol *findParameter(IdNode *parameterId, List *parameterSymbols)
{
  List *p;

  for (p = parameterSymbols; p != NULL; p = p->next) {
    if (p->element.sym->kind != PARAMETER_SYMBOL)
      syserr("Not a parameter symbol in analyzeRestriction()");
    if (equalId(parameterId, p->element.sym->fields.parameter.element->id))
      return p->element.sym;
  }
  return NULL;
}


/*======================================================================

  lookup()

  Look for a symbol. If found return a pointer to its symnod, else NULL.

  */
Symbol *lookup(char *idString)
{
  Symbol *s1,*s2;               /* Traversal pointers */
  int comp;                     /* Result of comparison */

  if (idString == NULL) syserr("NULL string in lookup()");

  s1 = symbolTree;
  s2 = NULL;

  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(idString, s1->string);
    if (comp == 0)
      return(s1);
    else if (comp < 0)
      s1 = s1->lower;
    else
      s1 = s1->higher;
  }

  return(NULL);
}


/*======================================================================

  lookupInContext()

  Look for a symbol using the context. If found return a pointer to its
  symnod, else NULL.

  */
Symbol *lookupInContext(char *idString, Context *context)
{
  Symbol *foundSymbol = NULL;

  if (context != NULL) {
    switch (context->kind){
    case VERB_CONTEXT:
      foundSymbol = lookupInParameterList(idString, context->verb->fields.verb.parameterSymbols);
      break;
    case EVENT_CONTEXT:
    case RULE_CONTEXT:
    case INSTANCE_CONTEXT:
      foundSymbol = lookup(idString);
      break;
    default:
      syserr("Unexpected context kind in lookupInContext()");
      break;
    }
    if (foundSymbol != NULL)
      return foundSymbol;
  }

  return lookup(idString);
}




/*======================================================================

  setParent()

  Set the parent of a Class Symbol to be another Symbol

  */
void setParent(Symbol *child, Symbol *parent)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    syserr("Not a CLASS or INSTANCE in setParent()");
  child->fields.entity.parent = parent;
}


/*======================================================================

  parentOf()

  Get the parent of a Class Symbol

  */
Symbol *parentOf(Symbol *child)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    syserr("Not a CLASS or INSTANCE in parentOf()");
  return child->fields.entity.parent;
}


/*======================================================================

  inheritsFrom()

  Test inheritance from specified ClassSymbol.

  */
Bool inheritsFrom(Symbol *child, Symbol *ancestor)
{
  Symbol *p;

  if (child == NULL || ancestor == NULL) return FALSE;

  if (child->kind == PARAMETER_SYMBOL)
    child = child->fields.parameter.class;

  if ((child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL) ||
      (ancestor->kind != CLASS_SYMBOL))
    syserr("Not a CLASS or INSTANCE in inheritsFrom()");

  p = child;			/* To be the class itself is OK */
  while (p && p != ancestor)
    p = p->fields.entity.parent;

  return (p != NULL);
}


/*======================================================================

  symcheck()

  Check if an Id exists and if so if it is of an allowed kind in this context

*/
Symbol *symcheck(IdNode *id,
		 SymbolKind kind,
		 Context *context)
{
  Symbol *sym = lookupInContext(id->string, context);

  if (!sym) 
    lmLog(&id->srcp, 310, sevERR, id->string);
  else if (sym->kind == PARAMETER_SYMBOL) {
    if (sym->fields.parameter.element->kind != ID_RESTRICTION)
      lmLogv(&id->srcp, 319, sevERR, id->string, "a parameter that is restricted to instances of a class", NULL);      
  } else if (sym->kind != kind) {
    lmLogv(&id->srcp, 319, sevERR, id->string, symbolKind(kind), NULL);
    return NULL;
  }

  id->symbol = sym;
  return sym;
}




/*======================================================================

  setParameters()

  Set the list of parameters (ElmNodes) as parameters in the verb symbol.

*/
void setParameters(Symbol *verb, List *parameters)
{
  List *parameterSymbols = NULL;
  List *parameter;

  if (verb == NULL) return;

  if (verb->kind != VERB_SYMBOL) syserr("Not a verb in setParamters()");

  if (parameters == NULL) return;

  if (parameters->kind != ELEMENT_LIST) syserr("Not a parameter list in setParameter()");

  for (parameter = parameters; parameter != NULL; parameter = parameter->next) {
    Symbol *parameterSymbol = newParameterSymbol(parameter->element.elm->id->string, parameter->element.elm);
    parameterSymbols = concat(parameterSymbols, parameterSymbol, SYMBOL_LIST);
    parameter->element.elm->id->symbol = parameterSymbol; /* Cross refer */
  }

  verb->fields.verb.parameterSymbols = parameterSymbols;
}

/*======================================================================

  inheritCheck()

  Check that the given identifier inherits the class passed as a string.
  This will only be used for built in class checks (location, actor etc.)

*/
void inheritCheck(IdNode *id, char classOrInstance[], char className[])
{
  Symbol *theClassSymbol = lookup(className);

  if (theClassSymbol == NULL) syserr("There is no such class in inheritCheck()");

  if (!inheritsFrom(id->symbol, theClassSymbol))
    lmLogv(&id->srcp, 351, sevERR, classOrInstance, "location", NULL);
}



/*----------------------------------------------------------------------

  definingSymbolOfAttribute()

  Find the symbol which defines an attribute by traversing its parents.

*/
static Symbol *definingSymbolOfAttribute(Symbol *symbol, IdNode *id)
{
  Attribute *foundAttribute;

  if (symbol == NULL)
    return NULL;

  if (symbol->kind != CLASS_SYMBOL && symbol->kind != INSTANCE_SYMBOL)
    return NULL;

  if ((foundAttribute = findAttribute(symbol->fields.entity.props->attributes, id)) == NULL)
    return definingSymbolOfAttribute(symbol->fields.entity.parent, id);
  else
    return symbol;
}



/*======================================================================

  findInheritedAttribute()

  From a symbol traverse its inheritance tree to find a named attribute.

*/
Attribute *findInheritedAttribute(Symbol *symbol, IdNode *id)
{
  Symbol *definingSymbol =
    definingSymbolOfAttribute(symbol->fields.entity.parent, id);

  if (definingSymbol == NULL) return NULL;

  return findAttribute(definingSymbol->fields.entity.props->attributes, id);
}


/*----------------------------------------------------------------------

  numberAttributes()

*/
static void numberAttributes(Symbol *symbol)
{
  List *theList;
  Attribute *inheritedAttribute;
  Symbol *definingSymbol;

  if (symbol->fields.entity.attributesNumbered) return;

  for (theList = symbol->fields.entity.props->attributes; theList != NULL;
       theList = theList->next){
    inheritedAttribute = findInheritedAttribute(symbol, theList->element.atr->id);
    if (inheritedAttribute != NULL) {
      if (!equalTypes(inheritedAttribute->type, theList->element.atr->type)) {
	definingSymbol = definingSymbolOfAttribute(symbol, theList->element.atr->id);
	lmLog(&theList->element.atr->srcp, 332, sevERR, definingSymbol->string);
      }
      theList->element.atr->id->code = inheritedAttribute->id->code;
      theList->element.atr->inheritance = INHERITED_REDEFINED;
    } else {
      theList->element.atr->id->code = ++attributeCount;
      theList->element.atr->inheritance = LOCAL;
    }
  }

  symbol->fields.entity.props->attributes = sortAttributes(symbol->fields.entity.props->attributes);
  symbol->fields.entity.attributesNumbered = TRUE;
}


/*----------------------------------------------------------------------

  numberParentAttributes()

  Recurse the parental chain and number the attributes.

*/
static void numberParentAttributes(Symbol *symbol)
{
  if (symbol == NULL || symbol->fields.entity.attributesNumbered) return;

  numberParentAttributes(symbol->fields.entity.parent);
  numberAttributes(symbol);
}


/*----------------------------------------------------------------------

  numberAttributesRecursively()

  Recurse the parent to number its attributes.
  Number all attributes in the symbol (if it is a class or an instance);
*/
static void numberAttributesRecursively(Symbol *symbol)
{
  if (symbol == NULL) return;

  if (symbol->kind == CLASS_SYMBOL || symbol->kind == INSTANCE_SYMBOL) {
    /* Only a class or instance have attributes */

    numberParentAttributes(symbol->fields.entity.parent);
    numberAttributes(symbol);
  }

  /* Recurse in the symbolTree */
  if (symbol->lower != NULL) numberAttributesRecursively(symbol->lower);
  if (symbol->higher != NULL) numberAttributesRecursively(symbol->higher);

}



/*======================================================================

  numberAllAttributes()

  Traverse all classes and instances in the symbol table and give all
  attributes unique numbers. Start by recursing through the
  parents. Remember where we have been by looking at the code which is
  might already have been set.

*/
void numberAllAttributes(void)
{
  numberAttributesRecursively(symbolTree);
}


static Bool haveParent(Symbol *s) {return s->fields.entity.parent != NULL;}
static Properties *propertiesOf(Symbol *s) {return s->fields.entity.props;}
static Properties *propertiesOfParentOf(Symbol *s) {return s->fields.entity.parent->fields.entity.props;}

/*----------------------------------------------------------------------*/
static void replicateAttributes(Symbol *symbol)
{
  propertiesOf(symbol)->attributes = combineAttributes(propertiesOf(symbol)->attributes,
						  propertiesOfParentOf(symbol)->attributes);
}


/*----------------------------------------------------------------------*/
static void replicateContainer(Symbol *symbol)
{
  if (propertiesOf(symbol)->container == NULL)
    propertiesOf(symbol)->container = propertiesOfParentOf(symbol)->container;
}


/*----------------------------------------------------------------------*/
static void replicate(Symbol *symbol)
{
  if (haveParent(symbol)) {
    replicateAttributes(symbol);
    replicateContainer(symbol);
  }
}


/*----------------------------------------------------------------------

  replicateParent()

  Recurse the parental chain and replicate any inherited things that
  requires local replicated data.

*/
static void replicateParent(Symbol *symbol)
{
  if (symbol == NULL || symbol->fields.entity.replicated)
    return;

  replicateParent(symbol->fields.entity.parent);
  replicate(symbol);
  symbol->fields.entity.replicated = TRUE;
}


/*----------------------------------------------------------------------

  replicateRecursively()

*/
static void replicateRecursively(Symbol *symbol)
{
  if (symbol == NULL) return;

  if (symbol->kind == CLASS_SYMBOL || symbol->kind == INSTANCE_SYMBOL) {

    /* We have not replicated this symbol yet, so do it now */
    replicateParent(symbol->fields.entity.parent);
    replicate(symbol);
  }

  /* Recurse in the symbolTree */
  if (symbol->lower != NULL) replicateRecursively(symbol->lower);
  if (symbol->higher != NULL) replicateRecursively(symbol->higher);
}



/*======================================================================

  replicateInherited()

  Traverse the heritage of the symbol and replicate all inherited
  attributes, scripts etc. that are not locally redefined, thus
  creating a complete list of all attributes etc. that this symbol has.

  We will mark a completed symbol with "replicated" so that we can use
  it directly if we encounter it later instead of redoing it.

*/
void replicateInherited(void)
{
  replicateRecursively(symbolTree);
}


/*----------------------------------------------------------------------

  dumpSymbolKind()

*/
static void dumpSymbolKind(SymbolKind kind)
{
  switch (kind) {
  case CLASS_SYMBOL: put("CLASS"); break;
  case INSTANCE_SYMBOL: put("INSTANCE"); break;
  case VERB_SYMBOL: put("VERB"); break;
  case DIRECTION_SYMBOL: put("DIRECTION"); break;
  case PARAMETER_SYMBOL: put("PARAMETER"); break;
  case EVENT_SYMBOL: put("EVENT"); break;
  default: put("*** UNKNOWN ***"); break;
  }
}

/*----------------------------------------------------------------------

  dumpSymbol()

*/
static void dumpSymbol(Symbol *symbol)
{
  if (symbol == NULL) {
    put("NULL");
    return;
  }

  put("SYMBOL: "); dumpPointer(symbol); dumpSymbolKind(symbol->kind); in();
  put("string: "); dumpString(symbol->string);
  put(", code: "); dumpInt(symbol->code); nl();
  put("lower: "); dumpPointer(symbol->lower); put("higher: "); dumpPointer(symbol->higher); out();
}


/*----------------------------------------------------------------------

  dumpSymbolsRecursively()

*/
static void dumpSymbolsRecursively(Symbol *symbol)
{
  if (symbol == NULL) return;
  dumpSymbolsRecursively(symbol->lower);
  if (firstSymbolDumped) firstSymbolDumped = FALSE; else nl();
  dumpSymbol(symbol);
  dumpSymbolsRecursively(symbol->higher);
}


/*======================================================================

  dumpSymbols()

*/
void dumpSymbols(void)
{
  dumpPointer(symbolTree);
  in();
  dumpSymbolsRecursively(symbolTree);
  out();
}
