/*----------------------------------------------------------------------*\

  dumpacd


  Dump an .ACD file in something like a symblic format.

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"

#include "reverse.h"

#define endOfTable(x) ((*(Aword *) x) == EOF)


/* The Amachine memory */
Aword *memory;
AcdHdr *header;

static int memTop = 0;			/* Top of load memory */

static char *acdfnm;



/* Dump flags */

static int dictionaryFlag, classesFlag, instanceFlag, parseFlag, syntaxesFlag,
  initFlag, verbsFlag, eventsFlag, exitsFlag, containersFlag, rulesFlag, statementsFlag,
  messagesFlag;



int eot(Aword *adr)
{
  return *adr == EOF;
}



static void syserr(char str[])
{
  printf("ERROR - %s\n", str);
  exit(0);
}


static void indent(int level)
{
  int i;

  for (i = 0; i < level; i++)
    printf("|   ");
}


static char *dumpBoolean(Bool bool)
{
  return bool?"true":"false";
}


static char *dumpAddress(Aword address)
{
  static char buffer[100];
  sprintf(buffer, "%ld(0x%lx)", address, address);
  return buffer;
}


/*----------------------------------------------------------------------*/
static void dumpAwords(Aaddr awords)
{
  Aword *adr;

  if (awords == 0) return;

  if (*(Aword *)pointerTo(awords) == EOF) return;

  for (adr = pointerTo(awords); *(adr+1) != EOF; adr++)
    printf("%ld, ", *adr);
  printf("%ld", *adr);
}




/*----------------------------------------------------------------------*/
static void dumpWrdClass(Aword class)
{
  if ((class&VERB_BIT) != 0) printf("Verb ");
  if ((class&CONJUNCTION_BIT) != 0) printf("Conjunction ");
  if ((class&EXCEPT_BIT) != 0) printf("Except ");
  if ((class&THEM_BIT) != 0) printf("Them ");
  if ((class&IT_BIT) != 0) printf("It ");
  if ((class&NOUN_BIT) != 0) printf("Noun ");
  if ((class&ADJECTIVE_BIT) != 0) printf("Adjective ");
  if ((class&PREPOSITION_BIT) != 0) printf("Preposition ");
  if ((class&ALL_BIT) != 0) printf("All ");
  if ((class&DIRECTION_BIT) != 0) printf("Direction ");
  if ((class&NOISE_BIT) != 0) printf("Noise ");
  if ((class&SYNONYM_BIT) != 0) printf("Synonym ");
  if ((class&PRONOUN_BIT) != 0) printf("Pronoun ");
  printf("\n");
}



/*----------------------------------------------------------------------*/
static void dumpDict(int level, Aword dictionary)
{
  DictionaryEntry *wrd;
  int w = 0;

  if (dictionary == 0) return;

  for (wrd = (DictionaryEntry *)pointerTo(dictionary); !endOfTable(wrd); wrd++) {
    indent(level);
    printf("WORD: [%d]\n", w);
    indent(level+1);
    printf("word: %ld(0x%lx)", wrd->wrd, wrd->wrd);
    if (wrd->wrd != 0)
      printf(" -> \"%s\"", (char *)&memory[wrd->wrd]);
    printf("\n");
    indent(level+1);
    printf("class: %ld = ", wrd->classBits); dumpWrdClass(wrd->classBits);
    indent(level+1);
    printf("code: %ld\n", wrd->code);
    if (wrd->classBits&ADJECTIVE_BIT) {
      indent(level+1);
      printf("adjective references: %s", dumpAddress(wrd->adjectiveRefs));
      if (wrd->adjectiveRefs != 0) {
	printf(" -> {");
	dumpAwords(wrd->adjectiveRefs);
      }
      printf("}\n");
    }
    if (wrd->classBits&NOUN_BIT) {
      indent(level+1);
      printf("noun references: %s", dumpAddress(wrd->nounRefs));
      if (wrd->nounRefs != 0) {
	printf(" -> {");
	dumpAwords(wrd->nounRefs);
      }
      printf("}\n");
    }
    if (wrd->classBits&PRONOUN_BIT) {
      indent(level+1);
      printf("pronoun references: %s", dumpAddress(wrd->pronounRefs));
      if (wrd->pronounRefs != 0) {
	printf(" -> {");
	dumpAwords(wrd->pronounRefs);
      }
      printf("}\n");
    }
    w++;
  }
}



