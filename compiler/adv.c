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
#include "context_x.h"
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
#include "dump.h"
#include "../interpreter/acode.h"



/* PUBLIC */
AdvNod adv;


/* PRIVATE */
static Aword end;


/*======================================================================*/
void initAdventure(void)
{
  initSymbols();
  initClasses();
  initInstances();
  initDumpNodeList();
}


/*======================================================================

  symbolizeAdv()

*/
void symbolizeAdventure()
{
  symbolizeClasses();
  symbolizeInstances();

  symbolizeWhere(adv.whr);
}


/*----------------------------------------------------------------------

  analyzeStartAt

*/
static void analyzeStartAt(void)
{
  /* START has the same environment as a RULE */
  Context *context = newContext(RULE_CONTEXT);

  if (adv.whr != NULL) 
    switch (adv.whr->kind) {
    case WHERE_AT:
      if (adv.whr->what->kind == WHAT_ID) {
	inheritCheck(adv.whr->what->id, "an instance", "location");
      } else
	lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    default:
      lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    }

  analyzeStatements(adv.stms, context);
}


/*====================================================================== */
void analyzeAdventure(void)
{
  addHero();
  symbolizeAdventure();
  addAdditions();
  numberAllAttributes();
  replicateInherited();

  prepwrds();			/* Prepare words in the dictionary */
  prepmsgs();			/* Prepare standard and user messages */
  prepscos();			/* Prepare score handling */

  if (verbose) printf("\n\tSyntax definitions: ");
  anstxs();

  if (verbose) printf("\n\tVerbs: ");
  analyzeVerbs(adv.vrbs, NULL);

  if (verbose) printf("\n\tClasses: ");
  analyzeClasses();

  if (verbose) printf("\n\tInstances: ");
  analyzeInstances();
  theHero->fields.entity.props->whr = adv.whr;

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
void geadv(char acodeFileName[], char textFileName[], char dataFileName[])
{
  initEmit(acodeFileName);		/* Initialise code emit */
  initEncoding(textFileName, dataFileName);	/* Initialise encoding */
  if (lmSeverity() > sevWAR)
    return;
  
  if (verbose) printf("\n\tDictionary: ");
  acdHeader.dictionary = gewrds();	/* Dictionary */

  if (verbose) printf("\n\tSyntax Definitions: ");
  acdHeader.syntaxTableAddress = gestxs();	/* Syntax definitions */ 

  if (verbose) printf("\n\tVerbs: ");
  acdHeader.verbTableAddress = generateVerbs(adv.vrbs); /* Global verbs */

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
  generateStatements(adv.stms);
  emit0(C_STMOP, I_RETURN);

  /* String initialisation table */
  acdHeader.init = generateStringInit();

  terminateEncoding();
  terminateEmit();
  emitTextDataToAcodeFile(dataFileName);
  emitHeader();
  if (verbose) printf("\n");
}




/*====================================================================== */
void dumpAdventure(enum dmpKd dmp)
{
  if (dmp&DMPALL)
    dmp = (enum dmpKd)-1L;


  put("ADV: "); in();

  put("synonyms: ");
  if (dmp&DMPSYN)
    dumpList(adv.syns, SYNONYM_LIST);
  else
    put("--");
  nl();

  put("syntaxes: ");
  if (dmp&DMPSTX)
    dumpList(adv.stxs, SYNTAX_LIST);
  else
    put("--");
  nl();

  put("verbs: ");
  if (dmp&DMPVRB)
    dumpList(adv.vrbs, VERB_LIST);
  else
    put("--");
  nl();

  put("classes: ");
  if (dmp&DMPCLA) {
    dumpList(adv.clas, CLASS_LIST);
    nl();
    put("additions: ");
    dumpList(adv.adds, ADD_LIST);
  } else
    put("--");
  nl();

  put("instances: ");
  if (dmp&DMPINS)
    dumpList(adv.inss, INSTANCE_LIST);
  else
    put("--");
  nl();

  put("containers: ");
  if (dmp&DMPCNT)
    dumpList(adv.cnts, CONTAINER_LIST);
  else
    put("--");
  nl();

  put("events: ");
  if (dmp&DMPEVT)
    dumpList(adv.evts, EVENT_LIST);
  else
    put("--");
  nl();

  put("rules: ");
  if (dmp&DMPRUL)
    dumpList(adv.ruls, RULE_LIST);
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
  dumpList(adv.stms, STATEMENT_LIST);
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
  if (verbCount != 0) {
    (void)sprintf(str, "        Verbs:                  %6d", verbCount);
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
