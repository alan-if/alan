/*----------------------------------------------------------------------*\

				 Id.c

			       Id Nodes

\*----------------------------------------------------------------------*/

#include "Id.h"

#include "dump.h"


/*======================================================================

  newId()

  Creates a new Id from a Srcp and a string

  */
#ifdef _PROTOTYPES_
Id *newId(Srcp *srcp,
	  char string[])
#else
Id *newId(srcp, string)
     Srcp *srcp;
     char string[];
#endif
{
  Id *new;			/* The newly created Id */

  new = NEW(Id);

  new->srcp = *srcp;
  new->string = string;
  
  return(new);
}


/*======================================================================

  equalIds()

  Check if two Ids are equal.

  */
#ifdef _PROTOTYPES_
Bool equalIds(Id *id1,
	      Id *id2)
#else
Bool equalIds(id1, id2)
     Id *id1;
     Id *id2;
#endif
{
  return (strcmp(id1->string, id2->string) == 0);
}



/*======================================================================

  dumpId()

  Dump an identifier.

  */
#ifdef _PROTOTYPES_
void dumpId(Id *id)
#else
void dumpId(id)
     Id *id;
#endif
{
  if (id == NULL) {
    put("NULL");
    return;
  }

  put("ID: "); dumpSrcp(&id->srcp); in();
  put("string: "); dumpString(id->string); out();
}
