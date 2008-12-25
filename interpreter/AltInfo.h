#ifndef _ALTINFO_H_
#define _ALTINFO_H_
/*----------------------------------------------------------------------*\

  An info node about the Verb Alternatives found and possibly executed

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"

/* CONSTANTS */

#define GLOBAL_LEVEL (0)
#define LOCATION_LEVEL (1)
#define PARAMETER_LEVEL (2)


#define NO_PARAMETER (-1)
#define NO_INSTANCE (-1)
#define NO_CLASS (-1)

/* tryCheck() flags */
#ifndef EXECUTE_CHECK_BODY_ON_FAIL
#define EXECUTE_CHECK_BODY_ON_FAIL TRUE
#define DONT_EXECUTE_CHECK_BODY_ON_FAIL FALSE
#endif


/* TYPES */

typedef struct AltInfo {
	Bool end;			/* Indicator of end in AltInfoArray, first empty has TRUE here */
	AltEntry *alt;
	Bool done;
	int level;			/* 0 - Global, 1 - location, 2 - parameter */
	int class;			/* In which class, only used for tracing */
	int instance;		/* In which instance the Alternative was found,
						   used to set current.instance and tracing */
	int parameter;		/* In which parameter, only used for tracing */
} AltInfo;



/* DATA */


/* FUNCTIONS */
extern void primeAltInfo(AltInfo *altInfo, int level, int parameter, int instance, int class);
extern Bool executedOk(AltInfo *altInfo);
extern Bool checkFailed(AltInfo *altInfo, Bool execute);
extern Bool executable(AltInfo *altInfo);

#endif
