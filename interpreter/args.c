/*----------------------------------------------------------------------*\

  args - Argument handling for arun

  Handles the various startup methods on all machines.

  Main function args() will set up global variable adventureName and the flags,
  the terminal will also be set up and connected.

\*----------------------------------------------------------------------*/

#ifndef __PACIFIC__
#include "args.h"
#else
/* I have no idea at all why the include does not work in Pacific C ... */
extern void args(int argc, char* argv[]);
#endif

#ifdef __win__
#include <windows.h>
#endif

#include "main.h"

#ifdef __mac__
#include "macArgs.h"
#endif

#ifdef __amiga__
#include <libraries/dosextens.h>
#ifdef AZTEC_C
struct FileHandle *con = NULL;
#else
/* Geek Gadgets GCC */
BPTR window;
BPTR cd;
#endif
#endif

#ifdef GLK
#include "glk.h"
#include "glkio.h"
#endif

#ifdef _PROTOTYPES_
static void switches(
     unsigned argc,
     char *argv[]
)
#else
static void switches(argc, argv)
     unsigned argc;
     char *argv[];
#endif
{
  int i;
  
  adventureName = "";
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
#ifdef GLK
      switch (glk_char_to_lower(argv[i][1]))
#else
      switch (tolower(argv[i][1]))
#endif
      {
      case 'i':
	ignoreErrorOption = FALSE;
	break;
      case 't':
	sectionTraceOption = TRUE;
	switch (argv[i][2]) {
	case '4' : traceStackOption = TRUE;
	case '3' : tracePushOption = TRUE;
	case '2' : singleStepOption = TRUE;
	case '\0':
	case '1': sectionTraceOption = TRUE;
	}
	break;
      case 'd':
	debugOption = TRUE;
	break;
      case 'l':
	transcriptOption = TRUE;
	logOption = FALSE;
	break;
      case 'v':
	verbose = TRUE;
	break;
      case 'n':
	statusLineOption = FALSE;
	break;
      case 'c':
	logOption = TRUE;
	transcriptOption = FALSE;
	break;
      case 'r':
	regressionTestOption = TRUE;
	break;
      default:
	printf("Unrecognized switch, -%c\n", argv[i][1]);
	usage();
	terminate(0);
      }
    } else {
      adventureName = argv[i];
      if (adventureName[0] == '"') {
	/* Probably Windows quoting names with spaces... */
	char *str = allocate(strlen(adventureName)+1);
	strcpy(str, &adventureName[1]);
	adventureName = str;
	adventureName[strlen(adventureName)-1] = '\0';
      }
      if (strcmp(&adventureName[strlen(adventureName)-4], ".a3c") == 0
	  || strcmp(&adventureName[strlen(adventureName)-4], ".A3C") == 0)
		adventureName[strlen(adventureName)-4] = '\0';
    }
  }
}



#ifdef __amiga__

#include <intuition/intuition.h>
#include <workbench/workbench.h>

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/icon_protos.h>

#include <fcntl.h>

extern struct Library *IconBase;

#ifndef AZTEC_C
/* Actually Geek Gadgets GCC with libnix */

/* Aztec C has its own pre-main wbparse which was used in Arun 2.7, with GCC we
   need to do it ourselves. */

#include <clib/intuition_protos.h>

extern unsigned long *__stdfiledes; /* The libnix standard I/O file descriptors */

void
wb_parse(void)
{
  char *cp;
  struct DiskObject *dop;
  struct FileHandle *fhp;

  if (_WBenchMsg->sm_NumArgs == 1) /* If no argument use program icon/info */
    dop = GetDiskObject((UBYTE *)_WBenchMsg->sm_ArgList[0].wa_Name);
  else {
    BPTR olddir = CurrentDir(_WBenchMsg->sm_ArgList[1].wa_Lock);
    dop = GetDiskObject((UBYTE *)_WBenchMsg->sm_ArgList[1].wa_Name);
    CurrentDir(olddir);
  }
  if (dop != 0 && (cp = (char *)FindToolType((UBYTE **)dop->do_ToolTypes, 
					     (UBYTE *)"WINDOW")) != NULL)
    ;
  else /* Could not find a WINDOW tool type */
    cp = "CON:10/10/480/160/Arun:Default Window/CLOSE";
  if ((window = Open((UBYTE *)cp, (long)MODE_OLDFILE))) {
    fhp = (struct FileHandle *) ((long)window << 2);
    SetConsoleTask(fhp->fh_Type);
    SelectInput(window);
    SelectOutput(window);
    __stdfiledes[0] = Input();
    __stdfiledes[1] = Output();
  } else
    exit(-1L);
  FreeDiskObject(dop);
}
#endif
#endif


#ifdef _PROTOTYPES_
void args(
     int argc,
     char * argv[]
)
#else
void args(argc, argv)
    int argc;
    char *argv[];
