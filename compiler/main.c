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



/*======================================================================

   SPA Option handling 

*/

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
     SPA_FLAG("full", "full source in the list file", fulflg, FALSE, NULL)
     SPA_INTEGER("height <lines>", "height of pages in list file", lcount, 74, NULL)
     SPA_INTEGER("width <characters>", "width of pages in list file", ccount, 112, NULL)
     SPA_FLAG("listing", "create listing file", lstflg, FALSE, NULL)
     SPA_FLAG("debug", "force debug option in adventure", dbgflg, FALSE, NULL)
     SPA_FLAG("pack", "force pack option in adventure", packflg, FALSE, NULL)
     SPA_FLAG("summary", "print a summary", sumflg, FALSE, NULL)
#ifndef THINK_C
     SPA_BITS("dump", "dump the internal form, where\n\
synonyms\n\
syntax\n\
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
after code generation", dmpflg, "yxsvciker!123", NULL, NULL)
#endif
/*     SPA_FLAG("prettyprint", "pretty print the adventure", ppflg, FALSE, NULL) */
SPA_END

/*------------ main --------------------------------------------*/

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
