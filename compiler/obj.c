/*----------------------------------------------------------------------*\

                               OBJ.C
                            Object Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "exp.h"		/* EXP-nodes */
#include "atr.h"		/* ATR-nodes */
#include "whr.h"		/* WHR-nodes */
#include "stm.h"		/* STM-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "loc.h"		/* LOC-nodes */
#include "wrd.h"		/* WRD-nodes */
#include "vrb.h"		/* VRB-nodes */

#include "emit.h"

#include "acode.h"

#include "dump.h"


/* PUBLIC: */

int oatrmax = 0;
int objmin, objmax;
int objcount = 0;



/*======================================================================

  newobj()

  Allocates and initialises an objnod.

 */
#ifdef _PROTOTYPES_
ObjNod *newobj(Srcp *srcp, NamNod *nam, List *namslst, WhrNod *whr, CntNod *props, List *atrs, List *dscr, List *art, List *ment, List *vrbs)
#else
ObjNod *newobj(srcp, nam, namslst, whr, props, atrs, dscr, art, ment, vrbs)
     Srcp *srcp;		/* IN - Source Position */
     NamNod *nam;		/* IN - The object name */
     List *namslst;		/* IN - List of adjectives and a noun */
     WhrNod *whr;		/* IN - Where initially */
     CntNod *props;		/* IN - Properties */
     List *atrs;		/* IN - Attributes */
     List *dscr,;		/* IN - And its description statements */
     List *art;			/* IN - Article statments */
     List *ment;		/* IN - Short description (mentioned) */
     List *vrbs;		/* IN - The verbs handled by the object */
#endif
{
  ObjNod *new;			/* The newly allocated area */
  SymNod *sym;
  List *lst, *lstlst;		/* List and list of list traversal ptrs */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ObjNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->namslst = namslst;
  new->whr = whr;
  new->props = props;
  new->atrs = atrs;
  new->dscr = dscr;
  new->art = art;
  new->ment = ment;
  new->vrbs = vrbs;

  sym = lookup(nam->str);
  if (sym == NULL) {
    new->nam->code = newsym(nam->str, NAMOBJ, new);
    new->nam->kind = NAMOBJ;
  } else
    redefined(&nam->srcp, sym, nam->str);

  /* Note object in the dictionary */
  if (namslst == NULL)		/* Use the object name */
    newwrd(nam->str, WRD_NOUN, new->nam->code, nam);
  else {
    for (lstlst = namslst; lstlst != NULL; lstlst = lstlst->next) {
      for (lst = lstlst->element.lst; lst->next != NULL; lst = lst->next)
	newwrd(lst->element.nam->str, WRD_ADJ, 0, nam);
      newwrd(lst->element.nam->str, WRD_NOUN, lst->element.nam->code, nam);
    }
  }

  return(new);
}




/*----------------------------------------------------------------------

  prepobjs()

  Prepare all objects. Set attribute codes and propagate
  the object code to the container.

 */
#ifdef _PROTOTYPES_
void prepobjs(void)
#else
void prepobjs()
#endif
{
  ObjNod *obj;			/* Object */
  List *olst;			/* List pointer */
  List *alst;			/* List pointer */
  AtrNod *atr;			/* Attribute node */
  int i;			/* and a counter */
  
  /* Number the local attributes for all objects */
  for (olst = adv.objs; olst != NULL; olst = olst->next) {
    obj = olst->element.obj;
    /* If it is a container set object number in cntnod */
    if (obj->props != NULL)
      obj->props->parent = obj->nam;
    anatrs(obj->atrs);
    /* Then number all attributes */
    i = oatrmax + 1;	/* First local is higher than global attrs. */
    for (alst = obj->atrs; alst != NULL; alst = alst->next) {
      atr = findatr(alst->element.atr->nam->str, adv.oatrs, adv.atrs);
      if (atr == NULL)
	/* Must be a local attribute */
	alst->element.atr->nam->code = i++;
      else {
	/* Else use default attribute code */
	alst->element.atr->nam->code = atr->nam->code;
	if (!eqtyp(alst->element.atr->typ, atr->typ))
	  lmLogv(&alst->element.atr->srcp, 332, sevERR, "local", "default", NULL);
      }
    }
    sortatr(&obj->atrs);
  }
}



/*----------------------------------------------------------------------

  anobj()

  Analyze one object.

 */
#ifdef _PROTOTYPES_
static void anobj(ObjNod *obj)
                 	/* IN - The object to analyze */
#else
static void anobj(obj)
     ObjNod *obj;	/* IN - The object to analyze */
#endif
{
  SymNod *sym;		/* Sym table entry */
  ElmNod *elm;
  long fpos;
  int len;
  StmNod *stm;


  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* Make sure there always is a short description */
  if (obj->ment == NULL) {
    /* First create list of name printing statements (use first of multiple) */
    fpos = ftell(txtfil);
    if (obj->namslst == NULL)
      len = annams(NULL, obj->nam, FALSE);
    else
      len = annams(obj->namslst->element.lst, obj->nam, FALSE);
    /* Then create a PRINT statement */
    stm = newstm(&nulsrcp, STM_PRINT);
    stm->fields.print.fpos = fpos;
    stm->fields.print.len = len;
    /* Make this a list */
    obj->ment = concat(NULL, stm);
  }

  /* Check its initial location */
  switch (obj->whr->whr) {
  case WHR_DEFAULT:
    break;
  case WHR_AT:
    if (obj->whr->wht->wht == WHT_ID)
      symcheck(&sym, &elm, obj->whr->wht->nam, NAMLOC, NAMANY, NULL);
    else
      lmLog(&obj->srcp, 414, sevERR, "object");
    break;
  case WHR_IN:
    cntcheck(obj->whr->wht, NULL);
    break;
  case WHR_HERE:
  case WHR_NEAR:
    lmLog(&obj->srcp, 414, sevERR, "object");
    break;
  default:
    syserr("Unrecognized switch in anobj()");
    break;
  }
  
  if (obj->props != NULL)
    ancnt(obj->props);
  
  anstms(obj->dscr, NULL, NULL, NULL);
  anstms(obj->art, NULL, NULL, NULL);
  anstms(obj->ment, NULL, NULL, NULL);
  anvrbs(obj->vrbs, obj, NULL);
}


