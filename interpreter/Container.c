#include "Container.h"

#include "main.h"
#include "exe.h"
#include "syserr.h"
#include "inter.h"


/*----------------------------------------------------------------------*/
static int countInContainer(int cnt)	/* IN - the container to count in */
{
  int i, j = 0;
  
  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt, TRUE))
      /* Then it's in this container also */
      j++;
  return(j);
}



/*----------------------------------------------------------------------*/
static int sumAttributesInContainer(
    Aint cnt,			/* IN - the container to sum */
    Aint atr			/* IN - the attribute to sum over */
) {
  int i;
  int sum = 0;

  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt, TRUE)) {		/* Then it's directly in this cont */
      if (instance[i].container != 0)	/* This is also a container! */
	sum = sum + sumAttributesInContainer(i, atr);
      sum = sum + attributeOf(i, atr);
    }
  return(sum);
}


/*======================================================================*/
Bool passesContainerLimits(
     Aint theContainer,		/* IN - Container id */
     Aint theAddedInstance	/* IN - The object to add */
) {
  LimEntry *lim;
  Aword props;

  if (!isContainer(theContainer))
    syserr("Checking limits for a non-container.");

  /* Find the container properties */
  props = instance[theContainer].container;

  if (container[props].limits != 0) { /* Any limits at all? */
    for (lim = (LimEntry *) pointerTo(container[props].limits); !endOfTable(lim); lim++)
      if (lim->atr == 1-I_COUNT) {
	if (countInContainer(theContainer) >= lim->val) {
	  interpret(lim->stms);
	  return(FALSE);
	}
      } else {
	if (sumAttributesInContainer(theContainer, lim->atr) + attributeOf(theAddedInstance, lim->atr) > lim->val) {
	  interpret(lim->stms);
	  return(FALSE);
	}
      }
  }
  return(TRUE);
}
