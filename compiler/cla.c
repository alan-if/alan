/*----------------------------------------------------------------------*\

				CLA.C
			     Class Nodes

\*----------------------------------------------------------------------*/

#include "cla_x.h"

/* IMPORT */
#include <stdio.h>
#include "types.h"

#include "srcp_x.h"
#include "id_x.h"
#include "sym_x.h"
#include "slt_x.h"

#include "emit.h"
#include "util.h"
#include "dump.h"
#include "lmList.h"



/* PRIVATE DATA */

static List *allClasses = NULL;





/*======================================================================

  initClasses()

*/
void initClasses()
{
  allClasses = NULL;
}



/*======================================================================

  newcla()

  Allocates and initialises a clanod.

  */
ClaNod *newcla(Srcp *srcp,	/* IN - Source Position */
	       IdNode *id,
	       IdNode *parent,
	       SlotsNode *slt)
{
  ClaNod *new;                  /* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ClaNod);

  new->srcp = *srcp;
  new->id = id;
  new->parent = parent;
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
  SymNod *parent;

  symbolizeId(cla->id);

  if (cla->parent != NULL) {
    parent = lookup(cla->parent->string);
    if (parent == NULL)
      lmLog(&cla->parent->srcp, 310, sevERR, cla->parent->string);
    else if (parent->kind != CLASS_SYMBOL)
      lmLog(&cla->parent->srcp, 350, sevERR, "");
    else {
      cla->parent->symbol = parent;
      setParent(cla->symbol, cla->parent->symbol);
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

  analyzeClass()

  Analyze a Class node.

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


/*----------------------------------------------------------------------

  generateClass

*/
static void generateClass(ClaNod *cla)
{
  cla->adr = emadr();

  emit(cla->symbol->code);		/* First own code */
  if (cla->parent == NULL)	/* Then parents */
    emit(0);
  else
    emit(cla->parent->symbol->code);
}


/*----------------------------------------------------------------------

  generateClassEntry

*/
static void generateClassEntry(ClaNod *cla)
{
  emit(cla->adr);
}


/*======================================================================

  generateClasses()

  Generate all Class nodes.

 */
Aaddr generateClasses(void)
{
  List *l;
  Aaddr adr;

  for (l = allClasses; l; l = l->next)
    generateClass(l->element.cla);

  adr = emadr();
  for (l = allClasses; l; l = l->next)
    generateClassEntry(l->element.cla);

  return (adr);
}



/*======================================================================

  dumpClass()

  Dump a Class node.

 */
void dumpClass(ClaNod *cla)
{
  put("CLA: "); dumpSrcp(&cla->srcp); in();
  put("id: "); dumpId(cla->id); nl();
  put("parent: "); dumpId(cla->parent); nl();
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
