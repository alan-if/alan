#ifndef _OPTION_H_
#define _OPTION_H_
/*----------------------------------------------------------------------*\

				OPTION.H

			   Option Handling

\*----------------------------------------------------------------------*/

/* USE: */
#include "types.h"
#include "Id.h"
#include "acode.h"


/* Types: */

/* Options, their names, types and values */
#define NOPTIONS 5

typedef enum OptionKind {
  LANGUAGE_OPTION,
  WIDTH_OPTION,
  LENGTH_OPTION,
  PACK_OPTION,
  DEBUG_OPTION
} OptionKind;

typedef enum OptionType {
  INTOPTION,
  ENUMOPTION,
  BOOLOPTION
} OptionType;


/* The Option Definitions */
typedef struct OptionDef {
  OptionType type;
  Bool used;
  char *name;
  short value;
} OptionDef;


/* Option values for LANGUAGE */
typedef enum LanguageKind {
  LANGUAGE_ENGLISH,
  LANGUAGE_SWEDISH
} LanguageKind;



/* Data: */

extern OptionDef options[NOPTIONS];


/* Functions: */
#ifdef _PROTOTYPES_

/* Register an integer option */
extern void integerOption(Id *id,
			  int value);

/* Register an enumerated option */
extern void enumeratedOption(Id *id,
			     Id *value);

/* Register a Boolean option */
extern void booleanOption(Id *id);


/* Generate all option values */
extern void generateOptions(AcdHdr *header);
#else
extern void integerOption();
extern void enumeratedOption();
extern void booleanOption();
extern void generateOptions();
#endif

#endif
