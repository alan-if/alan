/*----------------------------------------------------------------------*\

  c26to27.c

  Handles conversion of data format from 2.6 to 2.7

\*----------------------------------------------------------------------*/

#include <stdio.h>
#include "types.h"
#include "acode.h"


/* The Amachine memory */
static Aword *memory;
static AcdHdr dummyHeader;	/* Dummy to use until memory allocated */
static AcdHdr *header = &dummyHeader;

static int memTop = 0;		/* Top of load memory */

static FILE *datfil = NULL;	/* The .dat file */



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
    printf("Out of memory!\n");
    exit(1);
  }

  return p;
}




/*======================================================================*\

  Start of conversion routines!

\*======================================================================*/

#ifdef _PROTOTYPES_
static void c26to27Elms(Aword adr)
#else
static void c26to27Elms(adr)
     Aword adr;
#endif
{
  ElmElem *e;

  if (adr == 0)
    return;

  /* In v2.7 the multiple flag changed to flags, which flagged both
     the multiple and the omni property */

  for (e = (ElmElem *) &memory[adr]; !endOfTable(e); e++) {
    if (e->flags) e->flags = MULTIPLEBIT;
    if (e->code != EOS) c26to27Elms(e->next);
  }
}    


#ifdef _PROTOTYPES_
static void c26to27Stxs(Aword adr)
#else
static void c26to27Stxs(adr)
     Aword adr;
#endif
{
  StxElem *e;

  if (adr == 0)
    return;

  for (e = (StxElem *) &memory[adr]; !endOfTable(e); e++){
    c26to27Elms(e->elms);
  }
}    


#ifdef _PROTOTYPES_
static void c26to27Msgs(Aword adr)
#else
static void c26to27Msgs(adr)
     Aword adr;
#endif
{
  MsgElem26 *msgs = (MsgElem26 *)addrTo(adr);
  MsgKind msgidx;
  Aword stmadr = memTop;
  MsgElem *new = (MsgElem *)addrTo(adr);
  MsgKind newidx = M_HUH;
  static char *quitaction_text = "Do you want to RESTART, RESTORE or QUIT ? ";

  if (adr == 0)
    return;

  /* In v2.7 the message table changed from fpos/len to statement
     pointers, so we must create statements for the messages. Also the
     number of messages increased with one, moving M_ARTICLE one step
     up, M_ARTICLE26 now holds the old value of M_ARTICLE. This must
     also be moved */

  for (msgidx = M_HUH; msgidx < M_MSGMAX26; msgidx++) {

    /* Store the statement address in the new table */
    new[newidx].stms = stmadr;
    newidx++;

    /* Construct the PRINT statment, unless M_ARTICLE26s fpos&len == 0
     then it is probably a converted 2.5 game so only generate RETURN
     statment which will give an empty default article message */
    if (msgidx != M_ARTICLE26 || (msgs[msgidx].fpos != 0 &&
				  msgs[msgidx].len != 0)) {
      memory[stmadr++] = msgs[msgidx].len;
      memory[stmadr++] = msgs[msgidx].fpos;
      memory[stmadr++] =
	((Aword)C_STMOP<<28)|((Aword)I_PRINT&0x0fffffff);
    }
    memory[stmadr++] = ((Aword)C_STMOP<<28)|((Aword)I_RETURN&0x0fffffff);
  }
  /* Remember top of memory */
  memTop = stmadr;

  /* Now move the article statements to its correct place... */
  new[M_ARTICLE].stms = new[M_ARTICLE26].stms;

  /* ... create a print statement for the QUITACTION */
  fseek(datfil, 0, SEEK_END);
  new[M_QUITACTION].stms = stmadr;
  memory[stmadr++] = strlen(quitaction_text);
  memory[stmadr++] = ftell(datfil);
  memory[stmadr++] =
	((Aword)C_STMOP<<28)|((Aword)I_PRINT&0x0fffffff);
  memory[stmadr++] = ((Aword)C_STMOP<<28)|((Aword)I_RETURN&0x0fffffff);

  /* ... and append a standard QUITACTION message to the .dat file */
  if (datfil)
    fprintf(datfil, quitaction_text);
}    



/*----------------------------------------------------------------------

  c26to27ACD()

  Between v2.6 and v2.7 the following two changes was made in the format:

  1) The multiple flag in syntax elements (ElmElem) changed to a bit
  pattern which flagged for both MULTIPLE and the new OMNI property

  2) The message table changed from a direct fpos/len to a statement
  pointer

  The first one is easy to fix (done in c26to27Elms). The second
  requires the message table to be compacted and a number of print
  statements generated.

  This program does The message table compaction simply by doing that.
  The 'lost' words are not recovered, so no move is required.

  */
