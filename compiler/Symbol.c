/*----------------------------------------------------------------------*\

				Symbol.c
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/

#include "types.h"

#include "Symbol.h"

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
     Id *id;			/* IN - Name of the new symbol */
     SymbolKind kind;		/* IN - and its class */
#endif
{
  Symbol *new;			/* The newly created Symbol */
  Symbol *s1,*s2;		/* Traversal pointers */
  int comp;			/* Result of comparison */
  
  if (id == NULL || id->string == NULL)
    return (0);
  
  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(id->string, s1->string);
    if (comp < 0)
      s1 = s1->lower;
    else if (comp != 0)
      s1 = s1->higher;
    else {
      redefined(id);
      s1->kind = UNKNOWN_SYMBOL;
      return s1;
    }
  }
  
  new = NEW(Symbol);
  
  new->kind = kind;
  new->string = id->string;
  
  new->lower = NULL;
  new->higher = NULL;
  
  s1 = symbols;
  s2 = NULL;
  
  if (s2 == NULL)
    symbols = new;
  else if(comp < 0)
    s2->lower = new;
  else
    s2->higher = new;
  
  switch (kind) {
  case CLASS_SYMBOL: new->code = ++count.class; break;
  case INSTANCE_SYMBOL: new->code = ++count.instance; break;
  case DIRECTION_SYMBOL: new->code = ++count.direction; break;
  case VERB_SYMBOL: new->code = ++count.verb; break;
  case UNKNOWN_SYMBOL: break;
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
  Symbol *s1,*s2;		/* Traversal pointers */
  int comp;			/* Result of comparison */

  if (string == NULL) return(NULL);

  s1 = symbols;
  s2 = NULL;

  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(string, s1->string);
    if (comp == 0)
      return(s1);
    else if (comp < 0)
      s1 = s1->lower;
    else
      s1 = s1->higher;
  }

  return(NULL);
}
