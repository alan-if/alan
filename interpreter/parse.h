/*----------------------------------------------------------------------*\

  PARSE.H

  Parse data for ALAN interpreter module.

\*----------------------------------------------------------------------*/

extern int wrds[];		/* List of Parsed Word */
extern int wrdidx;		/* and an index into it */

extern ParamEntry *params;	/* List of parameters */
extern Bool plural;

extern LiteralEntry literal[];
extern int litCount;

extern int verbWord;


/* Functions: */

/* Parse a new player command */
extern void parse(void);
extern void initParse(void);

