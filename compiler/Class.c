/*----------------------------------------------------------------------*\

			       Class.c

			     Class Nodes

\*----------------------------------------------------------------------*/

#include "types.h"

#include "Class.h"
#include "Symbol.h"

#include "dump.h"


/* PRIVATE */


/*======================================================================

  newClass()

  Creates a new Class and insert it in the symbol table.

  */
#ifdef _PROTOTYPES_
Class *newClass(Id *id,
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
Class *newClass(id, heritage, name, where, attributes, container, surroundings,
		description, mentioned, does, exits, verbs, scripts)
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
  Class *new;			/* The newly created Class */
  Symbol *symbol;

  new = NEW(Class);

  symbol = newSymbol(id, CLASS_SYMBOL);
  symbol->info.class = new;

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

  analyseClassOrInstance()

  Do the common analysis for a Class or an Instance

  */
#ifdef _PROTOTYPES_
void analyseClassOrInstance(List *heritage,
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
void analyseClassOrInstance(heritage, name, where, attributes, container,
			    surroundings, description, mentioned, does,
			    exits, verbs, scripts)
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
  /* 4f - Verify that the inherited classes exist */
  /* 4f - Check for inheritance clashes */
  /* 4f - Check for circular inheritance */
  /* 4f - Analyse the name */
  /* 4f - Analyse the where */
  /* 4f - Analyse the attributes */
  /* 4f - Analyse the container */
  /* 4f - Analyse the surroundings */
  /* 4f - Analyse the description */
  /* 4f - Analyse the mentioned */
  /* 4f - Analyse the does */
  /* 4f - Analyse the exits */
  /* 4f - Analyse the verbs */
  /* 4f - Analyse the scripts */
}


/*======================================================================

  analyseClass()

  Do the analysis for a Class.

  */
#ifdef _PROTOTYPES_
void analyseClass(Class *class)
#else
void analyseClass(class)
     Class *class;
#endif
{
  analyseClassOrInstance(class->heritage,
			 class->name,
			 class->where,
			 class->attributes,
			 class->container,
			 class->surroundings,
			 class->description,
			 class->mentioned,
			 class->does,
			 class->exits,
			 class->verbs,
			 class->scripts);
}



/*======================================================================

  dumpClassOrInstance()

  Do the common analysis for a Class or an Instance

  */
#ifdef _PROTOTYPES_
void dumpClassOrInstance(List *heritage,
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
void dumpClassOrInstance(heritage, name, where, attributes, container,
			 surroundings, description, mentioned, does,
			 exits, verbs, scripts)
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
  put("heritage: "); dumpList(heritage, ID_NODE);
  put("name: "); dumpList(name, ID_NODE);
  put("where: "); dumpWhere(where);
  put("attributes: "); dumpList(name, ATTRIBUTE_NODE);
  put("container: "); dumpContainer(container);
  put("surroundings: "); dumpList(name, STATEMENT_NODE);
  put("description: "); dumpList(name, STATEMENT_NODE);
  put("mentioned: "); dumpList(name, STATEMENT_NODE);
  put("does: "); dumpDoes(does);
  put("exits: "); dumpList(name, EXIT_NODE);
  put("verbs: "); dumpList(name, VERB_NODE);
  put("scripts: "); dumpList(name, SCRIPT_NODE);
}


/*======================================================================

  dumpClass()

  Dump a Class.

  */
#ifdef _PROTOTYPES_
void dumpClass(Class *class)
#else
void dumpClass(class)
     Class *class;
#endif
{
  if (class == NULL) {
    put("NULL");
    return;
  }

  put("CLASS: "); dumpSrcp(&class->id->srcp); in();
  dumpClassOrInstance(class->heritage,
		      class->name,
		      class->where,
		      class->attributes,
		      class->container,
		      class->surroundings,
		      class->description,
		      class->mentioned,
		      class->does,
		      class->exits,
		      class->verbs,
		      class->scripts);
  out();
}
