/*----------------------------------------------------------------------*\

				STR.C
			   Dynamic Strings

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"

#include "alan.h"
#include "srcp.h"
#include "str.h"


/* Private: */

static struct strnod {         /* A tree containing all ids */
  struct strnod *low, *high;
  char *str;
} *strtree = NULL;




/*======================================================================

  newstr()

  To minimize string space required, all strings (for identifiers) are
  inserted into a tree where the same string only occurs once. This
  routine inserts a string in the tree if it is not already there. Else
  dynamically allocates space for it. Returns a pointer to the string.

  */
#ifdef _PROTOTYPES_
char *newstr(char *str)
                		/* IN - The string to insert */
#else
char *newstr(str)
     char str[];		/* IN - The string to insert */
#endif
{
  struct strnod *s, *next;	/* To traversal pointers */
  int comp = 1;			/* Comparison value */

  next = strtree;
  while (next != NULL && comp != 0) {
    s = next;
    comp = strcmp(str, s->str);
    if (comp == 0) return(s->str);
    if (comp < 0)
      next = s->low;
    else
      next = s->high;
  }

  next = NEW(struct strnod);
  next->low = NULL;
  next->high = NULL;
  next->str = (char *) allocate(strlen(str)+1);
  strcpy(next->str, str);

  if (strtree == NULL)
    strtree = next;
  else if (comp < 0)
    s->low = next;
  else
    s->high = next;

  return(next->str);
}
