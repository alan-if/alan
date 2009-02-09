/*----------------------------------------------------------------------*\

				ADV.C
			    Adventure Node

\*----------------------------------------------------------------------*/

/* Own: */
#include "adv.h"

/* Imports: */
#include "alan.h"
#include "util.h"

#include "lmList.h"
#include "smScan.h"

#include "add_x.h"
#include "cla_x.h"
#include "cnt_x.h"
#include "context_x.h"
#include "ins_x.h"
#include "lst_x.h"
#include "scr_x.h"
#include "srcp_x.h"
#include "stm_x.h"
#include "stx_x.h"
#include "sym_x.h"
#include "vrb_x.h"
#include "whr_x.h"
#include "wrd_x.h"
#include "atr_x.h"
#include "rul_x.h"
#include "ifid_x.h"
#include "resource_x.h"
#include "dump_x.h"


#include "ext.h"		/* EXT-nodes */
#include "evt.h"		/* EVT-nodes */
#include "syn.h"		/* SYN-nodes */
#include "msg.h"		/* MSG-nodes */

#include "sco.h"		/* SCORES */
#include "opt.h"		/* OPTIONS */
#include "options.h"		/* OPTIONS */

#include "emit.h"
#include "encode.h"
#include "acode.h"



/* PUBLIC */
Adventure adv;


/* PRIVATE */
static SourceFileEntry *sourceFileEntries;



/*======================================================================*/
void initAdventure(void)
{
  initSymbols();
  initClasses();
  initInstances();
  initDumpNodeList();
  adv.ifids = initIfids();
}


/*======================================================================*/
void symbolizeAdventure()
{
  symbolizeClasses();
  symbolizeInstances();

  symbolizeWhere(adv.whr);
}


/*----------------------------------------------------------------------*/
static void analyzeStartAt(void)
{
  /* START has the same environment as a RULE */
  Context *context = newRuleContext();

  if (adv.whr != NULL)
    switch (adv.whr->kind) {
    case WHERE_AT: {
      What *what = adv.whr->what->fields.wht.wht;
      if (what->kind == WHAT_ID) {
	instanceCheck(what->id, "START statement", "location");
      } else
	lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    }
    default:
      lmLog(&adv.whr->srcp, 211, sevERR, "");
      break;
    }

  analyzeStatements(adv.stms, context);
}

/*----------------------------------------------------------------------*/
static void analyzeSourceFilenames() {
  List *currentFile;
  int count = 0;

  sourceFileEntries = allocate(length(fileNames)*sizeof(SourceFileEntry));
  TRAVERSE(currentFile,fileNames) {
    sourceFileEntries[count].fpos = ftell(txtfil);
    sourceFileEntries[count].len = strlen(currentFile->element.str);
    fprintf(txtfil, currentFile->element.str);
    count++;
  }
}



/*====================================================================== */
void analyzeAdventure(void)
{
  addHero();

  symbolizeAdventure();
  addAdditions();
  setupDefaultProperties();

  analyzeAllAttributes();	/* Make sure attributes are analyzed
				   and typed before expressions */
  numberAllAttributes();	/* Then we can number and type check
				   inherited attributes */
  replicateInherited();

  prepareWords();			/* Prepare words in the dictionary */
  prepareMessages();			/* Prepare standard and user messages */
  prepareScores();			/* Prepare score handling */

  if (verbose) printf("\n\tSyntax definitions: ");
  analyzeSyntaxes();

  if (verbose) printf("\n\tVerbs: ");
  analyzeVerbs(adv.vrbs, NULL);

  if (verbose) printf("\n\tClasses: ");
  analyzeClasses();

  if (verbose) printf("\n\tInstances: ");
  analyzeInstances();
  theHero->fields.entity.props->whr = adv.whr;

  numberContainers();

  if (verbose) printf("\n\tEvents: ");
  analyzeEvents();

  if (verbose) printf("\n\tRules: ");
  analyzeRules();

  if (verbose) printf("\n\tSynonyms: ");
  analyzeSynonyms();

  if (verbose) printf("\n\tMessages:");
  analyzeMessages();

  if (verbose) printf("\n");

  analyzeStartAt();
  analyzeAllWords();
  adv.resources = analyzeResources(adv.resources);

  if (debugFlag)
    analyzeSourceFilenames();
}


/*----------------------------------------------------------------------*/
static Aaddr generateSourceFileTable() {
  int count = 0;
  Aaddr adr = nextEmitAddress();

  if (opts[OPTDEBUG].value)
    for (count = 0; count < length(fileNames); count++) {
      encode(&sourceFileEntries[count].fpos, &sourceFileEntries[count].len);
      emitEntry(&sourceFileEntries[count], sizeof(SourceFileEntry));
    }
  emit(EOF);
  return adr;
}


