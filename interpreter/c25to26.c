/*----------------------------------------------------------------------*\

  c25to26.c

  Handles conversion of data format from 2.5 to 2.6

\*----------------------------------------------------------------------*/

#include <stdio.h>
#include "types.h"
#include "acode.h"


/* The Amachine memory */
static Aword *memory;
static AcdHdr dummyHeader;	/* Dummy to use until memory allocated */
static AcdHdr *header = &dummyHeader;

static int memTop = 0;			/* Top of load memory */


/* The table of address where to insert new words */
static Aword *c25to26tbl;
static int nPatches;		/* Number of patches */



/*======================================================================*\

  Some utilities copied from arun

\*======================================================================*/

#define endOfTable(x) eot((Aword *) x)

/* How to know we are at end of a table */
#ifdef _PROTOTYPES_
Boolean eot(Aword *adr)
#else
Boolean eot(adr)
     Aword *adr;
#endif
{
  return *adr == EOF;
}


#ifdef _PROTOTYPES_
void *allocate(unsigned long len)		/* IN - Length to allocate */
#else
void *allocate(len)
     unsigned long len;			/* IN - Length to allocate */
#endif
{
  void *p = (void *)malloc((size_t)len);

  if (p == NULL) {
    printf("Out of memory!");
    exit(1);
  }

  return p;
}





/*======================================================================*\

  Start of conversion routines!

\*======================================================================*/

#ifdef _PROTOTYPES_
void c25to26(Aaddr *addrp)		/* IN - Pointer to the Aaddr to convert */
#else
void c25to26(addrp)
     Aaddr *addrp;			/* IN - Pointer to the Aaddr to convert */
#endif
{
  int i;

  for (i = 0; i < nPatches; i++)
    if (*addrp < c25to26tbl[i])
      return;
    else
      (*addrp)++;
}


#ifdef _PROTOTYPES_
static void c25to26Stms(Aword adr)
#else
static void c25to26Stms(adr)
     Aword adr;
#endif
{
  /* Nothing to do for statements */
}


/*----------------------------------------------------------------------

  c25to26Table

  Convert a complete table of adresses

  */
#ifdef _PROTOTYPES_
static void c25to26Table(Aword adr)
#else
static void c25to26Table(adr)
     Aword adr;
#endif
{
  Aword *e;
  
  if (adr == 0)
    return;

  for (e = &memory[adr]; !endOfTable(e); e++)
    c25to26(e);
}


#ifdef _PROTOTYPES_
static void c25to26Wrds(Aword adr)
#else
static void c25to26Wrds(adr)
     Aword adr;
#endif
{
  WrdElem *e = (WrdElem *) &memory[adr];

  if (adr == 0)
    return;

  for (e = (WrdElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->wrd));
    c25to26(&(e->adjrefs));
    c25to26(&(e->nounrefs));
    if ((e->class & (1L<<WRD_SYN)) == 0) { /* Do not do this for synonyms */
      c25to26Table(e->adjrefs);
      c25to26Table(e->nounrefs);
    }
  }
}    

#ifdef _PROTOTYPES_
static void c25to26Chks(Aword adr)
#else
static void c25to26Chks(adr)
     Aword adr;
#endif
{
  ChkElem *e;

  if (adr == 0)
    return;

  for (e = (ChkElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->exp));
    c25to26Stms(e->exp);
    c25to26(&(e->stms));
    c25to26Stms(e->stms);
  }
}    

#ifdef _PROTOTYPES_
static void c25to26Alts(Aword adr)
#else
static void c25to26Alts(adr)
     Aword adr;
