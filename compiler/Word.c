/*----------------------------------------------------------------------*\

				Word.c

		     Word Nodes (the dictionary)

\*----------------------------------------------------------------------*/


#include "types.h"

#include "Word.h"

#include "dump.h"




/*======================================================================

  newWord()

  Allocates and initialises a Word.

 */
#ifdef _PROTOTYPES_
void newWord(char *string,	/* IN - The word to insert */
	     WordKind kind,	/* IN - Kind of word */
	     void *reference)	/* IN - The new reference */
#else
void newWord(string, kind, reference)
     char *string;
     WordKind kind;
     void *reference;
#endif
{
  /* 4f - Dummy implementation of the dictionary */
}
