#ifndef _SLOT_H_
#define _SLOT_H_
/*----------------------------------------------------------------------*\

			       Slot.h

			     Slot Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "List.h"
#include "Where.h"
#include "Container.h"
#include "Does.h"



/* Types: */

typedef enum AnalyseState {
  IDLE,
  LOOKING_FOR_CIRCLES,
  NUMBERING_ATTRIBUTES,
  FINISHED
} AnalyseState;

typedef struct Slot {		/* Slot Node */
  AnalyseState state;
  List *heritage;
  List *name;
  Where *where;
  List *attributes;
  List *inheritedAttributeLists;
  Container *container;
  List *surroundings;
  List *description;
  List *mentioned;
  Does *does;
  List *exits;
  List *verbs;
  List *scripts;
} Slot;


/* Data: */

/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Slot node */
extern Slot *newSlot(List *heritage,
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

/* Create predefined slots */
extern void initSlots(void);

/* Analyse the Slot */
extern void analyseSlot(Id *id, Slot *slot);

/* Dump a Slot */
extern void dumpSlot(Slot *slot);

#else
extern Slot *newSlot();
extern void analyseSlot();
extern void dumpSlot();
#endif

#endif