/*----------------------------------------------------------------------*/
static void dumpAtrs(int level, Aword atrs)
{
  AttributeEntry *atr;
  int atrno = 1;

  if (atrs == 0) return;

  for (atr = (AttributeEntry *)pointerTo(atrs); !endOfTable(atr); atr++, atrno++) {
    indent(level);
    printf("ATTRIBUTE: #%ld\n", atr->code);
    indent(level+1);
    printf("value: %ld\n", atr->value);
    indent(level+1);
    printf("string: %s", dumpAddress(atr->stringAddress));
    if (atr->stringAddress != 0)
      printf(" -> \"%s\"", (char *)&memory[atr->stringAddress]);
    printf("\n");
  }
}



/*----------------------------------------------------------------------*/
static void dumpChks(int level, Aword chks)
{
  ChkEntry *chk;

  if (chks == 0) return;

  for (chk = (ChkEntry *)pointerTo(chks); !endOfTable(chk); chk++) {
    indent(level);
    printf("CHECK:\n");
    indent(level+1);
    printf("expression: %s\n", dumpAddress(chk->exp));
    indent(level+1);
    printf("statements: %s\n", dumpAddress(chk->stms));
  }
}



/*----------------------------------------------------------------------*/
static void dumpQual(Aword qual)
{
  switch (qual) {
  case Q_DEFAULT: printf("Default"); break;
  case Q_AFTER: printf("After"); break;
  case Q_BEFORE: printf("Before"); break;
  case Q_ONLY: printf("Only"); break;
  }
}


/*----------------------------------------------------------------------*/
static void dumpAlts(int level, Aword alts)
{
  AltEntry *alt;

  if (alts == 0) return;

  for (alt = (AltEntry *)pointerTo(alts); !endOfTable(alt); alt++) {
    indent(level);
    printf("ALTERNATIVE:\n");
    indent(level+1);
    printf("parameter: %ld\n", alt->param);
    indent(level+1);
    printf("qualifier: "); dumpQual(alt->qual); printf("\n");
    indent(level+1);
    printf("checks: %s\n", dumpAddress(alt->checks));
    dumpChks(level+2, alt->checks);
    indent(level+1);
    printf("action: %s\n", dumpAddress(alt->action));
  }
}



/*----------------------------------------------------------------------*/
static void dumpContainers(int level, Aword cnts)
{
  ContainerEntry *cnt;

  if (cnts == 0) return;

  for (cnt = (ContainerEntry *)pointerTo(cnts); !endOfTable(cnt); cnt++) {
    indent(level);
    printf("CONTAINER:\n");
    indent(level+1);
    printf("owner: %ld\n", cnt->owner);
    indent(level+1);
    printf("taking: %ld\n", cnt->class);
    indent(level+1);
    printf("limits: %s\n", dumpAddress(cnt->limits));
    indent(level+1);
    printf("header: %s\n", dumpAddress(cnt->header));
    indent(level+1);
    printf("empty: %s\n", dumpAddress(cnt->empty));
  }
}



/*----------------------------------------------------------------------*/
static void dumpExts(int level, Aword exts)
{
  ExitEntry *ext;

  if (exts == 0) return;

  for (ext = (ExitEntry *)pointerTo(exts); !endOfTable(ext); ext++) {
    indent(level);
    printf("EXIT:\n");
    indent(level+1);
    printf("code: %ld\n", ext->code);
    indent(level+1);
    printf("checks: %s\n", dumpAddress(ext->checks));
    dumpChks(level+2, ext->checks);
    indent(level+1);
    printf("action: %s\n", dumpAddress(ext->action));
    indent(level+1);
    printf("target: %s\n", dumpAddress(ext->target));
  }
}



