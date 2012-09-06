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
#include "elm_x.h"
#include "cnt_x.h"
#include "id_x.h"
#include "atr_x.h"
#include "exp_x.h"
#include "ext_x.h"
#include "lst_x.h"
#include "type_x.h"
#include "prop_x.h"
#include "dump_x.h"
#include "emit.h"


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
Symbol *nowhere;
Symbol *messageVerbSymbolForInstance;
Symbol *messageVerbSymbolFor2Instances;
Symbol *messageVerbSymbolForString;
Symbol *messageVerbSymbolFor2Strings;
Symbol *messageVerbSymbolFor2Integers;



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
void idRedefined(IdNode *id, Symbol *sym, Srcp previousDefinition)
{
  int error_code = 0;

  switch (sym->kind) {
  case DIRECTION_SYMBOL: error_code = 301; break;
  case VERB_SYMBOL: error_code = 303; break;
  case INSTANCE_SYMBOL: error_code = 304; break;
  case CLASS_SYMBOL: error_code = 305; break;
  case EVENT_SYMBOL: error_code = 307; break;
  default: error_code = 308; break;
  }

  lmLog(&id->srcp, error_code, sevERR, id->string);
  lmLog(&previousDefinition, 399, sevINF, id->string);
}



/*----------------------------------------------------------------------*/
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
    SYSERR("Adding local variable without an active frame");

  if (currentFrame->localSymbols == NULL)
    new->fields.local.number = 1;
  else
    new->fields.local.number = currentFrame->localSymbols->member.sym->fields.local.number + 1;

  new->fields.local.level = currentFrame->level;

  currentFrame->localSymbols = concat(currentFrame->localSymbols, new, SYMBOL_LIST);
}


/*----------------------------------------------------------------------*/
static void anotherSymbolKindAsString(SymbolKind kind, Bool found, char *string)
{
  if (found) strcat(string, " or ");
  switch(kind) {
  case CLASS_SYMBOL: strcat(string, "a Class"); break;
  case INSTANCE_SYMBOL:strcat(string, "an Instance"); break;
  case VERB_SYMBOL: strcat(string, "a Verb"); break;
  case EVENT_SYMBOL: strcat(string, "an Event"); break;
  case LOCAL_SYMBOL:
  case DIRECTION_SYMBOL:
  case PARAMETER_SYMBOL:
  case FUNCTION_SYMBOL:
  case ERROR_SYMBOL:
  case MAX_SYMBOL:
    SYSERR("Unimplemented case in '%s()'");
  }
}


