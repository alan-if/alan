#ifndef _OPT_H_
#define _OPT_H_
/*----------------------------------------------------------------------*\

				OPT.H
			   Option Handling

\*----------------------------------------------------------------------*/

/* USE: */
#include "types.h"
#include "acode.h"


/* Types: */

/* Options, their names, types and values */
#define NOPT 5

typedef enum OptKind {
  OPTLANG,
  OPTWIDTH,
  OPTLEN,
  OPTPACK,
  OPTDEBUG
} OptKind;

typedef enum OptTyp {
  INTOPT,
  ENUMOPT,
  BOOLOPT
} OptTyp;


/* The Option Definitions */
typedef struct OptDef {
  OptTyp type;
  Bool used;
  char *name;
  short value;
} OptDef;


/* Option values for LANGUAGE */
typedef enum LangKind {
  L_ENGLISH,
  L_SWEDISH
} LangKind;



/* Data: */

extern OptDef opts[NOPT];


/* Functions: */
#ifdef _PROTOTYPES_

/* Register an integer option */
extern void optint(char id[],
		   Srcp *srcp,
		   int val);

/* Register an enumerated option */
extern void optenum(char id[],
		    Srcp *srcp,
		    char val[]);

/* Register a Boolean option */
extern void optBool(char id[],
		       Srcp *srcp);


/* Generate all option values */
extern void geopt(AcdHdr *header);
#else
extern void optint();
extern void optenum();
extern void optBool();
extern void geopt();
#endif

#endif
