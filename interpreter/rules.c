/*----------------------------------------------------------------------*\

  rules.c

  Rule handling unit of Alan interpreter module, ARUN.

\*----------------------------------------------------------------------*/
#include "rules.h"


/* IMPORTS */
#include "lists.h"
#include "inter.h"
#include "debug.h"
#include "current.h"
#include "options.h"

#ifdef HAVE_GLK
#include "glkio.h"
#endif


/* PUBLIC DATA */
RulEntry *ruls;         /* Rule table pointer */


/*=======================================================================*/
void rules(void)
{
  Bool change = TRUE;
  int i;

  for (i = 1; !isEndOfList(&ruls[i-1]); i++)
    ruls[i-1].run = FALSE;

  while (change) {
    change = FALSE;
    for (i = 1; !isEndOfList(&ruls[i-1]); i++)
      if (!ruls[i-1].run) {
	if (sectionTraceOption) {
	  printf("\n<RULE %d (at ", i);
	  traceSay(current.location);
	  if (!singleStepOption)
	    printf("), Evaluating");
	  else
	    printf("), Evaluating:>\n");
	}
	if (evaluate(ruls[i-1].exp)) {
	  change = TRUE;
	  ruls[i-1].run = TRUE;
	  if (sectionTraceOption) {
	    if (!singleStepOption)
	      printf(", Executing:>\n");
	    else {
	      printf("\nRULE %d (at ", i);
	      traceSay(current.location);
	      printf("), Executing:>\n");
	    }
	  }
	  interpret(ruls[i-1].stms);
	} else if (sectionTraceOption && !singleStepOption)
	  printf(":>\n");
      }
  }
}
