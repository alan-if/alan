/*----------------------------------------------------------------------*\

				ATR.C
			   Attribute Nodes

\*----------------------------------------------------------------------*/

#include "util.h"

#include "adv.h"		/* ADV-node */
#include "lst.h"		/* LST-nodes */
#include "atr.h"		/* ATR-nodes */
#include "ins.h"		/* INS-nodes */
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
	       IdNode *id,	/* IN - The id */
	       int val,		/* IN - The initial value */
	       long int fpos,	/* IN - File position for initial string */
	       int len)		/* IN - D:o length */
{
  AtrNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(AtrNod);

  new->srcp = *srcp;
  new->typ = typ;
  new->id = id;
  new->val = val;
  new->stradr = 0;
  new->encoded = FALSE;
  new->fpos = fpos;
  new->len = len;

  return(new);
}


/*======================================================================

  findatr()

  Finds an attribute node in a supplied list by its id.

 */
AtrNod *findatr(char *atr,	/* IN - name of attribute to find */
		List *atrlst,	/* IN - attribute list */
		List *defatrs)	/* IN - default attributes to search also */
{
  List *lst;

  for (lst = atrlst; lst != NULL; lst = lst->next)
    if (strcmp(lst->element.atr->id->string, atr) == 0)
      return (lst->element.atr);

  for (lst = defatrs; lst != NULL; lst = lst->next)
    if (strcmp(lst->element.atr->id->string, atr) == 0)
      return (lst->element.atr);

  return(NULL);
}



/*======================================================================

  paramatr()

  Verify the existence of an attribute for a particular parameter.

  */
AtrNod *paramatr(IdNode *id, ElmNod *elm)
{
  AtrNod *atr = NULL;

#ifndef FIXME
      syserr("UNIMPLEMENTED: paramatr()");
#else
  if (elm->res == NULL || elm->res->single) {
    /* No restriction (default = OBJECT) or explicit single class! */
    if (elm->res == NULL || (elm->res->classbits & NAMOBJ) != 0 || (elm->res->classbits & NAMCOBJ) != 0)
      /* Object or Container Object! */
      atr = findatr(id->string, adv.oatrs, adv.atrs);
    else if ((elm->res->classbits & NAMACT) != 0 || (elm->res->classbits & NAMCACT) != 0)
      /* Actor or Container Actor! */
      atr = findatr(id->string, adv.aatrs, adv.atrs);
    else /* Containers, Integers and Strings have no attributes... */
      lmLog(&id->srcp, 406, sevERR, "");
  } else
    /* Multiple classes, so can only be in general default attributes */
    atr = findatr(id->string, adv.atrs, NULL);
#endif

  return atr;
}


/*======================================================================

  symatr()

  Verify the existence of an attribute for a symbol.

  */
AtrNod *symatr(IdNode *id, SymNod *sym)
{
  AtrNod *atr = NULL;

#ifndef FIXME
  syserr("UNIMPLEMENTED: symatr() - traverse class hierarchy");
#else
  switch (sym->kind) {
  case NAMOBJ:
    atr = findatr(id->string, ((ObjNod *)sym->ref)->atrs, adv.oatrs);
    break;
  case NAMLOC:
    atr = findatr(id->string, ((LocNod *)sym->ref)->atrs, adv.latrs);
    break;
  case NAMACT:
    atr = findatr(id->string, ((ActNod *)sym->ref)->atrs, adv.aatrs);
    break;
  }
  if (atr == NULL) {	/* Attribute not found locally */
    /* Try general default attributes */
    atr = findatr(id->string, adv.atrs, NULL);
  }
#endif
  return atr;
}

  

/*======================================================================

  sortatr()

  Sort a list of attributes.

 */
