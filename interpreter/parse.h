#ifndef _PARSE_H_
#define _PARSE_H_
/*----------------------------------------------------------------------*\

  PARSE.H

  Parse data for ALAN interpreter module.

\*----------------------------------------------------------------------*/

#include "types.h"


/* Types: */

typedef struct WordEntry {
  int code;			/* The dictionary index for that word */
  char *start;			/* Where does it start */
  char *end;			/* .. and end */
} Word;



/* Data: */

extern Word playerWords[];	/* List of Parsed Word */
extern int wordIndex;		/* and an index into it */
extern int firstWord;
extern int lastWord;

extern Parameter *parameters;	/* List of parameters */

extern LiteralEntry literal[];

extern int verbWord;


/* Functions: */

/* Parse a new player command */
extern void forceNewPlayerInput();
extern void parse(Parameter parameters[]);
extern void initParse(void);
extern int literalFromInstance(Aint instance);
extern Aint instanceFromLiteral(int literalIndex);
extern void addParameterForInstance(Parameter *parameter, Aint instance);
extern void addParameterForInteger(Parameter *parameter, Aint value);
extern void addParameterForString(Parameter *parameter, char *value);
extern void restoreParameters();
extern char *playerWordsAsCommandString(void);

#endif
