/*----------------------------------------------------------------------*\

                               RES.C
		   Element Class Restriction Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "res.h"                /* RES-nodes */

#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stm.h"                /* STM-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"


/* PUBLIC: */


/*======================================================================

  newres()

  Allocates and initialises an element class restriction node.

 */
ResNod *newres(Srcp *srcp,	/* IN - Source Position */
	       NamNod *nam,	/* IN - The name */
	       Bool single,	/* IN - TRUE if single class */
	       NamKind classes,	/* IN - Allowed classes */
	       List *stms)	/* IN - Statements to execute otherwise */
{
  ResNod *new;			/* The newly created node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ResNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->single = single;
  new->classes = classes;
  new->stms = stms;

  return(new);
}



/*----------------------------------------------------------------------

  anres()

  Analyzes one element class restriction node.

 */
static void anres(
     ResNod *res,		/* IN - Restriction node to analyze */
     List *params		/* IN - Possible syntax parameters */
)
{
  Bool found = FALSE;
  List *p;

  /* Check for the id in the parameter list */
  for (p = params; p != NULL; p = p->next)
    if (eqnams(res->nam, p->element.elm->nam)) {
      found = TRUE;
      break;
    }
  if (!found)
    lmLog(&res->nam->srcp, 222, sevERR, res->nam->str);
  anstms(res->stms, NULL, NULL, params);
}



/*======================================================================

  anress()

  Analyzes all class restriction nodes in a list by calling the analyzer
  for each.

 */
void anress(
    List *ress,			/* IN - List of nodes to analyze */
    List *params		/* IN - Possible syntax parameters */
)
{
  List *lst;

  for (lst = ress; lst != NULL; lst = lst->next)
    anres(lst->element.res, params);
}



/*----------------------------------------------------------------------

  geres()

  Generate code for one syntax class restriction node.

 */
static void geres(ResNod *res)	/* IN - Node to generate */
{
  res->stmadr = emadr();
  gestms(res->stms, NULL);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geresent()

  Generate an entry for one restriction node.

 */
static void geresent(ResNod *res) /* IN - Node to generate */
{
  emit(res->nam->code);
  emit(res->classes);
  emit(res->stmadr);
}



/*======================================================================

  geress()

  Generate the data structure for the element class restrictions.

 */
Aaddr geress(List *ress,	/* IN - The element class restriction nodes */
	     StxNod *stx)	/* IN - Syntax node containing the res */
{
  List *lst;
  Aaddr resadr;

  for (lst = ress; lst != NULL; lst = lst->next)
    geres(lst->element.res);

  resadr = emadr();
  for (lst = ress; lst != NULL; lst = lst->next)
    geresent(lst->element.res);

  /* End it with an End of file and the code for the verb */
  emit(EOF);
  genam(stx->nam);

  return(resadr);
}



/*======================================================================

  dures()

  Dump a syntax element Class restriction node.

 */
void dures(ResNod *res)
{
  if (res == NULL) {
    put("NULL");
    return;
  }

  put("RES: "); dusrcp(&res->srcp); in();
  put("nam: "); dunam(res->nam); nl();
  put("classes: "); if (res->single) put("(single)"); nl();
  put("stms: "); dulst(res->stms, STMNOD); out();
}