/*----------------------------------------------------------------------*/
static void dumpClasses(int level, Aword classes)
{
  ClassEntry *class;

  if (classes == 0) return;

  for (class = (ClassEntry *)pointerTo(classes); !endOfTable(class); class++) {
    indent(level);
    printf("CLASS #%ld:\n", class->code);
    indent(level+1);
    printf("idAdress: %s", dumpAddress(class->idAddress));
    if (class->idAddress != 0)
      printf(" \"%s\"", (char *)pointerTo(class->idAddress));
    printf("\n");
    indent(level+1);
    printf("parent: %ld\n", class->parent);
    indent(level+1);
    printf("descriptionChecks: %s\n", dumpAddress(class->descriptionChecks));
    indent(level+1);
    printf("description: %s\n", dumpAddress(class->description));
    indent(level+1);
    printf("entered: %s\n", dumpAddress(class->entered));
    indent(level+1);
    printf("definite: %s\n", dumpAddress(class->definite));
    indent(level+1);
    printf("definiteIsForm: %s\n", dumpBoolean(class->indefiniteIsForm));
    indent(level+1);
    printf("indefinite: %s\n", dumpAddress(class->indefinite));
    indent(level+1);
    printf("indefiniteIsForm: %s\n", dumpBoolean(class->indefiniteIsForm));
    indent(level+1);
    printf("mentioned: %s\n", dumpAddress(class->mentioned));
    indent(level+1);
    printf("verbs: %s\n", dumpAddress(class->verbs));
  }
}


/*----------------------------------------------------------------------*/
static void dumpInstance(int instanceCode, InstanceEntry *instances) {
  InstanceEntry *instance = &instances[instanceCode-1];
  int level = 1;

  indent(level);
  printf("INSTANCE #%ld:\n", instance->code);
  indent(level+1);
  printf("idAdress: %s", dumpAddress(instance->idAddress)); {
    printf(" \"%s\"", (char *)pointerTo(instance->idAddress)); printf("\n");
  }
  indent(level+1);
  printf("parent: %ld\n", instance->parent);
  indent(level+1);
  printf("location: %ld\n", instance->initialLocation);
  indent(level+1);
  printf("container: %ld\n", instance->container);
  indent(level+1);
  printf("attributes: %s\n", dumpAddress(instance->initialAttributes));
  if (instance->initialAttributes)
    dumpAtrs(level+1, instance->initialAttributes);
  indent(level+1);
  printf("checks: %s\n", dumpAddress(instance->checks));
  indent(level+1);
  printf("description: %s\n", dumpAddress(instance->description));
  indent(level+1);
  printf("mentioned: %s\n", dumpAddress(instance->mentioned));
  indent(level+1);
  printf("article: %s\n", dumpAddress(instance->indefinite));
  indent(level+1);
  printf("exits: %s\n", dumpAddress(instance->exits));
  if (exitsFlag && instance->exits)
    dumpExts(level+2, instance->exits);
  indent(level+1);
  printf("verbs: %s\n", dumpAddress(instance->verbs));
}




/*----------------------------------------------------------------------*/
static void dumpInstances(Aword instanceTableAddress)
{
  InstanceEntry *instanceEntries = (InstanceEntry *)pointerTo(instanceTableAddress);
  int i;

  if (instanceTableAddress == 0) return;

  for (i = 0; !endOfTable(&instanceEntries[i]); i++)
    dumpInstance(i+1, instanceEntries);
}



/*----------------------------------------------------------------------*/
static void dumpVrbs(int level, Aword vrbs)
{
  VerbEntry *vrb;

  if (vrbs == 0) return;

  for (vrb = (VerbEntry *)pointerTo(vrbs); !endOfTable(vrb); vrb++) {
    indent(level);
    printf("verb:\n");
    indent(level+1);
    printf("code: %ld\n", vrb->code);
    indent(level+1);
    printf("alternative: %s\n", dumpAddress(vrb->alts));
    dumpAlts(level+2, vrb->alts);
  }
}



/*----------------------------------------------------------------------*/
static void dumpElms(int level, Aword elms)
{
  ElementEntry *elm;

  if (elms == 0) return;

  for (elm = (ElementEntry *)pointerTo(elms); !endOfTable(elm); elm++) {
    indent(level);
    if (elm->code == -2)
      printf("element: Last Element in this Parse Tree\n");
    else if (elm->code != 0)
      printf("element: Word #%ld\n", elm->code);
    else
      printf("parameter (code == 0)\n");
    indent(level+1);
    printf("flags: %s\n", dumpAddress(elm->flags));
    indent(level+1);
    printf("next (%s): %s\n", elm->code==-2?"class restrictions":"element", dumpAddress(elm->next));
    if(elm->code != EOS) {
      dumpElms(level+2, elm->next);
    }
  }
}



