/*----------------------------------------------------------------------*\

  main.c

  Alan compiler main program unit - command-line version

\*----------------------------------------------------------------------*/

#include "util.h"
#include "alan.h"
#include "spa.h"
#include "options.h"
#include "alan.version.h"
#include "lst_x.h"



/*======================================================================*/

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
  includePaths = concat(includePaths, spaArgument(1), STRING_LIST);
  /* Now we can skip the include path */
  spaSkip(1);
}

static char *charsets[] = {"iso", "mac", "dos"};


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
     SPA_KEYWORD("charset <set>", "which character set source is in (iso|mac|dos)", charset, charsets, 0, NULL)
     SPA_FLAG("cc", "show messages on the screen in old 'cc' format\n", ccflg, FALSE, NULL)
     SPA_FLAG("full", "full source in the list file (or on screen)", fulflg, FALSE, NULL)
     SPA_INTEGER("height <lines>", "height of pages in list file", lcount, 74, NULL)
     SPA_INTEGER("width <characters>", "width of pages in list file", ccount, 112, NULL)
     SPA_FLAG("listing", "create listing file", lstflg, FALSE, NULL)
     SPA_FLAG("debug", "force debug option in adventure", debugOption, FALSE, NULL)
     SPA_FLAG("pack", "force pack option in adventure", packflg, FALSE, NULL)
     SPA_FLAG("summary", "print a summary", sumflg, FALSE, NULL)
#ifndef THINK_C
     SPA_BITS("dump", "dump the internal form, where\n\
synonyms\n\
syntaxes\n\
symbols\n\
verbs\n\
classes\n\
instances\n\
containers\n\
events\n\
rules\n\
everything\n\
after parse (will abort after dump)\n\
after analysis (will abort after dump)\n\
after code generation", dmpflg, "ypxsvciker!123", NULL, NULL)
#endif
/*     SPA_FLAG("prettyprint", "pretty print the adventure", ppflg, FALSE, NULL) */
SPA_END

/*======================================================================*\
  MAIN
\*======================================================================*/
#ifdef WINGUI
#include <windows.h>

#define FILENAMESIZE 1000
static char inFileName[FILENAMESIZE];
static char fullInFileName[FILENAMESIZE];
static char outFileName[FILENAMESIZE];
static char fullOutFileName[FILENAMESIZE];

static OPENFILENAME ofn;

static int getInFileName() {
  static char filter[] =
    "Alan Source Files (*.alan)\0*.alan\0"\
    "ALA Source Files (*.ala)\0*.ala\0"\
    "All Files (*.*)\0*.*\0\0";

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hInstance = NULL;
  ofn.lpstrFilter = filter;
  ofn.lpstrCustomFilter = NULL;
  ofn.lpstrFile = fullInFileName;
  ofn.lpstrTitle = "Choose an Alan V3 source file to compile";
  ofn.nMaxFile = FILENAMESIZE;
  ofn.lpstrFileTitle = inFileName;
  ofn.nMaxFileTitle = FILENAMESIZE;
  ofn.Flags = OFN_HIDEREADONLY;
  return GetOpenFileName(&ofn);
}

static int getOutFileName() {
  ofn.lpstrFile = fullOutFileName;
  ofn.lpstrFileTitle = outFileName;
  ofn.lpstrTitle = "Choose a file to store Alan V3 source output in";
  ofn.Flags = OFN_OVERWRITEPROMPT;
  return GetSaveFileName(&ofn);
}

static int argc;
static char *argv[10];

static int splitCommandLine(char commandLine[])
{
  char *start = commandLine;
  char *end = strpbrk(commandLine, " \"");

  argv[0] = "alan";
  argc = 1;
  while (end) {
    argv[argc] = start;
    if (*end == '\"') {
      end = strpbrk(start+1, "\"");
      end++;
    }
    *end = '\0';
    start = end+1;
    while (*start == ' ') start++;
    end = strpbrk(start, " ");
    argc++;
  }
  if (*start != '\0') {
    argv[argc] = start;
    argc++;
  }
  return argc;
}

static char *removeExeResidue(char cmdLine[])
{
  /* MingW seems to forget to strip of the whole program name if it
     contains spaces, Windows surrounds those with quote-marks so any
     residue will end in: */
  static char *residue = ".exe\"";
  char *cp = strstr(cmdLine, residue);
  if (cp) {
    cp += strlen(residue);
    while (*cp == ' ') cp++;
  } else
    cp = cmdLine;
  return cp;
}


static void remapWindowsFilename(char string[])
{
  int i;

  if (string[0] == '"') {
    strcpy(string, &string[1]);
    string[strlen(string)-1] = '\0';
  }

#ifdef REMAPSLASH
  for (i = 0; string[i] != '\0'; i++)
    if (string[i] == '\\')
      string[i] = '/';
#endif
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
  int nArgs;
  int i;

  nArgs = splitCommandLine(removeExeResidue(cmdLine));

#ifdef ARGSDISPLAY
  MessageBox(NULL, "Hello!", "Alan V3 compiler", MB_OK);
  for (i = 0; i < nArgs; i++) {
    char buf[199];
    sprintf(buf, "arg %d :\"%s\"", i, argv[i]);
    MessageBox(NULL, buf, "Alan V3 compiler", MB_OK);
  }
#endif

  guiMode = TRUE;
  if (nArgs == 1) {
    if (!getInFileName())
      return -1;
    argv[1] = fullInFileName;
    argc = 2;
  }
  else
    /* If we run from a CMD windows we will see Windows-style filenames */
    remapWindowsFilename(argv[1]);

#ifdef ARGSDISPLAY
  MessageBox(NULL, argv[1], "Alan V3 compiler : argv[1]", MB_OK);
  MessageBox(NULL, fullInFileName, "Alan V3 compiler : fullInFileName", MB_OK);
  MessageBox(NULL, inFileName, "Alan V3 compiler : inFileName", MB_OK);
  MessageBox(NULL, fopen(argv[1], "r")!=NULL?"OK":"Not Ok", "Alan V3 compiler : open argv[1]", MB_OK);
#endif


  if (!AllocConsole()) {
    MessageBox(NULL, "Failed to allocate a console.\nCompilation will continue but can not display error messages.", "Error", MB_OK);
  } else
    freopen("con:", "w", stdout);

#else

int main(int argc,		/* IN - argument count */
	 char **argv		/* IN - program arguments */
)
{
  int nArgs;			/* Number of supplied args */

#ifdef __MWERKS__
#include <SIOUX.h>
  SIOUXSettings.setupmenus = FALSE;
  SIOUXSettings.asktosaveonclose = FALSE;
  SIOUXSettings.showstatusline = FALSE;
#endif

#endif

  /* -- get arguments -- */
  nArgs = spaProcess(argc, argv, arguments, options, paramError);
  /* Say hello ! */
  if (verbose)
    printf("%s\n\n", alan.longHeader);

  if (nArgs == 0) {
    usage(NULL, NULL, 0);
    terminate(EXIT_FAILURE);
  } else if (nArgs > 1)
    terminate(EXIT_FAILURE);
  
  compile();
  return(0);
}

