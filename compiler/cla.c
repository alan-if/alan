/*----------------------------------------------------------------------*\

  CLA.C
  Class Nodes

\*----------------------------------------------------------------------*/

#ifndef UNIT
#include "cla.h"
#endif


/* IMPORT */
#include <stdio.h>

#include "types.h"
#include "id.h"
#include "util.h"
#include "dump.h"
#include "lmList.h"

#include "sym.h"


/* PRIVATE DATA */

static List *allClasses = NULL;



/*======================================================================

  newcla()

  Allocates and initialises a clanod.

  */
ClaNod *newcla(Srcp *srcp,	/* IN - Source Position */
	       IdNode *id,
	       IdNode *heritage,
	       Slots *slt)
{
  ClaNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ClaNod);

  new->srcp = *srcp;
  new->id = id;
  new->heritage = heritage;
  new->slt = slt;

  new->symbol = newsym(id->string, CLASS_SYMBOL);

  allClasses = concat(allClasses, new, LIST_CLA);

  return(new);
}


/*----------------------------------------------------------------------

  symbolizeClass()

  Symbolize a Class node.

 */
static void symbolizeClass(ClaNod *cla)
{
  SymNod *heritage = lookup(cla->heritage->string);
  if (heritage->kind != CLASS_SYMBOL)
      syserr("UNIMPLEMENTED: symbolizeClass - Can not inherit from something not a class");

  cla->heritage->symbol = heritage;
  setParent(cla->symbol, cla->heritage->symbol);
}


/*======================================================================

  symbolizeClasses()

  Symbolize all Class nodes.

 */
void symbolizeClasses(void)
{
  List *l;

  for (l = allClasses; l; l = l->next)
    symbolizeClass(l->element.cla);
}



/*----------------------------------------------------------------------

  symbolizeClass()

  Symbolize a Class node.

 */
static void analyzeClass(ClaNod *cla)
{
}



/*======================================================================

  analyzeClasses()

  Analyze all Class nodes.

 */
void analyzeClasses(void)
{
  List *l;

  for (l = allClasses; l; l = l->next)
    analyzeClass(l->element.cla);
}


/*======================================================================

  generateClasses()

  Generate all Class nodes.

 */
Aaddr generateClasses(void)
{
  syserr("UNIMPL: generateClasses");
  return 0;
}



/*======================================================================

  dumpClass()

  Dump a Class node.

 */
void dumpClass(ClaNod *cla)
{
  put("CLA: "); dumpSrcp(&cla->srcp); in();
  put("id: "); dumpId(cla->id); nl();
  put("heritage: "); dumpId(cla->heritage); nl();
  put("slots: "); dumpSlots(cla->slt); out();
}


/*======================================================================

  dumpClasses()

  Dump all Class nodes.

 */
void dumpClasses(void)
{
  dulst(allClasses, LIST_CLA);
}