/*----------------------------------------------------------------------*/
static void dumpParameterMap(Aword mappingAddress)
{
  Aword *mapping;

  if (mappingAddress == 0) return;

  for (mapping = (Aword*)pointerTo(mappingAddress); !endOfTable(mapping); mapping++) {
    printf("[%ld]", *mapping);
  }
}



/*----------------------------------------------------------------------*/
static void dumpSyntaxTable(int level, Aword stxs)
{
  SyntaxEntry *stx;

  if (stxs == 0) return;

  for (stx = (SyntaxEntry *)pointerTo(stxs); !endOfTable(stx); stx++) {
    indent(level);
    printf("syntax map: Syntax #%ld\n", stx->syntaxNumber);
    indent(level+1);
    printf("parameter mapping: %s = ", dumpAddress(stx->parameterMapping));
    dumpParameterMap(stx->parameterMapping);
    printf("\n");
    indent(level+1);
    printf("verb code: #%ld\n", stx->verbCode);
  }
}



/*----------------------------------------------------------------------*/
static void dumpParseTable(int level, Aword stxs)
{
  ParseEntry *stx;

  if (stxs == 0) return;

  for (stx = (ParseEntry *)pointerTo(stxs); !endOfTable(stx); stx++) {
    indent(level);
    printf("syntax: Verb #%ld\n", stx->code);
    indent(level+1);
    printf("elements: %s\n", dumpAddress(stx->elms));
    dumpElms(level+2, stx->elms);
  }
}



