/*----------------------------------------------------------------------*\

				ATR.C
			   Attribute Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "adv.h"		/* ADV-node */
#include "lst.h"		/* LST-nodes */
#include "atr.h"		/* ATR-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "loc.h"		/* LOC-nodes */
#include "act.h"		/* ACT-nodes */
#include "opt.h"		/* OPT-nodes */

#include "acode.h"

#include "lmList.h"
#include "encode.h"
#include "dump.h"




/*======================================================================

  newatr()

  Allocates and initialises an atrnod.

 */
#ifdef _PROTOTYPES_
AtrNod *newatr(Srcp *srcp, TypeKind typ, NamNod *nam, int val, long int fpos, int len)
                		/* IN - Source Position */
                  		/* IN - Type of this atribute */
                 		/* IN - The name */
             			/* IN - The initial value */
               			/* IN - File position for initial string */
             			/* IN - D:o length */
#else
AtrNod *newatr(srcp, typ, nam, val, fpos, len)
     Srcp *srcp;		/* IN - Source Position */
     TypeKind typ;		/* IN - Type of this atribute */
     NamNod *nam;		/* IN - The name */
     int val;			/* IN - The initial value */
     long fpos;			/* IN - File position for initial string */
     int len;			/* IN - D:o length */
#endif

{
  AtrNod *new;			/* The newly allocated area */

  new = NEW(AtrNod);

  new->srcp = *srcp;
  new->typ = typ;
  new->nam = nam;
  new->val = val;
  new->stradr = 0;
  new->encoded = FALSE;
  new->fpos = fpos;
  new->len = len;

  return(new);
}


/*======================================================================

  findatr()

  Finds an attribute node in a supplied list by its name.

 */
#ifdef _PROTOTYPES_
AtrNod *findatr(char *atr, List *atrlst)
                		/* IN - name of attribute to find */
                  		/* IN - attribute list */
#else
AtrNod *findatr(atr, atrlst)
     char atr[];		/* IN - name of attribute to find */
     List *atrlst;		/* IN - attribute list */
#endif
{
  List *lst;

  for (lst = atrlst; lst != NULL; lst = lst->next)
    if (strcmp(lst->element.atr->nam->str, atr) == 0)
      return (lst->element.atr);

  return(NULL);
}



/*======================================================================

  prepatrs()

  Number all default attributes.

 */
#ifdef _PROTOTYPES_
void prepatrs(void)
#else
void prepatrs()
#endif
{
  List *lst;       /* List pointer*/
  
  /* Number all default attributes */
  for (lst = adv.aatrs; lst != NULL; lst = lst->next)
    lst->element.atr->nam->code = ++aatrmax;
  for (lst = adv.latrs; lst != NULL; lst = lst->next)
    lst->element.atr->nam->code = ++latrmax;
  for (lst = adv.oatrs; lst != NULL; lst = lst->next)
    lst->element.atr->nam->code = ++oatrmax;
}



/*======================================================================

  sortatr()

  Analyze a list of attributes.

 */
#ifdef _PROTOTYPES_
void anatrs(List *atrs)
                  		/* IN - pointer to a pointer to the list */
#else
void anatrs(atrs)
     List *atrs;		/* IN - pointer to a pointer to the list */
#endif
{
  List *al;

  while (atrs) {
    for (al = atrs->next; al; al = al->next) {
      if (eqnams(atrs->element.atr->nam, al->element.atr->nam))
	  lmLog(&al->element.atr->nam->srcp, 218, sevERR, al->element.atr->nam->str);
    }
    atrs = atrs->next;
  }
}




/*======================================================================

  sortatr()

  Sort a list of attributes.

 */
#ifdef _PROTOTYPES_
void sortatr(List **alstp)
                  		/* IN - pointer to a pointer to the list */
#else
void sortatr(alstp)
     List **alstp;		/* IN - pointer to a pointer to the list */
