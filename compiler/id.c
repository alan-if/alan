/*----------------------------------------------------------------------*\

  ID.C
  Identifier Nodes handling

\*----------------------------------------------------------------------*/

#include "id_x.h"

#include <stdio.h>

#include "srcp_x.h"
#include "sym_x.h"

#include "elm.h"
#include "str.h"
#include "util.h"
#include "dump.h"
#include "lmList.h"


/*======================================================================

  newId()

  Allocates and initialises a idnod.

  */
IdNode *newId(Srcp *srcp,	/* IN - Source Position */
	     char *str)		/* IN - The string */
{
  IdNode *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(IdNode);
  
  new->srcp = *srcp;
  new->string  = newstr(str);
  new->symbol = NULL;

  return(new);
}


/*======================================================================

  equalId()

  Check if two ids matches.

  */
Bool equalId(IdNode *id1,
	     IdNode *id2)
{
  if (id1 && id2)
    return (strcmp(id1->string, id2->string) == 0);
  else
    syserr("ID == NULL in equalId()");
  return FALSE;
}


/*======================================================================

  symboizeId()

  Symbolize an ID-node.

  */
void symbolizeId(IdNode *id)
{
  id->symbol = lookup(id->string);
  if (id->symbol == NULL) 
    lmLog(&id->srcp, 310, sevERR, id->string);
}


/*======================================================================

  geid()

  Generate an ID-node.

  */
void geid(IdNode *id)
{
  syserr("UNIMPL: geid() - body");
}



/*======================================================================

  dumpId()

  Dump an ID-node.

  */
void dumpId(IdNode *id)
{
  if (id == NULL) {
    put("NULL");
    return;
  }

  put("ID: "); dumpSrcp(&id->srcp); in();
  put("string: "); dumpString(id->string); out();
}
