/*----------------------------------------------------------------------*\

				ACT.C
			     Actor Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"

#include "adv.h"		/* ADV-node */
#include "sym.h"		/* SYM-nodes */
#include "exp.h"		/* EXPnodes */
#include "atr.h"		/* ATR-nodes */
#include "stm.h"		/* STM-nodes */
#include "elm.h"		/* ELM-nodes */
#include "scr.h"		/* SCR-nodes */
#include "act.h"		/* ACT-nodes */
#include "loc.h"		/* LOC-nodes */
#include "wrd.h"		/* WRD-nodes */
#include "vrb.h"		/* VRB-nodes */
#include "nam.h"		/* NAM-nodes */

#include "acode.h"

#include "emit.h"
#include "dump.h"
#include "lmList.h"



/* PUBLIC: */

int aatrmax = 0;
int actmin, actmax;
int actcount = 0;



/*======================================================================

  newact()

  Allocates and initialises a new actnod.

 */
#ifdef _PROTOTYPES_
ActNod *newact(Srcp *srcp, NamNod *nam, List *namslst, WhrNod *whr, CntNod *props, List *atrs, List *dscr, List *vrbs, List *scrs)
                		/* IN - Source Position */
                 		/* IN - Name of the actor */
                		/* IN - List of names */
                 		/* IN - Where initially */
                		/* IN - Attribute list */
                		/* IN - Description statements */
                		/* IN - Local verbs */
                		/* IN - List of scripts */
#else
ActNod *newact(srcp, nam, namslst, whr, props, atrs, dscr, vrbs, scrs)
     Srcp *srcp;		/* IN - Source Position */
     NamNod *nam;		/* IN - Name of the actor */
     List *namslst;		/* IN - List of lists of names */
     WhrNod *whr;		/* IN - Where initially */
     CntNod *props;		/* IN - Container properties */
     List *atrs;		/* IN - Attribute list */
     List *dscr;		/* IN - Description statements */
     List *vrbs;		/* IN - Local verbs */
     List *scrs;		/* IN - List of scripts */
