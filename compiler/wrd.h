#ifndef _WRD_H_
#define _WRD_H_
/*----------------------------------------------------------------------*\

				WRD.H
			Dictionary Word Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "acode.h"
#include "lst.h"
#include "id.h"


/* Types: */

typedef struct WrdNod {		/* DICTIONARY ENTRY */
  int classbits;		/* Class of this entry as a bit in the set */
  int code;			/* Code for the word */
  char *str;			/* Name of this entry */
  List *ref[WRD_CLASSES];	/* Lists of references (objects etc) */
  Aaddr stradr;			/* ACODE address to string */
  Aaddr nounrefadr;		/* ACODE address to reference table */
  Aaddr adjrefadr;		/* ACODE address to reference table */
  struct WrdNod *low, *high;	/* Links */
} WrdNod;


/* Data: */

extern int words[];



/* Functions: */

/* Find a Word in the dictonary */
extern WrdNod *findwrd(char str[]);

/* Insert a Word into the dictionary */
extern int newwrd(char wrd[],
		  WrdKind class,
		  int code,
		  void *id);

/* Prepare Words in dictionary */
extern void prepwrds(void);

/* Analyse the Words in dictionary */
extern void anwrds(void);

/* Generate all Words in the dictionary */
extern Aaddr gewrds(void);


#endif
