/*----------------------------------------------------------------------*\

  reverse.c

  Handles all reversing of data

\*----------------------------------------------------------------------*/


#include "types.h"
#include "main.h"

#include "reverse.h"


#define NATIVE(w)   \
    ( (((Aword)((w)[3])      ) & 0x000000ff)    \
    | (((Aword)((w)[2]) <<  8) & 0x0000ff00)    \
    | (((Aword)((w)[1]) << 16) & 0x00ff0000)    \
    | (((Aword)((w)[0]) << 24) & 0xff000000))

/*----------------------------------------------------------------------*/
Aword reversed(Aword w) /* IN - The ACODE word to swap bytes of */
{
#ifdef TRYNATIVE
  return NATIVE(&w);
#else
  Aword s;                      /* The swapped ACODE word */
  char *wp, *sp;
  int i;
  
  wp = (char *) &w;
  sp = (char *) &s;

  for (i = 0; i < sizeof(Aword); i++)
    sp[sizeof(Aword)-1 - i] = wp[i];

  return s;
#endif
}


void reverse(Aword *w)          /* IN - The ACODE word to reverse bytes in */
{
  *w = reversed(*w);
}


static void reverseTable(Aword adr, int len)
{
  Aword *e = &memory[adr];
  int i;

  if (adr != 0)
    while (!endOfTable(e)) {
      if (len < sizeof(Aword)) {
	printf("***Wrong size in 'reverseTable()' ***");
	exit(-1);
      }	       
      for (i = 0; i < len/sizeof(Aword); i++) {
	reverse(e);
	e++;
      }
    }
}


static void reverseStms(Aword adr)
{
  Aword *e = &memory[adr];

  if (adr != 0)
    while (TRUE) {
      reverse(e);
      if (*e == ((Aword)C_STMOP<<28|(Aword)I_RETURN)) break;
      e++;
    }
}


static void reverseMsgs(Aword adr)
{
  MsgEntry *e = (MsgEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(MsgEntry));
    while (!endOfTable(e)) {
      reverseStms(e->stms);
      e++;
    }
  }
}    


static void reverseDictionary(Aword adr)
{
  WrdEntry *e = (WrdEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(WrdEntry));
    while (!endOfTable(e)) {
      if ((e->class & (1L<<SYNONYM_WORD)) == 0) { /* Do not do this for synonyms */
	reverseTable(e->adjrefs, sizeof(Aword));
	reverseTable(e->nounrefs, sizeof(Aword));
      }
      e++;
    }
  }
}    


static void reverseChks(Aword adr)
{
  ChkEntry *e = (ChkEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ChkEntry));
    while (!endOfTable(e)) {
      reverseStms(e->exp);
      reverseStms(e->stms);
      e++;
    }
  }
}    


static void reverseAlts(Aword adr)
{
  AltEntry *e = (AltEntry *)&memory[adr];

  if (adr != 0 && !endOfTable(e) && !e->done) {
    reverseTable(adr, sizeof(AltEntry));
    e->done = TRUE;
    while (!endOfTable(e)) {
      reverseChks(e->checks);
      reverseStms(e->action);
      e++;
    }
  }
}    


static void reverseVerbs(Aword adr)
{
  VerbEntry *e = (VerbEntry *)&memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(VerbEntry));
    while (!endOfTable(e)) {
      reverseAlts(e->alts);
      e++;
    }
  }
}    


static void reverseSteps(Aword adr)
{
  StepEntry *e = (StepEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(StepEntry));
    while (!endOfTable(e)) {
      reverseStms(e->exp);
      reverseStms(e->stm);
      e++;
    }
  }
}    


static void reverseScrs(Aword adr)
{
  ScriptEntry *e = (ScriptEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ScriptEntry));
    while (!endOfTable(e)) {
      reverseStms(e->description);
      reverseSteps(e->steps);
      e++;
    }
  }
}    


static void reverseExits(Aword adr)
{
  ExitEntry *e = (ExitEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ExitEntry));
    while (!endOfTable(e)) {
      if (!e->done) {
	reverseChks(e->checks);
	reverseStms(e->action);
      }
      e++;
    }
  }
}    


static void reverseClasses(Aword adr)
{
  ClassEntry *e = (ClassEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ClassEntry));
    while (!endOfTable(e)) {
      reverseChks(e->checks);
      reverseStms(e->description);
      reverseVerbs(e->verbs);
      e++;
    }
  }
}


