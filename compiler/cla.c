/*----------------------------------------------------------------------*\

                               CLA.C
			 Element Class Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "cla.h"                /* CLA-nodes */
#include "stm.h"                /* STM-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"


/* PUBLIC: */


/*======================================================================

  newcla()

  Allocates and initialises an element class control node.

 */
#ifdef _PROTOTYPES_
ClaNod *newcla(Srcp *srcp, NamNod *nam, Boolean single, NamKind classes, List *stms)
                		/* IN - Source Position */
                 		/* IN - The name */
                    		/* IN - TRUE if single class */
                     		/* IN - Allowed classes */
                		/* IN - Statements to execute otherwise */
#else
ClaNod *newcla(srcp, nam, single, classes, stms)
     Srcp *srcp;		/* IN - Source Position */
     NamNod *nam;		/* IN - The name */
     Boolean single;		/* IN - TRUE if single class */
     NamKind classes;		/* IN - Allowed classes */
     List *stms;		/* IN - Statements to execute otherwise */
#endif
{
  ClaNod *new;			/* The newly created node */

  new = NEW(ClaNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->single = single;
  new->classes = classes;
  new->stms = stms;

  return(new);
}



/*----------------------------------------------------------------------

  ancla()

  Analyzes one element class node.

 */
#ifdef _PROTOTYPES_
static void ancla(
     ClaNod *cla,		/* IN - Class node to analyze */
     List *params		/* IN - Possible syntax parameters */
)
#else
static void ancla(cla, params)
     ClaNod *cla;		/* IN - Class node to analyze */
     List *params;		/* IN - Possible syntax parameters */
#endif
{
  Boolean found = FALSE;
  List *p;

  /* Check for the id in the parameter list */
  for (p = params; p != NULL; p = p->next)
    if (eqnams(cla->nam, p->element.elm->nam)) {
      found = TRUE;
      break;
    }
  if (!found)
    lmLog(&cla->nam->srcp, 222, sevERR, cla->nam->str);
  anstms(cla->stms, NULL, NULL, params);
}



/*======================================================================

  anclas()

  Analyzes all class nodes in a list by calling the analyzer for each.

 */
#ifdef _PROTOTYPES_
void anclas(
    List *clas,			/* IN - List of classes to analyze */
    List *params		/* IN - Possible syntax parameters */
)
#else
void anclas(clas, params)
     List *clas;		/* IN - List of classes to analyze */
     List *params;		/* IN - Possible syntax parameters */
#endif
{
  List *lst;

  for (lst = clas; lst != NULL; lst = lst->next)
    ancla(lst->element.cla, params);
}



/*----------------------------------------------------------------------

  gecla()

  Generate code for one syntax class node.

 */
#ifdef _PROTOTYPES_
static void gecla(ClaNod *cla)
                 		/* IN - Element class to generate */
#else
static void gecla(cla)
     ClaNod *cla;		/* IN - Element class to generate */
#endif
{
  cla->stmadr = emadr();
  gestms(cla->stms, NULL);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geclaent()

  Generate an entry for one cla node.

 */
#ifdef _PROTOTYPES_
static void geclaent(ClaNod *cla)
                 		/* IN - Element class to generate */
#else
static void geclaent(cla)
     ClaNod *cla;		/* IN - Element class to generate */
#endif
{
  emit(cla->nam->code);
  emit(cla->classes);
  emit(cla->stmadr);
}



/*======================================================================

  geclas()

  Generate the data structure for the element class controls.

 */
#ifdef _PROTOTYPES_
Aaddr geclas(List *clas, StxNod *stx)
                		/* IN - The element class nodes */
                 		/* IN - Syntax node containing the cla */
#else
Aaddr geclas(clas, stx)
     List *clas;		/* IN - The element class nodes */
     StxNod *stx;		/* IN - Syntax node containing the cla */
#endif
{
  List *lst;
  Aaddr claadr;

  for (lst = clas; lst != NULL; lst = lst->next)
    gecla(lst->element.cla);

  claadr = emadr();
  for (lst = clas; lst != NULL; lst = lst->next)
    geclaent(lst->element.cla);

  /* End it with an End of file and the code for the verb */
  emit(EOF);
  genam(stx->nam);

  return(claadr);
}



/*======================================================================

  ducla()

  Dump a syntax element Class node.

 */
#ifdef _PROTOTYPES_
void ducla(ClaNod *cla)
#else
void ducla(cla)
     ClaNod *cla;
#endif
{
  if (cla == NULL) {
    put("NULL");
    return;
  }

  put("CLA: "); dusrcp(&cla->srcp); in();
  put("nam: "); dunam(cla->nam); nl();
  put("classes: "); if (cla->single) put("(single)"); nl();
  put("stms: "); dulst(cla->stms, STMNOD); out();
}
