/*----------------------------------------------------------------------*\

				SYN.C
			    Synonym Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "lst.h"		/* LST-node */
#include "adv.h"		/* ADV-node */
#include "nam.h"		/* NAM-nodes */
#include "syn.h"		/* SYN-nodes */
#include "wrd.h"		/* WRD-nodes */

#include "emit.h"
#include "dump.h"



/*======================================================================

  newsyn()

  Create a new synonym node.

  */
SynNod *newsyn(Srcp *srcp,	/* IN - Source position of the synonym */
	       List *nams,	/* IN - List of synonyms */
	       NamNod *nam)	/* IN - For the target name */
{
  SynNod *new;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(SynNod);

  new->srcp = *srcp;
  new->nams = nams;
  new->nam = nam;

  return(new);
}



/*======================================================================

  ansyns()

  Analyze all synonyms by finding the target words in the dictionary,
  and inserting the synonyms.

  */
void ansyns(void)
{
  List *lst;		/* Traversal pointer for target list */
  List *slst;		/* Traversal pointer for synonyms lists */
  WrdNod *wrd;		/* Target word */
  WrdNod *swrd;		/* Synonym word */

  for (lst = adv.syns; lst != NULL; lst = lst->next) {
    if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }
    wrd = findwrd(lst->element.syn->nam->str);
    if (wrd == NULL)		/* Couldn't find target word */
      lmLog(&lst->element.syn->nam->srcp, 321, sevWAR, lst->element.syn->nam->str);
    else
      for (slst = lst->element.syn->nams; slst != NULL; slst = slst->next) {
	/* Look up the synonym */
        swrd = findwrd(slst->element.nam->str);
        if (swrd != NULL && (swrd->classbits&(1L<<WRD_SYN)))
	  lmLog(&slst->element.nam->srcp, 322, sevWAR, slst->element.nam->str);
	else
          newwrd(slst->element.nam->str, WRD_SYN, 0, (void *) wrd);
      }
  }
}


/*======================================================================

  dusyn()

  Dump the contents of a Synonym node.

  */
void dusyn(SynNod *syn)
{
  if (syn == NULL) {
    put("NULL");
    return;
  }

  put("SYN: "); dusrcp(&syn->srcp); in();
  put("nam: "); dunam(syn->nam); nl();
  put("nams: "); dulst(syn->nams, NAMNOD); out();
}
