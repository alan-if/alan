/*----------------------------------------------------------------------*\

                                SYM.C
                          Symbol Table Nodes

\*----------------------------------------------------------------------*/

#include "sym_x.h"

/* IMPORTS */
#include "sysdep.h"
#include "util.h"
#include "lmList.h"

#include "srcp_x.h"
#include "cla_x.h"
#include "cnt_x.h"
#include "id_x.h"
#include "atr_x.h"
#include "exp_x.h"
#include "lst_x.h"
#include "type_x.h"
#include "dump_x.h"


/* EXPORTS: */
int frameLevel = 0;

int classCount = 0;
int instanceCount = 0;
int directionCount = 0;
int attributeCount;
int verbCount = 0;
int eventCount = 0;

Symbol *entitySymbol;
Symbol *thingSymbol;
Symbol *objectSymbol;
Symbol *locationSymbol;
Symbol *actorSymbol;
Symbol *literalSymbol;
Symbol *stringSymbol;
Symbol *integerSymbol;
Symbol *theHero;



/* PRIVATE: */
static Symbol *symbolTree = NULL;
static Bool firstSymbolDumped = TRUE;

typedef struct Frame {
  /* A frame defines a local scope with local variables.
     To find a variable you need to do linear search in the list.
     Since frames may be nested you should recurse until outerFrame == NULL */
  int level;
  List *localSymbols;
  struct Frame *outerFrame;
} Frame;

static Frame *currentFrame = NULL;


/*======================================================================*/
void redefined(IdNode *id, Symbol *sym)
{
  int code = 0;                     /* Error code */

  switch (sym->kind) {
  case DIRECTION_SYMBOL: code = 301; break;
  case VERB_SYMBOL: code = 303; break;
  case INSTANCE_SYMBOL: code = 304; break;
  case CLASS_SYMBOL: code = 305; break;
  case EVENT_SYMBOL: code = 307; break;
  default: syserr("Unrecognized switch in '%s()'", __FUNCTION__);
  }

  lmLog(&id->srcp, code, sevERR, id->string);
}



/*----------------------------------------------------------------------------*/
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
    comp = compareStrings(symbol->string, s1->string);
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


/*----------------------------------------------------------------------*/
static void addLocal(Symbol *new)
{
  if (currentFrame == NULL)
    syserr("Adding local variable without an active frame", NULL);

  if (currentFrame->localSymbols == NULL)
    new->fields.local.number = 1;
  else
    new->fields.local.number = currentFrame->localSymbols->element.sym->fields.local.number + 1;

  new->fields.local.level = currentFrame->level;

  currentFrame->localSymbols = concat(currentFrame->localSymbols, new, SYMBOL_LIST);
}


/*----------------------------------------------------------------------*/
static char *symbolKindAsString(SymbolKind kind)
{
  switch (kind) {
  case CLASS_SYMBOL: return "a class";
  case INSTANCE_SYMBOL: return "an instance";
  case VERB_SYMBOL: return "a verb";
  default: syserr("Unimplemented case in '%s()'", __FUNCTION__);
  }
  return NULL;
}



/*----------------------------------------------------------------------*/
static Symbol *newParameterSymbol(char *string, Element *element)
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


/*======================================================================*/
Symbol *newSymbol(IdNode *id,		/* IN - Name of the new symbol */
		  SymbolKind kind) 	/* IN - What kind of symbol */
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

  if (kind == LOCAL_SYMBOL)
    addLocal(new);
  else
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
  case LOCAL_SYMBOL:
    break;
  default: syserr("Unexpected switch on SYMBOLKIND in '%s()'", __FUNCTION__);
  }

  return new;
}

/*======================================================================*/
Symbol *newInstanceSymbol(IdNode *id, Properties *props, Symbol *parent) {
  Symbol *new = newSymbol(id, INSTANCE_SYMBOL);
  new->fields.entity.props = props;
  new->fields.entity.parent = parent;
  return new;
}

/*======================================================================*/
Symbol *newClassSymbol(IdNode *id, Properties *props, Symbol *parent) {
  Symbol *new = newSymbol(id, CLASS_SYMBOL);
  new->fields.entity.props = props;
  new->fields.entity.parent = parent;
  return new;
}

