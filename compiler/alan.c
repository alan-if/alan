/*----------------------------------------------------------------------*\

  alan.c

  Alan compiler main program unit

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"

#ifdef __sun__
#include <sys/unistd.h>
#endif

#include "alan.h"


/* IMPORTS */

#include "version.h"

#include "srcp.h"

#include "adv.h"
#include "opt.h"

#include "dump.h"

#include "lmList.h"
#include "pmParse.h"
#include "smScan.h"

#ifdef _PROTOTYPES_
extern Boolean smScanEnter(char fnm[]);
extern int scannedLines(void);	/* Number of scanned lines */
#else
extern Boolean smScanEnter();
extern int scannedLines();	/* Number of scanned lines */
#endif


/* PUBLIC DATA */

Srcp nulsrcp			/* NULL position for list */
  = {0,0,0};

FILE *txtfil;			/* File of collected text data */
FILE *datfil;			/* File of encoded text */

int fileNo = 0;			/* File number to use next */
Boolean verbose;		/* Verbose mode */


/* PRIVATE */

static void *heap;		/* Address to first free heap area - before */



/* Timing */
#include "time.h"

static TIBUF tbuf;
static TIBUF tot;
static TIBUF comp;

static struct {
  long int pars;
  long int sem;
  long int gen;
  long int comp;
  long int tot;
} tim;



/*----------------------------------------------------------------------

  starttot()

  Start total timing.

 */
#ifdef _PROTOTYPES_
static void starttot(void)
#else
static void starttot()
#endif
{
  tistart(&tot);
}


/*----------------------------------------------------------------------

  startcomp()

  Start compilation timing.

 */
#ifdef _PROTOTYPES_
static void startcomp(void)
#else
static void startcomp()
#endif
{
  tistart(&comp);
}


/*----------------------------------------------------------------------

  start()

  Start timing.

 */
#ifdef _PROTOTYPES_
static void start(void)
#else
static void start()
#endif
{
  tistart(&tbuf);
}


/*----------------------------------------------------------------------

  endpars()

  Note the time of parsing.

 */
