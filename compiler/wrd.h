#ifndef _WRD_H_
#define _WRD_H_
/*----------------------------------------------------------------------*\

				WRD.H
			Dictionary Word Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "acode.h"
#include "lst.h"
#include "nam.h"


/* Types: */

typedef struct WrdNod {		/* DICTIONARY ENTRY */
  int class;			/* Class of this entry as a bit in the set */
  int code;			/* Code for the word */
  char *str;			/* Name of this entry */
  List *ref;			/* List of references (objects etc) */
  Aaddr stradr;			/* ACODE address to string */
  Aaddr refadr;			/* ACODE address to reference table */
  struct WrdNod *low, *high;	/* Links */
} WrdNod;


/* Data: */

extern int words[];



/* Functions: */
#ifdef _PROTOTYPES_

/* Find a Word in the dictonary */
extern WrdNod *findwrd(char str[]);

/* Insert a Word into the dictionary */
extern int newwrd(char wrd[],
		  WrdKind class,
		  int code,
		  NamNod *nam);

/* Prepare Words in dictionary */
extern void prepwrds(void);

/* Generate all Words in the dictionary */
extern Aaddr gewrds(void);

#else
extern WrdNod *findwrd();
extern int newwrd();
extern void prepwrds();
extern Aaddr gewrds();
#endif

#endif
