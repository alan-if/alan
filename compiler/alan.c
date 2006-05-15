/*----------------------------------------------------------------------*\

  alan.c

  Alan compiler main program unit

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"
#include "util.h"

#ifdef __sun__
#include <unistd.h>
#endif

#include "alan.h"

/* IMPORTS */
#include "alan.version.h"
#include "opt.h"
#include "dump.h"
#include "lmList.h"
#include "pmParse.h"
#include "smScan.h"
#include "options.h"

#include "srcp_x.h"
#include "adv_x.h"

#ifdef WINGUI
#include <windows.h>
#endif


/* PUBLIC DATA */
FILE *txtfil;			/* File of collected text data */
FILE *datfil;			/* File of encoded text */

int fileNo = 0;			/* File number to use next */
long counter = 0;		/* Number of new's so far, for verbose */
#ifdef WINGUI
Bool guiMode = FALSE;		/* Using the GUI? Or console? */
#endif


/* PRIVATE */

static void *heap;		/* Address to first free heap area - before */


/* Timing */
#include "timing.h"


static TIBUF tbuf;
static TIBUF totalTime;
static TIBUF compilationTime;

static struct {
  long int pars;
  long int sem;
  long int gen;
  long int comp;
  long int tot;
} tim;



/*----------------------------------------------------------------------*/
static void startTimingTotal(void)
{
  tistart(&totalTime);
}


/*----------------------------------------------------------------------*/
static void startTimingCompilation(void)
{
  tistart(&compilationTime);
}


/*----------------------------------------------------------------------*/
static void startTiming(void)
{
  tistart(&tbuf);
}