#endif
{
  char *prgnam;

/***********************************************************************\

   MAC

\***********************************************************************/
#ifdef __mac__
#include <console.h>
#ifdef __MWERKS__
#include <SIOUX.h>
#endif
  short msg, files;
  static char advbuf[256], prgbuf[256];
  /*AppFile af;*/
  OSErr oe;

#ifdef __MWERKS__
  /*SIOUXSettings.setupmenus = FALSE;*/
  SIOUXSettings.autocloseonquit = FALSE;
  SIOUXSettings.asktosaveonclose = FALSE;
  SIOUXSettings.showstatusline = FALSE;
#endif

	GetMacArgs(advbuf);
	adventureName = advbuf;

#else



/***********************************************************************\

   AMIGA

\***********************************************************************/
#ifdef __amiga__

  if (argc == 0) { /* If started from Workbench get WbArgs : Aztec C & GG GCC */
    struct WBStartup *WBstart;

    if ((IconBase = OpenLibrary("icon.library", 0)) == NULL)
      syserr("Could not open 'icon.library'");
    /* If started from WB normal main is called with argc == 0 and argv = WBstartup message */
    WBstart = (struct WBStartup *)argv;
#ifndef AZTEC_C
    /* Geek Gadgets GCC */
    wb_parse();
#endif
    adventureName = prgnam = WBstart->sm_ArgList[0].wa_Name;
    if (WBstart->sm_NumArgs > 0) {
      cd = CurrentDir(DupLock(WBstart->sm_ArgList[1].wa_Lock));
      adventureName = WBstart->sm_ArgList[1].wa_Name;
    }
    /* Possibly other tooltypes ... */
  } else {
    /* Started from a CLI */
    if ((prgnam = strrchr(argv[0], '/')) == NULL
	&& (prgnam = strrchr(argv[0], ':')) == NULL)
      prgnam = argv[0];
    else
      prgnam++;
    /* Now look at the switches and arguments */
    switches(argc, argv);
    if (adventureName[0] == '\0')
      /* No game given, try program name */
      if (stricmp(prgnam, PROGNAME) != 0)
	adventureName = strdup(argv[0]);
  }
#else


/***********************************************************************\

   DOS & WINDOWS

\***********************************************************************/
#if defined(__dos__) || defined(__win__)
  if ((prgnam = strrchr(argv[0], '\\')) == NULL
      && (prgnam = strrchr(argv[0], '/')) == NULL
      && (prgnam = strrchr(argv[0], ':')) == NULL)
    prgnam = argv[0];
  else
    prgnam++;
  if (strlen(prgnam) > 4
      && (strcmp(&prgnam[strlen(prgnam)-4], ".EXE") == 0
	  || strcmp(&prgnam[strlen(prgnam)-4], ".exe") == 0))
    prgnam[strlen(prgnam)-4] = '\0';
  /* Now look at the switches and arguments */
  switches(argc, argv);
  if (adventureName[0] == '\0')
    /* No game given, try program name */
    if (stricmp(prgnam, PROGNAME) != 0)
      adventureName = strdup(argv[0]);

#else

/***********************************************************************\

   VMS

\***********************************************************************/
#if defined __vms__
  if ((prgnam = strrchr(argv[0], ']')) == NULL
      && (prgnam = strrchr(argv[0], '>')) == NULL
      && (prgnam = strrchr(argv[0], ':')) == NULL)
    prgnam = argv[0];
  else
    prgnam++;
  if (strrchr(prgnam, ';') != NULL)
    *strrchr(prgnam, ';') = '\0';
  if (strlen(prgnam) > 4
      && (strcmp(&prgnam[strlen(prgnam)-4], ".EXE") == 0
	  || strcmp(&prgnam[strlen(prgnam)-4], ".exe") == 0))
    prgnam[strlen(prgnam)-4] = '\0';
  /* Now look at the switches and arguments */
  switches(argc, argv);
  if (adventureName[0] == '\0')
    /* No game given, try program name */
    if (strcmp(prgnam, PROGNAME) != 0)
      adventureName = strdup(argv[0]);
#else

/***********************************************************************\

   UNIX

\***********************************************************************/
#if defined __unix__
  if ((prgnam = strrchr(argv[0], '/')) == NULL)
    prgnam = strdup(argv[0]);
  else
    prgnam = strdup(&prgnam[1]);
  if (strrchr(prgnam, ';') != NULL)
    *strrchr(prgnam, ';') = '\0';
  /* Now look at the switches and arguments */
  switches(argc, argv);
  if (adventureName == NULL)
    /* No game given, try program name */
    if (strcmp(prgnam, PROGNAME) != 0)
      adventureName = strdup(argv[0]);
#else


/***********************************************************************\

   UNIMPLEMENTED OS

\***********************************************************************/

#endif
#endif
#endif
#endif
#endif
}
