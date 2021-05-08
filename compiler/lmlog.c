#include "lmlog.h"

#include <stdarg.h>
#include <string.h>
#include <malloc.h>


/* Use: */
#include "lmList.h"
#include "util.h"
#include "srcp_x.h"


void lmlog(Srcp *pos, int ecode, lmSev sev, char *insert_strings) {
    lmLog(pos, ecode, sev, insert_strings);
}


void lmlogv(Srcp *pos, int ecode, lmSev sev, ...) {
    va_list ap;
    char *errorstring = (char *)malloc(256);
    int curlen = 256;
    int len = 0;
    char *tmp, *arg;

    va_start(ap, sev);          /* sev is last non-vararg */
    errorstring[0] = '\0';
    while ((arg = va_arg(ap, char *))) {
        if (len > 0) {
            errorstring[len] = lmSEPARATOR;
            errorstring[len+1] = '\0';
        }
        if (len + strlen(arg) >= curlen) {
            tmp = errorstring;
            curlen += 256;
            errorstring = (char *)malloc(curlen);
            /* Check errorstring */
            if (!errorstring) {
                SYSERR("Out of memory!", nulsrcp);
                return;
            }
            strcpy(errorstring, tmp);
            free(tmp);
        }
        strcat(errorstring, arg);
        len = strlen(errorstring);

    }

    va_end(ap);

    lmLog(pos, ecode, sev, errorstring);
    free(errorstring);
}