/*----------------------------------------------------------------------*/
static void dumpStatements(Aword pc)
{
  Aword i;
  InstClass stm;

  while(TRUE) {
    printf("\n0x%04lx (%ld):\t0x%08lx =\t ", pc, pc, memory[pc]);
    if (pc > memTop)
      syserr("Dumping outside program memory.");

 	   i = memory[pc++];
    
	switch (I_CLASS(i)) {
	case C_CONST:
	  printf("PUSH  \t%5ld", I_OP(i));
	  break;
    case C_CURVAR:
      switch (I_OP(i)) {
      case V_PARAM: printf("PARAM"); break;
      case V_CURLOC: printf("CURLOC"); break;
      case V_CURACT: printf("CURACT"); break;
      case V_CURVRB: printf("CURVRB"); break;
      case V_SCORE: printf("CURSCORE"); break;
      default: syserr("Unknown CURVAR instruction."); break;
      }
      break;
      
    case C_STMOP: 
      stm = I_OP(i);
      switch (stm) {
      case I_AGRCHECK: printf("AGRCHECK"); break;
      case I_AGREND: printf("AGREND"); break;
      case I_AGRSTART: printf("AGRSTART"); break;
      case I_AND: printf("AND"); break;
      case I_AT: printf("AT "); break;
      case I_ATTRIBUTE: printf("ATTRIBUTE"); break;
      case I_BTW: printf("BETWEEN "); break;
      case I_CANCEL: printf("CANCEL"); break;
      case I_CONCAT: printf("CONCAT"); break;
      case I_CONTAINS: printf("CONTAINS "); break;
      case I_COUNT: printf("COUNT "); break;
      case I_DECR: printf("DECR"); break;
      case I_DEPCASE: printf("DEPCASE"); break;
      case I_DEPELSE: printf("DEPELSE"); break;
      case I_DEPEND: printf("DEPEND"); break;
      case I_DEPEXEC: printf("DEPEXEC"); break;
      case I_DESCRIBE: printf("DESCRIBE "); break;
      case I_DIV: printf("DIV "); break;
      case I_DUP: printf("DUP "); break;
      case I_EACH: printf("EACH"); break;
      case I_ELSE: printf("ELSE"); break;
      case I_EMPTY: printf("EMPTY"); break;
      case I_ENDDEP: printf("ENDDEP"); break;
      case I_ENDEACH: printf("ENDEACH"); break;
      case I_ENDFRAME: printf("ENDBLOCK"); break;
      case I_ENDIF: printf("ENDIF"); break;
      case I_EQ: printf("EQ "); break;
      case I_EXCLUDE: printf("EXCLUDE"); break;
      case I_FRAME: printf("BLOCK"); break;
      case I_GE: printf("GE "); break;
      case I_GETLOCAL: printf("GETLOCAL"); break;
      case I_GETSTR: printf("GETST"); break;
      case I_GT: printf("GT "); break;
      case I_HERE: printf("HERE"); break;
      case I_IF: printf("IF"); break;
      case I_IN: printf("IN "); break;
      case I_INCLUDE: printf("INCLUDE"); break;
      case I_INCR: printf("INCR"); break;
      case I_INSET: printf("INSET "); break;
      case I_ISA: printf("ISA"); break;
      case I_LE: printf("LE "); break;
      case I_LINE: printf("LINE"); break;
      case I_LIST: printf("LIST"); break;
      case I_LOCATE: printf("LOCATE"); break;
      case I_LOCATION: printf("LOCATION"); break;
      case I_LOOK: printf("LOOK"); break;
      case I_LT: printf("LT "); break;
      case I_MAKE: printf("MAKE"); break;
      case I_MAX: printf("MAX "); break;
      case I_MIN: printf("MIN"); break;
      case I_MINUS: printf("MINUS "); break;
      case I_MULT: printf("MULT "); break;
      case I_NE: printf("NE"); break;
      case I_NEAR: printf("NEAR"); break;
      case I_NEXTEACH: printf("NEXTEACH"); break;
      case I_NOT: printf("NOT "); break;
      case I_OR: printf("OR"); break;
      case I_PLUS: printf("PLUS "); break;
      case I_POP: printf("POP"); break;
      case I_PRINT: printf("PRINT"); break;
      case I_QUIT: printf("QUIT"); break;
      case I_RESTART: printf("RESTART"); break;
      case I_RESTORE: printf("RESTORE"); break;
      case I_RETURN: printf("RETURN"); printf("\n"); return;
      case I_RND: printf("RND "); break;
      case I_RNDINCONT: printf("RNDINCONT "); break;
      case I_RNDINSET: printf("RNDINSET "); break;
      case I_SAVE: printf("SAVE"); break;
      case I_SAY: printf("SAY"); break;
      case I_SAYINT: printf("SAYINT"); break;
      case I_SAYSTR: printf("SAYSTR"); break;
      case I_SCHEDULE: printf("SCHEDULE"); break;
      case I_SCORE: printf("SCORE"); break;
      case I_SET: printf("SET"); break;
      case I_SETLOCAL: printf("SETLOCAL"); break;
      case I_SHOW: printf("SHOW"); break;
      case I_STOP: printf("STOP"); break;
      case I_STRATTR: printf("STRATTR"); break;
      case I_STREQ: printf("STREQ "); break;
      case I_STREXACT: printf("STREXACT "); break;
      case I_STRIP: printf("STRIP"); break;
      case I_STRSET: printf("STRSET"); break;
      case I_SUM: printf("SUM "); break;
      case I_SYSTEM: printf("SYSTEM"); break;
      case I_UMINUS: printf("UMINUS"); break;
      case I_USE: printf("USE"); break;
      case I_VISITS: printf("VISITS"); break;
      case I_WHERE: printf("WHERE"); break;
      case I_STYLE: printf("STYLE"); break;
      }
      break;

    default:
      printf("*** Unknown instruction class ***");
      break;
    }
  }
}


/*----------------------------------------------------------------------*/
void dumpMessages(Aaddr messageTable)
{
  MessageEntry *entry;
  int level = 1;
  int count = 0;

  if (messageTable == 0) return;

  for (entry = (MessageEntry *)pointerTo(messageTable); !endOfTable(entry); entry++) {
    indent(level);
    printf("MESSAGE: (%d) ", count++);
    printf("stms: %ld\n", entry->stms);
  }
}


/*----------------------------------------------------------------------*/
void dumpStringInit(Aaddr stringInitTable)
{
  StringInitEntry *entry;
  int level = 1;

  if (stringInitTable == 0) return;

  for (entry = (StringInitEntry *)pointerTo(stringInitTable); !endOfTable(entry); entry++) {
    indent(level);
    printf("STRINGINIT:\n");
    indent(level+1);
    printf("fpos: %ld\n", entry->fpos);
    indent(level+1);
    printf("len: %ld\n", entry->len);
    indent(level+1);
    printf("adr: %ld\n", entry->adr);
  }
}    


