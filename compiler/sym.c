/*----------------------------------------------------------------------*\

                                SYM.C
                          Symbol Table Nodes

\*----------------------------------------------------------------------*/

#include "sym_x.h"                /* SYMbol nodes */

/* IMPORTS */
#include "sysdep.h"
#include "util.h"
#include "lmList.h"

#include "srcp_x.h"
#include "cla_x.h"
#include "id_x.h"
#include "atr_x.h"
#include "exp_x.h"


int classCount = 0;
int instanceCount = 0;
int directionCount = 0;
int attributeCount = 0;
int verbCount = 0;


static SymNod *symTree = NULL;


/*======================================================================

  redefined()

  Calls lmLog() with correct code according to the symnod sent.

  */
void redefined(Srcp *srcp,      /* IN - Source position */
               SymNod *sym,     /* IN - The previous definition */
               char *str)       /* IN - The symbol name */
{
#ifndef FIXME
  syserr("UNIMPL: redefined() - old symbol kinds");
#else
  int code;                     /* Error code */

  switch (sym->class) {
  case NAMDIR: code = 301; break;
  case NAMLOC: code = 302; break;
  case NAMVRB: code = 303; break;
  case NAMOBJ: code = 304; break;
  case NAMCNT: code = 305; break;
  case NAMACT: code = 306; break;
  case NAMEVT: code = 307; break;
  default: syserr("Unrecognized switch in redefined()"); break;
  }

  lmLog(srcp, code, sevERR, str);
#endif
}



/*----------------------------------------------------------------------------

  insertSymbol()

  Insert a new symbol in the symbol tree

*/
static void insertSymbol(SymNod *symbol)
{
  SymNod *s1,*s2;               /* Traversal pointers */
  int comp;                     /* Result of comparison */

  symbol->lower = NULL;
  symbol->higher = NULL;

  s1 = symTree;
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
    symTree = symbol;
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
  case CLASS_SYMBOL: return "class";
  case INSTANCE_SYMBOL: return "instance";
  case VERB_SYMBOL: return "verb";
  default: syserr("Unimplemented case in symbolKind()");
  }
  return NULL;
}



/*======================================================================

  newSymbol()

  Creates a new symnod and links it in the symTree.

  */
SymNod *newSymbol(char *string,	/* IN - Name of the new symbol */
		  SymbolKind kind) /* IN - What kind of symbol */
{
  SymNod *new;                  /* The newly created symnod */
  
  if (string == NULL)
    return (0);
  
  new = NEW(SymNod);
  
  new->kind = kind;
  new->string = string;

  insertSymbol(new);

  switch (kind) {
  case CLASS_SYMBOL:
    new->code = ++classCount;
    new->fields.claOrIns.parent = NULL;
    new->fields.claOrIns.attributesAlreadyNumbered = FALSE;
    new->fields.claOrIns.attributesAlreadyReplicated = FALSE;
    break;
  case INSTANCE_SYMBOL:
    new->code = ++instanceCount;
    new->fields.claOrIns.parent = NULL;
    new->fields.claOrIns.attributesAlreadyNumbered = FALSE;
    new->fields.claOrIns.attributesAlreadyReplicated = FALSE;
    break;
  case DIRECTION_SYMBOL:
    new->code = ++directionCount;
    break;
  case VERB_SYMBOL:
    new->code = ++verbCount;
    break;
  default: syserr("Unexpected switch on SYMBOLKIND in newsym()"); break;
  }

  return new;
}



/*======================================================================

  initSymbols()

  Initialise the symbol table;

  */
void initSymbols()
{
  symTree = NULL;
  instanceCount = 0;
  classCount = 0;
  attributeCount = 0;
}


/*======================================================================

  lookup()

  Look for a symbol. If found return a pointer to its symnod, else NULL.

  */
SymNod *lookup(char *idString)	/* IN - The Id to look up */
{
  SymNod *s1,*s2;               /* Traversal pointers */
  int comp;                     /* Result of comparison */

  if (idString == NULL) return(NULL);

  s1 = symTree;
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

  setParent()

  Set the parent of a Class Symbol to be another Symbol

  */
void setParent(SymNod *child, SymNod *parent)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    syserr("Not a CLASS or INSTANCE in setParent()");
  child->fields.claOrIns.parent = parent;
}


/*======================================================================

  parentOf()

  Get the parent of a Class Symbol

  */
SymNod *parentOf(SymNod *child)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    syserr("Not a CLASS or INSTANCE in parentOf()");
  return child->fields.claOrIns.parent;
}


/*======================================================================

  inheritsFrom()

  Test inheritance from specified ClassSymbol.

  */
Bool inheritsFrom(SymNod *child, SymNod *ancestor)
{
  SymNod *p;

  if (child == NULL || ancestor == NULL) return FALSE;

  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL &&
      ancestor->kind != CLASS_SYMBOL)
    syserr("Not a CLASS or INSTANCE in inheritsFrom()");

  p = child;			/* To be the class itself is OK */
  while (p && p != ancestor)
    p = p->fields.claOrIns.parent;

  return (p != NULL);
}


