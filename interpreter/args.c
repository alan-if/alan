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
  argc = ccommand(&argv);
  if (prgnam = strrchr(argv[0], ':') == NULL)
    prgnam = argv[0];
  else
    prgnam++;
    if (strcmp(prgnam, "arun") == 0)
      switches(argc, argv);
    else
      /* Another program name use that as the name of the adventure */
      advnam = prgnam;
#else
#ifdef __amiga__
#include <workbench/startup.h>
#include <intuition/intuition.h>
#include <functions.h>
#include <fcntl.h>

  if (argc == 0) { /* If started from Workbench get WbArgs */
    struct WBStartup *WBstart;
    char connam[100];
    struct FileHandle *handle;
    struct Process *proc = (struct Process *)FindTask(0);
    extern struct _dev *_devtab;

    /* Create a console */
    strcpy(connam, "newcon:10/10/480/160/");
    strcat(connam, advnam);

    con = Open(connam, MODE_NEWFILE);
    handle = (struct FileHandle *)((long)con << 2);
    proc->pr_ConsoleTask = (APTR)(handle->fh_Type);
    _devtab[0].fd = _devtab[1].fd = _devtab[2].fd = (long)con;

    WBstart = (struct WBStartup *)argv;
    advnam = prgnam = WBstart->sm_ArgList[0].wa_Name;
    if (WBstart->sm_NumArgs > 0) {
      CurrentDir(WBstart->sm_ArgList[1].wa_Lock);
      advnam = WBstart->sm_ArgList[1].wa_Name;
    }
    /* 4f_ni - Get ToolTypes! */
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
  if ((prgnam = strrchr(argv[0], ':')) == NULL
      && (prgnam = strrchr(argv[0], '\\')) == NULL)
    prgnam = argv[0];
  else
    prgnam++;
  if (strlen(prgnam) > 4
      && (strcmp(&prgnam[strlen(prgnam)-4], ".EXE") == 0
	  || strcmp(&prgnam[strlen(prgnam)-4], ".exe") == 0))
    prgnam[strlen(prgnam)-4] = '\0';
  if (strcmp(prgnam, "arun") == 0)
    switches(argc, argv);    
  else
    advnam = prgnam;
#else
#if defined __vms__
  if ((prgnam = strrchr(argv[0], ':')) == NULL
      && (prgnam = strrchr(argv[0], '>')) == NULL
      && (prgnam = strrchr(argv[0], ']')) == NULL)
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
    strcpy(advnam, prgnam);
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
    strcpy(advnam, prgnam);
#else
  Unimplemented OS!
#endif
#endif
#endif
#endif
#endif
}
