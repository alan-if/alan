/*----------------------------------------------------------------------*\

				ADV.C
			    Adventure Node

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "lmList.h"

#include "add_x.h"
#include "cla_x.h"
#include "cnt_x.h"
#include "ins_x.h"
#include "lst_x.h"
#include "srcp_x.h"
#include "stm_x.h"
#include "stx_x.h"
#include "sym_x.h"
#include "vrb_x.h"
#include "whr_x.h"
#include "wrd_x.h"


#include "adv.h"		/* ADV-node */
#include "ext.h"		/* EXT-nodes */
#include "evt.h"		/* EVT-nodes */
#include "rul.h"		/* RUL-nodes */
#include "syn.h"		/* SYN-nodes */
#include "atr_x.h"		/* ATR-nodes */
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
static Aword end;


/*======================================================================

  initadv()

  Initialise any structures and data needed.

  */
void initadv(void)
{
  initSymbols();
  initClasses();
  initInstances();
  initDumpNodeList();
}


/*======================================================================

  symbolizeAdv()

*/
void symbolizeAdv()
{
  symbolizeClasses();
  symbolizeInstances();

  symbolizeWhr(adv.whr);
}


/*----------------------------------------------------------------------

  analyzeStartAt

*/
static void analyzeStartAt(void)
{
  Context context;

  context.kind = RULE_CONTEXT; /* START has the same environment as a RULE */

  if (adv.whr != NULL) 
    switch (adv.whr->kind) {
    case WHR_AT:
      if (adv.whr->wht->kind == WHT_ID) {
	inheritCheck(adv.whr->wht->id, "an instance", "location");
      } else
	lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    default:
      lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    }

  anstms(adv.stms, &context);
}


/*======================================================================

  anadv()

  Analyse the adventure.

 */
void anadv(void)
{
  addHero();
  symbolizeAdv();
  addAdditions();
  numberAllAttributes();
  replicateInheritedAttributes();

  prepwrds();			/* Prepare words in the dictionary */
  prepmsgs();			/* Prepare standard and user messages */
  prepscos();			/* Prepare score handling */

  if (verbose) printf("\n\tSyntax definitions: ");
  anstxs();

  if (verbose) printf("\n\tVerbs: ");
  anvrbs(adv.vrbs, NULL);

  if (verbose) printf("\n\tClasses: ");
  analyzeClasses();

  if (verbose) printf("\n\tInstances: ");
  analyzeInstances();
  theHero->fields.claOrIns.slots->whr = adv.whr;

  if (verbose) printf("\n\tEvents: ");
  anevts();

  if (verbose) printf("\n\tRules: ");
  analyzeRules();

  if (verbose) printf("\n\tSynonyms: ");
  ansyns();

  if (verbose) printf("\n\tMessages:");
  anmsgs();

  if (verbose) printf("\n");

  analyzeStartAt();

  anwrds();
}



/*======================================================================

  geadv()

  Generate the whole adventure.

 */
void geadv(char *acdfnm)	/* IN - ACODE file name */
{
  initEmit(acdfnm);		/* Initialise code emit */
  eninit();			/* Initialise encoding */
  if (lmSeverity() > sevWAR)
    return;
  
  if (verbose) printf("\n\tDictionary: ");
  acdHeader.dictionary = gewrds();	/* Dictionary */

  if (verbose) printf("\n\tSyntax Definitions: ");
  acdHeader.syntaxTableAddress = gestxs();	/* Syntax definitions */ 

  if (verbose) printf("\n\tVerbs: ");
  acdHeader.verbTableAddress = gevrbs(adv.vrbs, NULL); /* Global verbs */

  if (verbose) printf("\n\tClasses: ");
  acdHeader.classTableAddress = generateClasses();

  if (verbose) printf("\n\tInstances: ");
  generateInstances(&acdHeader);

  if (verbose) printf("\n\tContainers: ");
  acdHeader.containerTableAddress = generateContainers(&acdHeader);

  if (verbose) printf("\n\tEvents: ");
  acdHeader.eventTableAddress = generateEvents(&acdHeader);

  if (verbose) printf("\n\tRules: ");
  acdHeader.ruleTableAddress = generateRules();

  acdHeader.scores = gesco();	/* Scores */
  acdHeader.maxscore = scotot;	/* Total score */

  if (verbose) printf("\n\tMessages: ");
  acdHeader.messageTableAddress = gemsgs();

  if (verbose) printf("\n\tCharacter Encoding: ");
  acdHeader.freq = gefreq();	/* Character frequencies */

  /* Options */
  geopt(&acdHeader);

  /* Start statements */
  acdHeader.start = emadr();	/* Save ACODE address to start */
  gestms(adv.stms, NULL);
  emit0(C_STMOP, I_RETURN);

  /* String initialisation table */
  acdHeader.init = generateStringInit();

  end = emadr();		/* Last address */
  acdHeader.size = end;		/* Save size */
  enterm();			/* Terminate ENCODE */
  terminateEmit();
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

  put("synonyms: ");
  if (dmp&DMPSYN)
    dulst(adv.syns, LIST_SYN);
  else
    put("--");
  nl();

  put("syntaxes: ");
  if (dmp&DMPSTX)
    dulst(adv.stxs, LIST_STX);
  else
    put("--");
  nl();

  put("verbs: ");
  if (dmp&DMPVRB)
    dulst(adv.vrbs, LIST_VRB);
  else
    put("--");
  nl();

  put("classes: ");
  if (dmp&DMPCLA) {
    dulst(adv.clas, LIST_CLA);
    nl();
    put("additions: ");
    dulst(adv.adds, LIST_ADD);
  } else
    put("--");
  nl();

  put("instances: ");
  if (dmp&DMPINS)
    dulst(adv.inss, LIST_INS);
  else
    put("--");
  nl();

  put("containers: ");
  if (dmp&DMPCNT)
    dulst(adv.cnts, LIST_CNT);
  else
    put("--");
  nl();

  put("events: ");
  if (dmp&DMPEVT)
    dulst(adv.evts, LIST_EVT);
  else
    put("--");
  nl();

  put("rules: ");
  if (dmp&DMPRUL)
    dulst(adv.ruls, LIST_RUL);
  else
    put("--");
  nl();

  put("symbols: ");
  if (dmp&DMPSYM)
    dumpSymbols();
  else
    put("--");
  nl();

  put("whr: ");
  duwhr(adv.whr);
  nl();

  put("stms: ");
  dulst(adv.stms, LIST_STM);
  out();
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
