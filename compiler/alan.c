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
#include "images_x.h"


/* PUBLIC DATA */

char advnam[255];		/* Name of adventure (file name excl. ext.) */
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
  (void)sprintf(str,   "        Calculated       - \"\" -        = %ld bytes.",
	  allocated);
  lmLiPrint(str);
  lmLiPrint("");
}



/************************************************************************\

			PUBLIC routines

\************************************************************************/




/* -- local variables for main() -- */
  
static char srcfnm[255];	/* File name of source file */
static char txtfnm[255];	/*   - " -   of collected text file */
static char datfnm[255];	/*   - " -   of encoded data file */
static char acdfnm[255];	/*   - " -   of ACODE file */
static char lstfnm[255];	/*   - " -   of listing file */


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
  strcpy(advnam, srcptr);

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
    char *p = strrchr(advnam, '.');
    *p = '\0';
  }

  /* -- create list file name -- */
  strcpy(lstfnm, advnam);
  strcat(lstfnm, ".lis");
  
  /* -- create string data file names -- */
  strcpy(txtfnm, advnam);
  strcat(txtfnm, ".tmp");
  strcpy(datfnm, advnam);
  strcat(datfnm, ".dat");
  
  /* -- create ACODE file name -- */
  strcpy(acdfnm, advnam);
  strcat(acdfnm, ".a3c");
}



#ifdef __mac__
/*----------------------------------------------------------------------

	Write listing and/or error messages to screen or file

*/

static void listing(lmSev sevs)
{
  char *fnm;

  if (lstflg)
    fnm = lstfnm;
  else
    fnm = "";
#ifdef __MWERKS__
  _fcreator = 'ttxt';
  _ftype = 'TEXT';
#endif

  if (ccflg) {
    lmList(fnm, lcount, ccount, 0, 0);
    cc_listing(sevs);
  } else
    lmList(fnm, lcount, ccount, fulflg?liFULL:liTINY, sevs);

  if (dmpflg) {
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
}
#endif


/*----------------------------------------------------------------------

	Find out whether error message is of one of the severities
	being printed.

*/

/*----------------------------------------------------------------------*/
static int test_severity(char *err, lmSev sevs)
{
  /* Check if the severity was among the wanted ones */
  char c;
  lmSev sev;

  sscanf(err, "%*d %c", &c);
  switch (c) {
  case 'O': sev = sevOK;  break;
  case 'I': sev = sevINF; break;
  case 'W': sev = sevWAR; break;
  case 'E': sev = sevERR; break;
  case 'F': sev = sevFAT; break;
  case 'S': sev = sevSYS; break;
  }
  return sev & sevs;
}


/*----------------------------------------------------------------------*/
static void cc_listing(lmSev sevs)
{
  int i,j;
  char err[1024], line[1024];
  Srcp srcp;
  List *fnm;
  List nofile;
	
  nofile.element.str = "<no file>";
  for (i = 1; lmMsg(i, &srcp, err); i++) {
    if (test_severity(err, sevs)) {
      /* Advance to the correct file name */
      if (srcp.file == -1) 
	fnm = &nofile;
      else
	for (fnm = fileNames, j = 0; j < srcp.file; j++) 
	  fnm = fnm->next;
      sprintf(line, "\"%s\", line %d:%d: ALAN-%s (column %d)\n",
	      fnm->element.str, srcp.line, srcp.col, err, srcp.col);
      sprintf(line, "\"%s\", line %d(%d): %s\n",
	      fnm->element.str, srcp.line, srcp.col, err);
#ifdef __mac__
      lmLiPrint(line);
#else
      printf(line);
#endif
    }
  }
}


/************************************************************************\

			ALAN Main Procedure

\************************************************************************/

void compile(void) {
  lmSev sevs = 0;		/* Set of listing severities */


  /* Start timer */
  starttot();
			
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
    lmList("", 0, 79, liMSG, sevALL); /* TINY list on the screen*/
    terminate(EXIT_FAILURE);
  }

  /* OK, found it so now compile it! */
  if (verbose) printf("Parsing: ");
  startcomp();			/* Start timing compilation */
#ifdef __MWERKS__
  _fcreator = '?\?\?\?';
  _ftype = '?\?\?\?';
#endif
  txtfil = fopen(txtfnm, "w");	/* Open a temporary text file */
  if (!txtfil)
    syserr("Could not open output file '%s' for writing.", txtfnm);

  /* First initialise */
  initAdventure();

  /* Then parse the program and build internal representation */
  start();
  pmParse();
  endpars();			/* End of parsing pass */

  if ((dmpflg&DUMP_1) > 0) {
    lmList("", 0, 79, liTINY, sevs);
    lmSkipLines(0);
    dumpAdventure(dmpflg);
    terminate(EXIT_FAILURE);
  }

  /* Analyze the internal form */
  if (verbose) printf("\nAnalyzing:");
  start();
  analyzeAdventure();			/* Analyze the adventure */
  endsem();			/* End of semantic pass */

  if ((dmpflg&DUMP_2) > 0) {
    lmList("", 0, 79, liTINY, sevs);
    lmSkipLines(0);
    dumpAdventure(dmpflg);
    terminate(EXIT_FAILURE);
  }

  /* All text is output so close text file */
  fclose(txtfil);

  /* All image files found so sort, number and copy them */
  prepareImages();

  /* OK so far ? */
  if (lmSeverity() < sevERR) {
    /* Yes, so generate an adventure */
    if (verbose) printf("Generating:");
#ifdef __MWERKS__
    _fcreator = 'Arun';
    _ftype = 'Adat';
#endif
    opts[OPTDEBUG].value = debugOption; /* Debugging enabled? */
    if (packflg)		/* Force packing */
      opts[OPTPACK].value = TRUE;
    start();
    generateAdventure(acdfnm, txtfnm, datfnm);
    endgen();			/* End of generating pass */
  } else {
    lmLog(NULL, 999, sevINF, "");
  }
  endcomp();

  if (dmpflg == 0) {
    unlink(txtfnm);
    unlink(datfnm);
  }

  /* Check what messages to show on the screen */
  sevs = sevALL;
  if (!warnings)
    sevs &= ~sevWAR;
  if (!infos)
    sevs &= ~sevINF;

  /* Create listing files and list messages on the screen */
#ifdef __mac__
  listing(sevs);
#else
  if (lstflg) {			/* If -l option, create list file */
    lmList(lstfnm, lcount, ccount, fulflg?liFULL:liTINY, sevs /*sevALL*/);
    if (dmpflg) {
      lmSkipLines(0);
      dumpAdventure(dmpflg);
    }
    if (sumflg) {
      if (lmSeverity() < sevERR)
	summary();
      stats();
    }
  }

  if (ccflg)
    cc_listing(sevs);
  else
    lmList("", 0, 79, lstflg?liTINY:(fulflg?liFULL:liTINY), sevs);

  if (dmpflg != 0 && !lstflg) {
    lmSkipLines(0);
    dumpAdventure(dmpflg);
  }
  if (sumflg) {
    if (lmSeverity() < sevERR)
      summary();
    endtotal();			/* Stop timer */
    prtimes();
    stats();
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
