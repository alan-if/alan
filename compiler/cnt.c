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
CntNod *newcnt(Srcp *srcp,	/* IN - Source Position */
	       IdNod *id,	/* IN - The name of the container */
	       List *lims,	/* IN - Limits */
	       List *hstms,	/* IN - Header statements */
	       List *estms)	/* IN - Else (empty) statements */
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
      cnt->nam = new->nam;
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
void initcnt(void)
{
  adv.cnts = concat(NULL,
		    newcnt(&nulsrcp, newnam(&nulsrcp, "inventory"),
			   NULL, NULL, NULL),
		    CNTNOD);
}



/*======================================================================

  cntcheck()

  Check if a name is a container, if so set its code else give an
  error message.

 */
void cntcheck(WhtNod *wht,	/* IN - What to check */
	      List *pars)	/* IN - Possible parameters */
{
  SymNod *sym;			/* The symbol table node */
  ObjNod *obj;			/* Possible object node */
  ActNod *act;			/* Possible actor node */
  ElmNod *elm;			/* Syntax element */

  if (wht == NULL)
    return;

  switch (wht->wht) {
  case WHT_ID:
    sym = symcheck(&elm, wht->id, INSTANCE_SYMBOL, pars);
    syserr("UNIMPL: verify container property");
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
	  (pars->element.elm->res->classbits & NAMCNT) ||
	  (pars->element.elm->res->classbits & NAMCOBJ) ||
	  (pars->element.elm->res->classbits & NAMCACT))
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
void ancnt(CntNod *cnt)		/* IN - The container to analyze */
{
  long fpos;			/* File position of name text */
  int len;			/* and length */
  StmNod *stm;			/* Name printing statement */
  List *lims;			/* List of limits */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  if (cnt->parent == NULL &&	/* No parent object? */
      cnt->nam == NULL)		/* And no name */
      /* ?!? This is probably a duplicate object container, so create a fake name */
      cnt->nam = newnam(&cnt->srcp, "$container");

  if (cnt->nam != NULL) {	/* It got it's own name... */
    /* So it needs name printing statements, create it */
    fpos = ftell(txtfil);
    len = annams(NULL, cnt->nam, FALSE);
    stm = newstm(&nulsrcp, STM_PRINT);
    stm->fields.print.fpos = fpos;
    stm->fields.print.len = len;
    cnt->namstms = concat(NULL, stm, STMNOD);
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
void ancnts(void)
{
  List *cnts;	/* List of containers */

  for (cnts = adv.cnts; cnts != NULL; cnts = cnts->next)
    ancnt(cnts->element.cnt);
}



/*----------------------------------------------------------------------

  gecnt()

  Generate code for one container.

  */
static void gecnt(CntNod *cnt)	/* IN - The container to generate */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  cnt->limadr = gelims(cnt);

  if (cnt->nam != NULL)	{	/* Save the name of the container */
    cnt->namadr = emadr();
    gestms(cnt->namstms, NULL);
    emit0(C_STMOP, I_RETURN);
  } else if (cnt->parent != NULL)
    cnt->namadr = 0;
  else
    syserr("gecnt(): A container without a name and a parent.");

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
static void gecntent(CntNod *cnt) /* IN - The container to generate entry for */
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
Aaddr gecnts(void)
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
void ducnt(CntNod *cnt)
{
  if (cnt == NULL) {
    put("NULL");
    return;
  }

  put("CNT: "); dusrcp(&cnt->srcp); in();
  put("nam: "); dunam(cnt->nam); nl();
  put("namstms: "); dulst(cnt->namstms, STMNOD); nl();
  put("namadr: "); duadr(cnt->namadr); nl();
  put("code: "); duint(cnt->code); nl();
  put("parent: "); dunam(cnt->parent); nl();
  put("lims: "); dulst(cnt->lims, LIMNOD); nl();
  put("limadr: "); duadr(cnt->limadr); nl();
  put("hstms: "); dulst(cnt->hstms, STMNOD); nl();
  put("hadr: "); duadr(cnt->hadr); nl();
  put("estms: "); dulst(cnt->estms, STMNOD); nl();
  put("eadr: "); duadr(cnt->eadr); out();
}


