/*----------------------------------------------------------------------*\

	word

\*----------------------------------------------------------------------*/
#include "word.h"

/* IMPORTS */
#include "types.h"
#include "memory.h"


/* CONSTANTS */


/* PUBLIC DATA */

/* List of parsed words, index into dictionary */
Word playerWords[100] = { { EOF, NULL, NULL}};
int wordIndex; /* An index into it the list of playerWords */
int firstWord, lastWord;  /* Index for the first and last words for this command */

/* What did the user say? */
int verbWord; /* The word he used as a verb, dictionary index */
int verbWordCode; /* The code for that verb */


/* PRIVATE TYPES & DATA */


/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*======================================================================*/
char *playerWordsAsCommandString(void) {
    char *commandString;
    int size = playerWords[lastWord].end - playerWords[firstWord].start;
    commandString = allocate(size + 1);
    strncpy(commandString, playerWords[firstWord].start, size);
    commandString[size] = '\0';
    return commandString;
}
