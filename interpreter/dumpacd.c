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

static int dictionaryFlag, classesFlag, instancesFlag, parseFlag, syntaxesFlag,
verbsFlag, eventsFlag, containersFlag, rulesFlag, statementsFlag;



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

  for (adr = pointerTo(awords); *(adr+1) != EOF; adr++)
    printf("%ld, ", *adr);
  printf("%ld\n", *adr);
}




/*----------------------------------------------------------------------*/
static void dumpWrdClass(Aword class)
{
  if ((class&((Aword)1L<<VERB_WORD)) != 0) printf("Verb ");
  if ((class&((Aword)1L<<CONJUNCTION_WORD)) != 0) printf("Conjunction ");
  if ((class&((Aword)1L<<BUT_WORD)) != 0) printf("But ");
  if ((class&((Aword)1L<<THEM_WORD)) != 0) printf("Them ");
  if ((class&((Aword)1L<<IT_WORD)) != 0) printf("It ");
  if ((class&((Aword)1L<<NOUN_WORD)) != 0) printf("Noun ");
  if ((class&((Aword)1L<<ADJECTIVE_WORD)) != 0) printf("Adjective ");
  if ((class&((Aword)1L<<PREPOSITION_WORD)) != 0) printf("Preposition ");
  if ((class&((Aword)1L<<ALL_WORD)) != 0) printf("All ");
  if ((class&((Aword)1L<<DIRECTION_WORD)) != 0) printf("Direction ");
  if ((class&((Aword)1L<<NOISE_WORD)) != 0) printf("Noise ");
  if ((class&((Aword)1L<<SYNONYM_WORD)) != 0) printf("Synonym ");
  printf("\n");
}



/*----------------------------------------------------------------------*/
static void dumpDict(int level, Aword dict)
{
  WrdEntry *wrd;

  if (dict == 0) return;

  for (wrd = (WrdEntry *)pointerTo(dict); !endOfTable(wrd); wrd++) {
    indent(level);
    printf("WORD: \n");
    indent(level+1);
    printf("word: %ld(0x%lx)", wrd->wrd, wrd->wrd);
    if (wrd->wrd != 0)
      printf(" -> \"%s\"", (char *)&memory[wrd->wrd]);
    printf("\n");
    indent(level+1);
    printf("class: %ld = ", wrd->class); dumpWrdClass(wrd->class);
    indent(level+1);
    printf("code: %ld\n", wrd->code);
    indent(level+1);
    printf("adjective references: %s", dumpAddress(wrd->adjrefs));
    if (wrd->adjrefs != 0) {
      printf(" -> ");
      dumpAwords(wrd->adjrefs);
    } else
      printf("\n");
    indent(level+1);
    printf("noun references: %s", dumpAddress(wrd->nounrefs));
    if (wrd->nounrefs != 0) {
      printf(" -> ");
      dumpAwords(wrd->nounrefs);
    } else
      printf("\n");
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
    printf("ATTRIBUTE: #%d\n", atr->code);
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
    printf("limits: %s\n", dumpAddress(cnt->limits));
    indent(level+1);
    printf("header: %s\n", dumpAddress(cnt->header));
    indent(level+1);
    printf("empty: %s\n", dumpAddress(cnt->empty));
  }
}



