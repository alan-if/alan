/*----------------------------------------------------------------------*\

  reverse.c

  Handles all reversing of data

\*----------------------------------------------------------------------*/


#include "types.h"
#include "main.h"

#include "reverse.h"


/*----------------------------------------------------------------------

  reversed()

  Return the reversed bytes in the Aword

*/
#ifdef _PROTOTYPES_
Aword reversed(Aword w) /* IN - The ACODE word to swap bytes of */
#else
Aword reversed(w)
     Aword w;                   /* IN - The ACODE word to swap bytes of */
#endif
{
  Aword s;                      /* The swapped ACODE word */
  char *wp, *sp;
  int i;
  
  wp = (char *) &w;
  sp = (char *) &s;

  for (i = 0; i < sizeof(Aword); i++)
    sp[sizeof(Aword)-1 - i] = wp[i];

  return s;
}


#ifdef _PROTOTYPES_
void reverse(Aword *w)          /* IN - The ACODE word to reverse bytes in */
#else
void reverse(w)
     Aword *w;                  /* IN - The ACODE word to reverse bytes in */
#endif
{
  *w = reversed(*w);
}


#ifdef _PROTOTYPES_
static void reverseTable(Aword adr, int len)
#else
static void reverseTable(adr, len)
     Aword adr;
     int len;
#endif
{
  Aword *e = &memory[adr];
  int i;

  if (adr != 0)
    while (!endOfTable(e)) {
      for (i = 0; i < len/sizeof(Aword); i++) {
	reverse(e);
	e++;
      }
    }
}


#ifdef _PROTOTYPES_
static void reverseStms(Aword adr)
#else
static void reverseStms(adr)
     Aword adr;
#endif
{
  Aword *e = &memory[adr];

  if (adr != 0)
    while (TRUE) {
      reverse(e);
      if (*e == ((Aword)C_STMOP<<28|(Aword)I_RETURN)) break;
      e++;
    }
}


#ifdef _PROTOTYPES_
static void reverseMsgs(Aword adr)
#else
static void reverseMsgs(adr)
     Aword adr;
#endif
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


#ifdef _PROTOTYPES_
static void reverseDictionary(Aword adr)
#else
static void reverseDictionary(adr)
     Aword adr;
#endif
{
  WrdEntry *e = (WrdEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(WrdEntry));
    while (!endOfTable(e)) {
      if ((e->class & (1L<<WRD_SYN)) == 0) { /* Do not do this for synonyms */
	reverseTable(e->adjrefs, sizeof(Aword));
	reverseTable(e->nounrefs, sizeof(Aword));
      }
      e++;
    }
  }
}    

#ifdef _PROTOTYPES_
static void reverseChks(Aword adr)
#else
static void reverseChks(adr)
     Aword adr;
#endif
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

#ifdef _PROTOTYPES_
static void reverseAlts(Aword adr)
#else
static void reverseAlts(adr)
     Aword adr;
#endif
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


#ifdef _PROTOTYPES_
static void reverseVerbs(Aword adr)
#else
static void reverseVerbs(adr)
     Aword adr;
#endif
{
  VrbEntry *e = (VrbEntry *)&memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(VrbEntry));
    while (!endOfTable(e)) {
      reverseAlts(e->alts);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseSteps(Aword adr)
#else
static void reverseSteps(adr)
     Aword adr;
#endif
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


#ifdef _PROTOTYPES_
static void reverseScrs(Aword adr)
#else
static void reverseScrs(adr)
     Aword adr;
#endif
{
  ScrEntry *e = (ScrEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ScrEntry));
    while (!endOfTable(e)) {
      reverseStms(e->dscr);
      reverseSteps(e->steps);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseExits(Aword adr)
#else
static void reverseExits(adr)
     Aword adr;
#endif
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


#ifdef _PROTOTYPES_
static void reverseInstances(Aword adr)
#else
static void reverseInstances(adr)
     Aword adr;
#endif
{
  InstanceEntry *e = (InstanceEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(InstanceEntry));
    while (!endOfTable(e)) {
      reverseTable(e->attributes, sizeof(AttributeEntry));
      reverseStms(e->mentioned);
      reverseStms(e->description);
      reverseExits(e->exits);
      reverseVerbs(e->verbs);
      e++;
    }
  }
}


#ifdef _PROTOTYPES_
static void reverseRestrictions(Aword adr)
#else
static void reverseRestrictions(adr)
     Aword adr;
#endif
{
  RestrictionEntry *e = (RestrictionEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(RestrictionEntry));
    while (!endOfTable(e)) {
      reverseStms(e->stms);
      e++;
    }
  }
  if (adr)
    reverse(&((Aword *)e)[1]);  /* The verb code is stored after the table */
}    


#ifdef _PROTOTYPES_
static void reverseElms(Aword adr)
#else
static void reverseElms(adr)
     Aword adr;
#endif
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


#ifdef _PROTOTYPES_
static void reverseStxs(Aword adr)
#else
static void reverseStxs(adr)
     Aword adr;
#endif
{
  StxEntry *e = (StxEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(StxEntry));
    while (!endOfTable(e)) {
      reverseElms(e->elms);
      e++;
    }
  }
}    

#ifdef _PROTOTYPES_
static void reverseEvts(Aword adr)
#else
static void reverseEvts(adr)
     Aword adr;
#endif
{
  EvtEntry *e = (EvtEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(EvtEntry));
    while (!endOfTable(e)) {
      reverseStms(e->code);
      e++;
    }
  }
}    



#ifdef _PROTOTYPES_
static void reverseLims(Aword adr)
#else
static void reverseLims(adr)
     Aword adr;
#endif
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


#ifdef _PROTOTYPES_
static void reverseContainers(Aword adr)
#else
§static void reverseContainers(adr)
     Aword adr;
#endif
{
  ContainerEntry *e = (ContainerEntry *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ContainerEntry));
    while (!endOfTable(e)) {
      reverseLims(e->limits);
      reverseStms(e->header);
      reverseStms(e->empty);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseRuls(Aword adr)
#else
static void reverseRuls(adr)
     Aword adr;
#endif
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



/*----------------------------------------------------------------------

  reverseHdr()

  Reverse the header structure.

*/
#ifdef _PROTOTYPES_
void reverseHdr(AcdHdr *hdr)
#else
void reverseHdr(hdr)
     AcdHdr *hdr;
#endif
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
  reverseStxs(header->stxs);
  reverseVerbs(header->vrbs);
  reverseInstances(header->instanceTableAddress);
  reverseTable(header->classTableAddress, sizeof(ClassEntry));
  reverseContainers(header->containerTableAddress);
  reverseEvts(header->evts);
  reverseRuls(header->ruls);
  reverseTable(header->init, sizeof(IniEntry));
  reverseStms(header->start);
  reverseMsgs(header->msgs);
 
  reverseTable(header->scores, sizeof(Aword));
  reverseTable(header->freq, sizeof(Aword));
}