#endif
{
  AltElem *e = (AltElem *)&memory[adr];

  if (adr == 0 || e->done)
    return;

  e->done = TRUE;
  for (; !endOfTable(e); e++) {
    c25to26(&(e->checks));
    c25to26Chks(e->checks);
    c25to26(&(e->action));
    c25to26Stms(e->action);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Vrbs(Aword adr)
#else
static void c25to26Vrbs(adr)
     Aword adr;
#endif
{
  VrbElem *e;

  if (adr == 0)
    return;

  for (e = (VrbElem *)&memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->alts));
    c25to26Alts(e->alts);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Steps(Aword adr)
#else
static void c25to26Steps(adr)
     Aword adr;
#endif
{
  StepElem *e;

  if (adr == 0)
    return;

  for (e = (StepElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->exp));
    c25to26Stms(e->exp);
    c25to26(&(e->stm));
    c25to26Stms(e->stm);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Scrs(Aword adr)
#else
static void c25to26Scrs(adr)
     Aword adr;
#endif
{
  ScrElem *e;

  if (adr == 0)
    return;

  for (e = (ScrElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->dscr));
    c25to26Stms(e->dscr);
    c25to26(&(e->steps));
    c25to26Steps(e->steps);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Acts(Aword adr)
#else
static void c25to26Acts(adr)
     Aword adr;
#endif
{
  ActElem *e;

  if (adr == 0)
    return;

  for (e = (ActElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->nam));
    c25to26Stms(e->nam);
    c25to26(&(e->atrs));
    c25to26Table(e->atrs);
    c25to26(&(e->scradr));
    c25to26Scrs(e->scradr);
    c25to26(&(e->vrbs));
    c25to26Vrbs(e->vrbs);
    c25to26(&(e->dscr));
    c25to26Stms(e->dscr);
  }
}    

#ifdef _PROTOTYPES_
static void c25to26Objs(Aword adr)
#else
static void c25to26Objs(adr)
     Aword adr;
#endif
{
  ObjElem *e;

  if (adr == 0)
    return;

  for (e = (ObjElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->atrs));
    c25to26Table(e->atrs);
    c25to26(&(e->vrbs));
    c25to26Vrbs(e->vrbs);
    c25to26(&(e->dscr1));
    c25to26Stms(e->dscr1);
    c25to26(&(e->dscr2));
    c25to26Stms(e->dscr2);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Exts(Aword adr)
#else
static void c25to26Exts(adr)
     Aword adr;
#endif
{
  ExtElem *e;

  if (adr == 0)
    return;

  for (e = (ExtElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->checks));
    c25to26(&(e->action));
    if (!e->done) {
      c25to26Chks(e->checks);
      c25to26Stms(e->action);
    }
  }
}

#ifdef _PROTOTYPES_
static void c25to26Locs(Aword adr)
#else
static void c25to26Locs(adr)
     Aword adr;
#endif
{
  LocElem *e;

  if (adr == 0)
    return;

  for (e = (LocElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->nams));
    c25to26Stms(e->nams);
    c25to26(&(e->dscr));
    c25to26Stms(e->dscr);
    c25to26(&(e->does));
    c25to26Stms(e->does);
    c25to26(&(e->atrs));
    c25to26Table(e->atrs);
    c25to26(&(e->exts));
    c25to26Exts(e->exts);
    c25to26(&(e->vrbs));
    c25to26Vrbs(e->vrbs);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Clas(Aword adr)
#else
static void c25to26Clas(adr)
     Aword adr;
#endif
{
  ClaElem *e;

  if (adr == 0)
    return;

  for (e = (ClaElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->stms));
    c25to26Stms(e->stms);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Elms(Aword adr)
#else
static void c25to26Elms(adr)
     Aword adr;
#endif
{
  ElmElem *e;

  if (adr == 0)
    return;

  for (e = (ElmElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->next));
    if (e->code == EOS) c25to26Clas(e->next);
    else c25to26Elms(e->next);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Stxs(Aword adr)
#else
static void c25to26Stxs(adr)
     Aword adr;
#endif
{
  StxElem *e;

  if (adr == 0)
    return;

  for (e = (StxElem *) &memory[adr]; !endOfTable(e); e++){
    c25to26(&(e->elms));
    c25to26Elms(e->elms);
  }
}    

#ifdef _PROTOTYPES_
static void c25to26Evts(Aword adr)
#else
static void c25to26Evts(adr)
     Aword adr;
#endif
{
  EvtElem *e;

  if (adr == 0)
    return;

  for (e = (EvtElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->code));
    c25to26Stms(e->code);
  }
}    



#ifdef _PROTOTYPES_
static void c25to26Lims(Aword adr)
#else
static void c25to26Lims(adr)
     Aword adr;
#endif
{
  LimElem *e;

  if (adr == 0)
    return;

    for (e = (LimElem *) &memory[adr]; !endOfTable(e); e++) {
      c25to26(&(e->stms));
      c25to26Stms(e->stms);
    }
}    


#ifdef _PROTOTYPES_
static void c25to26Cnts(Aword adr)
#else
static void c25to26Cnts(adr)
     Aword adr;
#endif
{
  CntElem *e;

  if (adr == 0)
    return;

  for (e = (CntElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->lims));
    c25to26Lims(e->lims);
    c25to26(&(e->header));
    c25to26Stms(e->header);
    c25to26(&(e->empty));
    c25to26Stms(e->empty);
    c25to26(&(e->nam));
    c25to26Stms(e->nam);
  }
}    


#ifdef _PROTOTYPES_
static void c25to26Ruls(Aword adr)
#else
static void c25to26Ruls(adr)
     Aword adr;
#endif
{
  RulElem *e;

  if (adr == 0)
    return;

  for (e = (RulElem *) &memory[adr]; !endOfTable(e); e++) {
    c25to26(&(e->exp));
    c25to26Stms(e->exp);
    c25to26(&(e->stms));
    c25to26Stms(e->stms);
  }
}    



#ifdef _PROTOTYPES_
static void c25to26Msgs(Aword adr)
#else
static void c25to26Msgs(adr)
     Aword adr;
#endif
{
  MsgElem26 *msgs = (MsgElem26 *)addrTo(adr);

  if (adr == 0)
    return;

  /* To know that there is no article we need to reset the msgs[M_ARTICLE] */
  msgs[M_ARTICLE26].fpos = 0;
  msgs[M_ARTICLE26].len = 0;
}    



/*----------------------------------------------------------------------

  c25to26Hdr()

  Convert the header structure.

*/
#ifdef _PROTOTYPES_
void c25to26Hdr(AcdHdr *hdr)
#else
void c25to26Hdr(hdr)
     AcdHdr *hdr;
#endif
{
  /* Convert from hdr.dict to hdr.msgs */
  c25to26(&(hdr->dict));
  c25to26(&(hdr->oatrs));
  c25to26(&(hdr->latrs));
  c25to26(&(hdr->aatrs));
  c25to26(&(hdr->acts));
  c25to26(&(hdr->objs));
  c25to26(&(hdr->locs));
  c25to26(&(hdr->stxs));
  c25to26(&(hdr->vrbs));
  c25to26(&(hdr->evts));
  c25to26(&(hdr->cnts));
  c25to26(&(hdr->ruls));
  c25to26(&(hdr->init));
  c25to26(&(hdr->start));
  c25to26(&(hdr->msgs));
  c25to26(&(hdr->scores));
  c25to26(&(hdr->freq));
}


/*----------------------------------------------------------------------

  build25to26Table()

  Run through the ObjElems and create a table which contains the
  address of each position in which the extra word would go.

*/
void build25to26Table(void)
{
  ObjElem25 *obj25 = (ObjElem25 *) addrTo(header->objs);
  ObjElem *obj = (ObjElem *) addrTo(header->objs);
  int offset = ((Aword)&obj->art-(Aword)obj)/sizeof(Aword);

  long high, low, i, p;		/* Memory move variables */

  /* Allocate room for patch table */
  c25to26tbl = allocate(sizeof(Aword)*nPatches);

  /* Fill table with addresses to insert new words */
  for (p = 0; !endOfTable(obj25); obj25++, p++)
    c25to26tbl[p] = header->objs+p*sizeof(ObjElem25)/sizeof(Aword)+offset;
  /* Now add two words for the ARTICLE message (fpos & len) */
  c25to26tbl[p++] = header->msgs+2*(M_MSGMAX26-1);
  c25to26tbl[p++] = header->msgs+2*(M_MSGMAX26-1);

  /* Now we can also move the data so to make room for the new fields */
  high = memTop - 1;
  memTop += nPatches;
  for (p = nPatches; p > 0; p--) {
    /* Move data from the top up 'p' number of words */
    low = c25to26tbl[p-1];
    for (i = high; i >= low; i--) {
      memory[i+p] = memory[i];
}
    memory[i+p] = 0;
    high = low-1;
  }
}
    

/*----------------------------------------------------------------------

  cleardone()

  The done fields are used when reversing and converting the ACD.
  To ensure that some structures are not operated on twice a marker
  is used. This routine clears those markers.

  */

#ifdef _PROTOTYPES_
static void cleardone(AcdHdr *header)
#else
static void cleardone(header)
     AcdHdr *header;
#endif
{
  LocElem *loc;
  ExtElem *ext;
  ObjElem *obj;
  ObjElem25 *obj25;
  ActElem *act;
  VrbElem *vrb;
  AltElem *alt;

  /* The exits are connected to locations... */
  for (loc = (LocElem *)addrTo(header->locs); !endOfTable(loc); loc++) {
    if (loc->exts != 0) {
      ext = (ExtElem *)addrTo(loc->exts);
      ext->done = FALSE;
    }
    /* And it can have verbs in which the Alts are... */
    if (loc->vrbs != 0)
      for (vrb = (VrbElem *)addrTo(loc->vrbs); !endOfTable(vrb); vrb++)
	if (vrb->alts != 0) {
	  /* Only the first one is ever used... */
	  alt = (AltElem *)addrTo(vrb->alts);
	  alt->done = FALSE;
	}
  }

  /* Alts in Verbs in Objects... */
  if (header->vers[0] == 2 && header->vers[1] == 5) {
    /* Then its a v2.5 game use old style ObjElems */
    for (obj25 = (ObjElem25 *)addrTo(header->objs); !endOfTable(obj25); obj25++)
      if (obj25->vrbs != 0)
	for (vrb = (VrbElem *)addrTo(obj25->vrbs); !endOfTable(vrb); vrb++)
	  if (vrb->alts != 0) {
	    /* Only the first one is ever used... */
	    alt = (AltElem *)addrTo(vrb->alts);
	    alt->done = FALSE;
	  }
  } else {
    for (obj = (ObjElem *)addrTo(header->objs); !endOfTable(obj); obj++)
      if (obj->vrbs != 0)
	for (vrb = (VrbElem *)addrTo(obj->vrbs); !endOfTable(vrb); vrb++)
	  if (vrb->alts != 0) {
	    /* Only the first one is ever used... */
	    alt = (AltElem *)addrTo(vrb->alts);
	    alt->done = FALSE;
	  }
  }

  /* Alts in Verbs in Actors... */
  for (act = (ActElem *)addrTo(header->acts); !endOfTable(act); act++)
    if (act->vrbs != 0)
      for (vrb = (VrbElem *)addrTo(act->vrbs); !endOfTable(vrb); vrb++)
	if (vrb->alts != 0) {
	  /* Only the first one is ever used... */
	  alt = (AltElem *)addrTo(vrb->alts);
	  alt->done = FALSE;
	}

  /* And Alts in global verbs... */
  for (vrb = (VrbElem *)addrTo(header->vrbs); !endOfTable(vrb); vrb++)
    if (vrb->alts != 0)
      if (vrb->alts != 0) {
	/* Only the first one is ever used... */
	alt = (AltElem *)addrTo(vrb->alts);
	alt->done = FALSE;
      }
}


/*----------------------------------------------------------------------

  c25to26ACD()

  The only difference in the ACD-format between 2.5 and 2.6 is the
  introduction of the ARTICLE, which translates to a new field in the
  ObjElem, and an extra message entry (with two words, fpos and
  len). So we need to traverse the object list and find the addresses
  at which a new word has been inserted. Then we can traverse all the
  data structures and increment the addresses so that they point to
  the new addresses.  Finally a move of the data can be performed.

  */
#ifdef _PROTOTYPES_
static void c25to26ACD(void)
#else
static void c25to26ACD()
#endif
{
  /* This must be performed before we patch it */
  cleardone(header);

  /* Remember this is patched already ... */
  header->vers[0] = 2;
  header->vers[1] = 6;
  header->vers[2] = 0;
  header->vers[3] = 'x';

  /* Number of places to insert new words */
  nPatches = OBJMAX-OBJMIN+1+2;	/* The message table (fpos&len)! */
  header->size += nPatches;

  /* Allocate a larger memory */
  memory = realloc(memory, header->size*sizeof(Aword));
  header = (AcdHdr *) memory;

  build25to26Table();

  c25to26Hdr(header);		/* OK! */
  c25to26Wrds(header->dict);
  c25to26Table(header->oatrs);
  c25to26Table(header->latrs);
  c25to26Table(header->aatrs);
  c25to26Acts(header->acts);
  c25to26Objs(header->objs);
  c25to26Locs(header->locs);
  c25to26Stxs(header->stxs);
  c25to26Vrbs(header->vrbs);
  c25to26Evts(header->evts);
  c25to26Cnts(header->cnts);
  c25to26Ruls(header->ruls);
  c25to26Table(header->init);
  c25to26Stms(header->start);
  c25to26Msgs(header->msgs);

  c25to26Table(header->scores);
}



/*----------------------------------------------------------------------

  load()

 */
static void load(char acdfnm[])
{
  AcdHdr tmphdr;
  FILE *codfil;

  if ((codfil = fopen(acdfnm, "r")) == NULL) {
    printf("Could not open ACD-file '%s'\n", acdfnm);
    exit(1);
  }

  fread(&tmphdr, sizeof(tmphdr), 1, codfil);
  rewind(codfil);

  /* Allocate and load memory */

#ifdef REVERSED
  reverseHdr(&tmphdr);
#endif

  memory = allocate(tmphdr.size*sizeof(Aword));
  header = (AcdHdr *) memory;

  memTop = fread(addrTo(0), sizeof(Aword), tmphdr.size, codfil);
  if (memTop != tmphdr.size)
    printf("WARNING! Could not read all ACD code.");

#ifdef REVERSED
  printf("Hmm, this is a little-endian machine, please wait a moment while I fix byte ordering....\n");
  reverseACD();		/* Reverse all words in the ACD file */
  printf("OK.\n");
#endif
}




/*----------------------------------------------------------------------

  newcrc()

  Calculate a new CRC for the converted file

  */
static void newcrc(void)
{
  Aword *w;
  Aword crc = 0;

  for (w = &memory[sizeof(AcdHdr)/sizeof(Aword)]; w < &memory[header->size]; w++) {
    crc += *w&0xff;			/* Check sum calculation */
    crc += (*w>>8)&0xff;
    crc += (*w>>16)&0xff;
    crc += (*w>>24)&0xff;
  }
  header->acdcrc = crc;  
}


/*----------------------------------------------------------------------

  store()

  Store the converted ACD file on disk

  */
static void store(char outfnm[])
{
  FILE *newacd = fopen(outfnm, "w");

  if (newacd == NULL) {
    printf("Could not create the new file.");
    exit(1);
  }

  fwrite(memory, header->size, sizeof(Aword), newacd);
}




/*======================================================================*\

  c25to26

  Main entry point to program to convert v2.5 .ACD to v2.6

\*======================================================================*/


/* SPA Option handling */

#include "spa.h"


static char *acdfnm;
static char *outfnm;


static SPA_FUN(usage)
{
  printf("Usage: c25to26 [-help] [options] <v2.5file> <v2.6file>\n");
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
     SPA_STRING("v2.5file", "name of the original v2.5 ACD-file to convert", acdfnm, NULL, NULL)
     SPA_STRING("v2.6file", "name of the converted ACD-file in v2.6 format", outfnm, NULL, NULL)
     SPA_FUNCTION("", "extra argument", extraArg)
SPA_END

static SPA_DECLARE(options)
     SPA_HELP("help", "this help", usage, xit)
SPA_END


int main(int argc, char **argv) 
{
  int nArgs;

  nArgs = spaProcess(argc, argv, arguments, options, paramError);
  if (nArgs != 2) {
    usage(NULL, NULL, 0);
    exit(EXIT_FAILURE);
  }

  load(acdfnm);
  c25to26ACD();
  newcrc();
  store(outfnm);
  exit(0);
}
  
