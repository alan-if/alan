/*----------------------------------------------------------------------*\

				LST.C
			Generic lists handling

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"

#include "lst.h"		/* LST-nodes */
#include "nam.h"                /* NAM-nodes */

#include "act.h"
#include "alt.h"
#include "atr.h"
#include "chk.h"
#include "res.h"
#include "cnt.h"
#include "elm.h"
#include "evt.h"
#include "exp.h"
#include "ext.h"
#include "lim.h"
#include "loc.h"
#include "obj.h"
#include "rul.h"
#include "scr.h"
#include "stm.h"
#include "stp.h"
#include "stx.h"
#include "syn.h"
#include "vrb.h"

#include "dump.h"




/*======================================================================

  concat()

  Generic list concatenation.

  */
#ifdef _PROTOTYPES_
List *concat(List *list, void *element)
                		/* IN - List to concat to */
                   		/* IN - Pointer to any element type */
#else
List *concat(list, element)
     List *list;		/* IN - List to concat to */
     void *element;		/* IN - Pointer to any element type */
#endif
{
  List *new;			/* The newly created list node */

  if (element == NULL) return(list);

  new = NEW(List);

  new->element.nam = (NamNod *) element;

  new->next = NULL;
  if (list == NULL) {
    new->tail = new;		/* This node is tail */
    return(new);
  } else {
    list->tail->next = new;	/* Concat at end of list */
    list->tail = new;		/* New node is tail */
    return(list);
  }
}



/*======================================================================

  combine()

  Generic list combination.

  */
#ifdef _PROTOTYPES_
List *combine(List *list1, List *list2)
                         	/* IN - Lists to combine */
#else
List *combine(list1, list2)
     List *list1, *list2;	/* IN - Lists to combine */
#endif
{
  if (list1 == NULL) return(list2);
  if (list2 == NULL) return(list1);

  list1->tail->next = list2;	/* Combine at end of list1 */
  list1->tail = list2->tail;	/* Tail of list2 is tail */
  return(list1);
}



/*----------------------------------------------------------------------

  dunod()

  Dump a particular node.

  */
#ifdef _PROTOTYPES_
static void dunod(void *nod, NodKind class)
#else
static void dunod(nod, class)
    void *nod;
    NodKind class;
#endif
{
  switch (class){
  case ACTNOD: duact(nod); break;
  case ALTNOD: dualt(nod); break;
  case ATRNOD: duatr(nod); break;
  case CHKNOD: duchk(nod); break;
  case RESNOD: dures(nod); break;
  case CNTNOD: ducnt(nod); break;
  case ELMNOD: duelm(nod); break;
  case EVTNOD: duevt(nod); break;
  case EXPNOD: duexp(nod); break;
  case EXTNOD: duext(nod); break;
  case LIMNOD: dulim(nod); break;
  case LOCNOD: duloc(nod); break;
  case NAMNOD: dunam(nod); break;
  case OBJNOD: duobj(nod); break;
  case RULNOD: durul(nod); break;
  case SCRNOD: duscr(nod); break;
  case STMNOD: dustm(nod); break;
  case STPNOD: dustp(nod); break;
  case STXNOD: dustx(nod); break;
  case SYNNOD: dusyn(nod); break;
  case VRBNOD: duvrb(nod); break;
  default: put("Not implemented in DUMP."); nl(); break;
  }
}



/*======================================================================

  dulst()

  Dump a list of nodes

  */
#ifdef _PROTOTYPES_
void dulst(List *lst, NodKind class)
               			/* IN - The list */
                   		/* IN - Class of the elements */
#else
void dulst(lst, class)
     List *lst;			/* IN - The list */
     NodKind class;		/* IN - Class of the elements */
#endif
{
  if (lst == NULL) {
    put("NULL");
    return;
  }
  
  put("LST: "); duptr(lst); in();
  while (lst != NULL) {
    dunod((void *)lst->element.atr, class);
    lst = lst->next;
    if (lst != NULL) nl();
  }
  out();
}


/*======================================================================

  dulst2()

  Dump a list of lists of nodes

  */
#ifdef _PROTOTYPES_
void dulst2(List *lstlst, NodKind class)
               			/* IN - The list of lists*/
                   		/* IN - Class of the elements */
#else
void dulst2(lstlst, class)
     List *lstlst;			/* IN - The list */
     NodKind class;		/* IN - Class of the elements */
#endif
{
  if (lstlst == NULL) {
    put("NULL");
    return;
  }
  
  put("LST: "); duptr(lstlst); in();
  while (lstlst != NULL) {
    dulst((void *)lstlst->element.lst, class);
    lstlst = lstlst->next;
    if (lstlst != NULL) nl();
  }
  out();
}


