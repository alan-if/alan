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
#ifdef _PROTOTYPES_
ResNod *newres(Srcp *srcp, NamNod *nam, Boolean single, NamKind classes, List *stms)
                		/* IN - Source Position */
                 		/* IN - The name */
                    		/* IN - TRUE if single class */
                     		/* IN - Allowed classes */
                		/* IN - Statements to execute otherwise */
#else
ResNod *newres(srcp, nam, single, classes, stms)
     Srcp *srcp;		/* IN - Source Position */
     NamNod *nam;		/* IN - The name */
     Boolean single;		/* IN - TRUE if single class */
     NamKind classes;		/* IN - Allowed classes */
     List *stms;		/* IN - Statements to execute otherwise */
#endif
{
  ResNod *new;			/* The newly created node */

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
#ifdef _PROTOTYPES_
static void anres(
     ResNod *res,		/* IN - Restriction node to analyze */
     List *params		/* IN - Possible syntax parameters */
)
#else
static void anres(res, params)
     ResNod *res;		/* IN - Restriction node to analyze */
     List *params;		/* IN - Possible syntax parameters */
#endif
{
  Boolean found = FALSE;
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
#ifdef _PROTOTYPES_
void anress(
    List *ress,			/* IN - List of nodes to analyze */
    List *params		/* IN - Possible syntax parameters */
)
#else
void anress(ress, params)
     List *ress;		/* IN - List of nodes to analyze */
     List *params;		/* IN - Possible syntax parameters */
#endif
{
  List *lst;

  for (lst = ress; lst != NULL; lst = lst->next)
    anres(lst->element.res, params);
}



/*----------------------------------------------------------------------

  geres()

  Generate code for one syntax class restriction node.

 */
#ifdef _PROTOTYPES_
static void geres(ResNod *res)
                 		/* IN - Node to generate */
#else
static void geres(res)
     ResNod *res;		/* IN - Node to generate */
#endif
{
  res->stmadr = emadr();
  gestms(res->stms, NULL);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geresent()

  Generate an entry for one restriction node.

 */
#ifdef _PROTOTYPES_
static void geresent(ResNod *res)
                 		/* IN - Node to generate */
#else
static void geresent(res)
     ResNod *res;		/* IN - Node to generate */
#endif
{
  emit(res->nam->code);
  emit(res->classes);
  emit(res->stmadr);
}



/*======================================================================

  geress()

  Generate the data structure for the element class restrictions.

 */
#ifdef _PROTOTYPES_
Aaddr geress(List *ress, StxNod *stx)
                		/* IN - The element class restriction nodes */
                 		/* IN - Syntax node containing the res */
#else
Aaddr geress(ress, stx)
     List *ress;		/* IN - The element class restriction nodes */
     StxNod *stx;		/* IN - Syntax node containing the res */
#endif
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
#ifdef _PROTOTYPES_
void dures(ResNod *res)
#else
void dures(res)
     ResNod *res;
#endif
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
