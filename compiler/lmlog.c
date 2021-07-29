#include "lmlog.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>


/* Use: */
#include "lmList.h"
#include "util.h"
#include "srcp_x.h"
#include "converter.h"


void lmlog(Srcp *pos, int ecode, lmSev sev, char *insert_strings) {
    /* TODO: SYSERR if insert_strings contains lmSEPARATOR, should only be used if ONE insert string */
    char *recoded = ensureExternalEncoding(insert_strings);
    lmLog(pos, ecode, sev, recoded);
    free(recoded);
}


void lmlogv(Srcp *pos, int ecode, lmSev sev, ...) {
    va_list ap;
    int allocated = 1;
    char *insertstrings = (char *)malloc(allocated);
    int len = 0;
    char *arg;

    va_start(ap, sev);          /* sev is last non-vararg */
    insertstrings[0] = '\0';
    while ((arg = va_arg(ap, char *))) {
        if (len > 0) {
            insertstrings[len] = lmSEPARATOR;
            insertstrings[len+1] = '\0';
        }
        char *argument = ensureExternalEncoding(arg);
        if (len + strlen(argument)+2 >= allocated) {
            int extension = strlen(argument)+2; /* lmSeparator + null */
            allocated += extension;
            insertstrings = (char *)realloc(insertstrings, allocated);
            if (!insertstrings) {
                SYSERR("Out of memory!", nulsrcp);
                return;
            }
        }
        strcat(insertstrings, argument);
        len = strlen(insertstrings);
        free(argument);
    }

    va_end(ap);

    lmLog(pos, ecode, sev, insertstrings);
    free(insertstrings);
}
