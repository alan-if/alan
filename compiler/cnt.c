/*----------------------------------------------------------------------*\

				CNT.C
			   Container Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "adv.h"		/* ADV-node */
#include "sym.h"		/* SYM-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stm.h"		/* STM-nodes */
#include "elm.h"		/* ELM-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "lim.h"		/* LIM-nodes */
#include "obj.h"		/* OBJ-nodes */

#include "emit.h"

#include "dump.h"


/* PUBLIC: */

int cntmin, cntmax;
int cntcount = 0;



/*======================================================================

  newcnt()

  Allocates and initialises a cntnod.

 */
#ifdef _PROTOTYPES_
CntNod *newcnt(Srcp *srcp, NamNod *nam, List *lims, List *hstms, List *estms)
                	/* IN - Source Position */
                 	/* IN - The name of the container */
                	/* IN - Limits */
                 	/* IN - Header statements */
                 	/* IN - Else (empty) statements */
#else
CntNod *newcnt(srcp, nam, lims, hstms, estms)
     Srcp *srcp;	/* IN - Source Position */
     NamNod *nam;	/* IN - The name of the container */
     List *lims;	/* IN - Limits */
     List *hstms;	/* IN - Header statements */
     List *estms;	/* IN - Else (empty) statements */
#endif
{
  CntNod *new;		/* The newly allocated area */
  CntNod *cnt;
  SymNod *sym;

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(CntNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->lims = lims;
  new->hstms = hstms;
  new->estms = estms;

  if (nam != NULL) {
    sym = lookup(nam->str);
    if (sym == NULL) {
      new->nam->code = newsym(nam->str, NAMCNT, new);
      new->code = new->nam->code;
    } else if (strcmp(sym->str, "inventory") == 0) {
      cnt = (CntNod *) sym->ref;
      cnt->srcp = new->srcp;
      cnt->lims = new->lims;
      cnt->hstms = new->hstms;
      cnt->estms = new->estms;
      return(NULL);
    } else
      redefined(srcp, sym, nam->str);
  } else
    new->code = ++cntcount;

  new->parent = 0;		/* No parent yet */

  return(new);
}




/*======================================================================

  initcnt()

  Do initialisation of containers.

  */
#ifdef _PROTOTYPES_
void initcnt(void)
#else
void initcnt()
#endif
{
  adv.cnts = concat(NULL,
		    newcnt(&nulsrcp, newnam(&nulsrcp, "inventory"),
			   NULL, NULL, NULL));
}



/*======================================================================

  cntcheck()

  Check if a name is a container, if so set its code else give an
  error message.

 */
#ifdef _PROTOTYPES_
void cntcheck(WhtNod *wht, List *pars)
                 		/* IN - What to check */
                		/* IN - Possible parameters */
#else
void cntcheck(wht, pars)
     WhtNod *wht;		/* IN - What to check */
     List *pars;		/* IN - Possible parameters */
#endif
{
  SymNod *sym;			/* The symbol table node */
  ObjNod *obj;			/* Possible object node */
  ActNod *act;			/* Possible actor node */
  ElmNod *elm;			/* Syntax element */

  if (wht == NULL)
    return;

  switch (wht->wht) {
  case WHT_ID:
    symcheck(&sym, &elm, wht->nam, NAMANY, NAMCNT, pars);
    if (sym)
      switch (sym->class) {
      case NAMCNT:
	wht->nam->code = sym->code;
	break;
      case NAMOBJ:
	obj = (ObjNod *)sym->ref;
	if (obj->props == NULL) {
	  lmLog(&wht->srcp, 318, sevERR, wht->nam->str);
	  wht->nam->code = 0;
	} else
	  wht->nam->code = sym->code;
	break;
      case NAMACT:
	act = (ActNod *)sym->ref;
	if (act->props == NULL) {
	  lmLog(&wht->srcp, 318, sevERR, wht->nam->str);
	  wht->nam->code = 0;
	} else
	  wht->nam->code = sym->code;
	break;
      default:
	break;
      }
    break;

  case WHT_LOC:
  case WHT_ACT:
    lmLog(&wht->srcp, 311, sevERR, "a Container");
    break;

  case WHT_OBJ:
    /* This is a parameter make sure it is defined as a container */
    if (pars == NULL)
      lmLog(&wht->srcp, 409, sevERR, "");
    else
      if (pars->element.elm->res == NULL ||
	  (pars->element.elm->res->classes & NAMCNT) ||
	  (pars->element.elm->res->classes & NAMCOBJ) ||
	  (pars->element.elm->res->classes & NAMCACT))
	lmLog(&wht->srcp, 312, sevERR, "a Container");
    break;

  default:
    syserr("Unrecognized switch in cntcheck()");
    break;
  }
}



/*======================================================================

  ancnt()

  Analyze one container.

  */
#ifdef _PROTOTYPES_
void ancnt(CntNod *cnt)
                		/* IN - The container to analyze */
#else
void ancnt(cnt)
    CntNod *cnt;		/* IN - The container to analyze */
#endif
{
  long fpos;			/* File position of name text */
  int len;			/* and length */
  StmNod *stm;			/* Name printing statement */
  List *lims;			/* List of limits */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  if (cnt->parent == NULL) {	/* No parent object? */
    /* So it needs name printing statements, create it */
    fpos = ftell(txtfil);
    len = annams(NULL, cnt->nam, FALSE);
    stm = newstm(&nulsrcp, STM_PRINT);
    stm->fields.print.fpos = fpos;
    stm->fields.print.len = len;
    cnt->namstms = concat(NULL, stm);
  }

  /* Analyze the limits */
  for (lims = cnt->lims; lims != NULL; lims = lims->next)
    anlim(lims->element.lim);

  /* Analyze header and empty statments */
  anstms(cnt->hstms, NULL, NULL, NULL);
  anstms(cnt->estms, NULL, NULL, NULL);
}


/*======================================================================

  ancnts()

  Analyze the global containers of this adventure.

  */
#ifdef _PROTOTYPES_
void ancnts(void)
#else
void ancnts()
#endif
{
  List *cnts;	/* List of containers */

  for (cnts = adv.cnts; cnts != NULL; cnts = cnts->next)
    ancnt(cnts->element.cnt);
}



/*----------------------------------------------------------------------

  gecnt()

  Generate code for one container.

  */
#ifdef _PROTOTYPES_
static void gecnt(CntNod *cnt)
                 		/* IN - The container to generate */
#else
static void gecnt(cnt)
     CntNod *cnt;		/* IN - The container to generate */
#endif
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  cnt->limadr = gelims(cnt);

  if (cnt->parent == NULL)	{	/* Save the name of the container */
    cnt->namadr = emadr();
    gestms(cnt->namstms, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    cnt->namadr = 0;

  if (cnt->hstms != NULL) {
    cnt->hadr = emadr();
    gestms(cnt->hstms, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    cnt->hadr = 0;
  
  if (cnt->estms != NULL) {
    cnt->eadr = emadr();
    gestms(cnt->estms, NULL);
    emit0(C_STMOP, I_RETURN);
  } else
    cnt->eadr = 0;
}



/*----------------------------------------------------------------------

  gecntent()

  Generate an entry in the global container list.

  */
#ifdef _PROTOTYPES_
static void gecntent(CntNod *cnt)
                 		/* IN - The container to generate entry for */
#else
static void gecntent(cnt)
     CntNod *cnt;		/* IN - The container to generate entry for */
#endif
{
  emit(cnt->limadr);
  emit(cnt->hadr);
  emit(cnt->eadr);
  if (cnt->parent != NULL)
    genam(cnt->parent);
  else
    emit(0L);
  emit(cnt->namadr);
}


/*======================================================================

  gecnts()

  Generate code for all containers.

  */
#ifdef _PROTOTYPES_
Aaddr gecnts(void)
#else
Aaddr gecnts()
#endif
{
  List *lst;			/* The list of containers */
  Aaddr adr;

  if (adv.cnts == NULL)		/* Any containers at all? */
    adr = emadr();
  else {
    /* Limits, header and empty statements for the container */
    for (lst = adv.cnts; lst != NULL; lst = lst->next)
      gecnt(lst->element.cnt);
  
    adr = emadr();		/* Save ACODE address to container list */
    /* Container list */
    for (lst = adv.cnts; lst != NULL; lst = lst->next)
      gecntent(lst->element.cnt);
  }
  emit(EOF);
  return(adr);
}



/*======================================================================

  ducnt()

  Dump a Container node.

  */
#ifdef _PROTOTYPES_
void ducnt(CntNod *cnt)
#else
void ducnt(cnt)
     CntNod *cnt;
#endif
{
  if (cnt == NULL) {
    put("NULL");
    return;
  }

  put("CNT: "); dusrcp(&cnt->srcp); in();
  put("nam: "); dunam(cnt->nam); nl();
  put("namstms: "); dulst(cnt->namstms, STMNOD); nl();
  put("namadr: "); duint(cnt->namadr); nl();
  put("code: "); duint(cnt->code); nl();
  put("parent: "); dunam(cnt->parent); nl();
  put("lims: "); dulst(cnt->lims, LIMNOD); nl();
  put("limadr: "); duint(cnt->limadr); nl();
  put("hstms: "); dulst(cnt->hstms, STMNOD); nl();
  put("hadr: "); duint(cnt->hadr); nl();
  put("estms: "); dulst(cnt->estms, STMNOD); nl();
  put("eadr: "); duint(cnt->eadr); out();
}


