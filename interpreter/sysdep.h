/*----------------------------------------------------------------------*\

  sysdep.h                              Date: 1995-08-19/thoni@softlab.se

  System dependencies file for Alan Adventure Language system 

\*----------------------------------------------------------------------*/
#ifndef _SYSDEP_H_
#define _SYSDEP_H_


/* Place definitions of OS and compiler here if necessary */
#ifdef AZTEC_C
#define __amiga__
#endif

#ifndef __sun__
#ifdef sun
#define __sun__
#endif
#endif

#ifdef _INCLUDE_HPUX_SOURCE
#define __hp__
#endif

#ifndef __unix__
#ifdef unix
#define __unix__
#endif
#endif

#ifdef vax
#define __vms__
#endif

#ifdef THINK_C
#define __mac__
#endif
#ifdef __MWERKS__
#define __mac__
#endif

#ifdef DOS
#define __dos__
#endif

#ifdef __BORLANDC__
#define __dos__
#endif

/*----------------------------------------------------------------------

  Below follows OS and compiler dependent settings. They should not be
  changed except for introducing new sections when porting to new
  environments.

 */

/* ISO character sets? */
#if defined __dos__ || defined __mac__
#define ISO 0
#else
#define ISO 1
#endif

/* General settings and includes, which work for all environments */
#include <stdio.h>
#include <ctype.h>

/* Strings for file modes */
#define READ_MODE "r"
#define WRITE_MODE "w"



/* Special cases and definition overrides */

#ifdef __STDC__
#define _PROTOTYPES_
#include <stdlib.h>
#include <string.h>
#endif

#ifdef __sun__
#endif

#ifdef __unix__
#define USE_READLINE
#define MULTI
#endif


#ifdef __vms__
/* Our VAXC doesn't define __STDC__ */
#define _PROTOTYPES_
#define REVERSED                /* Byte order in strings */
#include <stdlib.h>
#include <string.h>

#define MULTI

extern char *strdup(char str[]);

/* Cheat implementation of strftime */
extern size_t strftime (char *, size_t, const char *, const struct tm *);

#endif

#ifdef __mac__
#define _PROTOTYPES_
#include <stdlib.h>
#include <string.h>
#include <unix.h>

/* File open mode (binary) */
#undef READ_MODE
#define READ_MODE "rb"
#undef WRITE_MODE
#define WRITE_MODE "wb"

extern char *strdup(char *str);

#endif

#ifdef __dos__

/* File open mode (binary) */
#undef READ_MODE
#define READ_MODE "rb"
#undef WRITE_MODE
#define WRITE_MODE "wb"

/* Byte ordering */
#define REVERSED

/* Return codes */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE  1

#endif


#ifdef linux
#ifndef __linux__
#define __linux__
#define REVERSED
#endif
#endif


#ifdef _PROTOTYPES_

/* Native character functions */
extern int isSpace(int c);      /* IN - Native character to test */
extern int isLower(int c);      /* IN - Native character to test */
extern int isUpper(int c);      /* IN - Native character to test */
extern int isLetter(int c);     /* IN - Native character to test */
extern int toLower(int c);      /* IN - Native character to convert */
extern int toUpper(int c);      /* IN - Native character to convert */
extern char *strlow(char str[]); /* INOUT - Native string to convert */
extern char *strupp(char str[]); /* INOUT - Native string to convert */

/* ISO character functions */
extern int isISOLetter(int c);  /* IN - ISO character to test */
extern char toLowerCase(int c); /* IN - ISO character to convert */
extern char toUpperCase(int c); /* IN - ISO character to convert */
extern char *stringLower(char str[]); /* INOUT - ISO string to convert */
extern char *stringUpper(char str[]); /* INOUT - ISO string to convert */

/* ISO string conversion functions */
extern void toIso(char copy[],  /* OUT - Mapped string */
		  char original[]); /* IN - string to convert */
extern void fromIso(char copy[], /* OUT - Mapped string */
		    char original[]); /* IN - string to convert */
#else
extern int isSpace();
extern int isLower();
extern int isUpper();
extern int isLetter();
extern int toLower();
extern int toUpper();
extern char *strlow();
extern char *strupp();

extern int isISOLetter();
extern char toLowerCase();
extern char toUpperCase();
extern char *stringLower();
extern char *stringUpper();

extern void toIso();
extern void fromIso();
#endif

#endif                          /* -- sysdep.h -- */