/*----------------------------------------------------------------------*/
void dumpSet(Aaddr setAddress)
{
  Aword *entry;

  if (setAddress == 0) return;

  for (entry = (Aword *)pointerTo(setAddress); !endOfTable(entry); entry++) {
    printf("%ld", *entry);
    if (*(entry+1) != EOF)
      printf(",");
  }  
}

 
/*----------------------------------------------------------------------*/
void dumpSetInit(Aaddr setInitTable)
{
  SetInitEntry *entry;
  int level = 1;

  if (setInitTable == 0) return;

  for (entry = (SetInitEntry *)pointerTo(setInitTable); !endOfTable(entry); entry++) {
    indent(level);
    printf("SETINIT:\n");
    indent(level+1);
    printf("size: %ld\n", entry->size);
    indent(level+1);
    printf("set: {"); dumpSet(entry->setAddress); printf("}\n");
    indent(level+1);
    printf("adr: %ld\n", entry->adr);
  }
}    
 


/*----------------------------------------------------------------------*/
static void dumpACD(void)
{
  Aword crc = 0;
  int i;

  printf("VERSION: %d.%d(%d)%c\n", header->vers[3],
	 header->vers[2], header->vers[1], header->vers[0]?header->vers[0]:' ');
  printf("SIZE: %s\n", dumpAddress(header->size));
  printf("PACK: %s\n", header->pack?"Yes":"No");
  printf("PAGE LENGTH: %ld\n", header->pageLength);
  printf("PAGE WIDTH: %ld\n", header->pageWidth);
  printf("DEBUG: %s\n", header->debug?"Yes":"No");
  printf("DICTIONARY: %s\n", dumpAddress(header->dictionary));
  if (dictionaryFlag) dumpDict(1, header->dictionary);
  printf("PARSE TABLE: %s\n", dumpAddress(header->parseTableAddress));
  if (parseFlag) dumpParseTable(1, header->parseTableAddress);
  printf("SYNTAX TABLE: %s\n", dumpAddress(header->syntaxTableAddress));
  if (syntaxesFlag) dumpSyntaxTable(1, header->syntaxTableAddress);
  printf("VERB TABLE: %s\n", dumpAddress(header->verbTableAddress));
  if (verbsFlag) dumpVrbs(1, header->verbTableAddress);
  printf("EVENT TABLE: %s\n", dumpAddress(header->eventTableAddress));
  printf("CONTAINER TABLE: %s\n", dumpAddress(header->containerTableAddress));
  if (containersFlag) dumpContainers(1, header->containerTableAddress);
  printf("CLASS TABLE: %s\n", dumpAddress(header->classTableAddress));
  if (classesFlag) dumpClasses(1, header->classTableAddress);
  printf("INSTANCE TABLE: %s\n", dumpAddress(header->instanceTableAddress));
  if (instanceFlag == 0) dumpInstances(header->instanceTableAddress);
  else if (instanceFlag != -1) dumpInstance(instanceFlag, pointerTo(header->instanceTableAddress));
  printf("RULE TABLE: %s\n", dumpAddress(header->ruleTableAddress));
  printf("STRINGINIT: %s\n", dumpAddress(header->stringInitTable));
  if (initFlag) dumpStringInit(header->stringInitTable);
  printf("SETINIT: %s\n", dumpAddress(header->setInitTable));
  if (initFlag) dumpSetInit(header->setInitTable);
  printf("START: %s\n", dumpAddress(header->start));
  printf("MESSAGE TABLE: %s\n", dumpAddress(header->messageTableAddress));
  if (messagesFlag) dumpMessages(header->messageTableAddress);
  printf("MAX SCORE: %ld\n", header->maxscore);
  printf("SCORES: %s\n", dumpAddress(header->scores));
  printf("FREQUENCY TABLE: %s\n", dumpAddress(header->freq));
  printf("ACDCRC: 0x%lx ", header->acdcrc);
  /* Calculate checksum */
  for (i = sizeof(*header)/sizeof(Aword)+1; i < memTop; i++) {
    crc += memory[i]&0xff;
    crc += (memory[i]>>8)&0xff;
    crc += (memory[i]>>16)&0xff;
    crc += (memory[i]>>24)&0xff;
  }
  if (crc != header->acdcrc)
    printf("WARNING! Expected 0x%lx\n", crc);
  else
    printf("Ok.\n");
  printf("TXTCRC: 0x%lx\n", header->txtcrc);
  if (statementsFlag != 0)
    dumpStatements(statementsFlag);
}



