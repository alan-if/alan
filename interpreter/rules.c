/*----------------------------------------------------------------------*\

  rules.c

  Rule handling unit of Alan interpreter module, ARUN.

\*----------------------------------------------------------------------*/

#include <stdio.h>

#include "types.h"
#include "arun.h"
#include "inter.h"
#include "stack.h"


#ifdef _PROTOTYPES_
void rules(void)
#else
void rules()
#endif
{
  Boolean change = TRUE;
  int i;
  
  for (i = 1; !endOfTable(&ruls[i-1]); i++)
    ruls[i-1].run = FALSE;
  
  while (change) {
    change = FALSE;
    for (i = 1; !endOfTable(&ruls[i-1]); i++) 
      if (!ruls[i-1].run) {
	if (trcflg)
	  printf("\nRULE %d: Evaluating", i);
	interpret(ruls[i-1].exp);
	if (pop()) {
	  change = TRUE;
	  ruls[i-1].run = TRUE;
	  if (trcflg)
	    if (!stpflg)
	      printf(", Executing!");
	    else
	      printf("\nRULE %d: Executing", i);
	  interpret(ruls[i-1].stms);
	}
      }
  }
}