/*----------------------------------------------------------------------*/
static void dumpInstances(int level, Aword instances)
{
  InstanceEntry *instance;

  if (instances == 0) return;

  for (instance = (InstanceEntry *)pointerTo(instances); !endOfTable(instance); instance++) {
    indent(level);
    printf("INSTANCE #%ld:\n", instance->code);
    indent(level+1);
    printf("idAdress: %s", dumpAddress(instance->idAddress)); {
      printf(" \"%s\"", pointerTo(instance->idAddress)); printf("\n");
    }
    indent(level+1);
    printf("parent: %ld\n", instance->parent);
    indent(level+1);
    printf("location: %ld\n", instance->location);
    indent(level+1);
    printf("container: %ld\n", instance->container);
    indent(level+1);
    printf("attributes: %s\n", dumpAddress(instance->attributes)); {
      dumpAtrs(level+1, instance->attributes);
    }
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
    indent(level+1);
    printf("verbs: %s\n", dumpAddress(instance->verbs));
  }
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
static void dumpExts(int level, Aword exts)
{
  ExitEntry *ext;

  if (exts == 0) return;

  for (ext = (ExitEntry *)pointerTo(exts); !endOfTable(ext); ext++) {
    indent(level);
    printf("exit:\n");
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
static void dumpElms(int level, Aword elms)
{
  ElementEntry *elm;

  if (elms == 0) return;

  for (elm = (ElementEntry *)pointerTo(elms); !endOfTable(elm); elm++) {
    indent(level);
    if (elm->code == -2)
      printf("element: Last Element in this Syntax\n");
    else
      printf("element: Word #%ld\n", elm->code);
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

  while(TRUE) {
    printf("\n0x%04lx (%ld):\t0x%04lx =\t ", pc, pc, memory[pc]);
    if (pc > memTop)
      syserr("Dumping outside program memory.");

    i = memory[pc++];
    
    switch (I_CLASS(i)) {
    case C_CONST:
      printf("PUSH  \t%5ld", I_OP(i));
      break;
    case C_CURVAR:
      switch (I_OP(i)) {
      case V_PARAM:
	printf("PARAM");
	break;
      case V_CURLOC:
	printf("CURLOC");
	break;
      case V_CURACT:
	printf("CURACT");
	break;
      case V_CURVRB:
	printf("CURVRB");
	break;
      case V_SCORE:
	printf("CURSCORE");
	break;
      default:
	syserr("Unknown CURVAR instruction.");
	break;
      }
      break;
      
    case C_STMOP: 
      switch (I_OP(i)) {
      case I_LINE: {
	printf("LINE");
	break;
      }
      case I_PRINT: {
	printf("PRINT");
	break;
      }
      case I_SYSTEM: {
	printf("SYSTEM");
	break;
      }
      case I_GETSTR: {
	printf("GETST");
	break;
      }
      case I_QUIT: {
	printf("QUIT");
	break;
      }
      case I_LOOK: {
	printf("LOOK");
	break;
      }
      case I_SAVE: {
	printf("SAVE");
	break;
      }
      case I_RESTORE: {
	printf("RESTORE");
	break;
      }
      case I_RESTART: {
	printf("RESTART");
	break;
      }
      case I_LIST: {
	printf("LIST");
	break;
      }
      case I_EMPTY: {
	printf("EMPTY");
	break;
      }
      case I_SCORE: {
	printf("SCORE");
	break;
      }
      case I_VISITS: {
	printf("VISITS");
	break;
      }
      case I_SCHEDULE: {
	printf("SCHEDULE");
	break;
      }
      case I_CANCEL: {
	printf("CANCEL");
	break;
      }
      case I_MAKE: {
	printf("MAKE");
	break;
      }
      case I_SET: {
	printf("SET");
	break;
      }
      case I_STRSET: {
	printf("STRSET");
	break;
      }
      case I_INCR: {
	printf("INCR");
	break;
      }
      case I_DECR: {
	printf("DECR");
	break;
      }
      case I_ATTRIBUTE: {
	printf("ATTRIBUTE");
	break;
      }
      case I_STRATTR: {
	printf("STRATTR");
	break;
      }
      case I_LOCATE: {
	printf("LOCATE");
	break;
      }
      case I_WHERE: {
	printf("WHERE");
	break;
      }
      case I_HERE: {
	printf("HERE");
	break;
      }
      case I_NEAR: {
	printf("NEAR");
	break;
      }
      case I_USE: {
	printf("USE");
	break;
      }
      case I_IN: {
	printf("IN ");
	break;
      }
      case I_DESCRIBE: {
	printf("DESCRIBE ");
	break;
      }
      case I_SAY: {
	printf("SAY");
	break;
      }
      case I_SAYINT: {
	printf("SAYINT");
	break;
      }
      case I_SAYSTR: {
	printf("SAYSTR");
	break;
      }
      case I_IF: {
	printf("IF");
	break;
      }
      case I_ELSE: {
	printf("ELSE");
	break;
      }
      case I_ENDIF: {
	printf("ENDIF");
	break;
      }
      case I_AND: {
	printf("AND");
	break;
      }
      case I_OR: {
	printf("OR");
	break;
      }
      case I_NE: {
	printf("NE");
	break;
      }
      case I_EQ: {
	printf("EQ ");
	break;
      }
      case I_STREQ: {
	printf("STREQ ");
	break;
      }
      case I_STREXACT: {
	printf("STREXACT ");
	break;
      }
      case I_LE: {
	printf("LE ");
	break;
      }
      case I_GE: {
	printf("GE ");
	break;
      }
      case I_LT: {
	printf("LT ");
	break;
      }
      case I_GT: {
	printf("GT ");
	break;
      }
      case I_PLUS: {
	printf("PLUS ");
	break;
      }
      case I_MINUS: {
	printf("MINUS ");
	break;
      }
      case I_MULT: {
	printf("MULT ");
	break;
      }
      case I_DIV: {
	printf("DIV ");
	break;
      }
      case I_NOT: {
	printf("NOT ");
	break;
      }
      case I_MAX: {
	printf("MAX ");
	break;
      }
      case I_SUM: {
	printf("SUM ");
	break;
      }
      case I_COUNT: {
	printf("COUNT ");
	break;
      }
      case I_RND: {
	printf("RANDOM ");
	break;
      }
      case I_BTW: {
	printf("BETWEEN ");
	break;
      }
      case I_CONTAINS: {
	printf("CONTAINS ");
	break;
      }

      case I_DEPEND:
	printf("DEPEND");
	break;

      case I_DEPCASE:
	printf("DEPCASE");
	break;

      case I_DEPEXEC: {
	printf("DEPEXEC");
	break;
      }
	
      case I_DEPELSE:
	printf("DEPELSE");
	break;

      case I_ENDDEP:
	printf("ENDDEP");
	break;

      case I_RETURN:
	printf("RETURN");
	printf("\n");
	return;

      case I_FRAME:
	printf("BLOCK");
	break;

      case I_ENDFRAME:
	printf("ENDBLOCK");
	break;

      case I_GETLOCAL:
	printf("GETLOCAL");
	break;

      case I_SETLOCAL:
	printf("SETLOCAL");
	break;

      case I_EACH:
	printf("EACH");
	break;

      case I_ENDEACH:
	printf("ENDEACH");
	break;

      case I_STRIP:
	printf("STRIP");
	break;

      case I_POP:
	printf("POP");
	break;

      default:
	printf("*** Unknown STMOP instruction ***");
	break;
      }
      break;

    default:
      printf("*** Unknown instruction class ***");
      break;
    }
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

  printf("INSTANCE TABLE: %s\n", dumpAddress(header->instanceTableAddress));
  if (instancesFlag) dumpInstances(1, header->instanceTableAddress);

  printf("RULE TABLE: %s\n", dumpAddress(header->ruleTableAddress));

  printf("INIT: %s\n", dumpAddress(header->stringInitTable));

  printf("START: %s\n", dumpAddress(header->start));

  printf("MESSAGE TABLE: %s\n", dumpAddress(header->messageTableAddress));

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
    printf("Could not open ACD-file: '%s'\n", acdfnm);
    exit(1);
  }

  fread(&tmphdr, sizeof(tmphdr), 1, codfil);
  if (tmphdr.tag[3] != 'A' && tmphdr.tag[1] != 'A' && tmphdr.tag[2] != 'A' && tmphdr.tag[0] != 'N') {
    printf("Not an ACD-file: '%s'\n", acdfnm);
    exit(1);
  }    

  rewind(codfil);

  /* Allocate and load memory */

#ifdef REVERSED
  reverseHdr(&tmphdr);
#endif

  memory = malloc(tmphdr.size*sizeof(Aword));
  header = (AcdHdr *) memory;

  memTop = fread(pointerTo(0), sizeof(Aword), tmphdr.size, codfil);
  if (memTop != tmphdr.size)
    printf("WARNING! Could not read all ACD code.");

#ifdef REVERSED
  printf("Hmm, this is a little-endian machine, please wait a moment while I fix byte ordering....\n");
  reverseACD();			/* Reverse all words in the ACD file */
  printf("OK.\n");
#endif
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
     SPA_FLAG("instances", "dump details on instance entries", instancesFlag, FALSE, NULL)
     SPA_FLAG("parses", "dump details on parse table entries", parseFlag, FALSE, NULL)
     SPA_FLAG("syntaxes", "dump details on syntax mapping entries", syntaxesFlag, FALSE, NULL)
     SPA_FLAG("verbs", "dump details on verb entries", verbsFlag, FALSE, NULL)
     SPA_FLAG("events", "dump details on event entries", eventsFlag, FALSE, NULL)
     SPA_FLAG("containers", "dump details on container entries", containersFlag, FALSE, NULL)
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