/*----------------------------------------------------------------------*/
static void load(char acdfnm[])
{
  AcdHdr tmphdr;
  FILE *codfil;

  if ((codfil = fopen(acdfnm, "rb")) == NULL) {
    printf("Could not open Acode-file: '%s'\n", acdfnm);
    exit(1);
  }

  fread(&tmphdr, sizeof(tmphdr), 1, codfil);
  if (tmphdr.tag[3] != 'A' && tmphdr.tag[1] != 'A' && tmphdr.tag[2] != 'A' && tmphdr.tag[0] != 'N') {
    printf("Not an ACD-file: '%s'\n", acdfnm);
    exit(1);
  }    

  rewind(codfil);

  /* Allocate and load memory */

  if (littleEndian())
    reverseHdr(&tmphdr);

  memory = malloc(tmphdr.size*sizeof(Aword));
  header = (AcdHdr *) memory;

  memTop = fread(pointerTo(0), sizeof(Aword), tmphdr.size, codfil);
  if (memTop != tmphdr.size)
    printf("WARNING! Could not read all ACD code.");

  if (littleEndian()) {
    printf("Hmm, this is a little-endian machine, please wait a moment while I fix byte ordering....\n");
    reverseACD();			/* Reverse all words in the ACD file */
    printf("OK.\n");
  }
}

/* SPA Option handling */

#include "spa.h"


static SPA_FUN(usage)
{
  printf("Usage: DUMPACD <acdfile> [-help] [options]\n");
}


static SPA_ERRFUN(paramError)
{
  char *sevstr;

  switch (sev) {
  case 'E': sevstr = "error"; break;
  case 'W': sevstr = "warning"; break;
  default: sevstr = "internal error"; break;
  }
  printf("Parameter %s: %s, %s\n", sevstr, msg, add);
  usage(NULL, NULL, 0);
  exit(EXIT_FAILURE);
}

static SPA_FUN(extraArg)
{
  printf("Extra argument: '%s'\n", rawName);
  usage(NULL, NULL, 0);
  exit(EXIT_FAILURE);
}

static SPA_FUN(xit) {exit(EXIT_SUCCESS);}

static SPA_DECLARE(arguments)
     SPA_STRING("acdfile", "file name of the ACD-file to dump", acdfnm, NULL, NULL)
     SPA_FUNCTION("", "extra argument", extraArg)
SPA_END

static SPA_DECLARE(options)
     SPA_HELP("help", "this help", usage, xit)
     SPA_FLAG("dictionary", "dump details on dictionary entries", dictionaryFlag, FALSE, NULL)
     SPA_FLAG("classes", "dump details on class entries", classesFlag, FALSE, NULL)
     SPA_INTEGER("instance", "dump details on instance entry, use 0 for all", instanceFlag, -1, NULL)
     SPA_FLAG("init", "dump string and set initialization tables", initFlag, FALSE, NULL)
     SPA_FLAG("parses", "dump details on parse table entries", parseFlag, FALSE, NULL)
     SPA_FLAG("syntaxes", "dump details on syntax mapping entries", syntaxesFlag, FALSE, NULL)
     SPA_FLAG("verbs", "dump details on verb entries", verbsFlag, FALSE, NULL)
     SPA_FLAG("events", "dump details on event entries", eventsFlag, FALSE, NULL)
     SPA_FLAG("containers", "dump details on container entries", containersFlag, FALSE, NULL)
     SPA_FLAG("messages", "dump details on messages entries", messagesFlag, FALSE, NULL)
     SPA_FLAG("exits", "dump details on exits in any instance having exits", exitsFlag, FALSE, NULL)
     SPA_INTEGER("statements <address>", "dump statement opcodes starting at <address>", statementsFlag, 0, NULL)
SPA_END



/*======================================================================

  main()

  Open the file, load it and start dumping...


  */
int main(int argc, char **argv)
{
  int nArgs;

  nArgs = spaProcess(argc, argv, arguments, options, paramError);
  if (nArgs == 0) {
    usage(NULL, NULL, 0);
    exit(EXIT_FAILURE);
  } else if (nArgs > 1)
    exit(EXIT_FAILURE);

  load(acdfnm);
  dumpACD();
  exit(0);
}