/*----------------------------------------------------------------------*/
static char *symbolKindsAsString(SymbolKind kinds)
{
  Bool found = FALSE;
  char *string = allocate(100);
  int i;

  string[0] = '\0';
  for (i = 1; i <= MAX_SYMBOL; i = i<<1) {
    if (kinds&i) {
      anotherSymbolKindAsString(kinds&i, found, string);
      found = TRUE;
    }
  }
  return string;
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


/*----------------------------------------------------------------------*/
static Bool mayOverride(SymbolKind overridingKind, SymbolKind originalKind) {
  switch (overridingKind) {
  case LOCAL_SYMBOL:
    switch (originalKind) {
    case DIRECTION_SYMBOL:
    case VERB_SYMBOL:
      return TRUE;
    default:
      return FALSE;
    }
  default:
    return FALSE;
  }
}


/*======================================================================*/
Symbol *newSymbol(IdNode *id, SymbolKind kind)
{
  Symbol *new;                  /* The newly created symnod */
  
  if (id == NULL)
    return NULL;
  
  new = lookup(id->string);
  if (new != NULL && !mayOverride(kind, new->kind))
    idRedefined(id, new, new->srcp);

  new = NEW(Symbol);
  
  new->kind = kind;
  new->string = id->string;
  new->srcp = id->srcp;

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
  case ERROR_SYMBOL:
    break;
  default:
    SYSERR("Unexpected switch on SYMBOLKIND");
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
Symbol *newVerbSymbol(IdNode *id) {
  Symbol *new = newSymbol(id, VERB_SYMBOL);
  new->fields.verb.parameterSymbols = NULL;
  new->fields.verb.firstSyntax = NULL;
  return new;
}

/*----------------------------------------------------------------------*/
static void setParameterClass(Symbol *s, int parameter, Symbol *class) {
  List *pl = s->fields.verb.parameterSymbols;
  int p;

  for (p = 1; p < parameter; p++)
    pl = pl->next;

  pl->member.sym->fields.parameter.class = class;
  if (class == stringSymbol)
    pl->member.sym->fields.parameter.type = STRING_TYPE;
  else if (class == integerSymbol)
    pl->member.sym->fields.parameter.type = INTEGER_TYPE;
  else
    pl->member.sym->fields.parameter.type = INSTANCE_TYPE;
}


/*----------------------------------------------------------------------*/
static Symbol *createMessageVerb(int parameterCount, Symbol *typeSymbol) {
  Symbol *symbol;
  char name[50];
  int p;
  IdNode *id;
  List *parameterList = NULL;

  sprintf(name, "$message%d%s$", parameterCount, typeSymbol->string);
  symbol = newVerbSymbol(newId(nulsrcp, name));

  for (p = 1; p <= parameterCount; p++) {
    sprintf(name, "parameter%d", p);
    id = newId(nulsrcp, name);
    parameterList = concat(parameterList,
			 newParameterElement(nulsrcp, id, 0),
			 ELEMENT_LIST);
    id->code = p;
  }
  
  setParameters(symbol, parameterList);

  for (p = 1; p <= parameterCount; p++)
    setParameterClass(symbol, p, typeSymbol);
  return(symbol);
}



/*======================================================================*/
void createMessageVerbs() {
  messageVerbSymbolForInstance = createMessageVerb(1, entitySymbol);
  messageVerbSymbolForString = createMessageVerb(1, stringSymbol);
  messageVerbSymbolFor2Integers = createMessageVerb(2, integerSymbol);
  messageVerbSymbolFor2Strings = createMessageVerb(2, stringSymbol);
  messageVerbSymbolFor2Instances = createMessageVerb(2, entitySymbol);
}


/*======================================================================*/
void initSymbols()
{
  symbolTree = NULL;
  instanceCount = 0;
  classCount = 0;
  attributeCount = PREDEFINEDATTRIBUTES; /* Set number of attributes
					    predefined so counting
					    starts at next number */
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
    if (compareStrings(idString, l->member.sym->fields.parameter.element->id->string) == 0)
      return l->member.sym;
  return NULL;
}


/*======================================================================*/
Symbol *lookupParameter(IdNode *parameterId, List *parameterSymbols)
{
  List *p;

  for (p = parameterSymbols; p != NULL; p = p->next) {
    if (p->member.sym->kind == PARAMETER_SYMBOL)
      if (equalId(parameterId, p->member.sym->fields.parameter.element->id))
	return p->member.sym;
  }
  return NULL;
}


/*======================================================================*/
Symbol *lookup(char *idString)
{
  Symbol *s1,*s2;               /* Traversal pointers */
  int comp;                     /* Result of comparison */

  if (idString == NULL) SYSERR("NULL string");

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
      if (compareStrings(idString, localSymbolList->member.sym->string) == 0)
	return localSymbolList->member.sym;
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
    case START_CONTEXT:
    case CLASS_CONTEXT:
    case INSTANCE_CONTEXT:
      foundSymbol = lookup(idString);
      break;
    default:
      SYSERR("Unexpected context kind");
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
    case LOCAL_SYMBOL:
      theSymbol = theSymbol->fields.local.class;
      scripts = theSymbol->fields.entity.props->scripts;
      break;
    default:
      SYSERR("Unexpected symbol kind");
      return NULL;
    }
    while (scripts != NULL) {
      if (equalId(scriptName, scripts->member.script->id))
	return scripts->member.script;
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
    case LOCAL_SYMBOL:
      return symbolIsContainer(symbol->fields.local.class);
    default:
      SYSERR("Unexpected Symbol kind");
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
    case ERROR_SYMBOL:
      break;
    default:
      SYSERR("Unexpected Symbol kind");	
    }
  }
  return NULL;
}



/*======================================================================*/
void setParent(Symbol *child, Symbol *parent)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    SYSERR("Not a CLASS or INSTANCE");
  child->fields.entity.parent = parent;
}


/*======================================================================*/
Symbol *parentOf(Symbol *child)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    SYSERR("Not a CLASS or INSTANCE");
  return child->fields.entity.parent;
}