/*----------------------------------------------------------------------*/
static void endParseTiming(void)
{
  tistop(&tbuf);
#ifdef MULTI
  tim.pars = tbuf.pu_elapsed;
#else
  tim.pars = tbuf.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------*/
static void endSemanticsTiming(void)
{
  tistop(&tbuf);
#ifdef MULTI
  tim.sem = tbuf.pu_elapsed;
#else
  tim.sem = tbuf.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------*/
static void endGenerationTiming(void)
{
  tistop(&tbuf);
#ifdef MULTI
  tim.gen = tbuf.pu_elapsed;
#else
  tim.gen = tbuf.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------*/
static void endCompilationTiming(void)
{
  tistop(&compilationTime);
#ifdef MULTI
  tim.comp = compilationTime.pu_elapsed;
#else
  tim.comp = compilationTime.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------*/
static void endTotalTiming(void)
{
  tistop(&totalTime);
#ifdef MULTI
  tim.tot = totalTime.pu_elapsed;
#else
  tim.tot = totalTime.real_elapsed*1000;
#endif
}


/*----------------------------------------------------------------------*/
static void printTimes(void)
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



/*----------------------------------------------------------------------*/
static void statistics(void)
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
  (void)sprintf(str,   "        Calculated       - \"\" -        = %ld bytes.",
	  allocated);
  lmLiPrint(str);
  lmLiPrint("");
}


/* -- local variables for main() -- */
  
static char srcfnm[255];	/* File name of source file */
static char txtfnm[255];	/*   - " -   of collected text file */
static char datfnm[255];	/*   - " -   of encoded data file */
static char acdfnm[255];	/*   - " -   of ACODE file */
static char lstfnm[255];	/*   - " -   of listing file */
static char ifidfnm[255];	/*   - " -   of IFID file */


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
     && (srcptr = strrchr(srcfnm, '\\')) == NULL
     && (srcptr = strrchr(srcfnm, ':')) == NULL)
    srcptr = &srcfnm[0];
  else
    srcptr++;
  /* Save the basename as the name of the adventure */
  adv.name = strdup(srcptr);

  /* -- check for .ALAN suffix and add one if missing -- */
  if(strrchr(srcptr, '.') == NULL) { /* Point to last '.' */
#ifdef __dos__
    strcat(srcptr, ".ala");	/* Was there none add */
#else
#ifndef __mac__
    strcat(srcptr, ".alan");	/* Was there none add */
#endif
#endif
  } else {
    char *p = strrchr(adv.name, '.');
    *p = '\0';
  }

  /* -- create list file name -- */
  strcpy(lstfnm, adv.name);
  strcat(lstfnm, ".lis");
  
  /* -- create IFID file name -- */
  strcpy(ifidfnm, adv.name);
  strcat(ifidfnm, ".ifid");
  
  /* -- create string data file names -- */
  strcpy(txtfnm, adv.name);
  strcat(txtfnm, ".tmp");
  strcpy(datfnm, adv.name);
  strcat(datfnm, ".dat");
  
  /* -- create ACODE file name -- */
  strcpy(acdfnm, adv.name);
  strcat(acdfnm, ".a3c");
}



/************************************************************************\

			ALAN Main Procedure

\************************************************************************/

void compile(void) {
  lmSev sevs = 0;		/* Set of listing severities */

  /* Start timer */
  startTimingTotal();
			
  /* Process the arguments */
  prepareNames();

  /* -- compile -- */
#ifdef MALLOC
  malloc_debug(2);
#endif
  heap = malloc((size_t)10000);		/* Remember where heap starts */
  free(heap);
  lmLiInit(alan.shortHeader, srcfnm, lm_ENGLISH_Messages);
  setCharacterSet(charset);

  if (!smScanEnter(srcfnm, FALSE)) {
    /* Failed to open the source file */
    lmLog(NULL, 199, sevFAT, srcfnm);
    listing("", 0, 79, liMSG, sevALL);
    terminate(EXIT_FAILURE);
  }

  /* OK, found it so now compile it! */
  if (verbose) printf("Parsing: ");
  startTimingCompilation();			/* Start timing compilation */
#ifdef __MWERKS__
  _fcreator = '?\?\?\?';
  _ftype = '?\?\?\?';
#endif

  txtfil = fopen(txtfnm, "w+");	/* Open a temporary text file */
  if (!txtfil) {
    char errorString[1000];
    sprintf(errorString, "Could not open output file '%s' for writing", txtfnm);
    SYSERR(errorString);
  }

  /* First initialise */
  initAdventure();

  /* Then parse the program and build internal representation */
  startTiming();
  pmParse();
  endParseTiming();			/* End of parsing pass */

  if ((dumpFlags&DUMP_1) > 0) {
    listing("", 0, 79, liTINY, sevs);
    lmSkipLines(0);
    dumpAdventure(dumpFlags);
    terminate(EXIT_FAILURE);
  }

  /* Analyze the internal form */
  if (verbose) printf("\nAnalyzing:");
  startTiming();
  analyzeAdventure();			/* Analyze the adventure */
  endSemanticsTiming();			/* End of semantic pass */

  if ((dumpFlags&DUMP_2) > 0) {
    listing("", 0, 79, liTINY, sevs);
    lmSkipLines(0);
    dumpAdventure(dumpFlags);
    terminate(EXIT_FAILURE);
  }

  /* All text is output so close text file */
  fclose(txtfil);

  /* OK so far ? */
  if (lmSeverity() < sevERR) {
    /* Yes, so generate an adventure */
    if (verbose) printf("Generating:");
#ifdef __MWERKS__
    _fcreator = 'Arun';
    _ftype = 'Adat';
#endif
    opts[OPTDEBUG].value = debugFlag; /* Debugging enabled? */
    if (packFlag)		/* Force packing */
      opts[OPTPACK].value = TRUE;
    startTiming();
    generateAdventure(acdfnm, txtfnm, datfnm);
    endGenerationTiming();			/* End of generating pass */
  } else {
    lmLog(NULL, 999, sevINF, "");
  }
  endCompilationTiming();

  if (dumpFlags == 0) {
    unlink(txtfnm);
    unlink(datfnm);
  }

  /* Check what messages to show on the screen */
  sevs = sevALL;
  if (!warningFlag)
    sevs &= ~sevWAR;
  if (!infoFlag)
    sevs &= ~sevINF;

  /* Create listing files and list messages on the screen */
#ifdef __mac__
  listing(sevs);
#else
  if (listingFlag) {			/* If -l option, create list file */
    listing(lstfnm, lcount, ccount, fullFlag?liFULL:liTINY, sevs /*sevALL*/);
    if (dumpFlags) {
      lmSkipLines(0);
      dumpAdventure(dumpFlags);
    }
    if (summaryFlag) {
      if (lmSeverity() < sevERR)
	summary();
      statistics();
    }
  }

  listing("", 0, 79, listingFlag?liTINY:(fullFlag?liFULL:liTINY), sevs);

  if (dumpFlags != 0 && !listingFlag) {
    lmSkipLines(0);
    dumpAdventure(dumpFlags);
  }
  if (summaryFlag) {
    if (lmSeverity() < sevERR)
      summary();
    endTotalTiming();			/* Stop timer */
    printTimes();
    statistics();
  }

  lmLiTerminate();
#endif

#ifdef MALLOC
  if (malloc_verify() == 0) printf("Error in heap!\n");
#endif

  /* if (ppflg) pp(); */

  if (lmSeverity() < sevERR)
    terminate(EXIT_SUCCESS);
  else
    terminate(EXIT_FAILURE);
}