#ifdef _PROTOTYPES_
static void endpars(void)
#else
static void endpars()
#endif
{
  tistop(&tbuf);
#ifdef MULTI
  tim.pars = tbuf.pu_elapsed;
#else
  tim.pars = tbuf.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------

  endsem()

  Note the time of semantic analysis.

 */
#ifdef _PROTOTYPES_
static void endsem(void)
#else
static void endsem()
#endif
{
  tistop(&tbuf);
#ifdef MULTI
  tim.sem = tbuf.pu_elapsed;
#else
  tim.sem = tbuf.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------

  endgen()

  Note the time of back-end acode generation.

 */
#ifdef _PROTOTYPES_
static void endgen(void)
#else
static void endgen()
#endif
{
  tistop(&tbuf);
#ifdef MULTI
  tim.gen = tbuf.pu_elapsed;
#else
  tim.gen = tbuf.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------

  endcomp()

  Note compilation time.

 */
#ifdef _PROTOTYPES_
static void endcomp(void)
#else
static void endcomp()
#endif
{
  tistop(&comp);
#ifdef MULTI
  tim.comp = comp.pu_elapsed;
#else
  tim.comp = comp.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------

  endtotal()

  Note total time.

 */
#ifdef _PROTOTYPES_
static void endtotal(void)
#else
static void endtotal()
#endif
{
  tistop(&tot);
#ifdef MULTI
  tim.tot = tot.pu_elapsed;
#else
  tim.tot = tot.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------

  prtimes()

  Print elapsed times.

 */
#ifdef _PROTOTYPES_
static void prtimes(void)
#else
static void prtimes()
#endif
{
  char str[80];
  
  lmSkipLines(8);
  lmPrint("");
  lmPrint(       "        Timing");
  lmPrint(       "        ------");
  sprintf(str,   "        Parse Time:             %6d", tim.pars);
  lmPrint(str);
  if (tim.sem != 0) {
    sprintf(str, "        Analysis Time:          %6d", tim.sem);
    lmPrint(str);
  }
  if (tim.gen != 0) {
    sprintf(str, "        Code Generation Time:   %6d", tim.gen);
    lmPrint(str);
  }
  sprintf(str,   "        ------------------------------");
  lmPrint(str);
  sprintf(str,   "        Compilation Time:       %6d", tim.comp);
  lmPrint(str);
  lmPrint("");
  sprintf(str,   "        Total Time:             %6d", tim.tot);
  lmPrint(str);
}



/*----------------------------------------------------------------------

  stats()

  Print some statistics.

 */
#ifdef _PROTOTYPES_
static void stats(void)
#else
static void stats()
#endif
{
  int lins;
  char str[80];
  
  lmSkipLines(7);
  lmPrint("");
  
  lins = scannedLines();
  if (fileNo > 1)
    sprintf(str, "        %d source lines read from %d files.", lins, fileNo);
  else
    sprintf(str, "        %d source lines read.", lins);
  lmPrint(str);
  lmPrint("");
  
  if (tim.tot != 0 && tim.comp != 0) {
    sprintf(str, "        %d lines/CPUminute.",
	    (int)(60L*1000L*(long)lins/tim.comp));
    lmPrint(str);
    lmPrint("");
  }
  
  sprintf(str,   "        Estimated dynamic memory usage = %d bytes.",
	  (char *)malloc(10000)-(char *)heap);
  lmPrint(str);
  lmPrint("");
}



/************************************************************************\

			PUBLIC routines

\************************************************************************/


/*======================================================================

  unimpl()

  An unimplemented constrution was encountered.

 */
#ifdef _PROTOTYPES_
void unimpl(Srcp *srcp, char *phase)
                		/* IN  - Where? */
                 		/* IN  - What phase? */
#else
void unimpl(srcp, phase)
     Srcp *srcp;		/* IN  - Where? */
     char *phase;		/* IN  - What phase? */
#endif
{
  lmLog(srcp, 998, sevWAR, phase);
}


/*======================================================================

  syserr()

  Some kind of internal system error was detected. Log it.

 */
#ifdef _PROTOTYPES_
void syserr(char *str)
#else
void syserr(str)
     char str[];
#endif
{
  lmLog(&nulsrcp, 997, sevSYS, str);
}


/*======================================================================

  allocate()

  Safely allocate new memory.

*/
#ifdef _PROTOTYPES_
void *allocate(int len)
             			/* IN - Length to allocate */
#else
void *allocate(len)
     int len;			/* IN - Length to allocate */
#endif
{
  void *p = malloc(len);

  if (p == NULL)
    syserr("Out of memory");

  return p;
}




/* -- local variables for main() -- */
  
static char *ext;		/* pointer to extension */
static char *name;		/* pointer to name of file (excl. directory) */
static char srcfnm[255];	/* File name of source file */
static char txtfnm[255];	/*   - " -   of collected text file */
static char datfnm[255];	/*   - " -   of encoded data file */
static char acdfnm[255];	/*   - " -   of ACODE file */
static char lstfnm[255];	/*   - " -   of listing file */


/* SPA Option handling */

#include "spa.h"


static char *srcptr;		/* Pointer to adventure name */
static Boolean warnings;	/* Show warnings */
static Boolean infos;		/* Show informational messages */
static Boolean fulflg;		/* Full listing on the screen */
static Boolean lstflg;		/* Create listing file */
static int lcount;		/* Number of lines per page */
static int ccount;		/* -"-    columns */
static DmpKind dmpflg = 0;	/* Dump internal form flags */
static char *dmpchars = "!sxvlocear";
static Boolean dbgflg = 0;	/* Debug option flags */
static Boolean packflg = 0;	/* Pack option flags */
static Boolean sumflg;		/* Print a summary */
static Boolean revflg;		/* Reverse byte ordering in .ACD file,
				   relative to native ordering */

SPA_FUNCTION(usage)
{
  printf("Usage: ALAN <adventure> [-help] [options]\n");
}


SPA_FUNCTION(paramError)
{
  usage(NULL, NULL, 0, 0);
  exit(EXIT_FAILURE);
}

SPA_FUNCTION(xit) {exit(EXIT_SUCCESS);}

SPA_DECLARE(arguments)
#ifdef __dos__
     SPA_ITEM("adventure", "file name, default extension '.ala'", SPA_Argument, &srcptr, NULL)
#else
     SPA_ITEM("adventure", "file name, default extension '.alan'", SPA_Argument, &srcptr, NULL)
#endif
     SPA_ITEM("", "", SPA_Function, paramError, NULL)
SPA_END

SPA_DECLARE(options)
     SPA_ITEM("help", "this help", SPA_Help, usage, xit)
     SPA_ITEM("verbose", "verbose messages", SPA_Flag, &verbose, FALSE)
     SPA_ITEM("warnings", "[don't] show warning messages", SPA_Flag, &warnings, TRUE)
     SPA_ITEM("infos", "[don't] show informational messages", SPA_Flag, &infos, FALSE)
     SPA_ITEM("full", "full listing on the screen", SPA_Flag, &fulflg, FALSE)
     SPA_ITEM("height <lines)", "height of pages in listing", SPA_Numeric, &lcount, 74)
     SPA_ITEM("width <characters>", "width of pages in listing", SPA_Numeric, &ccount, 80)
     SPA_ITEM("listing", "create listing file", SPA_Flag, &lstflg, FALSE)
     SPA_ITEM("dump", "dump internal form, where\n\
everything\n\
symbols\n\
syntax\n\
verbs\n\
locations\n\
objects\n\
containers\n\
events\n\
actors\n\
rules", SPA_Set, &dmpflg, &dmpchars)
     SPA_ITEM("debug", "force debug option in adventure", SPA_Flag, &dbgflg, FALSE)
     SPA_ITEM("pack", "force pack option in adventure", SPA_Flag, &packflg, FALSE)
     SPA_ITEM("summary", "print a summary", SPA_Flag, &sumflg, FALSE)
/*     SPA_ITEM("prettyprint", "pretty print the adventure", SPA_Flag, &ppflg, FALSE) */
     SPA_ITEM("reverse", "reversed (relative to native) byte ordering in .ACD file", SPA_Flag, &revflg, FALSE)
SPA_END


/*----------------------------------------------------------------------

  prepareNames()

  Prepare all file names needed.

 */
#ifdef _PROTOTYPES_
static void prepareNames(void)
#else
static void prepareNames()
#endif
{
  int i;			/* Index to extension in file name */

  strcpy(srcfnm, srcptr);

  /* -- first strip directory -- */
  if((name = strrchr(srcfnm, ']')) == NULL
     && (name = strrchr(srcfnm, '>')) == NULL
     && (name = strrchr(srcfnm, '/')) == NULL
     && (name = strrchr(srcfnm, ':')) == NULL)
    name = &srcfnm[0];
  else
    name++;
  
  /* -- check for .ALAN suffix and add one if missing -- */
  if((ext = strrchr(name, '.')) == NULL) { /* Point to last '.' */
#ifdef __dos__
    strcat(name, ".ala");	/* Was there none add */
#else
    strcat(name, ".alan");	/* Was there none add */
#endif
    ext = strrchr(name, '.');
  }
  i = (ext-name)/sizeof(char);	/* Calculate index to '.' */
  
  /* -- create list file name -- */
  strcpy(lstfnm, name);
  strcpy(&lstfnm[i], ".lis");
  
  /* -- create string data file names -- */
  strcpy(txtfnm, name);
  strcpy(&txtfnm[i], ".txt");
  strcpy(datfnm, name);
  strcpy(&datfnm[i], ".dat");
  
  /* -- create ACODE file name -- */
  strcpy(acdfnm, name);
  strcpy(&acdfnm[i], ".acd");
}



/************************************************************************\

			ALAN Main Procedure

\************************************************************************/

#ifdef _PROTOTYPES_
int main(int argc,		/* IN - argument count */
	 char *argv[]		/* IN - program arguments */
)
#else
int main(argc,argv)
     int argc;			/* IN - argument count */
     char *argv[];		/* IN - program arguments */
#endif
{
  int nArgs;			/* Number of supplied args */
  lmSev sevs;			/* Set of listing severities */

  starttot();			/* Start timer */
  /* -- get arguments -- */
  nArgs = spaProcess(argc, argv, arguments, options, paramError);
  /* Say hello ! */
  if (verbose)
    printf("%s\n\n", product.longHeader);

  if (nArgs == 0) {
    paramError(NULL, NULL, 0, 0);
    exit(EXIT_FAILURE);
  } else if (nArgs > 1)
    exit(EXIT_FAILURE);

  /* Process the arguments */
  prepareNames();

  /* -- compile -- */
#ifdef MALLOC
  malloc_debug(2);
#endif
  heap = malloc(10000);		/* Remember where heap starts */
  free(heap);
  lmInit(product.shortHeader, srcfnm, lm_english_Messages);
  if (!smScanEnter(srcfnm)) {
    /* Failed to open the source file */
    lmLog(NULL, 199, sevFAT, srcfnm);
    lmList("", 0, 78, liERR, sevALL); /* TINY list on the screen*/
    exit(EXIT_FAILURE);
  }

  /* OK, found it so now compile it! */
  if (verbose) printf("Parsing");
  startcomp();			/* Start timing compilation */
  txtfil = fopen(txtfnm, "w");	/* Open a temporary text file */

  /* First initialise */
  initadv();

  /* Then parse the program and build internal representation */
  start();
  pmParse();
  endpars();			/* End of parsing pass */
    
  /* Analyze the internal form */
  if (verbose) printf("\nAnalyzing:");
  start();
  anadv();			/* Analyze the adventure */
  endsem();			/* End of semantic pass */

  /* All text is output so close text file */
  fclose(txtfil);
    
  /* OK so far ? */
  if (lmSeverity() < sevERR) {
    /* Yes, so generate an adventure */
    if (verbose) printf("Generating");
    datfil = fopen(datfnm, WRITE_MODE);
    txtfil = fopen(txtfnm, "r");
    if (dbgflg)			/* Force debugging */
      opts[OPTDEBUG].value = TRUE;
    if (packflg)		/* Force packing */
      opts[OPTPACK].value = TRUE;
    start();
    geadv(acdfnm, revflg);
    endgen();			/* End of generating pass */
    fclose(txtfil);
    fclose(datfil);
  } else {
    lmLog(NULL, 999, sevINF, "");
  }
  endcomp();

  if (dmpflg == 0)
#ifdef __vms__
    delete(txtfnm);
#else
    unlink(txtfnm);
#endif    

  if (dmpflg & DMPALL != 0)	/* Dump everything */
    dmpflg = ~0;		/* Set all bits */

  /* Create listing files and list messages on the screen */
  if (lstflg) {			/* If -l option, create list file */
    lmList(lstfnm, lcount, ccount, liFULL, sevALL);
    if (dmpflg) {
      lmSkipLines(0);
      duadv(dmpflg);
    }
    if (sumflg) {
      if (lmSeverity() < sevERR)
	summary();
      stats();
    }
  }

  /* Check what messages to show on the screen */
  if (fulflg)
    lmList("", 0, 80, liFULL, sevALL);
  else {
    sevs = sevALL;
    if (!warnings)
      sevs &= ~sevWAR;
    if (!infos)
      sevs &= ~sevINF;
    lmList("", 0, 80, liTINY, sevs);
  }
  if (dmpflg != 0 && !lstflg) {
    lmSkipLines(0);
    duadv(dmpflg);
  }
  if (sumflg) {
    if (lmSeverity() < sevERR)
      summary();
    endtotal();			/* Stop timer */
    prtimes();
    stats();
  }
  lmTerminate();
#ifdef MALLOC
  if (malloc_verify() == 0) printf("Error in heap!\n");
#endif
  /*    if (ppflg) pp(); */

  if (lmSeverity() < sevERR)
    exit(EXIT_SUCCESS);
  else
    exit(EXIT_FAILURE);
  return(0);
}
