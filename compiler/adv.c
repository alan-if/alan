/*----------------------------------------------------------------------*\

				ADV.C
			    Adventure Node

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lmList.h"

#include "sym_x.h"

#include "adv.h"		/* ADV-node */
#include "cla.h"		/* CLA-nodes */
#include "ins.h"		/* INS-nodes */
#include "whr.h"		/* WHR-nodes */
#include "lst.h"		/* LST-nodes */
#include "stm.h"		/* STM-nodes */
#include "vrb.h"		/* VRB-nodes */
#include "ext.h"		/* EXT-nodes */
#include "evt.h"		/* EVT-nodes */
#include "rul.h"		/* RUL-nodes */
#include "wrd.h"		/* WRD-nodes */
#include "syn.h"		/* SYN-nodes */
#include "atr.h"		/* ATR-nodes */
#include "msg.h"		/* MSG-nodes */

#include "sco.h"		/* SCORES */
#include "opt.h"		/* OPTIONS */
#include "options.h"		/* OPTIONS */

#include "emit.h"
#include "encode.h"
#include "acode.h"
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
  initDumpNodeList();

#ifdef FIXME
  initcnt();			/* Must do this first to create the inventory */
#endif
}



#ifdef FIXME
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
#endif


/*----------------------------------------------------------------------

  symbolizeAdv()

*/
static void symbolizeAdv()
{
  symbolizeClasses();
}


/*----------------------------------------------------------------------

  analyzeStartAt

*/
static void analyzeStartAt(void)
{
  SymNod *sym;
  ElmNod *elm;

  if (adv.whr != NULL) 
    switch (adv.whr->whr) {
    case WHR_AT:
      if (adv.whr->wht->wht == WHT_ID)
	sym = symcheck(&elm, adv.whr->wht->id, INSTANCE_SYMBOL, NULL);
      else
	lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    default:
      lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    }

  anstms(adv.stms, NULL, NULL, NULL);
}


/*======================================================================

  anadv()

  Analyse the adventure.

 */
void anadv(void)
{
  symbolizeAdv();

  anatrs(adv.atrs);
  anatrs(adv.oatrs);
  anatrs(adv.latrs);
  anatrs(adv.aatrs);
#ifdef FIXME
  prepcodes();			/* Set up the codes for all entities */
  prepatrs();			/* Number default attributes */
  prepinss();			/* Sort out the local attributes etc. */
#endif
  prepwrds();			/* Prepare words in the dictionary */
  prepmsgs();			/* Prepare standard and user messages */
  prepscos();			/* Prepare score handling */

  if (verbose) printf("\n\tSyntax definitions: ");
  anstxs();
  if (verbose) printf("\n\tVerbs: ");
  anvrbs(adv.vrbs, NULL);
  if (verbose) printf("\n\tClasses: ");
  analyzeClasses();
  if (verbose) printf("\n\tInstaces: ");
  analyzeInstances();
  if (verbose) printf("\n\tEvents: ");
  anevts();
  if (verbose) printf("\n\tRules: ");
  anruls();
  if (verbose) printf("\n\tSynonyms: ");
  ansyns();
  if (verbose) printf("\n\tMessages:");
  anmsgs();
  if (verbose) printf("\n");

  analyzeStartAt();

  anwrds();
}




/*----------------------------------------------------------------------

  gecodes()

  Set the max and min codes for objects, actors etc. in the header.

  */
static void gecodes(AcdHdr *hdr) /* IN - The header to fill in */
{
#ifdef FIXME
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
#endif
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
  if (verbose) printf("\n\tClasses: ");
  header.clas = generateClasses();
  if (verbose) printf("\n\tInstances: ");
  header.inss = generateInstances();
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
  put("atrs: "); dulst(adv.atrs, LIST_ATR); nl();
  put("oatrs: "); dulst(adv.oatrs, LIST_ATR); nl();
  put("latrs: "); dulst(adv.latrs, LIST_ATR); nl();
  put("aatrs: "); dulst(adv.aatrs, LIST_ATR); nl();
  put("scores: "); nl();
  put("syns: "); if (dmp&DMPSYN) dulst(adv.syns, LIST_SYN); else put("--"); nl();
  put("stxs: "); if (dmp&DMPSTX) dulst(adv.stxs, LIST_STX); else put("--"); nl();
  put("vrbs: "); if (dmp&DMPVRB) dulst(adv.vrbs, LIST_VRB); else put("--"); nl();
  put("clas: "); if (dmp&DMPCLA) dulst(adv.clas, LIST_CLA); else put("--"); nl();
  put("inss: "); if (dmp&DMPINS) dulst(adv.inss, LIST_INS); else put("--"); nl();
  put("cnts: "); if (dmp&DMPCNT) dulst(adv.cnts, LIST_CNT); else put("--"); nl();
  put("evts: "); if (dmp&DMPEVT) dulst(adv.evts, LIST_EVT); else put("--"); nl();
  put("ruls: "); if (dmp&DMPRUL) dulst(adv.ruls, LIST_RUL); else put("--"); nl();
  put("whr: "); duwhr(adv.whr); nl();
  put("stms: "); dulst(adv.stms, LIST_STM); out();
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
  if (vrbcount != 0) {
    (void)sprintf(str, "        Verbs:                  %6d", vrbcount);
    lmLiPrint(str);
  }
  if (classCount != 0) {
    (void)sprintf(str, "        Classes:                %6d", classCount);
    lmLiPrint(str);
  }
  if (instanceCount != 0) {
    (void)sprintf(str, "        Instances:              %6d", instanceCount);
    lmLiPrint(str);
  }
  (void)sprintf(str  , "        Words:                  %6d", words[WRD_CLASSES]);
  lmLiPrint(str);
  (void)sprintf(str,   "        Acode:                  %6d words (%d bytes)", (int)end, (int)((long)end*(long)sizeof(Aword)));
  lmLiPrint(str);
  (void)sprintf(str,   "        Text data:              %6d bytes", txtlen);
  lmLiPrint(str);
}
