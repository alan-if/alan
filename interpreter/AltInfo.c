#include "AltInfo.h"

/*----------------------------------------------------------------------*\

  An info node about the Verb Alternatives found and possibly executed

\*----------------------------------------------------------------------*/

#include "types.h"
#include "options.h"
#include "main.h"
#include "inter.h"
#include "parse.h"
#include "debug.h"
#include "CheckEntryArray.h"


/*======================================================================*/
void primeAltInfo(AltInfo *altInfo, int level, int parameter, int instance, int class)
{
  altInfo->level = level;
  altInfo->parameter = parameter;
  altInfo->instance = instance;
  altInfo->class = class;
  altInfo->done = FALSE;
  altInfo->end = FALSE;
}


/*----------------------------------------------------------------------*/
static char *idOfClass(int theClass) {
	return (char *)pointerTo(classes[theClass].id);
}


/*----------------------------------------------------------------------*/
static void traceInstanceAndItsClass(AltInfo *alt)
{
    traceSay(parameters[alt->parameter-1].instance);
    printf(")");
    if (alt->class != NO_CLASS)
      printf(", inherited from class #%d (%s)", alt->class, idOfClass(alt->class));
}


/*----------------------------------------------------------------------*/
static void traceAltInfo(AltInfo *alt) {
  switch (alt->level) {
  case GLOBAL_LEVEL:
    printf("GLOBAL");
    break;
  case LOCATION_LEVEL:
    printf("in location #%d (", alt->instance);
    traceInstanceAndItsClass(alt);
    break;
  case PARAMETER_LEVEL:
    printf("in parameter #%d (", alt->parameter);
    traceInstanceAndItsClass(alt);
    break;
  }
}


/*----------------------------------------------------------------------*/
static void traceVerbCheck(AltInfo *alt, Bool execute)
{
	if (sectionTraceOption && execute) {
		printf("\n<VERB %d, ", current.verb);
		traceAltInfo(alt);
		printf(", CHECK:>\n");
	}
}


/*======================================================================*/
Bool checkFailed(AltInfo *altInfo, Bool execute)
{
	if (altInfo->alt != NULL && altInfo->alt->checks != 0) {
		traceVerbCheck(altInfo, execute);
		// TODO Why does this not generate a regression error with !
		// Need a new regression case?
		if (checksFailed(altInfo->alt->checks, execute)) return TRUE;
		if (fail) return TRUE;
	}
	return FALSE;
}


/*----------------------------------------------------------------------*/
static void traceVerbExecution(AltInfo *alt)
{
  if (sectionTraceOption) {
    printf("\n<VERB %d, ", current.verb);
    traceAltInfo(alt);
    printf(", DOES");
    switch (alt->alt->qual) {
    case Q_BEFORE: printf(" (BEFORE)"); break;
    case Q_ONLY: printf(" (ONLY)"); break;
    case Q_AFTER: printf(" (AFTER)"); break;
    case Q_DEFAULT: break;
    }
    printf(":>\n");  }
}


/*======================================================================*/
Bool executedOk(AltInfo *altInfo)
{
  fail = FALSE;
  if (!altInfo->done && altInfo->alt->action != 0) {
    traceVerbExecution(altInfo);
    current.instance = altInfo->instance;
    interpret(altInfo->alt->action);
  }
  altInfo->done = TRUE;
  return !fail;
}


/*======================================================================*/
Bool executable(AltInfo *altInfo) {
  return altInfo->alt != NULL && altInfo->alt->action != 0;
}

