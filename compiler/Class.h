#ifndef _CLASS_H_
#define _CLASS_H_
/*----------------------------------------------------------------------*\

			       Class.h

			     Class Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "List.h"
#include "Where.h"
#include "Container.h"
#include "Does.h"



/* Types: */

typedef struct Class {		/* Class Node */
  Srcp srcp;
  Id *id;
  int code;
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
} Class;


/* Data: */

/* Pseudo constants for the predefined classes */
extern int THING_CLASS, LOCATION_CLASS, OBJECT_CLASS, ACTOR_CLASS, CONTAINER_CLASS;


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Class node */
extern Class *newClass(Srcp *srcp,
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
		       List *scripts);

/* Create predefined classes */
extern void initClasses(void);

/* Analyse the class */
extern void analyseClass(Class *class);

/* Do the common analysis for Classes and Instances */
extern void analyseClassOrInstance(List *heritage,
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
				   List *scripts);

/* Dump a class */
extern void dumpClass(Class *class);

/* Dump common parts for Classes and Instances */
extern void dumpClassOrInstance(List *heritage,
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
				List *scripts);

#else
extern void initClasses();
extern Class *newClass();
extern void analyseClass();
extern void analyseClassOrInstance();
extern void dumpClass();
extern void dumpClassOrInstance();
#endif

#endif