#endif
{
  ActNod *new;			/* The newly allocated node */
  SymNod *sym;			/* Symbol table entry */
  ActNod *act;			/* Actor */
  List *lst, *lstlst;		/* List and list of list traversing ptrs */


  new = NEW(ActNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->namslst = namslst;
  new->whr = whr;
  new->props = props;
  new->atrs = atrs;
  new->dscr = dscr;
  new->vrbs = vrbs;
  new->scrs = scrs;

  sym = lookup(nam->str);
  if (sym == NULL) {
    new->nam->code = newsym(nam->str, NAMACT, new);
    new->nam->kind = NAMACT;
  } else if (strcmp(sym->str, "hero") == 0) {
    act = (ActNod *) sym->ref;
    /* Copy new info */
    act->srcp = new->srcp;
    act->namslst = new->namslst;
    act->whr = new->whr;
    act->props = new->props;
    act->atrs = new->atrs;
    act->dscr = new->dscr;
    act->vrbs = new->vrbs;
    act->scrs = new->scrs;
    return(NULL);
  } else
    redefined(srcp, sym, nam->str);

  /* Note actor in the dictionary unless it is the hero */
  if (new->nam->code != 1)
    if (namslst == NULL)		/* Use the actor name */
      (void)newwrd(nam->str, WRD_NOUN, new->nam->code, nam);
    else {
      for (lstlst = namslst; lstlst != NULL; lstlst = lstlst->next) {
	for (lst = lstlst->element.lst; lst->next != NULL; lst = lst->next)
	  (void)newwrd(lst->element.nam->str, WRD_ADJ, 0, nam);
	(void)newwrd(lst->element.nam->str, WRD_NOUN, lst->element.nam->code, nam);
      }
    }

  return(new);
}



/*======================================================================

  initact()

  Do initialisation of actors.

 */
#ifdef _PROTOTYPES_
void initact(void)
#else
void initact()
#endif
{
  /* The HERO */
  adv.acts = concat(NULL,
		    newact(&nulsrcp, newnam(&nulsrcp, "hero"),
			   NULL,
			   newwhr(&nulsrcp, WHR_DEFAULT, NULL),
			   NULL,
			   NULL,
			   NULL,
			   NULL,
			   NULL));
}




/*======================================================================

  prepacts()

  Prepare all actors and their attributes.

 */
#ifdef _PROTOTYPES_
void prepacts(void)
#else
void prepacts()
#endif
{
  List *lst;			/* List pointer */
  ActNod *act;			/* Actor node */
  AtrNod *atr;			/* Attribute node */
  List *alst;			/* Attribute list pointer */
  List *namslst, *namlst;	/* Name list list and name list pointer */
  int i;

  /* Number the local attributes for all actors */
  for (lst = adv.acts; lst != NULL; lst = lst->next) {
    act = lst->element.act;
    if (act->props != NULL)
      act->props->parent = act->nam;
    if (act->nam->code == 1) {	/* The hero */
      /* Note actor names in the dictionary, this is not done in newact() */
      /* because the user may want to redefine the heros name */
      if (act->namslst == NULL)		/* Use the actor name */
	(void)newwrd(act->nam->str, WRD_NOUN, act->nam->code, act->nam);
      else {
	for (namslst = act->namslst; namslst != NULL; namslst = namslst->next) {
	  for (namlst = namslst->element.lst; namlst->next != NULL; namlst = namlst->next)
	    (void)newwrd(namlst->element.nam->str, WRD_ADJ, 0, act->nam);
	  (void)newwrd(namlst->element.nam->str, WRD_NOUN, namlst->element.nam->code, act->nam);
	}
      }
    }
    anatrs(act->atrs);
    i = aatrmax + 1;		/* First local is higher than global attrs. */
    for (alst = act->atrs; alst != NULL; alst = alst->next) {
      atr = findatr(alst->element.atr->nam->str, adv.aatrs, adv.atrs);
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
    sortatr(&act->atrs);	/* Sort the attributes for the actor */
  }
}



/*----------------------------------------------------------------------
  
  anact()
  
  Analyze one actor.
  
  */
#ifdef _PROTOTYPES_
static void anact(ActNod *act)
#else
static void anact(act)
     ActNod *act;
#endif
{
  long fpos;
  int len;
  StmNod *stm;
  SymNod *sym;
  ElmNod *elm;


  if (act->nam->code == 1) {	/* The HERO */
    if (act->whr->whr != WHR_DEFAULT)
      lmLog(&act->whr->srcp, 411, sevWAR, "Initial position");
    /* Ok, now output the name, use first if multiple */
    fpos = ftell(txtfil);
    if (act->namslst == NULL)
      len = annams(NULL, act->nam, TRUE);
    else
      len = annams(act->namslst->element.lst, act->nam, TRUE);
  } else {
    /* Output the formatted actor name (first if multiple) to the text file */
    fpos = ftell(txtfil);
    if (act->namslst == NULL)
      len = annams(NULL, act->nam, TRUE);
    else
      len = annams(act->namslst->element.lst, act->nam, TRUE);

    /* Check its initial location */
    switch (act->whr->whr) {
    case WHR_DEFAULT:
      break;
    case WHR_AT:
      if (act->whr->wht->wht == WHT_ID)
	symcheck(&sym, &elm, act->whr->wht->nam, NAMLOC, NAMANY, NULL);
      else
	lmLog(&act->srcp, 414, sevERR, "actor");
      break;
    case WHR_IN:
      lmLog(&act->whr->srcp, 402, sevERR, "");
      break;
    case WHR_HERE:
    case WHR_NEAR:
      lmLog(&act->srcp, 414, sevERR, "actor");
      break;
      
    default:
      syserr("unrecognized switch in anact()");
      break;
    }
  }
  /* Then create a PRINT statement */
  stm = newstm(&nulsrcp, STM_PRINT);
  stm->fields.print.fpos = fpos;
  stm->fields.print.len = len;
  act->namstms = concat(NULL, stm);

  if (act->props != NULL)
    ancnt(act->props);

  anstms(act->dscr, act, NULL, NULL);
  anscrs(act->scrs, act);
  anvrbs(act->vrbs, NULL, act);
}




/*======================================================================

  anacts()

  Analyze the actors in this adventure.

 */
#ifdef _PROTOTYPES_
void anacts(void)
#else
void anacts()
#endif
{
  List *act;		/* Traversal pointer */

  for (act = adv.acts; act != NULL; act = act->next)
    anact(act->element.act);
}




/*----------------------------------------------------------------------

  geact()

  Generate the script routines etc. for one actor.

 */
#ifdef _PROTOTYPES_
static void geact(ActNod *act)
#else
static void geact(act)
     ActNod *act;
#endif
{
  /* Attributes */
  act->atradr = geatrs(act->atrs, adv.aatrs, adv.atrs);
  
  /* Scripts */
  act->scradr = gescrs(act);

  act->namadr = emadr();	/* Save ACODE address to name printing code */
  gestms(act->namstms, act);
  emit0(C_STMOP, I_RETURN);
  
  /* Local verbs */
  act->vrbadr = gevrbs(act->vrbs, act);

  /* Description routine */
  if (act->dscr != NULL) {
    act->dscradr = emadr();
    gestms(act->dscr, act);
    emit0(C_STMOP, I_RETURN);
  } else
    act->dscradr = 0;

  emit0(C_STMOP, I_RETURN);
}



/*----------------------------------------------------------------------

  geactent()

  Generate a table entry for one actor.

 */
#ifdef _PROTOTYPES_
static void geactent(ActNod *act)
                 		/* IN - The actor to generate */
#else
static void geactent(act)
     ActNod *act;		/* IN - The actor to generate */
#endif
{

  if (act->nam->code == 1) 	/* The hero! */
    gewhr(adv.whr);		/* Initial location */
  else
    switch (act->whr->whr) {	/* Initial location */
    case WHR_DEFAULT:
      emit(0);
      break;
    case WHR_AT:
      gewht(act->whr->wht);
      break;
    default:
      syserr("Unrecognized switch in geactent()");
      return;
    }

  emit(TRUE);			/* Description flag */
  emit(act->namadr);		/* Name printing code */
  emit(act->atradr);		/* Attributes */
  /* Container index, if any */
  if (act->props != NULL)
    emit(act->props->code-1+cntmin);
  else
    emit(0);
  emit(0);			/* Script number */
  emit(act->scradr);		/* Script table address */
  emit(0);			/* Step number */
  emit(0);			/* Count */
  emit(act->vrbadr);		/* Verbs for this actor */
  emit(act->dscradr);		/* Description code address */
}



/*======================================================================

  geacts()

  Generate code for all actors, returning an acode address to the
  actor table.

 */
#ifdef _PROTOTYPES_
Aword geacts(void)
#else
Aword geacts()
#endif
{
  List *lst;
  Aword adr;

  /* Actual actors */
  for (lst = adv.acts; lst != NULL; lst = lst->next)
    geact(lst->element.act);

  /* The actor table */
  adr = emadr();		/* Save address to actor table */
  for (lst = adv.acts; lst != NULL; lst = lst->next)
    geactent(lst->element.act);
  emit(EOF);
  return(adr);
}



/*======================================================================

  duact()

  Dump an Actor node.

 */
#ifdef _PROTOTYPES_
void duact(ActNod *act)
#else
void duact(act)
     ActNod *act;
#endif
{
  put("ACT: "); dusrcp(&act->srcp); in();
  put("nam: "); dunam(act->nam); nl();
  put("namslst: "); dulst2(act->namslst, NAMNOD); nl();
  put("whr: "); duwhr(act->whr); nl();
  put("atrs: "); dulst(act->atrs, ATRNOD); nl();
  put("atradr: "); duint(act->atradr); nl();
  put("namstms: "); dulst(act->namstms, STMNOD); nl();
  put("namadr: "); duint(act->namadr); nl();
  put("dscr: "); dulst(act->dscr, STMNOD); nl();
  put("dscradr: "); duint(act->dscradr); nl();
  put("vrbs: "); dulst(act->vrbs, VRBNOD); nl();
  put("vrbadr: "); duint(act->vrbadr); nl();
  put("scrs: "); dulst(act->scrs, SCRNOD); nl();
  put("scradr: "); duint(act->scradr); out();
}
