/*----------------------------------------------------------------------*\

  PARSE.H

  Parse data for ALAN interpreter module.

\*----------------------------------------------------------------------*/

extern int playerWords[];	/* List of Parsed Word */
extern int wordIndex;		/* and an index into it */

extern ParamEntry *parameters;	/* List of parameters */
extern Bool plural;

extern LiteralEntry literal[];
extern int litCount;

extern int verbWord;


/* Functions: */

/* Parse a new player command */
extern void parse(void);
extern void initParse(void);
extern int literalFromInstance(Aint instance);
extern Aint instanceFromLiteral(int literalIndex);
extern void setupParameterForInstance(int parameter, Aint instance);
extern void setupParameterForInteger(int parameter, Aint value);
extern void setupParameterForString(int parameter, char *value);
extern void restoreParameters();
