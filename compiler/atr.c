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


/* Exported data: */

int atrmax;



/*======================================================================

  newatr()

  Allocates and initialises an atrnod.

 */
AtrNod *newatr(Srcp *srcp,	/* IN - Source Position */
	       TypeKind typ,	/* IN - Type of this atribute */
	       NamNod *nam,	/* IN - The name */
	       int val,		/* IN - The initial value */
	       long int fpos,	/* IN - File position for initial string */
	       int len)		/* IN - D:o length */
{
  AtrNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

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
AtrNod *findatr(char *atr,	/* IN - name of attribute to find */
		List *atrlst,	/* IN - attribute list */
		List *defatrs)	/* IN - default attributes to search also */
{
  List *lst;

  for (lst = atrlst; lst != NULL; lst = lst->next)
    if (strcmp(lst->element.atr->nam->str, atr) == 0)
      return (lst->element.atr);

  for (lst = defatrs; lst != NULL; lst = lst->next)
    if (strcmp(lst->element.atr->nam->str, atr) == 0)
      return (lst->element.atr);

  return(NULL);
}



/*======================================================================

  paramatr()

  Verify the existence of a attribute for a particular parameter.

  */
AtrNod *paramatr(NamNod *nam, ElmNod *elm)
{
  AtrNod *atr = NULL;

  if (elm->res == NULL || elm->res->single) {
    /* No restriction (default = OBJECT) or explicit single class! */
    if (elm->res == NULL || (elm->res->classes & NAMOBJ) != 0 || (elm->res->classes & NAMCOBJ) != 0)
      /* Object or Container Object! */
      atr = findatr(nam->str, adv.oatrs, adv.atrs);
    else if ((elm->res->classes & NAMACT) != 0 || (elm->res->classes & NAMCACT) != 0)
      /* Actor or Container Actor! */
      atr = findatr(nam->str, adv.aatrs, adv.atrs);
    else /* Containers, Integers and Strings have no attributes... */
      lmLog(&nam->srcp, 406, sevERR, "");
  } else
    /* Multiple classes, so can only be in general default attributes */
    atr = findatr(nam->str, adv.atrs, NULL);

  return atr;
}


/*======================================================================

  symatr()

  Verify the existence of a attribute for a symbol.

  */
AtrNod *symatr(NamNod *nam, SymNod *sym)
{
  AtrNod *atr = NULL;

  switch (sym->class) {
  case NAMOBJ:
    atr = findatr(nam->str, ((ObjNod *)sym->ref)->atrs, adv.oatrs);
    break;
  case NAMLOC:
    atr = findatr(nam->str, ((LocNod *)sym->ref)->atrs, adv.latrs);
    break;
  case NAMACT:
    atr = findatr(nam->str, ((ActNod *)sym->ref)->atrs, adv.aatrs);
    break;
  }
  if (atr == NULL) {	/* Attribute not found locally */
    /* Try general default attributes */
    atr = findatr(nam->str, adv.atrs, NULL);
  }
  return atr;
}

  

/*======================================================================

  prepatrs()

  Number all default attributes.

 */
void prepatrs(void)
{
  List *lst;			/* List pointer*/
  AtrNod *atr;			/* Found default attribute */
  
  /* Number all default attributes */
  for (lst = adv.atrs; lst != NULL; lst = lst->next)
    lst->element.atr->nam->code = ++atrmax;

  /* Actor attributes */
  aatrmax = atrmax;
  for (lst = adv.aatrs; lst != NULL; lst = lst->next) {
    if ((atr = findatr(lst->element.atr->nam->str, adv.atrs, NULL)) == NULL) {
      /* New attribute for actors */
      lst->element.atr->nam->code = ++aatrmax;
    } else {			/* Use default attribute code */
      /* Was a redefined default attribute, type check it and use same code */
      if (!eqtyp(lst->element.atr->typ, atr->typ))
	lmLogv(&lst->element.atr->srcp, 332, sevERR, "actor", "default", NULL);
      lst->element.atr->nam->code = atr->nam->code;
    }
  }

  /* Location attributes */
  latrmax = atrmax;
  for (lst = adv.latrs; lst != NULL; lst = lst->next) {
    if ((atr = findatr(lst->element.atr->nam->str, adv.atrs, NULL)) == NULL) {
      /* New attribute for locations */
      lst->element.atr->nam->code = ++latrmax;
    } else {			/* Use default attribute code */
      /* Was a redefined default attribute, type check it and use same code */
      if (!eqtyp(lst->element.atr->typ, atr->typ))
	lmLogv(&lst->element.atr->srcp, 332, sevERR, "location", "default", NULL);
      lst->element.atr->nam->code = atr->nam->code;
    }
  }

  /* Object attributes */
  oatrmax = atrmax;
  for (lst = adv.oatrs; lst != NULL; lst = lst->next) {
    if ((atr = findatr(lst->element.atr->nam->str, adv.atrs, NULL)) == NULL) {
      /* New attribute for objects */
      lst->element.atr->nam->code = ++oatrmax;
    } else {			/* Use default attribute code */
      /* Was a redefined default attribute, type check it and use same code */
      if (!eqtyp(lst->element.atr->typ, atr->typ))
	lmLogv(&lst->element.atr->srcp, 332, sevERR, "object", "default", NULL);
      lst->element.atr->nam->code = atr->nam->code;
    }
  }
}



/*======================================================================

  anatrs()

  Analyze a list of attributes.

 */
void anatrs(List *atrs)		/* IN - pointer to a pointer to the list */
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
void sortatr(List **alstp)	/* IN - pointer to a pointer to the list */
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
static void geatr(AtrNod *atr)	/* IN - Attribute to generate for */
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
    adv.stratrs = concat(adv.stratrs, new, ATRNOD);
  }
  emit(atr->val);
  emit(atr->stradr);
}