static void reverseInstances(Aword adr)
{
  InstanceEntry *e = (InstanceEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(InstanceEntry));
    while (!endOfTable(e)) {
      reverseTable(e->attributes, sizeof(AttributeEntry));
      reverseStms(e->mentioned);
      reverseStms(e->article);
      reverseChks(e->checks);
      reverseStms(e->description);
      reverseExits(e->exits);
      reverseVerbs(e->verbs);
      e++;
    }
  }
}


static void reverseRestrictions(Aword adr)
{
  RestrictionEntry *e = (RestrictionEntry *) &memory[adr];
  static Aword *done = NULL;
  static int numberDone = 0;
  int i;
  Boolean found = FALSE;

  if (adr == 0) return;

  /* Have we already done it? */
  for (i = 0; i < numberDone; i++) if (done[i] == adr) {found = TRUE; break;}
  if (found) return;

  done = realloc(done, (numberDone+1)*sizeof(Aword));
  done[numberDone] = adr;
  numberDone++;

  if (!endOfTable(e)) {
    reverseTable(adr, sizeof(RestrictionEntry));
    while (!endOfTable(e)) {
      reverseStms(e->stms);
      e++;
    }
  }
}    


static void reverseElms(Aword adr)
{
  ElementEntry *e = (ElementEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ElementEntry));
    while (!endOfTable(e)) {
      if (e->code == EOS) reverseRestrictions(e->next);
      else reverseElms(e->next);
      e++;
    }
  }
}    


static void reverseParseTable(Aword adr)
{
  ParseEntry *e = (ParseEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ParseEntry));
    while (!endOfTable(e)) {
      reverseElms(e->elms);
      e++;
    }
  }
}    


static void reverseSyntaxTable(Aword adr)
{
  SyntaxEntry *e = (SyntaxEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(SyntaxEntry));
    while (!endOfTable(e)) {
      reverseTable(e->parameterMapping, sizeof(Aword));
      e++;
    }
  }
}    


static void reverseEvts(Aword adr)
{
  EventEntry *e = (EventEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(EventEntry));
    while (!endOfTable(e)) {
      reverseStms(e->code);
      e++;
    }
  }
}    


static void reverseLims(Aword adr)
{
  LimEntry *e = (LimEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(LimEntry));
    while (!endOfTable(e)) {
      reverseStms(e->stms);
      e++;
    }
  }
}    


static void reverseContainers(Aword adr)
{
  ContainerEntry *e = (ContainerEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ContainerEntry));
    while (!endOfTable(e)) {
      reverseLims(e->limits);
      reverseStms(e->header);
      reverseStms(e->empty);
      reverseChks(e->extractChecks);
      reverseStms(e->extractStatements);
      e++;
    }
  }
}    


static void reverseRuls(Aword adr)
{
  RulEntry *e = (RulEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(RulEntry));
    while (!endOfTable(e)) {
      reverseStms(e->exp);
      reverseStms(e->stms);
      e++;
    }
  }
}    



/*----------------------------------------------------------------------*/
void reverseHdr(AcdHdr *hdr)
{
  int i;

  /* Reverse all words in the header except the first (version marking) */
  for (i = 1; i < sizeof(AcdHdr)/sizeof(Aword); i++)
    reverse(&((Aword *)hdr)[i]);
}

/*----------------------------------------------------------------------

  reverseACD()

  Traverse all the data structures and reverse all integers.
  Only performed in architectures with reversed byte ordering, which
  makes the .ACD files fully compatible across architectures

  */
#ifdef _PROTOTYPES_
void reverseACD(Boolean v2_5)
#else
void reverseACD(v2_5)
     Boolean v2_5;
#endif
{
  reverseHdr(header);
  reverseDictionary(header->dictionary);
  reverseParseTable(header->parseTableAddress);
  reverseSyntaxTable(header->syntaxTableAddress);
  reverseVerbs(header->verbTableAddress);
  reverseClasses(header->classTableAddress);
  reverseInstances(header->instanceTableAddress);
  reverseScrs(header->scriptTableAddress);
  reverseContainers(header->containerTableAddress);
  reverseEvts(header->eventTableAddress);
  reverseRuls(header->ruleTableAddress);
  reverseTable(header->init, sizeof(IniEntry));
  reverseStms(header->start);
  reverseMsgs(header->messageTableAddress);
 
  reverseTable(header->scores, sizeof(Aword));
  reverseTable(header->freq, sizeof(Aword));
}
