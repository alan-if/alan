#ifndef _INSTANCE_H_
#define _INSTANCE_H_
/*----------------------------------------------------------------------*\

			       Instance.h

			     Instance Nodes

\*----------------------------------------------------------------------*/


  /* USE: */
#include "Srcp.h"
#include "List.h"
#include "Id.h"
#include "Where.h"
#include "Container.h"
#include "Does.h"

/* Types: */

typedef struct Instance {		/* Instance Node */
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
} Instance;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Instance node */
extern Instance *newInstance(Id *id,
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

#else
extern Instance *newInstance();
#endif

#endif