/*======================================================================

  geatrs()

  Generate all entries in an attribute list.

 */
Aword geatrs(List *atrs,	/* IN - List of attribute nodes */
	     List *datrs,	/* IN - List of class default attributes */
	     List *gatrs)	/* IN - List of general default attributes */
{
  Aaddr adr;
  List *lst, *dlst, *glst;
  int state;

  if (atrs == NULL && datrs == NULL && gatrs == NULL)
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
    if (gatrs != NULL && gatrs->element.atr->stradr == 0)
      for (lst = gatrs; lst != NULL; lst = lst->next) {
	lst->element.atr->stradr = emadr();
	emitstr(lst->element.atr->nam->str);
      }
  }

  adr = emadr();

  lst = atrs;
  dlst = datrs;
  glst = gatrs;

  /* This might look like magic but the state is simply a vector indicating
     which lists are not NULL, lst != NULL is bit 0 */
  do {
    state = (lst?1:0) | (dlst?1:0)<<1 | (glst?1:0)<<2;

    switch (state) {

      /* Single lists first */
    case 1:
      geatr(lst->element.atr);
      lst = lst->next;
      break;
    case 2:
      geatr(dlst->element.atr);
      dlst = dlst->next;
      break;
    case 4:
      geatr(glst->element.atr);
      glst = glst->next;
      break;

      /* Two lists remaining */
    case 3:			/* lst, dlst != NULL */
      if (dlst->element.atr->nam->code < lst->element.atr->nam->code) {
	/* There is a default attribute with lower number, so generate it */
	geatr(dlst->element.atr);
	dlst = dlst->next;
      } else {
	geatr(lst->element.atr);
	lst = lst->next;
	dlst = dlst->next;
      }
      break;

    case 5:			/* lst, glst != NULL */
      if (glst->element.atr->nam->code < lst->element.atr->nam->code) {
	/* There is a default attribute with lower number, so generate it */
	geatr(glst->element.atr);
	glst = glst->next;
      } else {
	geatr(lst->element.atr);
	lst = lst->next;
	glst = glst->next;
      }
      break;

    case 6:			/* dst, glst != NULL */
      if (glst->element.atr->nam->code < dlst->element.atr->nam->code) {
	/* There is a general default attribute with lower number */
	geatr(glst->element.atr);
	glst = glst->next;
      } else {
	geatr(dlst->element.atr);
	dlst = dlst->next;
	glst = glst->next;
      }
      break;

      /* And now for the most complex case, all lists != NULL */
    case 7:
      if (glst->element.atr->nam->code < dlst->element.atr->nam->code && glst->element.atr->nam->code < lst->element.atr->nam->code) {
	/* There is a general default attribute with lower number than
	   the one in the class default list and in the local list,
	   so generate it */
	geatr(glst->element.atr);
	glst = glst->next;
      } else if (dlst->element.atr->nam->code < lst->element.atr->nam->code) {
	/* There is a class default attribute with lower number than
	   the one in the local list, so generate it */
	geatr(dlst->element.atr);
	if (glst->element.atr->nam->code == dlst->element.atr->nam->code)
	  /* If the global attribute has the same number advance it too */
	  glst = glst->next;
	/* Advance the class default */
	dlst = dlst->next;
      } else {
	/* The local attribute is a local instance of some default attribute */
	geatr(lst->element.atr);
	/* If the general deafult attribute has the same number advance it */
	if (glst->element.atr->nam->code == lst->element.atr->nam->code)
	  glst = glst->next;
	/* If the class default attribute has the same number advance it */
	if (dlst->element.atr->nam->code == lst->element.atr->nam->code)
	  glst = glst->next;
	/* Advance local attribute */
	lst = lst->next;
      }
      break;

    }
  } while (state != 0);
  emit(EOF);

  return(adr);
}



/*======================================================================

  geinit()

  Dump an Attribute node.

 */
Aaddr geinit(void)
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
void duatr(AtrNod *atr)
{
  put("ATR: "); dusrcp(&atr->srcp); in();
  put("typ: "); dutyp(atr->typ); nl();
  put("nam: "); dunam(atr->nam); nl();
  put("stradr: "); duadr(atr->stradr); nl();
  put("val: "); duint(atr->val); nl();
  put("fpos: "); duint(atr->fpos); nl();
  put("len: "); duint(atr->len); out();
}