/*======================================================================*/
void initSymbols()
{
  symbolTree = NULL;
  instanceCount = 0;
  classCount = 0;
  attributeCount = PREDEFINEDATTRIBUTES;
}


/*======================================================================*/
void newFrame(void)
{
  Frame *theNew = NEW(Frame);

  theNew->localSymbols = NULL;
  if (currentFrame == NULL)
    theNew->level = 1;
  else
    theNew->level = currentFrame->level + 1;

  theNew->outerFrame = currentFrame;
  currentFrame = theNew;
}

/*======================================================================*/
void deleteFrame(void)
{
  Frame *outerFrame = currentFrame->outerFrame;
  List *locals, *next;

  for (locals = currentFrame->localSymbols; locals != NULL; locals = next) {
    next = locals->next;
    free(locals);
  }
  
  free(currentFrame);
  currentFrame = outerFrame;
}


/*----------------------------------------------------------------------*/
static Symbol *lookupInParameterList(char *idString, List *parameterSymbols)
{
  List *l;

  for (l = parameterSymbols; l != NULL; l = l->next)
    if (compareStrings(idString, l->element.sym->fields.parameter.element->id->string) == 0)
      return l->element.sym;
  return NULL;
}


/*======================================================================*/
Symbol *lookupParameter(IdNode *parameterId, List *parameterSymbols)
{
  List *p;

  for (p = parameterSymbols; p != NULL; p = p->next) {
    if (p->element.sym->kind == PARAMETER_SYMBOL)
      if (equalId(parameterId, p->element.sym->fields.parameter.element->id))
	return p->element.sym;
  }
  return NULL;
}


/*======================================================================*/
Symbol *lookup(char *idString)
{
  Symbol *s1,*s2;               /* Traversal pointers */
  int comp;                     /* Result of comparison */

  if (idString == NULL) syserr("NULL string in '%s()'", __FUNCTION__);

  s1 = symbolTree;
  s2 = NULL;

  while (s1 != NULL) {
    s2 = s1;
    comp = compareStrings(idString, s1->string);
    if (comp == 0)
      return(s1);
    else if (comp < 0)
      s1 = s1->lower;
    else
      s1 = s1->higher;
  }

  return(NULL);
}


/*----------------------------------------------------------------------*/
static Symbol *lookupInFrames(char *idString)
{
  Frame *thisFrame = currentFrame;
  List *localSymbolList;

  while (thisFrame != NULL) {
    for (localSymbolList = thisFrame->localSymbols; localSymbolList != NULL; localSymbolList = localSymbolList->next) {
      if (compareStrings(idString, localSymbolList->element.sym->string) == 0)
	return localSymbolList->element.sym;
    }
    thisFrame = thisFrame->outerFrame;
  }
  return NULL;
}


/*----------------------------------------------------------------------*/
static Symbol *lookupInContext(char *idString, Context *context)
{
  Symbol *foundSymbol = NULL;

  if ((foundSymbol = lookupInFrames(idString)) != NULL)
    return foundSymbol;

  if (context != NULL) {
    switch (context->kind){
    case VERB_CONTEXT:
      if (context->verb != NULL)
	foundSymbol = lookupInParameterList(idString, context->verb->fields.verb.parameterSymbols);
      break;
    case EVENT_CONTEXT:
    case RULE_CONTEXT:
    case CLASS_CONTEXT:
    case INSTANCE_CONTEXT:
      foundSymbol = lookup(idString);
      break;
    default:
      syserr("Unexpected context kind in '%s()'", __FUNCTION__);
      break;
    }
    if (foundSymbol != NULL)
      return foundSymbol;
  }

  return lookup(idString);
}


/*======================================================================*/
Script *lookupScript(Symbol *theSymbol, IdNode *scriptName)
{
  List *scripts;

  while (theSymbol != NULL) {
    switch (theSymbol->kind) {
    case INSTANCE_SYMBOL:
    case CLASS_SYMBOL:
      scripts = theSymbol->fields.entity.props->scripts;
      break;
    case PARAMETER_SYMBOL:
      theSymbol = theSymbol->fields.parameter.class;
      scripts = theSymbol->fields.entity.props->scripts;
      break;
    default:
      syserr("Unexpected symbol kind in '%s()'", __FUNCTION__);
    }
    while (scripts != NULL) {
      if (equalId(scriptName, scripts->element.script->id))
	return scripts->element.script;
      scripts = scripts->next;
    }
    theSymbol = parentOf(theSymbol);
  }

  return NULL;
}


