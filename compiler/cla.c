/*----------------------------------------------------------------------*\

  CLA.C
  Class Nodes

\*----------------------------------------------------------------------*/

#include "cla.h"

/* IMPORT */
#include <stdio.h>
#include "types.h"

#include "srcp_x.h"
#include "id_x.h"
#include "sym_x.h"

#include "util.h"
#include "dump.h"
#include "lmList.h"



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
  SymNod *heritage;

  if (cla->heritage != NULL) {
    heritage = lookup(cla->heritage->string);
    if (heritage == NULL || heritage->kind != CLASS_SYMBOL)
      lmLog(&cla->heritage->srcp, 350, sevERR, "");
    else {
      cla->heritage->symbol = heritage;
      setParent(cla->symbol, cla->heritage->symbol);
    }
  }
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
