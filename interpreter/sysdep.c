/*----------------------------------------------------------------------*\
  sysdep.c

  This file contains necessary system dependent routines and routines
  possibbly not implemented in the current environment.

  strdup()

  realloc()
  strlow()
  strupp()
  memcpy()
  memset()

\*----------------------------------------------------------------------*/

#include <time.h>
#include "sysdep.h"


#ifdef __vms__

char *strdup(char str[])		/* IN - String to duplicate */
{
  char *new = (char *) malloc(strlen(str)+1);

  strcpy(new, str);
  return new;
}

#endif


#ifdef AZTEC_C

char *strdup(str)
     char str[];			/* IN - String to duplicate */
{
  char *new = (char *) malloc(strlen(str)+1);

  strcpy(new, str);
  return new;
}


void memcpy(out, in, length)
    char *out;
    char *in;
    int length;
{
  while(length--)
    *out++ = *in++;
}


void memset(mem, val, length)
    char *mem;
    char val;
    int length;
{
  while(length--)
    *mem++ = val;
}

void *realloc(old, newLength)
     void *old;			/* IN - address to the old area */
     int newLength;		/* IN - new length */
{
  void *new;

  new = malloc(newLength);
  memcpy(new, old, newLength);
  free(old);
  return new;
}

#endif

#ifdef __vms__

/* Cheat implementation of strftime */
size_t strftime (
		 char *outbuf,
		 size_t len,
		 const char *format,
		 const struct tm *t)
{
  char buf[100];
  time_t ticks;

  time(&ticks);
  strcpy(buf, ctime(&ticks));
  buf[strlen(buf)-1] = '\0';
  strcpy(outbuf, &buf[4]);
}

#endif

#ifdef __dos__

/* Cheat implementation of strftime */
size_t strftime (
		 char *outbuf,
		 size_t len,
		 const char *format,
		 const struct tm *t)
{
  char buf[100];
  time_t ticks;

  time(&ticks);
  strcpy(buf, ctime(&ticks));
  buf[strlen(buf)-1] = '\0';
  strcpy(outbuf, &buf[4]);
}

#endif


#ifdef _PROTOTYPES_
int isSpace(int c)		/* IN - character to test */
#else
int isSpace(c)
     int c;			/* IN - character to test */
#endif
{
  static char spcChrs[] = " \t\n";

  return (c != '\0' && strchr(spcChrs, c) != 0);
}


#ifdef _PROTOTYPES_
int isLower(int c)			/* IN - character to test */
#else
int isLower(c)
     int c;			/* IN - character to test */
#endif
{
  static char lowChrs[] = "abcdefghijklmnopqrstuvwxyz|{}\340\341\342\343\344\345\346\347\360\351\352\353\354\355\356\357\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377";

  return (strchr(lowChrs, c) != 0);
}


#ifdef _PROTOTYPES_
int isUpper(int c)		/* IN - character to test */
#else
int isUpper(c)
     int c;			/* IN - character to test */
#endif
{
  static char uppChrs[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ][\\\300\301\302\303\304\305\306\307\300\311\312\313\314\315\316\317\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337";

  return (strchr(uppChrs, c) != 0);
}


#ifdef _PROTOTYPES_
char lowerCase(int c)
#else
char lowerCase(c)
     int c;
#endif
{
  return (isUpper(c)? c + ('a' - 'A'): c);
}


#ifdef _PROTOTYPES_
char upperCase(int c)
#else
char upperCase(c)
     int c;
#endif
{
  return (isLower(c)? c - ('a' - 'A'): c);
}


#ifdef _PROTOTYPES_
int isLetter(int c)		/* IN - character to test */
#else
int isLetter(c)
     int c;			/* IN - character to test */
#endif
{
  return(isLower(c)? !0: isUpper(c));
}


#ifdef _PROTOTYPES_
char *strlow(char str[])	/* INOUT - string to convert */
#else
char *strlow(str)
     char str[];		/* INOUT - string to convert */
#endif
{
  char *s;

  for (s = str; *s; s++)
    *s = lowerCase(*s);
  return(str);
}


#ifdef _PROTOTYPES_
char *strupp(char str[])	/* INOUT - string to convert */
#else
char *strupp(str)
     char str[];		/* INOUT - string to convert */
#endif
{
  char *s;

  for (s = str; *s; s++)
    *s = upperCase(*s);
  return(str);
}
