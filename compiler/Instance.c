/*----------------------------------------------------------------------*\

			       Instance.c

			     Instance Nodes

\*----------------------------------------------------------------------*/

#include "types.h"

#include "Instance.h"
#include "Symbol.h"
#include "Class.h"

#include "lmList.h"
#include "dump.h"


/* PRIVATE */


/*======================================================================

  newInstance()

  Creates a new Instance and insert it in the symbol table.

  */
#ifdef _PROTOTYPES_
Instance *newInstance(Srcp *srcp,
		      Id *id,
		      Slot *slot)
#else
Instance *newInstance(srcp, id, slot)
     Srcp *srcp;
     Id *id;
     Slot *slot;
#endif
{
  Instance *new;			/* The newly created Instance */
  Symbol *symbol;

  new = NEW(Instance);

  symbol = newSymbol(id, INSTANCE_SYMBOL);
  symbol->info.instance = new;

  new->srcp = *srcp;
  new->id = id;
  new->slot = slot;
  
  return(new);
}



/*======================================================================

  initInstances()

  Initialise any predefines Instances (the Hero..)

  */
#ifdef _PROTOTYPES_
void initInstances(void)
#else
void initInstances()
#endif
{
  /* 4f - initialise the predefined instances (hero, inventory?) */
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
  if (listLength(instance->slot->heritage) > 1)
    lmLog(&instance->srcp, 227, sevERR, NULL);
  else
    analyseSlot(instance->id, instance->slot);
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
  dumpId(instance->id); nl();
  dumpSlot(instance->slot); out();
}
