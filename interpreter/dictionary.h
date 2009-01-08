#ifndef DICTIONARY_H_
#define DICTIONARY_H_
/*----------------------------------------------------------------------*\

  Dictionary of player words in Alan interpreter

\*----------------------------------------------------------------------*/

/* Imports: */
#include "acode.h"
#include "types.h"

/* Constants: */


/* Types: */


/* Data: */
extern DictionaryEntry *dictionary; /* Dictionary pointer */
extern int dictionarySize;      /* Number of entries in dictionary */


/* Functions: */
extern Bool isVerbWord(int wordIndex);
extern Bool isConjunctionWord(int wordIndex);
extern Bool isButWord(int wordIndex);
extern Bool isThemWord(int wordIndex);
extern Bool isItWord(int wordIndex);
extern Bool isNounWord(int wordIndex);
extern Bool isAdjectiveWord(int wordIndex);
extern Bool isPrepositionWord(int wordIndex);
extern Bool isAllWord(int wordIndex);
extern Bool isDirectionWord(int wordIndex);
extern Bool isPronounWord(int wordIndex);
extern Bool isLiteralWord(int wordIndex);

extern Bool isConjunction(int wordCode);
extern Bool isAll(int wordCode);
extern Bool isNoise(int wordCode);
extern Bool isPronoun(int wordCode);

#endif /* DICTIONARY_H_ */
