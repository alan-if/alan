/*----------------------------------------------------------------------*\

  reverse.c

  Handles all reversing of data

\*----------------------------------------------------------------------*/


#include "types.h"
#include "arun.h"

#include "reverse.h"


/*----------------------------------------------------------------------

  reversed()

  Return the reversed bytes in the Aword

*/
#ifdef _PROTOTYPES_
Aword reversed(Aword w)	/* IN - The ACODE word to swap bytes of */
#else
Aword reversed(w)
     Aword w;			/* IN - The ACODE word to swap bytes of */
#endif
{
  Aword s;			/* The swapped ACODE word */
  char *wp, *sp;
  int i;
  
  wp = (char *) &w;
  sp = (char *) &s;

  for (i = 0; i < sizeof(Aword); i++)
    sp[sizeof(Aword)-1 - i] = wp[i];

  return s;
}


#ifdef _PROTOTYPES_
void reverse(Aword *w)		/* IN - The ACODE word to reverse bytes in */
#else
void reverse(w)
     Aword *w;			/* IN - The ACODE word to reverse bytes in */
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
static void reverseWrds(Aword adr)
#else
static void reverseWrds(adr)
     Aword adr;
#endif
{
  WrdElem *e = (WrdElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(WrdElem));
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
  ChkElem *e = (ChkElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ChkElem));
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
  AltElem *e = (AltElem *)&memory[adr];

  if (adr != 0 && !endOfTable(e) && !e->rev) {
    reverseTable(adr, sizeof(AltElem));
    e->rev = TRUE;
    while (!endOfTable(e)) {
      reverseChks(e->checks);
      reverseStms(e->action);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseVrbs(Aword adr)
#else
static void reverseVrbs(adr)
     Aword adr;
#endif
{
  VrbElem *e = (VrbElem *)&memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(VrbElem));
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
  StepElem *e = (StepElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(StepElem));
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
  ScrElem *e = (ScrElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ScrElem));
    while (!endOfTable(e)) {
      reverseStms(e->dscr);
      reverseSteps(e->steps);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseActs(Aword adr)
#else
static void reverseActs(adr)
     Aword adr;
#endif
{
  ActElem *e = (ActElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ActElem));
    while (!endOfTable(e)) {
      reverseStms(e->nam);
      reverseTable(e->atrs, sizeof(AtrElem));
      reverseScrs(e->scradr);
      reverseVrbs(e->vrbs);
      reverseStms(e->dscr);
      e++;
    }
  }
}    

#ifdef _PROTOTYPES_
static void reverseObjs(Aword adr)
#else
static void reverseObjs(adr)
     Aword adr;
#endif
{
  ObjElem *e = (ObjElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ObjElem));
    while (!endOfTable(e)) {
      reverseTable(e->atrs, sizeof(AtrElem));
      reverseVrbs(e->vrbs);
      reverseStms(e->dscr1);
      reverseStms(e->dscr2);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseExts(Aword adr)
#else
static void reverseExts(adr)
     Aword adr;
#endif
{
  ExtElem *e = (ExtElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ExtElem));
    while (!endOfTable(e)) {
      if (!e->rev) {
	reverseChks(e->checks);
	reverseStms(e->action);
      }
      e++;
    }
  }
}    

#ifdef _PROTOTYPES_
static void reverseLocs(Aword adr)
#else
static void reverseLocs(adr)
     Aword adr;
#endif
{
  LocElem *e = (LocElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(LocElem));
    while (!endOfTable(e)) {
      reverseStms(e->nams);
      reverseStms(e->dscr);
      reverseStms(e->does);
      reverseTable(e->atrs, sizeof(AtrElem));
      reverseExts(e->exts);
      reverseVrbs(e->vrbs);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseClas(Aword adr)
#else
static void reverseClas(adr)
     Aword adr;
#endif
{
  ClaElem *e = (ClaElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ClaElem));
    while (!endOfTable(e)) {
      reverseStms(e->stms);
      e++;
    }
  }
  if (adr)
    reverse(&((Aword *)e)[1]);	/* The verb code is stored after the table */
}    


#ifdef _PROTOTYPES_
static void reverseElms(Aword adr)
#else
static void reverseElms(adr)
     Aword adr;
#endif
{
  ElmElem *e = (ElmElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(ElmElem));
    while (!endOfTable(e)) {
      if (e->code == EOS) reverseClas(e->next);
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
  StxElem *e = (StxElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(StxElem));
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
  EvtElem *e = (EvtElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(EvtElem));
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
  LimElem *e = (LimElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(LimElem));
    while (!endOfTable(e)) {
      reverseStms(e->stms);
      e++;
    }
  }
}    


#ifdef _PROTOTYPES_
static void reverseCnts(Aword adr)
#else
static void reverseCnts(adr)
     Aword adr;
#endif
{
  CntElem *e = (CntElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(CntElem));
    while (!endOfTable(e)) {
      reverseLims(e->lims);
      reverseStms(e->header);
      reverseStms(e->empty);
      reverseStms(e->nam);
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
  RulElem *e = (RulElem *) &memory[adr];

  if (adr != 0 && !endOfTable(e)) {
    reverseTable(adr, sizeof(RulElem));
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

  for (i = 0; i < sizeof(AcdHdr)/sizeof(Aword); i++)
    reverse(&((Aword *)hdr)[i]);
}

/*----------------------------------------------------------------------

  reverseACD()

  Traverse all the data structures and reverse all integers.
  Only performed in architectures with reversed byte ordering, which
  makes the .ACD files fully compatible across architectures

  */
#ifdef _PROTOTYPES_
void reverseACD(void)
#else
void reverseACD()
#endif
{
  reverseHdr(header);
  reverseWrds(header->dict);
  reverseTable(header->oatrs, sizeof(AtrElem));
  reverseTable(header->latrs, sizeof(AtrElem));
  reverseTable(header->aatrs, sizeof(AtrElem));
  reverseActs(header->acts);
  reverseObjs(header->objs);
  reverseLocs(header->locs);
  reverseStxs(header->stxs);
  reverseVrbs(header->vrbs);
  reverseEvts(header->evts);
  reverseCnts(header->cnts);
  reverseRuls(header->ruls);
  reverseTable(header->init, sizeof(IniElem));
  reverseStms(header->start);
  reverseTable(header->msgs, sizeof(MsgElem));
 
  reverseTable(header->scores, sizeof(Aword));
  reverseTable(header->freq, sizeof(Aword));
}  
