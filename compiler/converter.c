#include "converter.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "types.h"
#include "util.h"
#include "charset.h"
#include "srcp_x.h"

#ifdef UNITTESTING
#include <cgreen/mocks.h>

/* If we are compiling for unittest, replace read() with a Cgreen mock */
ssize_t mocked_read(int fd, void *buf, size_t nbytes) {
    return mock(fd, buf, nbytes);
}
#define read(fd, buf, nbytes) mocked_read(fd, buf, nbytes)

/* And we must be able to set errno */

int getErrno(void) {
    return mock();
}

#else

int getErrno(void) {
    return errno;
}

#endif

iconv_t initUtf8Conversion() {
    // This should be called when a new file is opened for reading
    iconv_t cd = iconv_open("ISO_8859-1", "UTF-8");
    if (cd == (iconv_t) -1) {
        SYSERR("iconv_open failed!", nulsrcp);
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

    if (iconv(cd, (char **)in_buf, &in_left, (char **)out_buf, &out_left)  == (size_t) -1) {
        /*
          switch(errno) {
          case 22 - EINVAL: printf("ERROR iconv: incomplete multibyte sequence\n"); break;
          case 84 - EILSEQ: printf("ERROR iconv: invalid multibyte sequence\n"); break;
          case  7 - E2BIG:  printf("ERROR iconv: output buffer full\n"); break;
          }
        */
        return -1;
    }

    return in_available-out_left;   /* = out_used */
}

/* This should be called when a file is processed and no more conversion will take place */
void finishUtf8Conversion(iconv_t cd) {
    iconv_close(cd);
}

/* Outgoing - will always return an alloc'd string that needs to be freed */
char *ensureExternalEncoding(char input[]) {
    if (charset == CHARSET_UTF8) {
        iconv_t cd = iconv_open("UTF-8", "ISO_8859-1");
        if (cd == (iconv_t) -1)
            SYSERR("iconv_open() failed!", nulsrcp);

        /* UTF-8 might be longer than ISO8859-1 encoding so double */
        char *in_p = input;
        size_t in_left = strlen(input);
        size_t out_left = strlen(input)*2;
        char *converted = (char *)malloc(out_left+1);
        char *out_p = converted;

        if (iconv(cd, &in_p, &in_left, &out_p, &out_left) == (size_t) -1) {
            /* Internal code (ISO8859-1) to UTF should never fail */
            SYSERR("Conversion of output to UTF-8 failed", nulsrcp);
        }
        *out_p = '\0';

        iconv_close(cd);
        return converted;
    } else
        return strdup(input);
}


/* Buffered reader with conversion from UTF-8 to internal, null-termination */
int readWithConversionFromUtf8(int fd, iconv_t cd, uchar *buffer, int wanted_length) {
    static uchar utfBuffer[1000];
    static int residueCount = 0; /* How much residue in utfBuffer from last conversion */
    static uchar residue[3];    /* Can max be 3 bytes since UTF-8 is max 4 */
    static uchar isoBuffer[1000];
    int toRead = wanted_length<(int)sizeof(utfBuffer)?wanted_length:sizeof(utfBuffer);
    int actuallyRead;

    /* Any residue from last round must now be copied to beginning of utfBuffer */
    if (residueCount > 0)
        memcpy(utfBuffer, residue, residueCount);

    actuallyRead = read(fd, (char *)&utfBuffer[residueCount], toRead-residueCount);
    if (actuallyRead == 0)
        return 0;               /* End of file */

    uchar *input_p = &utfBuffer[0];
    uchar *output_p = &isoBuffer[0];
    int convertedCount = convertUtf8ToInternal(cd, &input_p, &output_p, actuallyRead+residueCount);
    if (convertedCount == -1) {
        if (getErrno() == EINVAL) { /* Invalid! */
            /* Cut off in the middle of multi-byte, input_p points
               after successfully converted input and output_p points
               after successful output */
            convertedCount = output_p - &isoBuffer[0]; /* How many did we actually convert? */
            residueCount = toRead - (input_p - &utfBuffer[0]); /* How much residue? */
            memcpy(residue, input_p, residueCount);            /* Save it for next round */
        } else {
            /* Real error */
            SYSERR("error converting from UTF-8", nulsrcp);
        }
    } else
        residueCount = 0;

    /* "convertedCount" bytes of ISO encoded input is now in isoBuffer, copy it out */
    memcpy(buffer, isoBuffer, convertedCount);
    return convertedCount;
}
