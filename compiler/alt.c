/*----------------------------------------------------------------------*\

                               ALT.C
		       Verb Alternatives Nodes

\*----------------------------------------------------------------------*/

#include "util.h"

#include "srcp.h"
#include "lmList.h"

#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "alt.h"                /* ALT-nodes */
#include "chk.h"                /* CHK-nodes */
#include "stm.h"                /* STM-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"


/* PUBLIC: */


/*======================================================================

  newalt()

  Allocates and initialises a syntax element node.

 */
AltNod *newalt(Srcp *srcp,	/* IN - Source Position */
	       IdNode *id,	/* IN - The name */
	       List *chks,	/* IN - Checks */
	       QualKind qual,	/* IN - Action qualifier */
	       List *stms)	/* IN - Statements (does-part) */
{
  AltNod *new;			/* The newly created node */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(AltNod);

  new->srcp = *srcp;
  new->id = id;
  new->chks = chks;
  new->qual = qual;
  new->stms = stms;

  return(new);
}



/*----------------------------------------------------------------------

  analt()

  Analyzes one verb alternative node.

 */
static void analt(AltNod *alt,	/* IN - Alternative to analyze */
		  InsNod *ins,	/* IN - Possibly inside an Instance? */
		  List *pars)	/* IN - Possible parameters */
{
  List *lst;

  if (alt->id != NULL) {
    /* Alternatives given, find out which one */
    for (lst = pars; lst != NULL; lst = lst->next)
      if (equalId(lst->element.elm->id, alt->id))
	break;
    if (lst == NULL)
      lmLog(&alt->id->srcp, 214, sevERR, alt->id->string);
    else {
#ifndef FIXME
      syserr("UNIMPLEMENTED: analt");
#else
      alt->id->kind = NAMPAR;
      alt->id->code = lst->element.elm->nam->code;
#endif
    }
  } 

  anchks(alt->chks, ins, pars);
  anstms(alt->stms, ins, NULL, pars);
}



/*======================================================================

  analts()

  Analyzes all alternatives in a list by calling the analyzer for
  each.

 */
void analts(List *alts,		/* IN - List of alternative to analyze */
	    InsNod *ins,	/* IN - Possibly inside Instance? */
	    List *pars)		/* IN - Possible parameter list */
{
  List *lst;

  for (lst = alts; lst != NULL; lst = lst->next)
    analt(lst->element.alt, ins, pars);
}



/*----------------------------------------------------------------------

  gealtent()

  Generate an entry in an alt-table
  
  */
static void gealtent(AltNod *alt) /* IN - The alt to make an entry for */
{
  emit(0);			/* Auto-Reverse flag */

  if (alt->id != NULL)
    emit(alt->parameterNumber);
  else
    emit(0);

  switch (alt->qual) {
  case QUAL_BEFORE:
    emit(Q_BEFORE);
    break;
    
  case QUAL_AFTER:
    emit(Q_AFTER);
    break;
    
  case QUAL_ONLY:
    emit(Q_ONLY);
    break;
    
  case QUAL_DEFAULT:
    emit(Q_DEFAULT);
    break;
    
  default:
    syserr("Unrecognized switch in gealtent()");
    break;
  }

  emit(alt->chkadr);
  emit(alt->stmadr);
}


/*----------------------------------------------------------------------

  gealt()

  Generate code for one syntax element node.

 */
static void gealt(AltNod *alt,	/* IN - Syntax element to generate */
		  InsNod *ins)	/* IN - Inside any Instance? */
{
  /* First the action, if there is any */
  if (alt->stms == NULL)
    alt->stmadr = 0;
  else {
    alt->stmadr = emadr();
    gestms(alt->stms, ins);
    emit0(C_STMOP, I_RETURN);
  }
  
  /* Then possible CHECKs */
  if (alt->chks == NULL)
    alt->chkadr = 0;
  else
    alt->chkadr = gechks(alt->chks, ins);
}



/*======================================================================

  gealts()

  Generate the data structure for the syntax elements.

 */
Aaddr gealts(List *alts,	/* IN - The elements */
	     InsNod *ins)	/* IN - Inside any Instance? */
{
  List *lst;
  Aaddr altadr;

  for (lst = alts; lst != NULL; lst = lst->next)
    gealt(lst->element.alt, ins);

  altadr = emadr();
  for (lst = alts; lst != NULL; lst = lst->next)
    gealtent(lst->element.alt);
  emit(EOF);

  return(altadr);
}



/*----------------------------------------------------------------------

  duqual()

  Dump a verb qualifier.

  */
static void duqual (QualKind qual)
{
  switch (qual) {
  case QUAL_AFTER:   put("AFTER"); break;
  case QUAL_BEFORE:  put("BEFORE"); break;
  case QUAL_ONLY:    put("ONLY"); break;
  case QUAL_DEFAULT: put("DEFAULT"); break;
  default: put("*** ERROR ***");
  }
}



/*======================================================================

  dualt()

  Dump a verb Alternative node.

 */
void dualt(AltNod *alt)
{
  if (alt == NULL) {
    put("NULL");
    return;
  }

  put("ALT: "); dumpSrcp(&alt->srcp); in();
  put("id: "); dumpId(alt->id); nl();
  put("qual: "); duqual(alt->qual); nl();
  put("chks: "); dulst(alt->chks, LIST_CHK); nl();
  put("stms: "); dulst(alt->stms, LIST_STM); out();
}
