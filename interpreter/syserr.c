#include "main.h"

/*======================================================================*/
void syserr(char *str)
{
  output("$n$nAs you enter the twilight zone of Adventures, you stumble \
and fall to your knees. In front of you, you can vaguely see the outlines \
of an Adventure that never was.$n$nSYSTEM ERROR: ");
  output(str);
  output("$n$n");

  if (logOption)
    fclose(logfil);
  newline();

#ifdef __amiga__
#ifdef AZTEC_C
  {
    char buf[80];

    if (con) { /* Running from WB, wait for user ack. */
      printf("press RETURN to quit");
      gets(buf);
    }
  }
#endif
#endif

  terminate(0);
}