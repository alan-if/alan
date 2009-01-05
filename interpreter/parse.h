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
} WordEntry;



/* Data: */

extern WordEntry playerWords[];	/* List of Parsed Word */
extern int wordIndex;		/* and an index into it */
extern int firstWord;
extern int lastWord;

extern ParamEntry *parameters;	/* List of parameters */

extern LiteralEntry literal[];

extern int verbWord;


/* Functions: */

/* Parse a new player command */
extern void forceNewPlayerInput();
extern void parse(ParamEntry parameters[]);
extern void initParse(void);
extern int literalFromInstance(Aint instance);
extern Aint instanceFromLiteral(int literalIndex);
extern void addParameterForInstance(ParamEntry *parameter, Aint instance);
extern void addParameterForInteger(ParamEntry *parameter, Aint value);
extern void addParameterForString(ParamEntry *parameter, char *value);
extern void restoreParameters();
extern char *playerWordsAsCommandString(void);

#endif
