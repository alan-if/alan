#ifndef _WORD_H_
#define _WORD_H_
/*----------------------------------------------------------------------*\

				Word.h

			Dictionary Word Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "List.h"
#include "Id.h"

#include "acode.h"


/* Types: */

/* WordKind is imported from acode.h because of required compatability */


typedef struct Word {		/* DICTIONARY ENTRY */
  int code;			/* 4f - temporary implementation */
  struct Word *lower, *higher;	/* Links */
} Word;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Insert a Word into the dictionary */
extern int newWord(char word[],
		  WordKind kind,
		  void *references);

/* Generate the dictionary */
extern Aaddr generateWords(void);

#else
extern int newWord();
extern Aaddr generateWords();
#endif

#endif
