/*----------------------------------------------------------------------*\

  ARUN.C

  Main program for interpreter for ALAN Adventure Language


\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "main.h"
#include "term.h"

#ifdef HAVE_SHORT_FILENAMES
#include "av.h"
#else
#include "alan.version.h"
#endif
#include "args.h"

#ifdef HAVE_GLK
#include "glkio.h"
#ifdef HAVE_WINGLK
#include "WinGlk.h"
#else
#include "glk.h"
#endif
#endif

#ifdef DMALLOC
#include "dmalloc.h"
#endif


/*----------------------------------------------------------------------*/
static char *gameName(char *fullPathName) {
  char *foundGameName;
  if ((foundGameName = strrchr(fullPathName, '\\')) == NULL
      && (foundGameName = strrchr(adventureFileName, '>')) == NULL
      && (foundGameName = strrchr(adventureFileName, ']')) == NULL
      && (foundGameName = strrchr(adventureFileName, '/')) == NULL
      && (foundGameName = strrchr(fullPathName, ':')) == NULL)
    foundGameName = strdup(fullPathName);
  else
    foundGameName = strdup(foundGameName+1);
  
  foundGameName[strlen(foundGameName)-4] = '\0'; /* Strip off .A3C */
  return foundGameName;
}


/*======================================================================

  main()

  Main program of main unit in Alan interpreter module, ARUN

  */

#ifdef HAVE_GLK
void glk_main(void)
#else
int main(
     int argc,
     char *argv[]
)
#endif
{
#ifdef MALLOC
  malloc_debug(2);
#endif
#ifdef DMALLOC
  /*
   * Get environ variable DMALLOC_OPTIONS and pass the settings string
   * on to dmalloc_debug_setup to setup the dmalloc debugging flags.
   */
  dmalloc_debug_setup(getenv("DMALLOC_OPTIONS"));
#endif

  /* Set up page format in case we get a system error */
  lin = col = 1;
  header->pageLength = 24;
  header->pageWidth = 70;

  getPageSize();

#ifdef HAVE_GLK
  /* args() is called from glkstart.c */
#else
  args(argc, argv);
#endif

  if ((debugOption && ! regressionTestOption) || verbose) {
    if (debugOption) printf("<");
    printf("Arun, Adventure Interpreter version %s (%s %s)",
	   alan.version.string, alan.date, alan.time);
    if (debugOption) printf(">");
    newline();
  }
  
  if (adventureFileName == NULL || strcmp(adventureFileName, "") == 0) {
#ifdef HAVE_WINGLK
    adventureFileName = (char*)winglk_get_initial_filename(NULL, "Arun : Select an Alan game file",
							   "Alan Game Files (*.a3c)|*.a3c||");
    if (adventureFileName == NULL) {
      printf("You should supply a game file to play.\n");
      usage();
      terminate(0);
    }
  }
#else
    printf("You should supply a game file to play.\n");
    usage();
    terminate(0);
  }
#endif

  adventureName = gameName(adventureFileName);

#ifdef HAVE_WINGLK
  winglk_app_set_name(adventureName);
  winglk_window_set_title(adventureName);
#endif


#ifdef TRYING_TO_SET_ICON_HAVE_WINGLK
  HWND mainWindow = FindWindow(NULL, adventureName);
  CloseWindow(mainWindow);
#endif

  run();

#ifdef HAVE_GLK
  return;
#else
  return(EXIT_SUCCESS);
#endif
}

