#ifndef _WRD_X_H_
#define _WRD_X_H_
/*----------------------------------------------------------------------*\

				WRD_X.H
		     Dictionary Word Nodes Export

\*----------------------------------------------------------------------*/

#include "wrd.h"


/* USE: */
#include "../interpreter/acode.h"
#include "ins.h"


/* DATA: */

extern int words[];



/* FUNCTIONS: */

/* Find a Word in the dictonary */
extern Word *findWord(char str[]);

/* Insert a Word into the dictionary */
extern int newWord(char wrd[],
		  WrdKind class,
		  int code,
		  void *ref);
extern int newPronounWord(char *theWord, Instance *reference);

extern void prepareWords(void);
extern void analyzeWords(void);
extern Aaddr generateAllWords(void);


#endif
