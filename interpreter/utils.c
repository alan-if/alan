#include "utils.h"


/* Imports: */
#include <stdio.h>
#include "alan.version.h"
#include "main.h"
#include "options.h"
#include "args.h"


/*======================================================================

  terminate()

  Terminate the execution of the adventure, e.g. close windows,
  return buffers...

 */
void terminate(int code)
{
#ifdef __amiga__
#ifdef AZTEC_C
#include <fcntl.h>
  extern struct _dev *_devtab;
  char buf[85];

  if (con) { /* Running from WB, created a console so kill it */
    /* Running from WB, so we created a console and
       hacked the Aztec C device table to use it for all I/O
       so now we need to make it close it (once!) */
    _devtab[1].fd = _devtab[2].fd = 0;
  } else
#else
  /* Geek Gadgets GCC */
#include <workbench/startup.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>

  if (_WBenchMsg != NULL) {
    Close(window);
    if (_WBenchMsg->sm_ArgList != NULL)
      UnLock(CurrentDir(cd));
  } else
#endif
#endif
    newline();
  if (memory)
      free(memory);
  if (transcriptOption|| logOption)
#ifdef HAVE_GLK
    glk_stream_close(logFile, NULL);
#else
    fclose(logFile);
#endif

#ifdef __MWERKS__
  printf("Command-Q to close window.");
#endif

#ifdef HAVE_GLK
  glk_exit();
#else
  exit(code);
#endif
}

/*======================================================================*/
void usage(void)
{
  printf("\nArun, Adventure Interpreter version %s (%s %s)\n\n",
     alan.version.string, alan.date, alan.time);
  printf("Usage:\n\n");
  printf("    %s [<switches>] <adventure>\n\n", PROGNAME);
  printf("where the possible optional switches are:\n");
#ifdef HAVE_GLK
  glk_set_style(style_Preformatted);
#endif
  printf("    -v       verbose mode\n");
  printf("    -l       log transcript to a file\n");
  printf("    -c       log player commands to a file\n");
  printf("    -n       no Status Line\n");
  printf("    -d       enter debug mode\n");
  printf("    -t[<n>]  trace game execution, higher <n> gives more trace\n");
  printf("    -i       ignore version and checksum errors\n");
  printf("    -r       refrain from printing timestamps and paging (making regression testing easier)\n");
#ifdef HAVE_GLK
  glk_set_style(style_Normal);
#endif
}


