#ifndef _CONVERTER_H_
#define _CONVERTER_H_

/* Converts a string in one encoding to internal ISO8859-1 encoding */

/* Cross-building sets iconv args to const using WINICONV_CONST, so
 * nullify that to remove warnings... */
#define WINICONV_CONST
#include <iconv.h>

#include "types.h"


/* USE other definitions */

/* TYPES: */

/* FUNCTIONS */
extern iconv_t initUtf8Conversion(void);
extern int convertUtf8ToInternal(iconv_t cd, uchar **in_buf, uchar **out_buf, int available);
extern void finishUtf8Conversion(iconv_t cd);
extern char *ensureExternalEncoding(char input[]);
extern int readWithConversionFromUtf8(int fd, iconv_t cd, uchar *smBuffer, int toRead);

#endif
