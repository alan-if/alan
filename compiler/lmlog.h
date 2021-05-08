#ifndef _LMLOG_H_
#define _LMLOG_H_
/*----------------------------------------------------------------------* \

  lmlog

  An adapter for lmList's lmLog() and lmLogv() to ensure encoding in
  messages are in accordance with the external encoding. Often converting
  to UTF-8.

\*----------------------------------------------------------------------*/

/* Use: */
#include "srcp.h"

typedef int lmSev;


/* Log a message at a source position */
extern void lmlog(Srcp *pos, int ecode, lmSev sev, char *istrs);
/* Log a message at a source position using va_arg handling */
extern void lmlogv(Srcp *pos, int ecode, lmSev sev, ...);


#endif
