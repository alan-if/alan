/*----------------------------------------------------------------------*\

  args - Argument handling for arun

  Handles the various startup methods on all machines.

  Main function args() will set up global variable advnam and the flags,
  the terminal will also be set up and connected.

\*----------------------------------------------------------------------*/

#include "args.h"
#include "arun.h"


#ifdef __amiga__
#include <libraries/dosextens.h>
struct FileHandle *con = NULL;
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
  
  advnam = "";
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (tolower(argv[i][1])) {
      case 'i':
	errflg = FALSE;
	break;
      case 't':
	trcflg = TRUE;
	break;
      case 'd':
	dbgflg = TRUE;
	break;
      case 's':
	trcflg = TRUE;
	stpflg = TRUE;
	break;
      case 'l':
	logflg = TRUE;
	break;
      default:
	printf("Unrecognized switch, -%c\n", argv[i][1]);
	terminate(0);
      }
    } else {
      advnam = argv[i];
      if ((strcmp(&advnam[strlen(advnam)-4], ".acd") == 0)
		  || (strcmp(&advnam[strlen(advnam)-4], ".dat") == 0))
		advnam[strlen(advnam)-4] = '\0';
    }
  }
}


#ifdef _PROTOTYPES_
void args(
     unsigned argc,
     char *argv[]
)
#else
void args(argc, argv)
     unsigned argc;
     char *argv[];
#endif
{
  char *prgnam;

#ifdef __mac__
#include <console.h>
  short msg, files;
  static char advbuf[256], prgbuf[256];
  AppFile af;
  OSErr oe;

  CountAppFiles(&msg, &files);
  if (msg==0 && files>0) {		/* Found files! */
    GetAppFiles(1, &af);
    advnam = (char *)af.fName;
    strncpy(advbuf, (char *)&af.fName[1], af.fName[0]);
    advbuf[af.fName[0]] = '\0';
    advnam = advbuf;
    if (strcmp(&advnam[strlen(advnam)-4], ".acd") == 0
     || strcmp(&advnam[strlen(advnam)-4], ".dat") == 0)
       advnam[strlen(advnam)-4] = '\0';
    oe = SetVol(NULL, af.vRefNum);
  } else {
    strncpy(prgbuf, (char *)&CurApName[1], CurApName[0]);
    prgbuf[CurApName[0]] = '\0';
    if (strcmp(prgbuf, "arun") != 0 &&
	strcmp(prgbuf, "Arun.project") != 0)
      /* Another program name use that as the name of the adventure */
      advnam = prgbuf;
    else {
      int argc;
      char **argv[100];
      
      argc = ccommand(argv);
      switches(argc, *argv);
    }
    oe = SetVol(NULL, af.vRefNum); /* 4f_ti Should use volume of program */
  }
#else
#ifdef __amiga__
#include <workbench/startup.h>
#include <intuition/intuition.h>
#include <workbench/workbench.h>
#include <functions.h>
#include <fcntl.h>
extern struct Library *IconBase;

  if (argc == 0) { /* If started from Workbench get WbArgs */
    struct WBStartup *WBstart;
    char connam[100];
    struct FileHandle *handle;
    struct Process *proc = (struct Process *)FindTask(0);
    extern struct _dev *_devtab;
    struct DiskObject *existingIcon;

    if ((IconBase = OpenLibrary("icon.library", 0)) == NULL)
      syserr("Could not open 'icon.library'");
    WBstart = (struct WBStartup *)argv;
    advnam = prgnam = WBstart->sm_ArgList[0].wa_Name;
    if (WBstart->sm_NumArgs > 0) {
      CurrentDir(WBstart->sm_ArgList[1].wa_Lock);
      advnam = WBstart->sm_ArgList[1].wa_Name;
    }
    /* Possibly other tooltypes ... */
  } else {
    if ((prgnam = strrchr(argv[0], '/')) == NULL
	&& (prgnam = strrchr(argv[0], ':')) == NULL)
      prgnam = argv[0];
    else
      prgnam++;
    if (strcmp(prgnam, "arun") == 0)
      switches(argc, argv);
    else {
      /* Another program name use that as the name of the adventure */
      advnam = prgnam;
    }
  }
#else
#ifdef __dos__
  if ((prgnam = strrchr(argv[0], '\\')) == NULL
      && (prgnam = strrchr(argv[0], ':')) == NULL)
    prgnam = argv[0];
  else
    prgnam++;
  if (strlen(prgnam) > 4
      && (strcmp(&prgnam[strlen(prgnam)-4], ".EXE") == 0
	  || strcmp(&prgnam[strlen(prgnam)-4], ".exe") == 0))
    prgnam[strlen(prgnam)-4] = '\0';
  if (strcmp(prgnam, "arun") == 0 || strcmp(prgnam, "ARUN") == 0)
    switches(argc, argv);    
  else
    advnam = prgnam;
#else
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
  if (strcmp(prgnam, "arun") == 0)
    switches(argc, argv);
  else
    /* Another program name use that as the name of the adventure */
    advnam = prgnam;
#else
#if defined __unix__
  if ((prgnam = strrchr(argv[0], '/')) == NULL)
    prgnam = argv[0];
  else
    prgnam++;
  if (strrchr(prgnam, ';') != NULL)
    *strrchr(prgnam, ';') = '\0';
  if (strcmp(prgnam, "arun") == 0)
    switches(argc, argv);
  else
    /* Another program name use that as the name of the adventure */
    advnam = prgnam;
#else
  Unimplemented OS!
#endif
#endif
#endif
#endif
#endif
}
