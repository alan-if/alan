/*----------------------------------------------------------------------*\

  alan.c

  Alan compiler main program unit

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"

#ifdef __sun__
#include <unistd.h>
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


/* PUBLIC DATA */

Srcp nulsrcp			/* NULL position for list */
  = {0,0,0};

char advnam[255];		/* Name of adventure (file name excl. ext.) */
FILE *txtfil;			/* File of collected text data */
FILE *datfil;			/* File of encoded text */

int fileNo = 0;			/* File number to use next */
Bool verbose;			/* Verbose mode */
long counter = 0;		/* Number of new's so far, for verbose */

List *includePaths = NULL;	/* List of additional include paths */

/* PRIVATE */

static void *heap;		/* Address to first free heap area - before */



/* Timing */
#include "timing.h"

static TIBUF tbuf;
static TIBUF totTime;
static TIBUF compTime;

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
static void starttot(void)
{
  tistart(&totTime);
}


/*----------------------------------------------------------------------

  startcomp()

  Start compilation timing.

 */
static void startcomp(void)
{
  tistart(&compTime);
}


/*----------------------------------------------------------------------

  start()

  Start timing.

 */
static void start(void)
{
  tistart(&tbuf);
}


/*----------------------------------------------------------------------

  endpars()

  Note the time of parsing.

 */
