/*----------------------------------------------------------------------*\

				ATR.C
			   Attribute Nodes

\*----------------------------------------------------------------------*/

#include "atr_x.h"


/* IMPORT: */
#include "srcp_x.h"
#include "id_x.h"
#include "lst_x.h"

#include "util.h"
#include "emit.h"

#include "adv.h"		/* ADV-node */
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
	       int value,	/* IN - The initial value */
	       long int fpos,	/* IN - File position for initial string */
	       int len)		/* IN - D:o length */
{
  AtrNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(AtrNod);

  new->srcp = *srcp;
  new->typ = typ;
  new->id = id;
  new->value = value;
  new->stringAddress = 0;
  new->encoded = FALSE;
  new->fpos = fpos;
  new->len = len;

  return(new);
}


/*======================================================================

  findAttribute()

*/
AtrNod *findAttribute(List *attributes, IdNode *id)
{
  List *this;

  for (this = attributes; this != NULL; this = this->next)
    if (equalId(this->element.atr->id, id))
      return this->element.atr;
  return NULL;
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

  generateAttribute()

  */
static void generateAttribute(AtrNod *attribute) /* IN - Attribute to generate for */
{
  AttributeEntry entry;
  AtrNod *new;

  if (attribute->typ == TYPSTR) {
    if (!attribute->encoded) {
      encode(&attribute->fpos, &attribute->len);
      attribute->encoded = TRUE;
    }
    attribute->address = emadr(); /* Record on which Aadress to put it */

    /* Now make a copy to use for initialisation if attribute is
       inherited, else the address will be overwritten by generation
       of other instances of the same attribute */
    new = newatr(&attribute->srcp, TYPSTR, NULL, attribute->value,
		 attribute->fpos, attribute->len);
    new->address = attribute->address;
    adv.stratrs = concat(adv.stratrs, new, LIST_ATR);
  }

  entry.code = attribute->code;
  entry.value = attribute->value;
  entry.stringAddress = attribute->stringAddress;
  emitN(&entry, ACDsizeOf(entry));
}


/*======================================================================

  generateAttrributes()

  Generate all entries in an attribute list.

 */
Aword generateAttributes(List *atrs) /* IN - List of attribute nodes */
{
  Aaddr adr;
  List *lst;

  /* First generate the names of the attributes if needed */ 
  if ((Bool) opts[OPTDEBUG].value) {
    for (lst = atrs; lst != NULL; lst = lst->next) {
      lst->element.atr->stringAddress = emadr();
      emitstr(lst->element.atr->id->string);
    }
  }

  adr = emadr();

  for (lst = atrs; lst != NULL; lst = lst->next)
    generateAttribute(lst->element.atr);
  emit(EOF);

  return(adr);
}



/*======================================================================

  generateStringInit()

  Generate initialisation value table for string attributes.

 */
Aaddr generateStringInit(void)
{
  List *atrs;
  Aaddr adr = emadr();

  for (atrs = adv.stratrs; atrs != NULL; atrs = atrs->next) {
    emit(atrs->element.atr->fpos);
    emit(atrs->element.atr->len);
    emit(atrs->element.atr->address);
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
  put("stringAddress: "); duadr(atr->stringAddress); nl();
  put("address: "); duadr(atr->address); nl();
  put("value: "); duint(atr->value); nl();
  put("fpos: "); duint(atr->fpos); nl();
  put("len: "); duint(atr->len); out();
}
