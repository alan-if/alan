/*----------------------------------------------------------------------*\

				SYN.C
			    Synonym Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lst_x.h"
#include "adv_x.h"
#include "wrd_x.h"

#include "lmList.h"
#include "../interpreter/acode.h"

#include "id_x.h"
#include "syn.h"		/* SYN-nodes */

#include "emit.h"
#include "dump.h"



/*======================================================================

  newsyn()

  Create a new synonym node.

  */
SynNod *newsyn(Srcp *srcp,	/* IN - Source position of the synonym */
	       List *ids,	/* IN - List of synonyms */
	       IdNode *id)	/* IN - For the target name */
{
  SynNod *new;

  showProgress();

  new = NEW(SynNod);

  new->srcp = *srcp;
  new->ids = ids;
  new->id = id;

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
  WordNode *wrd;		/* Target word */
  WordNode *swrd;		/* Synonym word */

  for (lst = adv.syns; lst != NULL; lst = lst->next) {
    showProgress();
    wrd = findWord(lst->element.syn->id->string);
    if (wrd == NULL)		/* Couldn't find target word */
      lmLog(&lst->element.syn->id->srcp, 321, sevWAR, lst->element.syn->id->string);
    else
      for (slst = lst->element.syn->ids; slst != NULL; slst = slst->next) {
	/* Look up the synonym */
        swrd = findWord(slst->element.id->string);
        if (swrd != NULL && (swrd->classbits&(1L<<SYNONYM_WORD)))
	  lmLog(&slst->element.id->srcp, 322, sevWAR, slst->element.id->string);
	else
          newWord(slst->element.id->string, SYNONYM_WORD, 0, (void *) wrd);
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

  put("SYN: "); dumpSrcp(&syn->srcp); in();
  put("id: "); dumpId(syn->id); nl();
  put("ids: "); dumpList(syn->ids, ID_LIST); out();
}