/*======================================================================

  anobjs()

  Analyze all objects in a list.

 */
#ifdef _PROTOTYPES_
void anobjs(void)
#else
void anobjs()
#endif
{
  List *obj;

  for (obj = adv.objs; obj != NULL; obj = obj->next)
    anobj(obj->element.obj);
}



/*----------------------------------------------------------------------

  geobjdscrs()

  Generate the code for the descriptions of this object.

 */
#ifdef _PROTOTYPES_
static void geobjdscrs(ObjNod *obj)
                 	/* IN - The object to generate for */
#else
static void geobjdscrs(obj)
     ObjNod *obj;	/* IN - The object to generate for */
#endif
{
  if (obj->dscr != NULL) {
    obj->dscradr = emadr();
    gestms(obj->dscr, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    obj->dscradr = 0;

  /* Generate article statements */
  if (obj->art != NULL) {
    obj->artadr = emadr();
    gestms(obj->art, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    obj->artadr = 0;

  obj->mentadr = emadr();
  gestms(obj->ment, NULL);
  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geobjent()

  Generate the entry in the objlst for one object.

 */
#ifdef _PROTOTYPES_
static void geobjent(ObjNod *obj)
                 	/* IN - The object to generate */
#else
static void geobjent(obj)
     ObjNod *obj;	/* IN - The object to generate */
#endif
{
  /* Initial location or containment */
  switch (obj->whr->whr) {
  case WHR_DEFAULT:
    emit(0);
    break;
  case WHR_AT:
    gewht(obj->whr->wht);
    break;
  case WHR_IN:
    genam(obj->whr->wht->nam);
    break;
  default:
    unimpl(&obj->srcp, "Code Generator");
    return;
  }

  /* Then a TRUE for description */
  emit(TRUE);

  /* Pointer to attributes */
  emit(obj->atradr);
  
  /* Container index, if any */
  if (obj->props != NULL)
    emit(obj->props->code-1+cntmin);
  else
    emit(0);
  
  emit(obj->vrbadr);
  
  /* The descriptions */
  emit(obj->dscradr);
  emit(obj->artadr);
  emit(obj->mentadr);
}



/*----------------------------------------------------------------------

  geobj()

  Generate the code for an object.

 */
#ifdef _PROTOTYPES_
static void geobj(ObjNod *obj)
                 		/* IN - The object */
#else
static void geobj(obj)
     ObjNod *obj;		/* IN - The object */
#endif
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  obj->atradr = geatrs(obj->atrs, adv.oatrs, adv.atrs);
  geobjdscrs(obj);
  obj->vrbadr = gevrbs(obj->vrbs, NULL);
}



/*======================================================================

  geobjs()

  Generate code for all global objects.

 */
#ifdef _PROTOTYPES_
Aaddr geobjs(void)
#else
Aaddr geobjs()
#endif
{
  List *lst;		/* Traversal pointer */
  Aaddr adr;

  if (adv.objs == NULL)
    return 0;

  /* First generate action procedures etc. for all global objects */
  for (lst = adv.objs; lst != NULL; lst = lst->next)
    geobj(lst->element.obj);

  /* and then the global objlst */
  adr = emadr();
  for (lst = adv.objs; lst != NULL; lst = lst->next)
    geobjent(lst->element.obj);
  emit(EOF);
  return(adr);
}



/*======================================================================

  duobj()

  Dump an Object node.

 */
#ifdef _PROTOTYPES_
void duobj(ObjNod *obj)
#else
void duobj(obj)
     ObjNod *obj;
#endif
{
  if (obj == NULL) {
    put("NULL");
    return;
  }

  put("OBJ: "); dusrcp(&obj->srcp); in();
  put("nam: "); dunam(obj->nam); nl();
  put("namslst: "); dulst2(obj->namslst, NAMNOD); nl();
  put("whr: "); duwhr(obj->whr); nl();
  put("props: "); ducnt(obj->props); nl();
  put("atrs: "); dulst(obj->atrs, ATRNOD); nl();
  put("atradr: "); duadr(obj->atradr); nl();
  put("dscr: "); dulst(obj->dscr, STMNOD); nl();
  put("dscradr: "); duadr(obj->dscradr); nl();
  put("art: "); dulst(obj->art, STMNOD); nl();
  put("artadr: "); duadr(obj->artadr); nl();
  put("ment: "); dulst(obj->ment, STMNOD); nl();
  put("mentadr: "); duadr(obj->mentadr); nl();
  put("vrbs: "); dulst(obj->vrbs, VRBNOD); nl();
  put("vrbadr: "); duadr(obj->vrbadr); out();
}