/*======================================================================*/
void generateAdventure(char acodeFileName[],
		       char textFileName[],
		       char dataFileName[])
{
  initEmit(acodeFileName);		/* Initialise code emit */
  initEncoding(textFileName, dataFileName);	/* Initialise encoding */
  if (lmSeverity() > sevWAR)
    return;

  acodeHeader.ifids = generateIfids(adv.ifids);

  if (verbose) printf("\n\tDictionary: ");
  acodeHeader.dictionary = generateAllWords();

  if (verbose) printf("\n\tSyntaxTable: ");
  acodeHeader.syntaxTableAddress = generateParseTable();

  if (verbose) printf("\n\tParameter Mapping: ");
  acodeHeader.parameterMapAddress = generateParameterMappingTable();
  acodeHeader.maxParameters = 10;	/* TODO calculate and move this to a better place */

  if (verbose) printf("\n\tVerbs: ");
  acodeHeader.verbTableAddress = generateVerbs(adv.vrbs);

  if (verbose) printf("\n\tClasses: ");
  acodeHeader.classTableAddress = generateClasses();

  if (verbose) printf("\n\tInstances: ");
  generateInstances(&acodeHeader);

  if (verbose) printf("\n\tContainers: ");
  acodeHeader.containerTableAddress = generateContainers(&acodeHeader);

  if (verbose) printf("\n\tScripts: ");
  acodeHeader.scriptTableAddress = generateScripts(&acodeHeader);

  if (verbose) printf("\n\tEvents: ");
  acodeHeader.eventTableAddress = generateEvents(&acodeHeader);

  if (verbose) printf("\n\tRules: ");
  acodeHeader.ruleTableAddress = generateRules();

  generateScores(&acodeHeader);

  if (verbose) printf("\n\tMessages: ");
  acodeHeader.messageTableAddress = gemsgs();

  if (verbose) printf("\n\tCharacter Encoding: ");
  acodeHeader.freq = gefreq();	/* Character frequencies */


  /* Options */
  generateOptions(&acodeHeader);

  /* Start statements */
  acodeHeader.start = nextEmitAddress();	/* Save ACODE address to start */
  generateStatements(adv.stms);
  emit0(I_RETURN);

  /* String & Set attribute initialisation tables */
  acodeHeader.stringInitTable = generateStringInit();
  acodeHeader.setInitTable = generateSetInit();

  /* Source filename and line table */
  acodeHeader.sourceFileTable = generateSourceFileTable();
  acodeHeader.sourceLineTable = generateSrcps();

  /* All resource files found so package them */
  generateResources(adv.resources);

  terminateEncoding();
  terminateEmit();
  emitTextDataToAcodeFile(dataFileName);
  emitHeader();
  if (verbose) printf("\n");
}




/*====================================================================== */
void dumpAdventure(enum dmpKd dmp)
{
  if (dmp&DUMP_ALL)
    dmp |= (-1L&~DUMP_ADDRESSES);


  put("ADV: "); indent();

  put("synonyms: ");
  if (dmp&DUMP_SYNONYMS)
    dumpList(adv.syns, SYNONYM_LIST);
  else
    put("--");
  nl();

  put("syntaxes: ");
  if (dmp&DUMP_SYNTAXTABLE)
    dumpList(adv.stxs, SYNTAX_LIST);
  else
    put("--");
  nl();

  put("verbs: ");
  if (dmp&DUMP_VERBTABLE)
    dumpList(adv.vrbs, VERB_LIST);
  else
    put("--");
  nl();

  put("classes: ");
  if (dmp&DUMP_CLASSES) {
    dumpList(adv.clas, CLASS_LIST);
    nl();
    put("additions: ");
    dumpList(adv.adds, ADD_LIST);
  } else
    put("--");
  nl();

  put("instances: ");
  if (dmp&DUMP_INSTANCES)
    dumpList(adv.inss, INSTANCE_LIST);
  else
    put("--");
  nl();

  put("containers: ");
  if (dmp&DUMP_CONTAINERS)
    dumpList(adv.cnts, CONTAINER_LIST);
  else
    put("--");
  nl();

  put("events: ");
  if (dmp&DUMP_EVENTS)
    dumpList(adv.evts, EVENT_LIST);
  else
    put("--");
  nl();

  put("rules: ");
  if (dmp&DUMP_RULES)
    dumpList(adv.ruls, RULE_LIST);
  else
    put("--");
  nl();

  put("symbols: ");
  if (dmp&DUMP_SYMBOLTABLE)
    dumpSymbols();
  else
    put("--");
  nl();

  put("whr: ");
  dumpWhere(adv.whr);
  nl();

  put("stms: ");
  dumpList(adv.stms, STATEMENT_LIST);
  out();
  nl();

  put("ifids: ");
  dumpList(adv.ifids, IFID_LIST);
  out();
  nl();
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
  (void)sprintf(str,   "        Acode:                  %6ld words (%ld bytes)",
		acodeHeader.size, acodeHeader.size*sizeof(Aword));
  lmLiPrint(str);
  (void)sprintf(str,   "        Text data:              %6d bytes", txtlen);
  lmLiPrint(str);
}
