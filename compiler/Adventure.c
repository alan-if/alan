/*----------------------------------------------------------------------*\

			     Adventure.c

			    Adventure Node

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "Srcp.h"
#include "lmList.h"

#include "acode.h"

#include "Adventure.h"
#include "Symbol.h"
#include "Class.h"
#include "Instance.h"
#include "Where.h"
#include "Statement.h"
#include "Verb.h"
#include "Event.h"
#include "Rule.h"
#include "Word.h"
#include "Synonym.h"
#include "Attribute.h"
#include "Message.h"

#include "Score.h"
#include "Option.h"

#include "emit.h"
#include "encode.h"

#include "dump.h"



/* PUBLIC */
Adventure adventure;


/* PRIVATE */
static AcdHdr header;	/* ACODE program header */
static Aword end;


/*======================================================================

  initAdventure()

  Initialise any structures and data needed.

  */
#ifdef _PROTOTYPES_
void initAdventure(void)
#else
void initAdventure()
#endif
{
  initClasses();		/* Create predefined classes */
  initInstances();		/* Create predefined instances */
}



/*======================================================================

  analyseAdventure()

  Analyse the adventure.

 */
#ifdef _PROTOTYPES_
void analyseAdventure(void)
#else
void analyseAdventure()
#endif
{

  analyseAttributes(adventure.objectAttributes);
  analyseAttributes(adventure.locationAttributes);
  analyseAttributes(adventure.actorAttributes);

  /* 4f - Do the analysis of the adventure! */

  if (adventure.where != NULL) 
    switch (adventure.where->where) {
    case WHERE_AT:
      if (adventure.where->what->what == WHAT_ID)
	symcheck(adventure.where->what->id, LOCATION_CLASS, NULL);
      else
	lmLog(&adventure.where->srcp, 211, sevERR, "");
      break;
    default:
      lmLog(&adventure.where->srcp, 211, sevERR, "");
      break;
    }

  if (adventure.statements != NULL)
    anstatements(adventure.statements, NULL, NULL, NULL);
}




/*======================================================================

  generateAdventure()

  Generate the whole adventure.

 */
#ifdef _PROTOTYPES_
void generateAdventure(char *acdFileName) /* IN - ACODE file name */
#else
void generateAdventure(acdFileName)
     char acdFileName[];
#endif
{
  initEmit(acdFileName);
  initEncoding();
  if (lmSeverity() > sevWAR)
    return;
  
  if (verbose) printf("\n\tDictionary...");
  header.dict = generateWords();

  if (verbose) printf("\n\tSyntax Definitions...");
  header.stxs = generateSyntaxes();

  if (verbose) printf("\n\tVerbs...");
  header.vrbs = generateVerbs(adventure.verbs, NULL); /* Global verbs */

  if (verbose) printf("\n\tClasses...");
  header.cnts = generateClasses();

  if (verbose) printf("\n\tInstances...");
  header.objs = generateInstances();

  if (verbose) printf("\n\tEvents...");
  header.evts = generateEvents();

  if (verbose) printf("\n\tRules...");
  header.ruls = generateRules();

  header.scores = generateScores();
  header.maxscore = maxScore;

  if (verbose) printf("\n\tMessages...");
  header.msgs = generateMessages();

  if (verbose) printf("\n\tCharacter Encoding...");
  header.freq = generateFreq();	/* Character frequencies */

  /* Options */
  generateOptions(&header);

  /* Start statements */
  header.start = emitAddress();	/* Save ACODE address to start */
  generateStatements(adventure.statements, NULL);
  emit0(C_STMOP, I_RETURN);

  /* String initialisation table */
  header.init = generateStringInit();

  end = emitAddress();		/* Last address */
  header.size = end;		/* Save size */
  terminateEncoding();
  terminateEmit(&header);
  if (verbose) printf("\n");
}




/*======================================================================

  dumpAdventure()

  Dump the Adventure.

 */
#ifdef _PROTOTYPES_
void dumpAdventure(DumpKind dumpFlags)
#else
void dumpAdventure(dumpFlags)
     DumpKind;
#endif
{
  if (dumpFlags&DMPALL)
    dumpFlags = (DumpKind)-1L;

  put("ADVENTURE: "); in();
  put("objectAttributes: "); dumpList(adventure.objectAttributes, ATTRIBUTE_NODE); nl();
  put("locationAttributes: "); dumpList(adventure.locationAttributes, ATTRIBUTE_NODE); nl();
  put("actorAttributes: "); dumpList(adventure.actorAttributes, ATTRIBUTE_NODE); nl();
  put("classes: "); if (dumpFlags&DUMP_CLASS) dumpList(adventure.classes, CLASS_NODE); else put("--"); nl();
  put("instances: "); if (dumpFlags&DUMP_INSTANCE) dumpList(adventure.instances, INSTANCE_NODE); else put("--"); nl();
  put("synonyms: "); if (dumpFlags&DUMP_SYNONYM) dumpList(adventure.synonyms, SYNONYM_NODE); else put("--"); nl();
  put("syntaxs: "); if (dumpFlags&DUMP_SYNTAX) dumpList(adventure.syntaxs, SYNTAX_NODE); else put("--"); nl();
  put("verbs: "); if (dumpFlags&DUMP_VERB) dumpList(adventure.verbs, VREB_NODE); else put("--"); nl();
  put("events: "); if (dumpFlags&DUMP_EVENT) dumpList(adventure.events, EVENT_NODE); else put("--"); nl();
  put("rules: "); if (dumpFlags&DUMP_RULE) dumpList(adventure.rules, RULE_NODE); else put("--"); nl();
  put("where: "); dumpWhere(adventure.where); nl();
  put("statements: "); dumpList(adventure.statements, STATEMENT_NODE); nl();
  put("scores: "); out();
}



/*======================================================================

  summary()

  Print out a short summary of the adventure.

 */
#ifdef _PROTOTYPES_
void summary(void)
#else
void summary()
#endif
{
  char str[80];
  
  lmSkipLines(8);
  
  lmLiPrint("");
  lmLiPrint("        Summary");
  lmLiPrint("        -------");
  if (loccount != 0) {
    sprintf(str, "        Locations:              %6d", loccount);
    lmLiPrint(str);
  }
  if (verbcount != 0) {
    sprintf(str, "        Verbs:                  %6d", verbcount);
    lmLiPrint(str);
  }
  if (objcount != 0) {
    sprintf(str, "        Objects:                %6d", objcount);
    lmLiPrint(str);
  }
  if (actcount > 1) {
    sprintf(str, "        Actors:                 %6d (incl. the Hero)", actcount);
    lmLiPrint(str);
  }
  sprintf(str  , "        Words:                  %6d", words[WRD_CLASSES]);
  lmLiPrint(str);
  sprintf(str,   "        Acode:                  %6d words (%d bytes)", end, (int)((long)end*(long)sizeof(Aword)));
  lmLiPrint(str);
  sprintf(str,   "        Text data:              %6d bytes", txtlen);
  lmLiPrint(str);
}
