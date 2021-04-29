#include "converter.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "types.h"
#include "util.h"


iconv_t initUtf8Conversion() {
    // This should be called when a new file is opened for reading
    iconv_t cd = iconv_open("ISO_8859-1", "UTF-8");
    if (cd == (iconv_t) -1) {
        SYSERR("iconv_open failed!", ((Srcp){0,0,0}));
        return (iconv_t)-1;
    } else
        return cd;
}

int convertUtf8ToInternal(iconv_t cd, uchar **in_buf, uchar **out_buf, int in_available) {
    // Adapted from https://stackoverflow.com/a/11156490/204658
    // Changed to work on non-terminated strings so it can be used on the fly while
    // reading chunks of a large file.
    // Input is not expected to be null-terminated, and output will not be.

    // NOTE: It is assumed that the available output space is at least as large as available input.
    size_t in_left = in_available;
    size_t out_left = in_available;

    do {
        size_t rc = iconv(cd, (char **)in_buf, &in_left, (char **)out_buf, &out_left);
        if (rc  == (size_t) -1) {
            /*
            switch(errno) {
            case EINVAL: printf("ERROR iconv: incomplete multibyte sequence\n"); break;
            case EILSEQ: printf("ERROR iconv: invalid multibyte sequence\n"); break;
            case E2BIG:  printf("ERROR iconv: output buffer full\n"); break;
            }
            */
            return -1;
        }
    } while (in_left > 0 && out_left > 0);

    return in_available-out_left;   /* = out_used */
}

/* This should be called when a file is processed and no more conversion will take place */
void finishUtf8Conversion(iconv_t cd) {
    iconv_close(cd);
}