/*======================================================================*/
Bool isClass(Symbol *symbol) {
  return symbol->kind == CLASS_SYMBOL;
}


/*======================================================================*/
TypeKind classToType(Symbol* symbol) {
  if (symbol->kind != CLASS_SYMBOL)
    SYSERR("Not a class");
  if (symbol == integerSymbol) return INTEGER_TYPE;
  else if (symbol == stringSymbol) return STRING_TYPE;
  else return INSTANCE_TYPE;
}


/*======================================================================*/
Bool symbolIsContainer(Symbol *symbol) {
  if (symbol != NULL) {
    switch (symbol->kind) {
    case PARAMETER_SYMBOL:
      return symbol->fields.parameter.restrictedToContainer
	|| symbolIsContainer(symbol->fields.parameter.class);
    case CLASS_SYMBOL:
    case INSTANCE_SYMBOL:
      return symbol->fields.entity.props->container != NULL
	|| symbolIsContainer(symbol->fields.entity.parent);
    default:
      syserr("Unexpected Symbol kind in '%s()'", __FUNCTION__);
    }
  }
  return FALSE;
}


/*======================================================================*/
Symbol *contentOfSymbol(Symbol *symbol) {
  Properties *props;
  if (symbol != NULL) {
    switch (symbol->kind) {
    case INSTANCE_SYMBOL:
    case CLASS_SYMBOL:
      props = symbol->fields.entity.props;
      if (props != NULL) {
	if (props->container != NULL)
	  return props->container->body->taking->symbol;
	else
	  return contentOfSymbol(symbol->fields.entity.parent);
      }
      break;
    case PARAMETER_SYMBOL:
      return contentOfSymbol(symbol->fields.parameter.class);
      break;
    default:
      syserr("Unexpected Symbol kind in '%s()'", __FUNCTION__);	
    }
  }
  return NULL;
}



/*======================================================================*/
void setParent(Symbol *child, Symbol *parent)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    syserr("Not a CLASS or INSTANCE in '%s()'", __FUNCTION__);
  child->fields.entity.parent = parent;
}


/*======================================================================*/
Symbol *parentOf(Symbol *child)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    syserr("Not a CLASS or INSTANCE in '%s()'", __FUNCTION__);
  return child->fields.entity.parent;
}


/*======================================================================*/
Bool inheritsFrom(Symbol *child, Symbol *ancestor)
{
  Symbol *p;

  if (child == NULL || ancestor == NULL) return FALSE;

  if (ancestor->kind == INSTANCE_SYMBOL)
    SYSERR("Can not inherit from an instance");

  if (child->kind == PARAMETER_SYMBOL)
    child = child->fields.parameter.class;

  if ((child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL) ||
      (ancestor->kind != CLASS_SYMBOL))
    return FALSE;		/* Probably spurious */

  p = child;			/* To be the class itself is OK */
  while (p && p != ancestor)
    p = p->fields.entity.parent;

  return (p != NULL);
}


/*======================================================================*/
Symbol *symcheck(IdNode *id, SymbolKind kind, Context *context)
{
  Symbol *sym = lookupInContext(id->string, context);

  if (!sym) {
    if (id->string[0] != '$')
      /* Error generated id, we don't want to report on those */
      lmLog(&id->srcp, 310, sevERR, id->string);
  } else if (sym->kind == PARAMETER_SYMBOL) {
    if (sym->fields.parameter.element->kind != ID_RESTRICTION)
      lmLogv(&id->srcp, 319, sevERR, id->string,
	     "a parameter that is restricted to instances of a class", NULL);
  } else if (sym->kind == LOCAL_SYMBOL) {
    ;
  } else if (sym->kind != kind) {
    lmLogv(&id->srcp, 319, sevERR, id->string, symbolKindAsString(kind), NULL);
    return NULL;
  }

  id->symbol = sym;
  return sym;
}




