/*----------------------------------------------------------------------*\

				LOC.C
			    Location Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp_x.h"
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
LocNod *newloc(Srcp *srcp,	/* IN - Source Position */
	       NamNod *nam,	/* IN - Name of the location */
	       List *nams,	/* IN - List of names */
	       List *atrs,	/* IN - Attribute list */
	       List *dscr,	/* IN - Description statements */
	       List *does,	/* IN - What to do when entered */
	       List *exts,	/* IN - List of exits */
	       List *vrbs)	/* IN - List of vrbs */
{
  LocNod *new;		/* The newly allocated node */
  SymNod *sym;		/* Symbol table entry */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

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
void preplocs(void)
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
	/* Else type check and use default attribute code */
	alst->element.atr->nam->code = atr->nam->code;
	if (!eqtyp(alst->element.atr->typ, atr->typ))
	  lmLogv(&alst->element.atr->srcp, 332, sevERR, "local", "default", NULL);
      }
    }
    sortatr(&llst->element.loc->atrs);
  }
}



/*----------------------------------------------------------------------
  anloc()

  Analyzes one location by calling the analyzers for the subtrees.

 */
static void anloc(LocNod *loc)	/* IN - Location to analyze */
{
  long fpos;
  int len = 0;
  StmNod *stm;


  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  /* First output the formatted location name to the text file */
  fpos = ftell(txtfil);
  len = annams(loc->nams, loc->nam, TRUE);

  /* Then create a PRINT statement */
  stm = newstm(&nulsrcp, STM_PRINT);
  stm->fields.print.fpos = fpos;
  stm->fields.print.len = len;
  loc->namstms = concat(NULL, stm, STMNOD);

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
void anlocs(void)
{
    List *loc;        /* Traversal pointer */

    for (loc = adv.locs;loc != NULL;loc = loc->next)
        anloc(loc->element.loc);
}



/*----------------------------------------------------------------------

  gelocnam()

  Generate the procedure to print the location name.

 */
static Aaddr gelocnam(LocNod *loc) /* IN - The location */
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
static Aaddr gelocdscr(LocNod *loc) /* IN - The location to describe */
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
static Aaddr gelocdoes(LocNod *loc) /* IN - The location */
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
static void geloc(LocNod *loc)	/* IN - The location to generate */
{
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

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
static void gelocent(LocNod *loc) /* IN - The location to generate for */
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
Aword gelocs(void)
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
void duloc(LocNod *loc)
{
  if (loc == NULL) {
    put("NULL");
    return;
  }

  put("LOC: "); dusrcp(&loc->srcp); in();
  put("nam: "); dunam(loc->nam); nl();
  put("nams: "); dulst(loc->nams, NAMNOD); nl();
  put("atrs: "); dulst(loc->atrs, ATRNOD); nl();
  put("atradr: "); duadr(loc->atradr); nl();
  put("namstms: "); dulst(loc->namstms, STMNOD); nl();
  put("namsadr: "); duadr(loc->namsadr); nl();
  put("dscr: "); dulst(loc->dscr, STMNOD); nl();
  put("dscradr: "); duadr(loc->dscradr); nl();
  put("does: "); dulst(loc->does, STMNOD); nl();
  put("doesadr: "); duadr(loc->doesadr); nl();
  put("vrbs: "); dulst(loc->vrbs, VRBNOD); nl();
  put("vrbadr: "); duadr(loc->vrbadr); nl();
  put("exts: "); dulst(loc->exts, EXTNOD); nl();
  put("extadr: "); duadr(loc->extadr); out();
}


