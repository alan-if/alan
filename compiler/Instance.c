/*----------------------------------------------------------------------*\

			       Instance.c

			     Instance Nodes

\*----------------------------------------------------------------------*/

#include "types.h"

#include "Instance.h"
#include "Symbol.h"
#include "Class.h"

#include "dump.h"


/* PRIVATE */


/*======================================================================

  newInstance()

  Creates a new Instance and insert it in the symbol table.

  */
#ifdef _PROTOTYPES_
Instance *newInstance(Srcp *srcp,
		      Id *id,
		      List *heritage,
		      List *name,
		      Where *where,
		      List *attributes,
		      Container *container,
		      List *surroundings,
		      List *description,
		      List *mentioned,
		      Does *does,
		      List *exits,
		      List *verbs,
		      List *scripts)
#else
Instance *newInstance(srcp, id, heritage, name, where, attributes, container,
		      surroundings, description, mentioned, does, exits, verbs,
		      scripts)
     Srcp *srcp;
     Id *id;
     List *heritage;
     List *name;
     Where *where;
     List *attributes;
     Container *container;
     List *surroundings;
     List *description;
     List *mentioned;
     Does *does;
     List *exits;
     List *verbs;
     List *scripts;
#endif
{
  Instance *new;			/* The newly created Instance */
  Symbol *symbol;

  new = NEW(Instance);

  symbol = newSymbol(id, INSTANCE_SYMBOL);
  symbol->info.instance = new;

  new->srcp = *srcp;
  new->id = id;
  new->heritage = heritage;
  new->name = name;
  new->where = where;
  new->attributes = attributes;
  new->container = container;
  new->surroundings = surroundings;
  new->description = description;
  new->mentioned = mentioned;
  new->does = does;
  new->exits = exits;
  new->verbs = verbs;
  new->scripts = scripts;
  
  return(new);
}



/*======================================================================

  analyseInstance()

  Do the analysis for a Instance.

  */
#ifdef _PROTOTYPES_
void analyseInstance(Instance *instance)
#else
void analyseInstance(instance)
     Instance *instance;
#endif
{
  analyseClassOrInstance(instance->heritage,
			 instance->name,
			 instance->where,
			 instance->attributes,
			 instance->container,
			 instance->surroundings,
			 instance->description,
			 instance->mentioned,
			 instance->does,
			 instance->exits,
			 instance->verbs,
			 instance->scripts);
}


/*======================================================================

  dumpInstance()

  Dump a Instance.

  */
#ifdef _PROTOTYPES_
void dumpInstance(Instance *instance)
#else
void dumpInstance(instance)
     Instance *instance;
#endif
{
  if (instance == NULL) {
    put("NULL");
    return;
  }

  put("INSTANCE: "); dumpSrcp(&instance->id->srcp); in();
  dumpClassOrInstance(instance->heritage,
		      instance->name,
		      instance->where,
		      instance->attributes,
		      instance->container,
		      instance->surroundings,
		      instance->description,
		      instance->mentioned,
		      instance->does,
		      instance->exits,
		      instance->verbs,
		      instance->scripts);
  out();
}
