/*----------------------------------------------------------------------*\

				ADV.C
			    Adventure Node

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "adv.h"		/* ADV-node */
#include "sym.h"		/* SYM-nodes */
#include "whr.h"		/* WHR-nodes */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "stm.h"		/* STM-nodes */
#include "vrb.h"		/* VRB-nodes */
#include "ext.h"		/* EXT-nodes */
#include "loc.h"		/* LOC-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "act.h"		/* ACT-nodes */
#include "evt.h"		/* EVT-nodes */
#include "rul.h"		/* RUL-nodes */
#include "wrd.h"		/* WRD-nodes */
#include "syn.h"		/* SYN-nodes */
#include "atr.h"		/* ATR-nodes */
#include "msg.h"		/* MSG-nodes */

#include "sco.h"		/* SCORES */
#include "opt.h"		/* OPTIONS */

#include "emit.h"
#include "encode.h"

#include "dump.h"



/* PUBLIC */
AdvNod adv;


/* PRIVATE */
static AcdHdr header;	/* ACODE program header */
static Aword end;


/*======================================================================

  initadv()

  Initialise any structures and data needed.

  */
void initadv(void)
{
  initcnt();			/* Must do this first to create the inventory */
  initact();
}



/*----------------------------------------------------------------------

  prepcodes()

  Calculate min and max codes for all entities.

  */
static void prepcodes(void)
{
  objmin = 1;
  objmax = objmin + objcount - 1;
  actmin = objmax + 1;
  actmax = actmin + actcount - 1;
  cntmin = actmax + 1;
  cntmax = cntmin + cntcount - 1;
  locmin = cntmax + 1;
  locmax = locmin + loccount - 1;
  dirmin = locmax + 1;
  dirmax = dirmin + dircount - 1;
  vrbmin = dirmax + 1;
  vrbmax = vrbmin + vrbcount - 1;
  evtmin = 1;
  evtmax = evtmin + evtcount - 1;
  rulmin = 1;
  rulmax = rulmin + rulcount - 1;
}


/*======================================================================

  anadv()

  Analyse the adventure.

 */
void anadv(void)
{
  SymNod *sym;
  ElmNod *elm;

  anatrs(adv.atrs);
  anatrs(adv.oatrs);
  anatrs(adv.latrs);
  anatrs(adv.aatrs);
  prepcodes();			/* Set up the codes for all entities */
  prepatrs();			/* Number default attributes */
  prepobjs();			/* Sort out the local attributes etc. */
  preplocs();			/* D:o */
  prepacts();			/* D:o */
  prepwrds();			/* Prepare words in the dictionary */
  prepmsgs();			/* Prepare standard and user messages */
  prepscos();			/* Prepare score handling */

  if (verbose) printf("\n\tSyntax definitions: ");
  anstxs();
  if (verbose) printf("\n\tVerbs: ");
  anvrbs(adv.vrbs, NULL, NULL);
  if (verbose) printf("\n\tLocations: ");
  anlocs();
  if (verbose) printf("\n\tObjects: ");
  anobjs();
  if (verbose) printf("\n\tActors: ");
  anacts();
  if (verbose) printf("\n\tContainers: ");
  ancnts();
  if (verbose) printf("\n\tEvents: ");
  anevts();
  if (verbose) printf("\n\tRules: ");
  anruls();
  if (verbose) printf("\n\tSynonyms: ");
  ansyns();
  if (verbose) printf("\n\tMessages:");
  anmsgs();
  if (verbose) printf("\n");

  if (adv.whr != NULL) 
    switch (adv.whr->whr) {
    case WHR_AT:
      if (adv.whr->wht->wht == WHT_ID)
	symcheck(&sym, &elm, adv.whr->wht->nam, NAMLOC, NAMANY, NULL);
      else
	lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    default:
      lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    }

  if (adv.stms != NULL)
    anstms(adv.stms, NULL, NULL, NULL);

  anwrds();
}




/*----------------------------------------------------------------------

  gecodes()

  Set the max and min codes for objects, actors etc. in the header.

  */
static void gecodes(AcdHdr *hdr) /* IN - The header to fill in */
{
  hdr->objmin = objmin;
  hdr->objmax = objmax;
  hdr->actmin = actmin;
  hdr->actmax = actmax;
  hdr->cntmin = cntmin;
  hdr->cntmax = cntmax;
  hdr->dirmin = dirmin;
  hdr->dirmax = dirmax;
  hdr->locmin = locmin;
  hdr->locmax = locmax;
  hdr->evtmin = evtmin;
  hdr->evtmax = evtmax;
  hdr->rulmin = rulmin;
  hdr->rulmax = rulmax;
}


/*======================================================================

  geadv()

  Generate the whole adventure.

 */