void sortatr(List **alstp)	/* IN - pointer to a pointer to the list */
{
  Bool change;			/* Change during sorting */
  List **lstp;			/* Pointer to a list pointer */
  List *tmp1, *tmp2;		/* Temporary pointers */

  if (*alstp != NULL) {
    change = TRUE;
    while (change) {
      change = FALSE;
      for (lstp = alstp; (*lstp)->next != NULL; lstp = &(*lstp)->next) {
	tmp1 = *lstp;
	tmp2 = tmp1->next;
	if (tmp1->element.atr->id->symbol->code > tmp2->element.atr->id->symbol->code){
	  change = TRUE;
	  tmp1->next = tmp2->next;
	  tmp2->next = tmp1;
	  *lstp = tmp2;
	}
      }
    }
  }
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
    lst->element.atr->id->symbol->code = ++atrmax;

#ifdef FIXME
  /* Actor attributes */
  aatrmax = atrmax;
  for (lst = adv.aatrs; lst != NULL; lst = lst->next) {
    if ((atr = findatr(lst->element.atr->id->string, adv.atrs, NULL)) == NULL) {
      /* New attribute for actors */
      lst->element.atr->id->symbol->code = ++aatrmax;
    } else {			/* Use default attribute code */
      /* Was a redefined default attribute, type check it and use same code */
      if (!eqtyp(lst->element.atr->typ, atr->typ))
	lmLogv(&lst->element.atr->srcp, 332, sevERR, "actor", "default", NULL);
      lst->element.atr->id->symbol->code = atr->id->symbol->code;
    }
  }
  sortatr(&adv.aatrs);

  /* Location attributes */
  latrmax = atrmax;
  for (lst = adv.latrs; lst != NULL; lst = lst->next) {
    if ((atr = findatr(lst->element.atr->id->string, adv.atrs, NULL)) == NULL) {
      /* New attribute for locations */
      lst->element.atr->id->symbol->code = ++latrmax;
    } else {			/* Use default attribute code */
      /* Was a redefined default attribute, type check it and use same code */
      if (!eqtyp(lst->element.atr->typ, atr->typ))
	lmLogv(&lst->element.atr->srcp, 332, sevERR, "location", "default", NULL);
      lst->element.atr->id->symbol->code = atr->id->symbol->code;
    }
  }
  sortatr(&adv.latrs);

  /* Object attributes */
  oatrmax = atrmax;
  for (lst = adv.oatrs; lst != NULL; lst = lst->next) {
    if ((atr = findatr(lst->element.atr->id->string, adv.atrs, NULL)) == NULL) {
      /* New attribute for objects */
      lst->element.atr->id->symbol->code = ++oatrmax;
    } else {			/* Use default attribute code */
      /* Was a redefined default attribute, type check it and use same code */
      if (!eqtyp(lst->element.atr->typ, atr->typ))
	lmLogv(&lst->element.atr->srcp, 332, sevERR, "object", "default", NULL);
      lst->element.atr->id->symbol->code = atr->id->symbol->code;
    }
  }
  sortatr(&adv.oatrs);
#endif
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
      if (equalId(atrs->element.atr->id, al->element.atr->id))
	  lmLog(&al->element.atr->id->srcp, 218, sevERR, al->element.atr->id->string);
    }
    atrs = atrs->next;
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
    adv.stratrs = concat(adv.stratrs, new, LIST_ATR);
  }
  emit(atr->val);
  emit(atr->stradr);
}


/*======================================================================

  geatrs()

  Generate all entries in an attribute list.

 */
Aword geatrs(List *atrs,        /* IN - List of attribute nodes */
             List *datrs,       /* IN - List of class default attributes */
             List *gatrs)       /* IN - List of general default attributes */
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
      emitstr(lst->element.atr->id->string);
    }
    if (datrs != NULL && datrs->element.atr->stradr == 0)
      for (lst = datrs; lst != NULL; lst = lst->next) {
        lst->element.atr->stradr = emadr();
        emitstr(lst->element.atr->id->string);
      }
    if (gatrs != NULL && gatrs->element.atr->stradr == 0)
      for (lst = gatrs; lst != NULL; lst = lst->next) {
        lst->element.atr->stradr = emadr();
        emitstr(lst->element.atr->id->string);
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
    case 3:                     /* lst, dlst != NULL */
      if (dlst->element.atr->id->symbol->code < lst->element.atr->id->symbol->code) {
        /* There is a default attribute with lower number, so generate it */
        geatr(dlst->element.atr);
        dlst = dlst->next;
      } else {
        geatr(lst->element.atr);
        lst = lst->next;
        dlst = dlst->next;
      }
      break;

    case 5:                     /* lst, glst != NULL */
      if (glst->element.atr->id->symbol->code < lst->element.atr->id->symbol->code) {
        /* There is a default attribute with lower number, so generate it */
        geatr(glst->element.atr);
        glst = glst->next;
      } else {
        geatr(lst->element.atr);
        lst = lst->next;
        glst = glst->next;
      }
      break;

    case 6:                     /* dst, glst != NULL */
      if (glst->element.atr->id->symbol->code < dlst->element.atr->id->symbol->code) {
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
      if (glst->element.atr->id->symbol->code < dlst->element.atr->id->symbol->code && glst->element.atr->id->symbol->code < lst->element.atr->id->symbol->code) {
        /* There is a general default attribute with lower number than
           the one in the class default list and in the local list,
           so generate it */
        geatr(glst->element.atr);
        glst = glst->next;
      } else if (dlst->element.atr->id->symbol->code < lst->element.atr->id->symbol->code) {
        /* There is a class default attribute with lower number than
           the one in the local list, so generate it */
        geatr(dlst->element.atr);
        if (glst->element.atr->id->symbol->code == dlst->element.atr->id->symbol->code)
          /* If the global attribute has the same number advance it too */
          glst = glst->next;
        /* Advance the class default */
        dlst = dlst->next;
      } else {
        /* The local attribute is a local instance of some default attribute */
        geatr(lst->element.atr);
        /* If the general deafult attribute has the same number advance it */
        if (glst->element.atr->id->symbol->code == lst->element.atr->id->symbol->code)
          glst = glst->next;
        /* If the class default attribute has the same number advance it */
        if (dlst->element.atr->id->symbol->code == lst->element.atr->id->symbol->code)
          dlst = dlst->next;
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

  Generate initialisation value table for string attributes.

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
  put("ATR: "); dumpSrcp(&atr->srcp); in();
  put("typ: "); dutyp(atr->typ); nl();
  put("id: "); dumpId(atr->id); nl();
  put("stradr: "); duadr(atr->stradr); nl();
  put("val: "); duint(atr->val); nl();
  put("fpos: "); duint(atr->fpos); nl();
  put("len: "); duint(atr->len); out();
}