/*======================================================================*/
void setParameters(Symbol *verb, List *parameters)
{
  /* Parameters are sent as a list of ElmNodes. Set it in the verb symbol. */
  List *parameterSymbols = NULL;
  List *param;

  if (verb == NULL) return;

  if (verb->kind != VERB_SYMBOL) {
    /* Probably a syntactic error! */
    verb->fields.verb.parameterSymbols = NULL;
    return;
    /*    syserr("Not a verb in setParameters()"); */
  }

  if (parameters == NULL) return;

  if (parameters->kind != ELEMENT_LIST)
    syserr("Not a parameter list in '%s()'", __FUNCTION__);

  TRAVERSE(param, parameters) {
    Symbol *parameterSymbol = newParameterSymbol(param->element.elm->id->string, param->element.elm);
    parameterSymbols = concat(parameterSymbols, parameterSymbol, SYMBOL_LIST);
    param->element.elm->id->symbol = parameterSymbol; /* Cross refer */
    /* Set initial type of parameters to entity */
    parameterSymbol->fields.parameter.class = entitySymbol;
    parameterSymbol->fields.parameter.type = INSTANCE_TYPE;
  }

  verb->fields.verb.parameterSymbols = parameterSymbols;
}

/*======================================================================*/
void inheritCheck(IdNode *id, char reference[], char toWhat[], char className[])
{
  /* Check that the given identifier inherits the class passed as a string.
     This will only be used for built in class checks (location, actor etc.)
  */

  Symbol *theClassSymbol = lookup(className);

  if (theClassSymbol == NULL) SYSERR("There is no such class");

  if (id->symbol != NULL && !inheritsFrom(id->symbol, theClassSymbol))
    lmLogv(&id->srcp, 351, sevERR, reference, toWhat, className, NULL);
}


