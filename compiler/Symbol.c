/*----------------------------------------------------------------------*\

				Symbol.c
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/

#include "types.h"

#include "Symbol.h"
#include "List.h"
#include "Class.h"
#include "Instance.h"

#include "alan.h"
#include "lmList.h"


/* PRIVATE */
static Symbol *symbols = NULL;
static struct {
  int class;
  int instance;
  int direction;
  int verb;
} count;



/*----------------------------------------------------------------------

  redefined()

  Calls lmLog() to announce redefined symbols.

  */
#ifdef _PROTOTYPES_
static void redefined(Id *id)	/* IN - The identifier */
#else
static void redefined(id)
     Id *id;			/* IN - The identifier */
#endif
{
  lmLog(&id->srcp, 301, sevERR, id->string);
}



/*======================================================================

  newSymbol()

  Creates a new Symbol and links it in the symtree.

  */
#ifdef _PROTOTYPES_
Symbol *newSymbol(Id *id,	/* IN - Name of the new symbol */
		  SymbolKind kind) /* IN - and its class */
#else
Symbol *newSymbol(id, kind)
     Id *id;
     SymbolKind kind;
#endif
{
  Symbol *new;			/* The newly created Symbol */
  Symbol *investigatedSymbol,*branchPoint; /* Traversal pointers */
  int comp;			/* Result of comparison */
  
  if (id == NULL || id->string == NULL)
    return (0);
  
  investigatedSymbol = symbols;
  branchPoint = NULL;
  while (investigatedSymbol != NULL) {
    branchPoint = investigatedSymbol;
    comp = strcmp(id->string, investigatedSymbol->string);
    if (comp < 0)
      investigatedSymbol = investigatedSymbol->lower;
    else if (comp > 0)
      investigatedSymbol = investigatedSymbol->higher;
    else {
      redefined(id);
      investigatedSymbol->kind = ERROR_SYMBOL;
      return investigatedSymbol;
    }
  }
  
  new = NEW(Symbol);
  
  new->kind = kind;
  new->string = id->string;
  
  new->lower = NULL;
  new->higher = NULL;
  
  if (symbols == NULL)
    symbols = new;
  else if (comp < 0)
    branchPoint->lower = new;
  else
    branchPoint->higher = new;
  
  switch (kind) {
  case CLASS_SYMBOL: new->code = ++count.class; break;
  case INSTANCE_SYMBOL: new->code = ++count.instance; break;
  case DIRECTION_SYMBOL: new->code = ++count.direction; break;
  case VERB_SYMBOL: new->code = ++count.verb; break;
  case ERROR_SYMBOL: break;
  default: syserr("Unknown symbol kind in newSymbol()"); break;
  }

  return(new);
}



/*======================================================================

  lookup()

  Look for a symbol. If found return a pointer to its instance, else
  NULL.

  */
#ifdef _PROTOTYPES_
Symbol *lookup(char *string)	/* IN - The name to look up */
#else
Symbol *lookup(string)
     char string[];
#endif
{
  Symbol *investigatedSymbol;		/* Traversal pointers */
  int comp;			/* Result of comparison */

  if (string == NULL) return(NULL);

  investigatedSymbol = symbols;

  while (investigatedSymbol != NULL) {
    comp = strcmp(string, investigatedSymbol->string);
    if (comp == 0)
      return(investigatedSymbol);
    else if (comp < 0)
      investigatedSymbol = investigatedSymbol->lower;
    else
      investigatedSymbol = investigatedSymbol->higher;
  }

  return(NULL);
}



/*----------------------------------------------------------------------

  symbolKindString()

  Return a pointer to a string containing the string representation of
  the SymbolKind

  */
#ifdef __PROTOTYPES_
static char *symbolKindString(SymbolKind kind)
#else
static char *symbolKindString(kind)
     SymbolKind kind;
#endif
{
  switch (kind) {
  case CLASS_SYMBOL: return "a class";
  case INSTANCE_SYMBOL: return "an instance";
  case VERB_SYMBOL: return "a verb";
  case DIRECTION_SYMBOL: return "a direction";
  default: syserr("Unrecognized symbol kind in symbolKindString()."); return "***ERRROR***";
  }
}


/*======================================================================

  symbolCheck()

  Verify that the identifier is a symbol of the correct kind.
  If not issue an error message and return NULL, else return a pointer
  to the symbol.

  */
#ifdef _PROTOTYPES_
Symbol *symbolCheck(Id *id,	/* IN - the Identifier to check */
		    SymbolKind kind) /* IN - The kind of the symbol it should be */
#else
Symbol *symbolCheck(id, kind)
     Id *id;
     SymbolKind kind;
#endif
{
  Symbol *symbol;

  symbol = lookup(id->string);
  if (symbol == NULL || (symbol->kind != kind && symbol->kind != ERROR_SYMBOL)) {
    if (symbol == NULL)
      symbol = newSymbol(id, ERROR_SYMBOL);
    lmLogv(&id->srcp, 300, sevERR, id->string, symbolKindString(kind), NULL);
    return NULL;
  } else
    return symbol;
}



/*======================================================================

  isA()

  Does the identifier inherit the class? Does not verify the symbol.

  */
#ifdef _PROTOTYPES_
Bool isA(Id *id,		/* IN - the Id to check */
	 char className[]	/* IN - the name of the class it should belong to*/
)
#else
Bool isA(id, className)
     Id *id;
     char className[];
#endif
{
  List *heritage, *h;
  Symbol *symbol;

  symbol = lookup(id->string);
  switch (symbol->kind) {
  case CLASS_SYMBOL: heritage = symbol->info.class->slot->heritage; break;
  case INSTANCE_SYMBOL: heritage = symbol->info.instance->slot->heritage; break;
  default: syserr("Unexpected symbol kind in isA()."); break;
  }

  for (h = heritage; h; h = h->next)
    if (strcmp(h->element.id->string, className) == 0)
      return TRUE;

  for (h = heritage; h; h = h->next)
    if (isA(h->element.id, className))
      return TRUE;

  return FALSE;
}



/*======================================================================

  classCheck()

  Verify that the identifier inherits the class indicated by the code.
  If not issue an error message.

  */
#ifdef _PROTOTYPES_
void classCheck(Id *id,		/* IN - the Identifier to check */
		char className[] /* IN - the name of the class it should belong to*/
)
#else
void classCheck(id, className)
     Id *id;
     char className[];
#endif
{
  Symbol *symbol;

  symbol = lookup(className);

  if (symbol == NULL) {		/* Didn't exist, so create an error symbol */
    lmLog(&id->srcp, 224, sevERR, className);
    symbol = newSymbol(newId(&nullSrcp, className), ERROR_SYMBOL);
    return;
  }

  if (symbol->kind != CLASS_SYMBOL && symbol->kind != ERROR_SYMBOL) {
    lmLog(&id->srcp, 225, sevERR, className);
    return;
  }

  if (!isA(id, className))
    lmLogv(&id->srcp, 223, sevERR, id->string, className, NULL);
}