#endif
{
  Bool change;		/* Change during sorting */
  List **lstp;			/* Pointer to a list pointer */
  List *tmp1, *tmp2;		/* Temporary pointers */

  if (*alstp != NULL) {
    change = TRUE;
    while (change) {
      change = FALSE;
      for (lstp = alstp; (*lstp)->next != NULL; lstp = &(*lstp)->next) {
	tmp1 = *lstp;
	tmp2 = tmp1->next;
	if (tmp1->element.atr->nam->code > tmp2->element.atr->nam->code){
	  change = TRUE;
	  tmp1->next = tmp2->next;
	  tmp2->next = tmp1;
	  *lstp = tmp2;
	}
      }
    }
  }
}


/*----------------------------------------------------------------------

  geatr()

  */
#ifdef _PROTOTYPES_
static void geatr(AtrNod *atr)
                 		/* IN - Attribute to generate for */
#else
static void geatr(atr)
     AtrNod *atr;		/* IN - Attribute to generate for */
#endif
{
  AtrNod *new;

  if (atr->typ == TYPSTR) {
    if (!atr->encoded) {
      encode(&atr->fpos, &atr->len);
      atr->encoded = TRUE;
    }
    atr->adr = emadr();		/* Remember where to put it */
    /* Now make a copy to use for initialisation if attribute is default */
    new = newatr(&atr->srcp, TYPSTR, NULL, atr->val, atr->fpos, atr->len);
    new->adr = atr->adr;
    adv.stratrs = concat(adv.stratrs, new);
  }
  emit(atr->val);
  emit(atr->stradr);
}


/*======================================================================

  geatrs()

  Generate all entries in an attribute list.

 */
#ifdef _PROTOTYPES_
Aword geatrs(List *atrs, List *datrs)
                	/* IN - List of attribute nodes */
                 	/* IN - List of default attributes */
#else
Aword geatrs(atrs, datrs)
     List *atrs;	/* IN - List of attribute nodes */
     List *datrs;	/* IN - List of default attributes */
#endif
{
  Aaddr adr;
  List *lst, *dlst;

  if (atrs == NULL && datrs == NULL)
    return(0);

  /* First generate the names of the attributes if needed */ 
  if ((Bool) opts[OPTDEBUG].value) {
    for (lst = atrs; lst != NULL; lst = lst->next) {
      lst->element.atr->stradr = emadr();
      emitstr(lst->element.atr->nam->str);
    }
    if (datrs != NULL && datrs->element.atr->stradr == 0)
      for (lst = datrs; lst != NULL; lst = lst->next) {
	lst->element.atr->stradr = emadr();
	emitstr(lst->element.atr->nam->str);
    }
  }

  adr = emadr();

  lst = atrs;
  dlst = datrs;
  while (lst != NULL || dlst != NULL) {
    if (dlst == NULL) {
      geatr(lst->element.atr);
      lst = lst->next;
    } else if (lst == NULL) {
      geatr(dlst->element.atr);
      dlst = dlst->next;
    } else if (dlst->element.atr->nam->code < lst->element.atr->nam->code) {
      geatr(dlst->element.atr);
      dlst = dlst->next;
    } else {
      geatr(lst->element.atr);
      lst = lst->next;
      dlst = dlst->next;
    }
  }
  emit(EOF);

  return(adr);
}



/*======================================================================

  geinit()

  Dump an Attribute node.

 */
#ifdef _PROTOTYPES_
Aaddr geinit(void)
#else
Aaddr geinit()
#endif
{
  List *atrs;
  Aaddr adr = emadr();

  for (atrs = adv.stratrs; atrs != NULL; atrs = atrs->next) {
    emit(atrs->element.atr->fpos);
    emit(atrs->element.atr->len);
    emit(atrs->element.atr->adr);
  }
  emit(EOF);
  return adr;
}



/*======================================================================

  duatr()

  Dump an Attribute node.

 */
#ifdef _PROTOTYPES_
void duatr(AtrNod *atr)
#else
void duatr(atr)
     AtrNod *atr;
#endif
{
  put("ATR: "); dusrcp(&atr->srcp); in();
  put("typ: "); dutyp(atr->typ); nl();
  put("nam: "); dunam(atr->nam); nl();
  put("stradr: "); duint(atr->stradr); nl();
  put("val: "); duint(atr->val); nl();
  put("fpos: "); duint(atr->fpos); nl();
  put("len: "); duint(atr->len); out();
}
