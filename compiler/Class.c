/*----------------------------------------------------------------------*\

			       Class.c

			     Class Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "Class.h"
#include "Symbol.h"

#include "dump.h"


/* PUBLIC */

/* Pseudo constants for the predefined classes */
int THING_CLASS, LOCATION_CLASS, OBJECT_CLASS, ACTOR_CLASS, CONTAINER_CLASS;


/* PRIVATE */


/*======================================================================

  newClass()

  Creates a new Class and insert it in the symbol table.

  */
#ifdef _PROTOTYPES_
Class *newClass(Srcp *srcp,
		Id *id,
		Slot *slot)
#else
Class *newClass(srcp, id, slot)
     Srcp *srcp;
     Id *id;
     Slot *slot;
#endif
{
  Class *new;			/* The newly created Class */
  Symbol *symbol;

  new = NEW(Class);

  symbol = newSymbol(id, CLASS_SYMBOL);
  symbol->info.class = new;
  new->code = symbol->code;

  new->srcp = *srcp;
  new->id = id;
  new->slot = slot;
  
  return(new);
}


/*======================================================================

  initClasses()

  Create initial predefined Classes

  */
#ifdef _PROTOTYPES_
void initClasses(void)
#else
void initClasses()
#endif
{
  Class *thing;
  Class *object;
  Class *location;
  Class *actor;
  Class *container;

  /* Create THING class */
  thing = newClass(&nullSrcp,
		   newId(&nullSrcp,
			 "thing"),
		   newSlot( NULL,	/* heritage */
			   NULL,	/* name */
			   newWhere(&nullSrcp, WHERE_DEFAULT, NULL),
			   NULL,	/* attributes */
			   NULL,	/* container */
			   NULL,	/* surroundings */
			   NULL,	/* description */
			   NULL,	/* mentioned */
			   NULL,	/* does */
			   NULL,	/* exits */
			   NULL,	/* verbs */
			   NULL));	/* scripts */
  THING_CLASS = thing->code;

  /* Create OBJECT class */
  object = newClass(&nullSrcp,
		    newId(&nullSrcp,
			  "object"),
		    newSlot(append(NULL, newId(&nullSrcp,
					       "thing")), /* heritage */
			    NULL,	/* name */
			    newWhere(&nullSrcp, WHERE_DEFAULT, NULL),
			    NULL,	/* attributes */
			    NULL,	/* container */
			    NULL,	/* surroundings */
			    NULL,	/* description */
			    NULL,	/* mentioned */
			    NULL,	/* does */
			    NULL,	/* exits */
			    NULL,	/* verbs */
			    NULL));	/* scripts */
  OBJECT_CLASS = object->code;

  /* Create LOCATION class */
  location = newClass(&nullSrcp,
		      newId(&nullSrcp,
			    "location"),
		      newSlot(append(NULL, newId(&nullSrcp,
						 "thing")), /* heritage */
			      NULL,	/* name */
			      newWhere(&nullSrcp, WHERE_DEFAULT, NULL),
			      NULL,	/* attributes */
			      NULL,	/* container */
			      NULL,	/* surroundings */
			      NULL,	/* description */
			      NULL,	/* mentioned */
			      NULL,	/* does */
			      NULL,	/* exits */
			      NULL,	/* verbs */
			      NULL));	/* scripts */
  LOCATION_CLASS = location->code;

  /* Create ACTOR class */
  actor = newClass(&nullSrcp,
		   newId(&nullSrcp,
			 "actor"),
		   newSlot(append(NULL, newId(&nullSrcp,
					      "thing")), /* heritage */
			   NULL,	/* name */
			   newWhere(&nullSrcp, WHERE_DEFAULT, NULL),
			   NULL,	/* attributes */
			   NULL,	/* container */
			   NULL,	/* surroundings */
			   NULL,	/* description */
			   NULL,	/* mentioned */
			   NULL,	/* does */
			   NULL,	/* exits */
			   NULL,	/* verbs */
			   NULL));	/* scripts */
  ACTOR_CLASS = actor->code;

  /* Create CONTAINER class */
  container = newClass(&nullSrcp,
		       newId(&nullSrcp,
			     "container"),
		       newSlot(append(NULL, newId(&nullSrcp,
						  "thing")), /* heritage */
			       NULL,	/* name */
			       newWhere(&nullSrcp, WHERE_DEFAULT, NULL),
			       NULL,	/* attributes */
			       NULL,	/* container */
			       NULL,	/* surroundings */
			       NULL,	/* description */
			       NULL,	/* mentioned */
			       NULL,	/* does */
			       NULL,	/* exits */
			       NULL,	/* verbs */
			       NULL));	/* scripts */
  CONTAINER_CLASS = container->code;
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
  analyseSlot(class->slot);
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
  dumpId(class->id); nl();
  dumpSlot(class->slot); out();
}