/*======================================================================

  symcheck()

  Check if an Id exists and if so if it is of an allowed kind in this context

*/
SymNod *symcheck(		/* OUT - Found symbol */
    IdNode *id,			/* IN - The Id to check */
    SymbolKind kind,		/* IN - Allowed identifier kind */
    List *parameters		/* IN - Possible parameters valid in this context */
    )
{
  SymNod *sym = lookup(id->string);

  if (!sym) 
    lmLog(&id->srcp, 310, sevERR, id->string);
  else if (sym->kind != kind)
    lmLogv(&id->srcp, 319, sevERR, id->string, symbolKind(kind), NULL );
  return sym;
}


/*======================================================================

  inheritCheck

  Check that the given identifier inherits the class passed as a string.
  This will only be used for built in class checks (location, actor etc.)

*/
void inheritCheck(IdNode *id, char classOrInstance[], char className[])
{
  SymNod *theClassSymbol = lookup(className);

  if (theClassSymbol == NULL) syserr("There is no such class in inheritCheck()");

  if (!inheritsFrom(id->symbol, theClassSymbol))
    lmLogv(&id->srcp, 351, sevERR, classOrInstance, "location", NULL);
}



/*----------------------------------------------------------------------

  definingSymbolOfAttribute()

  Find the symbol which defines an attribute by traversing its parents.

*/
static SymNod *definingSymbolOfAttribute(SymNod *symbol, IdNode *id)
{
  AtrNod *foundAttribute;

  if (symbol == NULL)
    return NULL;

  if (symbol->kind != CLASS_SYMBOL && symbol->kind != INSTANCE_SYMBOL)
    return NULL;

  if ((foundAttribute = findAttribute(symbol->fields.claOrIns.slots->attributes, id)) == NULL)
    return definingSymbolOfAttribute(symbol->fields.claOrIns.parent, id);
  else
    return symbol;
}



/*======================================================================

  findInheritedAttribute()

  From a symbol traverse its inheritance tree to find a named attribute.

*/
AtrNod *findInheritedAttribute(SymNod *symbol, IdNode *id)
{
  SymNod *definingSymbol =
    definingSymbolOfAttribute(symbol->fields.claOrIns.parent, id);

  if (definingSymbol == NULL) return NULL;

  return findAttribute(definingSymbol->fields.claOrIns.slots->attributes, id);
}



/*----------------------------------------------------------------------

  numberAttributesRecursively()

  Recurse the parent to number its attributes.
  Number all attributes in the symbol (if it is a class or an instance);
*/

static void numberAttributesRecursively(SymNod *symbol)
{
  List *theList;
  SymNod *definingSymbol;
  AtrNod *inheritedAttribute;

  if (symbol == NULL) return;
  if (symbol->kind != CLASS_SYMBOL && symbol->kind != INSTANCE_SYMBOL)
    return;			/* Only a class or instance have attributes */
  if (!symbol->fields.claOrIns.attributesAlreadyNumbered) {
    /* We have attributes that are not numbered already */
    if (symbol->fields.claOrIns.parent != NULL)
      numberAttributesRecursively(symbol->fields.claOrIns.parent);
  
    for (theList = symbol->fields.claOrIns.slots->attributes; theList != NULL;
	 theList = theList->next){
      inheritedAttribute = findInheritedAttribute(symbol, theList->element.atr->id);
      if (inheritedAttribute != NULL) {
	if (!eqtyp(inheritedAttribute->typ, theList->element.atr->typ)) {
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
    symbol->fields.claOrIns.attributesAlreadyNumbered = TRUE;

    /* Recurse in the symTree */
    if (symbol->lower != NULL) numberAttributesRecursively(symbol->lower);
    if (symbol->higher != NULL) numberAttributesRecursively(symbol->higher);
  }
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
  numberAttributesRecursively(symTree);
}


/*----------------------------------------------------------------------

  replicateAttributesRecursively()

*/
static void replicateAttributesRecursively(SymNod *symbol)
{
  if (symbol == NULL) return;

  if (symbol->kind != CLASS_SYMBOL && symbol->kind != INSTANCE_SYMBOL)
    return;			/* Only a class or instance have attributes */

  if (!symbol->fields.claOrIns.attributesAlreadyReplicated) {
    /* We have attributes that are not numbered already */
    if (symbol->fields.claOrIns.parent != NULL) {
      replicateAttributesRecursively(symbol->fields.claOrIns.parent);
      combineAttributes(symbol->fields.claOrIns.slots->attributes,
			symbol->fields.claOrIns.parent->fields.claOrIns.slots->attributes);
    }
    symbol->fields.claOrIns.attributesAlreadyReplicated = TRUE;

    /* Recurse in the symTree */
    if (symbol->lower != NULL) replicateAttributesRecursively(symbol->lower);
    if (symbol->higher != NULL) replicateAttributesRecursively(symbol->higher);
  }
}



/*======================================================================

  replicateInheritedAttributes()

  Traverse the heritage of the symbol and replicate all inherited
  attributes that are not locally redefined, thus creating a complete
  list of all attributes that this symbol has.

  We will mark a completed symbol with "complete" so that we can use
  it directly if we encounter it later instead of redoing it.

*/
void replicateInheritedAttributes(void)
{
  replicateAttributesRecursively(symTree);
}


