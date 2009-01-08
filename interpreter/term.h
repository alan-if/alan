#ifndef TERM_H
#define TERM_H
/*----------------------------------------------------------------------*\

  term.h

  Header file for terminal functions in ARUN, the Alan interpreter

\*----------------------------------------------------------------------*/

#ifdef HAVE_TERMIO

#ifdef __linux__
#include <sys/ioctl.h>
#include <asm/ioctls.h>
#endif

#ifdef __FreeBSD__
#include <sys/ioctl.h>
#endif

#endif /* HAVE_TERMIO */

extern void getPageSize(void);
extern void statusline(void);

#endif