static void endpars(void)
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
static void endsem(void)
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
static void endgen(void)
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
static void endcomp(void)
{
  tistop(&compTime);
#ifdef MULTI
  tim.comp = compTime.pu_elapsed;
#else
  tim.comp = compTime.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------

  endtotal()

  Note total time.

 */
static void endtotal(void)
{
  tistop(&totTime);
#ifdef MULTI
  tim.tot = totTime.pu_elapsed;
#else
  tim.tot = totTime.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------

  prtimes()

  Print elapsed times.

 */
static void prtimes(void)
{
  char str[80];
  
  lmSkipLines(8);
  lmLiPrint("");
  lmLiPrint(       "        Timing");
  lmLiPrint(       "        ------");
  (void)sprintf(str,   "        Parse Time:             %6ld", tim.pars);
  lmLiPrint(str);
  if (tim.sem != 0) {
    (void)sprintf(str, "        Analysis Time:          %6ld", tim.sem);
    lmLiPrint(str);
  }
  if (tim.gen != 0) {
    (void)sprintf(str, "        Code Generation Time:   %6ld", tim.gen);
    lmLiPrint(str);
  }
  (void)sprintf(str,   "        ------------------------------");
  lmLiPrint(str);
  (void)sprintf(str,   "        Compilation Time:       %6ld", tim.comp);
  lmLiPrint(str);
  lmLiPrint("");
  (void)sprintf(str,   "        Total Time:             %6ld", tim.tot);
  lmLiPrint(str);
}



/*----------------------------------------------------------------------

  stats()

  Print some statistics.

 */
static void stats(void)
{
  int lins;
  char str[80];
  
  lmSkipLines(7);
  lmLiPrint("");
  
  lins = scannedLines();
  if (fileNo > 1)
    (void)sprintf(str, "        %d source lines read from %d files.", lins, fileNo);
  else
    (void)sprintf(str, "        %d source lines read.", lins);
  lmLiPrint(str);
  lmLiPrint("");
  
  if (tim.tot != 0 && tim.comp != 0) {
    (void)sprintf(str, "        %d lines/CPUminute.",
	    (int)(60L*1000L*(long)lins/tim.comp));
    lmLiPrint(str);
    lmLiPrint("");
  }
  
  (void)sprintf(str,   "        Estimated dynamic memory usage = %d bytes.",
	  (char *)malloc(10000)-(char *)heap);
  lmLiPrint(str);
  lmLiPrint("");
}



/************************************************************************\

			PUBLIC routines

\************************************************************************/


/*======================================================================

  terminate()

  Terminate the program with an error code.

 */
void terminate(int ecode)
{
#ifdef __MWERKS__
	printf("Command-Q to quit.");
#endif
	exit(ecode);
}
/*======================================================================

  unimpl()

  An unimplemented constrution was encountered.

 */
void unimpl(Srcp *srcp,		/* IN  - Where? */
	    char *phase)	/* IN  - What phase? */
{
  lmLog(srcp, 998, sevWAR, phase);
}


/*======================================================================

  syserr()

  Some kind of internal system error was detected. Log it.

 */
void syserr(char *str)
{
  lmLog(&nulsrcp, 997, sevSYS, str);
  lmList("", 0, 79, liTINY, sevALL);
  terminate(EXIT_FAILURE);
}


/*======================================================================

  allocate()

  Safely allocate new memory.

*/
void *allocate(int len)		/* IN - Length to allocate */
{
  void *p = malloc((size_t)len);

  if (p == NULL)
    syserr("Out of memory");

  return p;
}




/* -- local variables for main() -- */
  
static char srcfnm[255];	/* File name of source file */
static char txtfnm[255];	/*   - " -   of collected text file */
static char datfnm[255];	/*   - " -   of encoded data file */
static char acdfnm[255];	/*   - " -   of ACODE file */
static char lstfnm[255];	/*   - " -   of listing file */

/* SPA Option handling */

#include "spa.h"


static char *srcptr;		/* Pointer to adventure name */
static Bool warnings;		/* Show warnings */
static Bool infos;		/* Show informational messages */
static Bool fulflg;		/* Full source listing */
static Bool lstflg;		/* Create listing file */
static Bool ccflg;		/* Show messages as old 'cc' */
static int lcount;		/* Number of lines per page */
static int ccount;		/* -"-    columns */
static DmpKind dmpflg = 0;	/* Dump internal form flags */
static Bool dbgflg = 0;		/* Debug option flags */
static Bool packflg = 0;	/* Pack option flags */
static Bool sumflg;		/* Print a summary */

static SPA_FUN(usage)
{
  printf("Usage: ALAN <adventure> [-help] [options]\n");
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
  terminate(EXIT_FAILURE);
}

static SPA_FUN(extraArg)
{
  printf("Extra argument: '%s'\n", rawName);
  usage(NULL, NULL, 0);
  terminate(EXIT_FAILURE);
}

static SPA_FUN(xit) {terminate(EXIT_SUCCESS);}

static SPA_FUN(addInclude)
{
  /* Add the include path to our list */
  includePaths = concat(includePaths, spaArgument(1));
  /* Now we can skip the include path */
  spaSkip(1);
}


static SPA_DECLARE(arguments)
#ifdef __dos__
     SPA_STRING("adventure", "file name, default extension '.ala'", srcptr, NULL, NULL)
#else
     SPA_STRING("adventure", "file name, default extension '.alan'", srcptr, NULL, NULL)
#endif
     SPA_FUNCTION("", "extra argument", extraArg)
SPA_END

static SPA_DECLARE(options)
#ifndef THINK_C
     SPA_HELP("help", "this help", usage, xit)
#endif
     SPA_FLAG("verbose", "verbose messages", verbose, FALSE, NULL)
     SPA_FLAG("warnings", "[don't] show warning messages", warnings, TRUE, NULL)
     SPA_FLAG("infos", "[don't] show informational messages", infos, FALSE, NULL)
     SPA_FUNCTION("include <path>", "additional directory to search before current when\nlooking for included files (may be repeated)", addInclude)
     SPA_FLAG("cc", "show messages on the screen in old 'cc' format\n", ccflg, FALSE, NULL)
     SPA_FLAG("full", "full source in the list file", fulflg, FALSE, NULL)
     SPA_INTEGER("height <lines)", "height of pages in listing", lcount, 74, NULL)
     SPA_INTEGER("width <characters>", "width of pages in listing", ccount, 112, NULL)
     SPA_FLAG("listing", "create listing file", lstflg, FALSE, NULL)
     SPA_FLAG("debug", "force debug option in adventure", dbgflg, FALSE, NULL)
     SPA_FLAG("pack", "force pack option in adventure", packflg, FALSE, NULL)
     SPA_FLAG("summary", "print a summary", sumflg, FALSE, NULL)
#ifndef THINK_C
     SPA_BITS("dump", "dump the internal form, where\n\
synonyms\n\
syntax\n\
verbs\n\
locations\n\
objects\n\
containers\n\
events\n\
actors\n\
rules\n\
everything", dmpflg, "sxvlocear!", NULL, NULL)
#endif
/*     SPA_FLAG("prettyprint", "pretty print the adventure", ppflg, FALSE, NULL) */
SPA_END


/*----------------------------------------------------------------------

  prepareNames()

  Prepare all file names needed.

 */
static void prepareNames(void)
{
  /* Save source file name */
  strcpy(srcfnm, srcptr);

  /* -- Then strip directory -- */
  if((srcptr = strrchr(srcfnm, ']')) == NULL
     && (srcptr = strrchr(srcfnm, '>')) == NULL
     && (srcptr = strrchr(srcfnm, '/')) == NULL
     && (srcptr = strrchr(srcfnm, ':')) == NULL)
    srcptr = &srcfnm[0];
  else
    srcptr++;
  /* Save the basename as the name of the adventure */
  strcpy(advnam, srcptr);

  /* -- check for .ALAN suffix and add one if missing -- */
  if(strrchr(srcptr, '.') == NULL) { /* Point to last '.' */
#ifdef __dos__
    strcat(srcptr, ".ala");	/* Was there none add */
#else
    strcat(srcptr, ".alan");	/* Was there none add */
#endif
  } else {
    char *p = strrchr(advnam, '.');
    *p = '\0';
  }
  
  /* -- create list file name -- */
  strcpy(lstfnm, advnam);
  strcat(lstfnm, ".lis");
  
  /* -- create string data file names -- */
  strcpy(txtfnm, advnam);
  strcat(txtfnm, ".txt");
  strcpy(datfnm, advnam);
  strcat(datfnm, ".dat");
  
  /* -- create ACODE file name -- */
  strcpy(acdfnm, advnam);
  strcat(acdfnm, ".acd");
}


/************************************************************************\

			ALAN Main Procedure

\************************************************************************/

int main(int argc,		/* IN - argument count */
	 char **argv		/* IN - program arguments */
)
{
  int nArgs;			/* Number of supplied args */
  lmSev sevs;			/* Set of listing severities */

#ifdef __MWERKS__
#include <SIOUX.h>
  SIOUXSettings.setupmenus = FALSE;
  SIOUXSettings.asktosaveonclose = FALSE;
  SIOUXSettings.showstatusline = FALSE;
#endif

  starttot();			/* Start timer */
  /* -- get arguments -- */
  nArgs = spaProcess(argc, argv, arguments, options, paramError);
  /* Say hello ! */
  if (verbose)
    printf("%s\n\n", product.longHeader);

  if (nArgs == 0) {
    usage(NULL, NULL, 0);
    terminate(EXIT_FAILURE);
  } else if (nArgs > 1)
    terminate(EXIT_FAILURE);

  /* Process the arguments */
  prepareNames();

  /* -- compile -- */
#ifdef MALLOC
  malloc_debug(2);
#endif
  heap = malloc((size_t)10000);		/* Remember where heap starts */
  free(heap);
  lmLiInit(product.shortHeader, srcfnm, lm_ENGLISH_Messages);
  if (!smScanEnter(srcfnm, FALSE)) {
    /* Failed to open the source file */
    lmLog(NULL, 199, sevFAT, srcfnm);
    lmList("", 0, 79, liERR, sevALL); /* TINY list on the screen*/
    terminate(EXIT_FAILURE);
  }

  /* OK, found it so now compile it! */
  if (verbose) printf("Parsing: ");
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
    if (verbose) printf("Generating:");
    datfil = fopen(datfnm, WRITE_MODE);
    txtfil = fopen(txtfnm, "r");
    if (dbgflg)			/* Force debugging */
      opts[OPTDEBUG].value = TRUE;
    if (packflg)		/* Force packing */
      opts[OPTPACK].value = TRUE;
    start();
    geadv(acdfnm);
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

  /* Create listing files and list messages on the screen */
  if (lstflg) {			/* If -l option, create list file */
    lmList(lstfnm, lcount, ccount, fulflg?liFULL:liTINY, sevALL);
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
  sevs = sevALL;
  if (!warnings)
    sevs &= ~sevWAR;
  if (!infos)
    sevs &= ~sevINF;
  if (ccflg) {
    int i,j;
    char err[1024];
    Srcp srcp;
    List *fnm;
    List nofile;

    nofile.element.str = "<no file>";
    /* Print a list on the terminal if errors detected. */
    for (i=1; lmMsg(i, &srcp, err); i++) {
      /* Advance to the correct file name */
      if (srcp.file == -1) 
	fnm = &nofile;
      else
	for (fnm = fileNames, j = 0; j < srcp.file; j++) fnm = fnm->next;
      printf("\"%s\", line %d: ALAN-%s (column %d)\n", fnm->element.str,
	     srcp.line, err, srcp.col);
    }
  } else
    lmList("", 0, 79, liTINY, sevs);

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
  lmLiTerminate();
#ifdef MALLOC
  if (malloc_verify() == 0) printf("Error in heap!\n");
#endif
  /*    if (ppflg) pp(); */

  if (lmSeverity() < sevERR)
    terminate(EXIT_SUCCESS);
  else
    terminate(EXIT_FAILURE);
  return(0);
}
