/*----------------------------------------------------------------------*\

				LOC.C
			    Location Nodes

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
#include "loc.h"		/* LOC-nodes */
#include "stm.h"		/* STM-nodes */
#include "vrb.h"		/* VRB-nodes */

#include "emit.h"
#include "acode.h"
#include "dump.h"


/* PUBLIC: */

int latrmax = 0;
int locmin, locmax;
int loccount = 0;



/*======================================================================
  newloc()

  Allocates and initialises a new locnod.

 */
#ifdef _PROTOTYPES_
LocNod *newloc(Srcp *srcp, NamNod *nam, List *nams, List *atrs, List *dscr, List *does, List *exts, List *vrbs)
                	/* IN - Source Position */
                 	/* IN - Name of the location */
                	/* IN - List of names */
                	/* IN - Attribute list */
                	/* IN - Description statements */
                	/* IN - What to do when entered */
                	/* IN - List of exits */
                	/* IN - List of vrbs */
#else
LocNod *newloc(srcp, nam, nams, atrs, dscr, does, exts, vrbs)
     Srcp *srcp;	/* IN - Source Position */
     NamNod *nam;	/* IN - Name of the location */
     List *nams;	/* IN - List of names */
     List *atrs;	/* IN - Attribute list */
     List *dscr;	/* IN - Description statements */
     List *does;	/* IN - What to do when entered */
     List *exts;	/* IN - List of exits */
     List *vrbs;	/* IN - List of vrbs */
#endif
{
  LocNod *new;		/* The newly allocated node */
  SymNod *sym;		/* Symbol table entry */

  new = NEW(LocNod);

  new->srcp = *srcp;
  new->nam = nam;
  new->nams = nams;
  new->atrs = atrs;
  new->dscr = dscr;
  new->does = does;
  new->exts = exts;
  new->vrbs = vrbs;

  sym = lookup(nam->str);
  if (sym == NULL)
    new->nam->code = newsym(nam->str, NAMLOC, new);
  else
    redefined(srcp, sym, nam->str);

  return(new);
}



/*======================================================================
  preplocs()

  Prepare all locations, like giving their attributes unique numbers.

 */
#ifdef _PROTOTYPES_
void preplocs(void)
#else
void preplocs()
#endif
{
  List *llst;		/* List pointer */
  List *alst;		/* List pointer */
  AtrNod *atr;			/* Attribute node */
  int i;			/* and a counter */
  

  /* Number the local attributes for all locations */
  for (llst = adv.locs; llst != NULL; llst = llst->next) {
    i = latrmax + 1;		/* First local is higher than global attrs. */
    anatrs(llst->element.loc->atrs);
    for (alst = llst->element.loc->atrs; alst != NULL; alst = alst->next) {
      atr = findatr(alst->element.atr->nam->str, adv.latrs, adv.atrs);
      if (atr == NULL)
	/* Must be a local attribute */
	alst->element.atr->nam->code = i++;
      else {
	/* Else use default attribute code */
	alst->element.atr->nam->code = atr->nam->code;
	if (!eqtyp(alst->element.atr->typ, atr->typ))
	  lmLog(&alst->element.atr->srcp, 332, sevERR, "");
      }
    }
    sortatr(&llst->element.loc->atrs);
  }
}



/*----------------------------------------------------------------------
  anloc()

  Analyzes one location by calling the analyzers for the subtrees.

 */
#ifdef _PROTOTYPES_
static void anloc(LocNod *loc)
                		/* IN - Location to analyze */
#else
static void anloc(loc)
    LocNod *loc;		/* IN - Location to analyze */
#endif
{
  long fpos;
  int len = 0;
  StmNod *stm;


  /* First output the formatted location name to the text file */
  fpos = ftell(txtfil);
  len = annams(loc->nams, loc->nam, TRUE);

  /* Then create a PRINT statement */
  stm = newstm(&nulsrcp, STM_PRINT);
  stm->fields.print.fpos = fpos;
  stm->fields.print.len = len;
  loc->namstms = concat(NULL, stm);

  /* Analyse the rest */
  anstms(loc->dscr, NULL, NULL, NULL);
  anstms(loc->does, NULL, NULL, NULL);
  if (loc->exts == NULL)
    lmLog(&loc->nam->srcp, 501, sevINF, loc->nam->str);
  else
    anexts(loc->exts);
  anvrbs(loc->vrbs, NULL, NULL);
}



/*======================================================================

  anlocs()

  Analyzes all locations by calling the location analyzer for all
  locations.

 */
#ifdef _PROTOTYPES_
void anlocs(void)
#else
void anlocs()
#endif
{
    List *loc;        /* Traversal pointer */

    for (loc = adv.locs;loc != NULL;loc = loc->next)
        anloc(loc->element.loc);
}



/*----------------------------------------------------------------------

  gelocnam()

  Generate the procedure to print the location name.

 */
