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
	       ResKind resKd,	/* IN - Kind of restriction (ID, STRING ...) */
	       List *classes,	/* IN - Allowed classes */
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

  /* Analyse the class list */
  /* FIXME Currently we only handle one class but should be able to handle a list of classes */

  /* FIXME Look up the classes and attach them to here */
  /* For now "actor", "object" IDs are converted to their restriction kinds by the parser */

  /* Analyse the statements to execute if the restrictions was not met */
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

  geresKind()

  Generate class bit pattern for a restriction (as per v2 format)

 */
static void geresKind(ResKind kd) /* IN - Node to generate */
{
  switch (kd) {
  case INTEGER_RESTRICTION: emit(NAMNUM); break;
  case STRING_RESTRICTION: emit(NAMSTR); break;
  case OBJECT_RESTRICTION: emit(NAMOBJ); break;
  case ACTOR_RESTRICTION: emit(NAMACT); break;
  case CONTAINER_RESTRICTION: emit(NAMCNT); break;
  case CONTAINEROBJECT_RESTRICTION: emit(NAMCOBJ);  break;
  case CONTAINERACTOR_RESTRICTION: emit(NAMCACT); break;
  default: syserr("Unexpected restriction kind in geresKd()");
  }
}



/*----------------------------------------------------------------------

  geresent()

  Generate an entry for one restriction node.

 */
static void geresent(ResNod *res) /* IN - Node to generate */
{
  emit(res->nam->code);
  geresKind(res->resKd);
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


/*----------------------------------------------------------------------

  dumpRestrictionKind()

*/
static void dumpRestrictionKind(ResKind resKd)
{
  switch (resKd) {
  case ID_RESTRICTION: put("ID"); break;
  case INTEGER_RESTRICTION: put("INTEGER"); break;
  case STRING_RESTRICTION: put("STRING"); break;
  case OBJECT_RESTRICTION: put("OBJECT"); break;
  case ACTOR_RESTRICTION: put("ACTOR"); break;
  case CONTAINER_RESTRICTION: put("CONTAINER"); break;
  case CONTAINEROBJECT_RESTRICTION: put("CONTAINEROBJECT"); break;
  case CONTAINERACTOR_RESTRICTION: put("CONTAINERACTOR"); break;
  default: syserr("Unknown restriction kind in dumpRestricitionKind()");
  }
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
  put("singel: "); dumpBool(res->single); nl();
  put("resKd: "); dumpRestrictionKind(res->resKd); nl();
  put("classes: "); dulst(res->classes, NAMNOD); nl();
  put("stms: "); dulst(res->stms, STMNOD); out();
}