/*======================================================================*/
Bool inheritsFrom(Symbol *child, Symbol *ancestor)
{
  Symbol *p;

  if (child == NULL || ancestor == NULL) return FALSE;

  if (ancestor->kind == INSTANCE_SYMBOL)
    SYSERR("Can not inherit from an instance");

  if (child->kind == ERROR_SYMBOL || ancestor->kind == ERROR_SYMBOL)
    return TRUE;

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


/*----------------------------------------------------------------------*/
static Bool multipleSymbolKinds(SymbolKind kind) {
  int i;
  Bool found = FALSE;

  for (i = 1; i < MAX_SYMBOL; i=i<<1)
    if (kind&i) {
      if (found)
	return TRUE;
      else
	found = TRUE;
    }
  return FALSE;
}


/*----------------------------------------------------------------------*/
static Symbol *lookupClass(IdNode *id, Symbol *symbol) {
  if (symbol != NULL && symbol->kind != CLASS_SYMBOL) {
    Symbol *otherSymbol = lookup(id->string);
    if (otherSymbol != NULL)
      return otherSymbol;
  }
  return symbol;
}


/*----------------------------------------------------------------------*/
static List *getParameterSymbols(Context *context)
{
	if (context->kind == VERB_CONTEXT)
		return context->verb->fields.verb.parameterSymbols;
	else
		return NULL;
}


/*----------------------------------------------------------------------*/
static char *identifierListForParameters(Context *context) {
	List *parameters = getParameterSymbols(context);
	char *identifiers = (char *)allocate(200);
	List *list;
	Bool first = TRUE;

	if (parameters == NULL)
		SYSERR("NULL parameters");

	TRAVERSE(list, parameters) {
		if (!first) {
			if (list->next != NULL)
				strcat(identifiers, "', '");
			else
				strcat(identifiers, "' and '");
		} else
			strcat(identifiers, "'");
		strcat(identifiers, list->member.sym->string);
		first = FALSE;
	}
	strcat(identifiers, "'");
	return identifiers;
}


/*======================================================================*/
void setParameters(Symbol *verb, List *parameters)
{
  /* Parameters are sent as a list of Elments. Set it in the verb symbol. */
  List *parameterSymbols = NULL;
  List *param;

  if (verb == NULL) return;

  if (verb->kind != VERB_SYMBOL) {
    /* Probably a syntactic error! */
    return;
  }

  if (parameters == NULL) return;

  if (parameters->kind != ELEMENT_LIST)
    SYSERR("Not a parameter list");

  TRAVERSE(param, parameters) {
    Symbol *parameterSymbol = newParameterSymbol(param->member.elm->id->string, param->member.elm);
    parameterSymbols = concat(parameterSymbols, parameterSymbol, SYMBOL_LIST);
  }

  verb->fields.verb.parameterSymbols = parameterSymbols;
}


/*======================================================================*/
char *verbHasParametersMessage(Context *context) {
	static char message[2000];
	message[0] = '\0';
	if (context && context->kind == VERB_CONTEXT) {
		List *parameterSymbols = getParameterSymbols(context);
		if (length(parameterSymbols) > 0)
			sprintf(message, " The verb '%s' has the parameter%s %s.",
					context->verb->string, length(parameterSymbols)>1?"s":"",
					identifierListForParameters(context));
	}
	return message;
}


/*======================================================================*/
char *verbHasParametersOrNoneMessage(Context *context) {
	char *message = verbHasParametersMessage(context);
	if (strlen(message) > 0)
		return message;
	else {
		static char noParametersMessage[2000];
		sprintf(noParametersMessage, " The verb '%s' has no parameters.", context->verb->string);
		return noParametersMessage;
	}
}


/*======================================================================*/
Symbol *symcheck(IdNode *id, SymbolKind requestedKinds, Context *context)
{
	Symbol *sym;

	sym = lookupInContext(id->string, context);
	if (requestedKinds == CLASS_SYMBOL)
		sym = lookupClass(id, sym);

	if (!sym) {
		if (!isGeneratedId(id)) {
			lmLogv(&id->srcp, 310, sevERR, id->string, verbHasParametersMessage(context), NULL);
		}
	} else if (sym->kind == PARAMETER_SYMBOL || sym->kind == LOCAL_SYMBOL) {
		if ((requestedKinds&INSTANCE_SYMBOL) == 0) {
			if (multipleSymbolKinds(requestedKinds))
				lmLogv(&id->srcp, 319, sevERR, id->string, "of correct type for this context", NULL);
			else
				lmLogv(&id->srcp, 319, sevERR, id->string, symbolKindsAsString(requestedKinds), NULL);
			return NULL;
		}
	} else
		if (requestedKinds != 0)
			if (sym->kind != ERROR_SYMBOL && (sym->kind&requestedKinds) == 0) {
				if (multipleSymbolKinds(requestedKinds))
					lmLogv(&id->srcp, 319, sevERR, id->string, "of correct type for this context", NULL);
				else
					lmLogv(&id->srcp, 319, sevERR, id->string, symbolKindsAsString(requestedKinds), NULL);
				return NULL;
			}
	id->symbol = sym;
	return sym;
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
void instanceCheck(IdNode *id, char reference[], char className[])
{
  /* Check that the given identifier inherits the class passed as a string.
     This will only be used for built in class checks (location, actor etc.)
  */

  Symbol *theClassSymbol = lookup(className);

  if (theClassSymbol == NULL) SYSERR("There is no such class");

  if (id->symbol != NULL)
    if (id->symbol->kind != ERROR_SYMBOL)
      if (id->symbol->kind != INSTANCE_SYMBOL || !inheritsFrom(id->symbol, theClassSymbol))
	lmLogv(&id->srcp, 351, sevERR, reference, "an instance", className, NULL);
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
    Attribute *thisAttribute = theList->member.atr;
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
  List *atr;

  propertiesOf(symbol)->attributes =
    combineAttributes(propertiesOf(symbol)->attributes,
		      propertiesOfParentOf(symbol)->attributes);

  /* Verify that there are no inherited, non-initialized, attributes */
  TRAVERSE(atr, propertiesOf(symbol)->attributes) {
    Attribute *thisAttribute = atr->member.atr;
    if (thisAttribute->type == REFERENCE_TYPE)
      if (!thisAttribute->initialized
	  && symbol->kind != CLASS_SYMBOL)
	lmLogv(&symbol->fields.entity.props->id->srcp, 328, sevERR,
	       thisAttribute->id->string,
	       thisAttribute->definingSymbol->string,
	       NULL);
  }
}


/*----------------------------------------------------------------------*/
static void replicateContainer(Symbol *symbol)
{
  /* A container node can be generated once, we only have to keep
     container code and owner property pointer local so the global
     part can just be pointed to. */

  if (propertiesOf(symbol)->container == NULL && propertiesOfParentOf(symbol)->container != NULL) {
    Properties *props = propertiesOf(symbol);
    /* Create a new Container Instance and link parents Container Body */
    props->container = newContainer(propertiesOfParentOf(symbol)->container->body);
    props->container->ownerProperties = props;

    /* Add OPAQUE attribute */
    addOpaqueAttribute(props, props->container->body->opaque);
  }
}

/*----------------------------------------------------------------------*/
static void replicateExits(Symbol *symbol)
{
  propertiesOf(symbol)->exits = combineExits(propertiesOf(symbol)->exits,
					     propertiesOfParentOf(symbol)->exits);
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
  /* Initialize is handled by interpreter */
  /* And so is Description Check and Description */
  /* ... and the Articles/Forms */
  replicateContainer(symbol);
  /* ... and the Verbs */
  replicateScripts(symbol);
  /* ... and Entered */
  replicateExits(symbol);
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


/*======================================================================*/
void generateSymbol(Symbol *symbol) {
    emitConstant(symbol->code);
}

/*----------------------------------------------------------------------*/
static void dumpSymbolKind(SymbolKind kind)
{
  switch (kind) {
  case CLASS_SYMBOL: put("CLASS"); break;
  case INSTANCE_SYMBOL: put("INSTANCE"); break;
  case VERB_SYMBOL: put("VERB"); break;
  case DIRECTION_SYMBOL: put("DIRECTION"); break;
  case PARAMETER_SYMBOL: put("PARAMETER"); break;
  case EVENT_SYMBOL: put("EVENT"); break;
  case LOCAL_SYMBOL: put("LOCAL"); break;
  default: put("*** UNKNOWN ***"); break;
  }
}

/*----------------------------------------------------------------------*/
static void dumpSymbolLeaf(Symbol *symbol)
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


/*----------------------------------------------------------------------*/
static void dumpSymbolsRecursively(Symbol *symbol)
{
  if (symbol == NULL) return;
  dumpSymbolsRecursively(symbol->lower);
  if (firstSymbolDumped) firstSymbolDumped = FALSE; else nl();
  dumpSymbolLeaf(symbol);
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


/*======================================================================*/
void dumpSymbol(Symbol *symbol)
{
  if (symbol == NULL) {
    put("NULL");
    return;
  }

  dumpPointer(symbol); dumpSymbolKind(symbol->kind); put(" ");
  dumpString(symbol->string); 
  put(":"); dumpInt(symbol->code);
}