#ifdef _PROTOTYPES_
static void c26to27ACD(void)
#else
static void c26to27ACD()
#endif
{
  /* Remember this is patched already ... */
  header->vers[0] = 2;
  header->vers[1] = 7;
  header->vers[2] = 0;
  header->vers[3] = 'x';

  /* The number of needed extra words are M_MSGMAX * 4 (a print
     statement, 3 words, and a return statement */
  header->size += MSGMAX * 4;

  /* Allocate a larger memory */
  memory = realloc(memory, header->size*sizeof(Aword));
  header = (AcdHdr *) memory;

  c26to27Stxs(header->stxs);

  c26to27Msgs(header->msgs);
}



/*----------------------------------------------------------------------

  copydat()

 */
static void copydat(char olddat[], char newdat[])
{
  FILE *olddatfil = fopen(olddat, "r");
  FILE *newdatfil = fopen(newdat, "w");
  char buf[1000];
  size_t nbytes;

  if (olddatfil == NULL)
    printf("WARNING - Could not open %s, for input to conversion.", olddat);
  else if (newdatfil == NULL)
    printf("WARNING - Could not open %s, for output from conversion.",
	   newdat);
  else {
    /* Copy the original .dat file */
    while ((nbytes = fread(buf, 1, 1000, olddatfil)) != 0)
      fwrite(buf, nbytes, 1, newdatfil);
    fclose(olddatfil);
    fclose(newdatfil);

    if ((datfil = fopen(newdat, "a")) == NULL) {
      printf("ERROR - Could not open copied .dat file!\n");
      exit(1);
    }
  }
}



/*----------------------------------------------------------------------

  checkvers()

 */
#ifdef _PROTOTYPES_
static void checkvers(AcdHdr *header)
#else
static void checkvers(header)
     AcdHdr *header;
#endif
{
  char vers[4] = {2, 6, 0, 'x'};
  char state[2];

  /* Compatible if version and revision match... */
  if (strncmp(header->vers, vers, 2) != 0) {
    state[0] = header->vers[3];
    state[1] = '\0';
    printf("Can not convert Alan programs of version %d.%d(%d)%s, Expected v%d.%d.\n",
	   (int)(header->vers[0]),
	   (int)(header->vers[1]),
	   (int)(header->vers[2]),
	   (header->vers[3])==0? "": state,
	   (int)vers[0], (int)vers[1]);
    exit(1);
  }
}


/*----------------------------------------------------------------------

  load()

 */
static void load(char acdfnm[])
{
  AcdHdr tmphdr;
  FILE *codfil;

  if ((codfil = fopen(acdfnm, "r")) == NULL) {
    printf("Could not open ACD-file '%s'.\n", acdfnm);
    exit(1);
  }

  fread(&tmphdr, sizeof(tmphdr), 1, codfil);
  rewind(codfil);
  checkvers(&tmphdr);

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
    printf("Could not create the new .acd file '%s' for output.\n", outfnm);
    exit(1);
  }

  fwrite(memory, header->size, sizeof(Aword), newacd);
}




/*======================================================================*\

  c26to27

  Main entry point to program to convert v2.6 .ACD to v2.7

  NOTE! Does not work on little-endians (noteably PCs) yet!!!!!

\*======================================================================*/


/* SPA Option handling */

#include "spa.h"


static char *oldnam;
static char *oldacd, *olddat;
static char *newnam;
static char *newacd, *newdat;


static SPA_FUN(usage)
{
  printf("Usage: c26to27 [-help] [options] <v2.6name> <v2.7name>\n");
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
     SPA_STRING("v2.6name", "game name of the original v2.6 game to convert", oldnam, NULL, NULL)
     SPA_STRING("v2.7name", "game name of the converted files in v2.7 format", newnam, NULL, NULL)
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

  oldacd = (char *)malloc(strlen(oldnam)+5);
  strcpy(oldacd, oldnam); strcat(oldacd, ".acd");

  olddat = (char *)malloc(strlen(oldnam)+5);
  strcpy(olddat, oldnam); strcat(olddat, ".dat");

  newacd = (char *)malloc(strlen(newnam)+5);
  strcpy(newacd, newnam); strcat(newacd, ".acd");

  newdat = (char *)malloc(strlen(newnam)+5);
  strcpy(newdat, newnam); strcat(newdat, ".dat");

  load(oldacd);
  copydat(olddat, newdat);
  c26to27ACD();
  newcrc();
  store(newacd);
  exit(0);
}
  
