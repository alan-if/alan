#ifndef CHARSET_H
#define CHARSET_H
/*----------------------------------------------------------------------* \

  charset.h

\*----------------------------------------------------------------------*/

/* The enum values here must match the index of option values for charset in main.c */
typedef enum {
    CHARSET_ISO = 0,
    CHARSET_DOS = 1,
    CHARSET_UTF8 = 2
} CharSet;

extern CharSet charset;			/* Which charset to read source in */

#endif
