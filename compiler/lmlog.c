#include "lmlog.h"

#include <stdarg.h>


/* Use: */
#include "lmList.h"


void lmlog(Srcp *pos, int ecode, lmSev sev, char *insert_strings) {
    lmLog(pos, ecode, sev, insert_strings);
}


void lmlogv(Srcp *pos, int ecode, lmSev sev, ...) {
    lmLog(pos, ecode, sev, "");
}
