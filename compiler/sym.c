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


int classCount = 0;
int instanceCount = 0;


static SymNod *symtree = NULL;



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

  symbol->low = NULL;
  symbol->high = NULL;

  s1 = symtree;
  s2 = NULL;
  
  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(symbol->string, s1->string);
    if (comp < 0)
      s1 = s1->low;
    else
      s1 = s1->high;
  }
  
  if (s2 == NULL)
    symtree = symbol;
  else if(comp < 0)
    s2->low = symbol;
  else
    s2->high = symbol;
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

  newsym()

  Creates a new symnod and links it in the symtree.

  */
SymNod *newsym(char *string,	/* IN - Name of the new symbol */
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
    new->fields.cla.parent = NULL;
    break;
  case INSTANCE_SYMBOL:
    new->code = ++instanceCount;
    new->fields.ins.parent = NULL;
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
  SymNod *thing;
  SymNod *object;
  SymNod *actor;
  SymNod *location;

  symtree = NULL;

  thing = newsym("thing", CLASS_SYMBOL);
  object = newsym("object", CLASS_SYMBOL);
  actor = newsym("actor", CLASS_SYMBOL);
  location = newsym("location", CLASS_SYMBOL);

  setParent(location, thing);
  setParent(object, thing);
  setParent(actor, thing);
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

  s1 = symtree;
  s2 = NULL;

  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(idString, s1->string);
    if (comp == 0)
      return(s1);
    else if (comp < 0)
      s1 = s1->low;
    else
      s1 = s1->high;
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
  child->fields.cla.parent = parent;
}


/*======================================================================

  parentOf()

  Get the parent of a Class Symbol

  */
SymNod *parentOf(SymNod *child)
{
  if (child->kind != CLASS_SYMBOL && child->kind != INSTANCE_SYMBOL)
    syserr("Not a CLASS or INSTANCE in parentOf()");
  return child->fields.cla.parent;
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

  p = child->fields.cla.parent;
  while (p && p != ancestor)
    p = p->fields.cla.parent;

  return (p != NULL);
}


/*======================================================================

  symcheck()

  Check if an Id exists and if so if it is of an allowed kind in this context

*/
SymNod *symcheck(		/* OUT - Found symbol */
    ElmNod **elm,		/* OUT - Found parameter  */
    IdNode *id,			/* IN - The Id to check */
    SymbolKind kind,		/* IN - Allowed identifier kind */
    List *pars			/* IN - Possible parameters valid in this context */
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

  if (theClassSymbol == NULL) syserr("There is no such class in classCheck()");

  if (!inheritsFrom(id->symbol, theClassSymbol))
    lmLogv(&id->srcp, 351, sevERR, classOrInstance, "location", NULL);
}