/*======================================================================*/
Symbol *definingSymbolOfAttribute(Symbol *symbol, IdNode *id)
{
  /* Find the symbol which defines an attribute by traversing its parents. */

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



/*======================================================================*/
Attribute *findInheritedAttribute(Symbol *symbol, IdNode *id)
{
  /* From a symbol traverse its inheritance tree to find a named attribute. */
  Symbol *definingSymbol =
    definingSymbolOfAttribute(symbol->fields.entity.parent, id);

  if (definingSymbol == NULL) return NULL;

  return findAttribute(definingSymbol->fields.entity.props->attributes, id);
}


/*----------------------------------------------------------------------*/
static void numberAttributes(Symbol *symbol)
{
  List *theList;
  Attribute *inheritedAttribute;

  if (symbol->fields.entity.attributesNumbered) return;

  for (theList = symbol->fields.entity.props->attributes; theList != NULL;
       theList = theList->next){
    Attribute *thisAttribute = theList->element.atr;
    inheritedAttribute = findInheritedAttribute(symbol, thisAttribute->id);
    if (inheritedAttribute != NULL) {
      thisAttribute->id->code = inheritedAttribute->id->code;
      thisAttribute->inheritance = INHERITED_REDEFINED;
    } else if (thisAttribute->id->code == 0) {
      thisAttribute->id->code = ++attributeCount;
      thisAttribute->inheritance = LOCAL;
    } /* Else its a pre-defined attribute which is numbered already! */
  }

  symbol->fields.entity.props->attributes = sortAttributes(symbol->fields.entity.props->attributes);
  symbol->fields.entity.attributesNumbered = TRUE;
}


/*----------------------------------------------------------------------*/
static void numberParentAttributes(Symbol *symbol)
{
  /* Recurse the parental chain and number the attributes. */
  if (symbol == NULL || symbol->fields.entity.attributesNumbered) return;

  numberParentAttributes(symbol->fields.entity.parent);
  numberAttributes(symbol);
}


/*----------------------------------------------------------------------*/
static void numberAttributesRecursively(Symbol *symbol)
{
  /* Recurse the parent to number its attributes.
     Number all attributes in the symbol (if it is a class or an instance);
  */

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



/*======================================================================*/
void numberAllAttributes(void)
{
  /* Traverse all classes and instances in the symbol table and give all
     attributes unique numbers. Start by recursing through the
     parents. Remember where we have been by looking at the code which
     might already have been set.
  */
  numberAttributesRecursively(symbolTree);
}


static Bool haveParent(Symbol *s) {return s->fields.entity.parent != NULL;}
static Properties *propertiesOf(Symbol *s) {return s->fields.entity.props;}
static Properties *propertiesOfParentOf(Symbol *s) {return s->fields.entity.parent->fields.entity.props;}


/*----------------------------------------------------------------------*/
static void replicateNames(Symbol *symbol)
{
  if (propertiesOf(symbol)->names == NULL)
    propertiesOf(symbol)->names = propertiesOfParentOf(symbol)->names;
  else if (propertiesOfParentOf(symbol)->names != NULL)
    propertiesOf(symbol)->names = combine(propertiesOf(symbol)->names,
					  propertiesOfParentOf(symbol)->names);
}


/*----------------------------------------------------------------------*/
static void replicatePronouns(Symbol *symbol)
{
  if (propertiesOf(symbol)->pronouns == NULL)
    propertiesOf(symbol)->pronouns = propertiesOfParentOf(symbol)->pronouns;
}


/*----------------------------------------------------------------------*/
static void replicateAttributes(Symbol *symbol)
{
  propertiesOf(symbol)->attributes =
    combineAttributes(propertiesOf(symbol)->attributes,
		      propertiesOfParentOf(symbol)->attributes);
}


/*----------------------------------------------------------------------*/
static void replicateContainer(Symbol *symbol)
{
  /* A container node can be generated once, we only have to keep
     container code and owner property pointer local so the global
     part can just be pointed to. */

  if (propertiesOf(symbol)->container == NULL && propertiesOfParentOf(symbol)->container != NULL) {
    Properties *props = propertiesOf(symbol);
#define OPTIMIZE_CONTAINER_BODY_GENERATION
#ifdef OPTIMIZE_CONTAINER_BODY_GENERATION
    /* Create a new Container Instance and link parents Container */
    props->container = newContainer(propertiesOfParentOf(symbol)->container->body);
#else
    /* Create a new Container Instance and copy parents Container Body */
    Properties *parentProps = propertiesOfParentOf(symbol);
    ContainerBody *body = newContainerBody(&parentProps->container->body->srcp,
					   parentProps->container->body->limits,
					   parentProps->container->body->hstms,
					   parentProps->container->body->estms,
					   parentProps->container->body->extractChecks,
					   parentProps->container->body->extractStatements);
    props->container = newContainer(body);
#endif
    props->container->ownerProperties = props;
  }
}


/*----------------------------------------------------------------------*/
static void replicateScripts(Symbol *symbol)
{
  /* The parent may and may not have scripts. Any of those should be
  accessible from the current instance, however if it is overridden it
  should use the local version. During run-time we simply lookup
  scripts using the parental chain. This means that an instance only
  have the local scripts in its list. So there is nothing to do
  here. */
}



/*----------------------------------------------------------------------*/
static void replicateInitialLocation(Symbol *symbol)
{
  if (symbol->fields.entity.props->whr == NULL)
    symbol->fields.entity.props->whr = propertiesOfParentOf(symbol)->whr;
}


/*----------------------------------------------------------------------*/
static void replicate(Symbol *symbol)
{
  replicateInitialLocation(symbol);
  replicateNames(symbol);
  replicatePronouns(symbol);
  replicateAttributes(symbol);
  replicateContainer(symbol);
  replicateScripts(symbol);
}


/*----------------------------------------------------------------------

  replicateParent()

  Recurse the parental chain and replicate any inherited things that
  requires local replicated data.

*/
static void replicateParent(Symbol *symbol)
{
  if (symbol == NULL) return;

  if (symbol->fields.entity.replicated) {
    return;
  }

  if (haveParent(symbol)) {
    replicateParent(symbol->fields.entity.parent);
    replicate(symbol);
  }
  symbol->fields.entity.replicated = TRUE;
}


/*----------------------------------------------------------------------*/
static void replicateSymbolTree(Symbol *symbol)
{
  if (symbol == NULL) return;

  if (symbol->kind == CLASS_SYMBOL || symbol->kind == INSTANCE_SYMBOL) {
      replicateParent(symbol);
  }

  /* Recurse in the symbolTree */
  if (symbol->lower != NULL) replicateSymbolTree(symbol->lower);
  if (symbol->higher != NULL) replicateSymbolTree(symbol->higher);
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
  replicateSymbolTree(symbolTree);
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

  put("SYMBOL: "); dumpPointer(symbol); dumpSymbolKind(symbol->kind); indent();
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


/*======================================================================*/
void dumpSymbols(void)
{
  dumpPointer(symbolTree);
  indent();
  dumpSymbolsRecursively(symbolTree);
  out();
}