void geadv(char *acdfnm)	/* IN - ACODE file name */
{
  eminit(acdfnm);		/* Initialise code emit */
  eninit();			/* Initialise encoding */
  if (lmSeverity() > sevWAR)
    return;
  
  /* Max and min codes */
  gecodes(&header);
  
  if (verbose) printf("\n\tDictionary: ");
  header.dict = gewrds();	/* Dictionary */
  if (verbose) printf("\n\tSyntax Definitions: ");
  header.stxs = gestxs();	/* Syntax definitions */ 
  if (verbose) printf("\n\tVerbs: ");
  header.vrbs = gevrbs(adv.vrbs, NULL); /* Global verbs */
  if (verbose) printf("\n\tContainers: ");
  header.cnts = gecnts();	/* Containers */
  if (verbose) printf("\n\tObjects: ");
  header.objs = geobjs();	/* Objects */
  if (verbose) printf("\n\tLocations: ");
  header.locs = gelocs();	/* Locations */
  if (verbose) printf("\n\tActors: ");
  header.acts = geacts();	/* Actors */
  if (verbose) printf("\n\tEvents: ");
  header.evts = geevts();	/* Events */
  if (verbose) printf("\n\tRules: ");
  header.ruls = geruls();	/* Rules */
  header.scores = gesco();	/* Scores */
  header.maxscore = scotot;	/* Total score */
  if (verbose) printf("\n\tMessages: ");
  header.msgs = gemsgs();	/* Messages */
  if (verbose) printf("\n\tCharacter Encoding: ");
  header.freq = gefreq();	/* Character frequencies */

  /* Options */
  geopt(&header);

  /* Start statements */
  header.start = emadr();	/* Save ACODE address to start */
  gestms(adv.stms, NULL);
  emit0(C_STMOP, I_RETURN);

  /* String initialisation table */
  header.init = geinit();

  end = emadr();		/* Last address */
  header.size = end;		/* Save size */
  enterm();			/* Terminate ENCODE */
  emterm(&header);		/* Terminate EMIT */
  if (verbose) printf("\n");
}




/*======================================================================

  duadv()

  Dump the Adventure.

 */
void duadv(enum dmpKd dmp)
{
  if (dmp&DMPALL)
    dmp = (enum dmpKd)-1L;

  put("ADV: "); in();
  put("atrs: "); dulst(adv.atrs, ATRNOD); nl();
  put("oatrs: "); dulst(adv.oatrs, ATRNOD); nl();
  put("latrs: "); dulst(adv.latrs, ATRNOD); nl();
  put("aatrs: "); dulst(adv.aatrs, ATRNOD); nl();
  put("scores: "); nl();
  put("syns: "); if (dmp&DMPSYN) dulst(adv.syns, SYNNOD); else put("--"); nl();
  put("stxs: "); if (dmp&DMPSTX) dulst(adv.stxs, STXNOD); else put("--"); nl();
  put("vrbs: "); if (dmp&DMPVRB) dulst(adv.vrbs, VRBNOD); else put("--"); nl();
  put("locs: "); if (dmp&DMPLOC) dulst(adv.locs, LOCNOD); else put("--"); nl();
  put("objs: "); if (dmp&DMPOBJ) dulst(adv.objs, OBJNOD); else put("--"); nl();
  put("cnts: "); if (dmp&DMPCNT) dulst(adv.cnts, CNTNOD); else put("--"); nl();
  put("evts: "); if (dmp&DMPEVT) dulst(adv.evts, EVTNOD); else put("--"); nl();
  put("acts: "); if (dmp&DMPACT) dulst(adv.acts, ACTNOD); else put("--"); nl();
  put("ruls: "); if (dmp&DMPRUL) dulst(adv.ruls, RULNOD); else put("--"); nl();
  put("whr: "); duwhr(adv.whr); nl();
  put("stms: "); dulst(adv.stms, STMNOD); out();
}



/*======================================================================

  summary()

  Print out a short summary of the adventure.

 */
void summary(void)
{
  char str[80];
  
  lmSkipLines(8);
  
  lmLiPrint("");
  lmLiPrint("        Summary");
  lmLiPrint("        -------");
  if (loccount != 0) {
    (void)sprintf(str, "        Locations:              %6d", loccount);
    lmLiPrint(str);
  }
  if (vrbcount != 0) {
    (void)sprintf(str, "        Verbs:                  %6d", vrbcount);
    lmLiPrint(str);
  }
  if (objcount != 0) {
    (void)sprintf(str, "        Objects:                %6d", objcount);
    lmLiPrint(str);
  }
  if (actcount > 1) {
    (void)sprintf(str, "        Actors:                 %6d (incl. the Hero)", actcount);
    lmLiPrint(str);
  }
  (void)sprintf(str  , "        Words:                  %6d", words[WRD_CLASSES]);
  lmLiPrint(str);
  (void)sprintf(str,   "        Acode:                  %6d words (%d bytes)", (int)end, (int)((long)end*(long)sizeof(Aword)));
  lmLiPrint(str);
  (void)sprintf(str,   "        Text data:              %6d bytes", txtlen);
  lmLiPrint(str);
}
