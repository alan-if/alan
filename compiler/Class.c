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

/* PUBLIC */

Class *thingClass;
Class *objectClass;
Class *actorClass;
Class *locationClass;
Class *containerClass;





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
  /* Create THING class */
  thingClass = newClass(&nullSrcp,
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
  THING_CLASS = thingClass->code;

  /* Create OBJECT class */
  objectClass = newClass(&nullSrcp,
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
  OBJECT_CLASS = objectClass->code;

  /* Create LOCATION class */
  locationClass = newClass(&nullSrcp,
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
  LOCATION_CLASS = locationClass->code;

  /* Create ACTOR class */
  actorClass = newClass(&nullSrcp,
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
  ACTOR_CLASS = actorClass->code;

  /* Create CONTAINER class */
  containerClass = newClass(&nullSrcp,
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
  CONTAINER_CLASS = containerClass->code;
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
  analyseSlot(class->id, class->slot);
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
