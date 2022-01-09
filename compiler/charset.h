#ifndef CHARSET_H
#define CHARSET_H
/*----------------------------------------------------------------------* \

  charset.h

\*----------------------------------------------------------------------*/

/* The enum values here must match the index of option values for charset in main.c */
typedef enum {
    CHARSET_ISO = 0,
    CHARSET_UTF8 = 1
} CharSet;

extern CharSet input_encoding;			/* Which charset to read source in */

#endif
