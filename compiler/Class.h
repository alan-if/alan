#ifndef _CLASS_H_
#define _CLASS_H_
/*----------------------------------------------------------------------*\

			       Class.h

			     Class Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "List.h"
#include "Slot.h"


/* Types: */

typedef struct Class {		/* Class Node */
  Srcp srcp;
  Id *id;
  int code;
  Slot *slot;
} Class;


/* Data: */

/* Pseudo constants for the predefined classes */
extern int THING_CLASS, LOCATION_CLASS, OBJECT_CLASS, ACTOR_CLASS, CONTAINER_CLASS;

extern Class *thingClass;
extern Class *objectClass;
extern Class *actorClass;
extern Class *locationClass;
extern Class *containerClass;


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Class node */
extern Class *newClass(Srcp *srcp,
		       Id *id,
		       Slot *slot);

/* Create predefined classes */
extern void initClasses(void);

/* Analyse the class */
extern void analyseClass(Class *class);

/* Dump a class */
extern void dumpClass(Class *class);

#else
extern void initClasses();
extern Class *newClass();
extern void analyseClass();
extern void dumpClass();
#endif

#endif