#ifdef _PROTOTYPES_
static Aaddr gelocnam(LocNod *loc)
                 		/* IN - The location */
#else
static Aaddr gelocnam(loc)
     LocNod *loc;		/* IN - The location */
#endif
{
  Aaddr namadr = emadr();

  gestms(loc->namstms, NULL);
  emit0(C_STMOP, I_RETURN);
  return(namadr);
}



/*----------------------------------------------------------------------

  gelocdscr()

  Generate the procedure to describe a location.

 */
#ifdef _PROTOTYPES_
static Aaddr gelocdscr(LocNod *loc)
                 		/* IN - The location to describe */
#else
static Aaddr gelocdscr(loc)
     LocNod *loc;		/* IN - The location to describe */
#endif
{
  Aaddr dscradr = emadr();

  if (loc->dscr == NULL)
    return(0);

  gestms(loc->dscr, NULL);
  emit0(C_STMOP, I_RETURN);
  return(dscradr);
}





/*----------------------------------------------------------------------

  gelocdoes()

  Generate the procedure to be executed upon entry of a location.

 */
#ifdef _PROTOTYPES_
static Aaddr gelocdoes(LocNod *loc)
                 		/* IN - The location */
#else
static Aaddr gelocdoes(loc)
     LocNod *loc;		/* IN - The location */
#endif
{
  Aaddr doesadr = emadr();

  if (loc->does == NULL)
    return(0);

  gestms(loc->does, NULL);
  emit0(C_STMOP, I_RETURN);
  return(doesadr);
}




/*----------------------------------------------------------------------
  geloc()

  Generate procedures and data for a location.

 */
#ifdef _PROTOTYPES_
static void geloc(LocNod *loc)
                 		/* IN - The location to generate */
#else
static void geloc(loc)
     LocNod *loc;		/* IN - The location to generate */
#endif
{
  loc->atradr = geatrs(loc->atrs, adv.latrs, adv.atrs);
  loc->namsadr = gelocnam(loc);
  loc->dscradr = gelocdscr(loc);
  loc->doesadr = gelocdoes(loc);
  loc->extadr = geexts(loc->exts);
  loc->vrbadr = gevrbs(loc->vrbs, NULL);
}




/*----------------------------------------------------------------------

  gelocent()

  Generate one entry in the location list.

 */
#ifdef _PROTOTYPES_
static void gelocent(LocNod *loc)
                 		/* IN - The location to generate for */
#else
static void gelocent(loc)
     LocNod *loc;		/* IN - The location to generate for */
#endif
{
  /* First pointer to description and procedures */
  emit(loc->namsadr);
  emit(loc->dscradr);
  emit(loc->doesadr);
  emit(0);			/* Description indicator flag */
  emit(loc->atradr);		/* Attribute list */
  emit(loc->extadr);		/* Exit list pointer */
  
  /* Local verbs */
  if (loc->vrbs != NULL)
    emit(loc->vrbadr);
  else
    emit(0);
}



/*======================================================================

  gelocs()

  Generate information and procedures for all locations and return an
  acode address to the location table.

 */
#ifdef _PROTOTYPES_
Aword gelocs(void)
#else
Aword gelocs()
#endif
{
  List *lst;		/* Traversal pointer */
  Aword adr;

  /* First generate description procedures and exit lists for all */
  for (lst = adv.locs; lst != NULL; lst = lst->next)
    geloc(lst->element.loc);
  
  adr = emadr();		/* Save address to location table */
  /* Now we can generate the location table */
  for (lst = adv.locs;lst != NULL;lst = lst->next)
    gelocent(lst->element.loc);
  emit(EOF);
  return(adr);
}



/*======================================================================

  duloc()

  Dump a location!

 */
#ifdef _PROTOTYPES_
void duloc(LocNod *loc)
#else
void duloc(loc)
     LocNod *loc;
#endif
{
  if (loc == NULL) {
    put("NULL");
    return;
  }

  put("LOC: "); dusrcp(&loc->srcp); in();
  put("nam: "); dunam(loc->nam); nl();
  put("nams: "); dulst(loc->nams, NAMNOD); nl();
  put("atrs: "); dulst(loc->atrs, ATRNOD); nl();
  put("atradr: "); duint(loc->atradr); nl();
  put("namstms: "); dulst(loc->namstms, STMNOD); nl();
  put("namsadr: "); duint(loc->namsadr); nl();
  put("dscr: "); dulst(loc->dscr, STMNOD); nl();
  put("dscradr: "); duint(loc->dscradr); nl();
  put("does: "); dulst(loc->does, STMNOD); nl();
  put("doesadr: "); duint(loc->doesadr); nl();
  put("vrbs: "); dulst(loc->vrbs, VRBNOD); nl();
  put("vrbadr: "); duint(loc->vrbadr); nl();
  put("exts: "); dulst(loc->exts, EXTNOD); nl();
  put("extadr: "); duint(loc->extadr); out();
}


