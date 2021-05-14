/*----------------------------------------------------------------------*\

  sysdep.h

  System dependencies file for Alan Adventure Language compiler

  N.B. The test for symbols used here should really be of three types
  - processor name (like PC, x86, ...)
  - os name (WIN32, Solaris2, ...)
  - compiler name and version (DJGPP, CYGWIN, GCC271, THINK-C, ...)

  The set symbols should indicate if a feature is on or off like the GNU
  AUTOCONFIG package does. This is not anywhere near completion yet!

\*----------------------------------------------------------------------*/
#ifndef _SYSDEP_H_
#define _SYSDEP_H_


/* Place definitions of OS and compiler here if necessary */
#ifndef __unix__
#ifdef unix
#define __unix__
#endif
#endif

#ifdef __APPLE__
#define __macosx__
#define __unix__
#endif

#ifdef __MINGW32__
#define __windows__
#endif

#ifdef __CYGWIN32__
#define __cygwin__
#endif


/*----------------------------------------------------------------------

  Below follows OS and compiler dependent settings. They should not be
  changed except for introducing new sections when porting to new
  environments.

 */

/************/
/* Includes */
/************/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#endif


#include "charset.h"


/**************/
/* File modes */
/**************/
#define READ_MODE "rb"
#define WRITE_MODE "wb"
#ifndef O_TEXT
#define O_TEXT 0
#endif


/****************/
/* Have termio? */
/****************/

#ifdef __cygwin__
#define HAVE_TERMIO
#endif

#ifdef __unix__
#define HAVE_TERMIO
#endif


/*******************************/
/* Is ANSI control available?  */
/*******************************/

#ifdef __cygwin__
#define HAVE_ANSI
#endif


/* Have times.h? */
#ifndef __MINGW32__
#define HAVE_TIMES_H
#endif

/* Special cases and definition overrides */

#ifdef __dos__

/* Return codes */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE  1

#endif


/* ISO character functions */
extern int compareStrings(char str1[], char str2[]); /* Case-insensitive compare */

/* ISO string conversion functions */
extern void toIso(char copy[],      /* OUT - Mapped string */
                  char original[],  /* IN - string to convert */
                  CharSet charset); /* IN - The current character set */

extern int littleEndian(void);

extern char *baseNameStart(char *fullPathName);

#endif                          /* -- sysdep.h -- */
