/*----------------------------------------------------------------------*\

  ID.C
  Identifier Nodes handling

\*----------------------------------------------------------------------*/

#include "id.h"

#include <stdio.h>

#include "elm.h"
#include "str.h"
#include "sym.h"
#include "util.h"
#include "dump.h"




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

  idcheck()

  Check if an identifier is known, if so return its symbol else give an error
  message indicating what it should have been. Also check the possible
  parameters, if any of those match use it instead. Will set elm if it
  was a parameter.

  */
SymNod *idcheck(		/* OUT - Found symbol */
    ElmNod **elm,               /* OUT - Found parameter  */
    IdNode *id,			/* IN - The name to check */
    IdKind kinds,		/* IN - A set of allowed symbol kinds */
    List *pars                  /* IN - Possible parameters valid in this context */
)
{
  syserr("UNIMPL: idcheck() - body"); return NULL;
}


/*======================================================================

  equalId()

  Check if two ids matches.

  */
Bool equalId(IdNode *id1,	/* IN - The first id */
	     IdNode *id2)	/* IN - Second id */
{
  if (id1 && id2)
    return (strcmp(id1->string, id2->string) == 0);
  else
    syserr("ID == NULL in eqids()");
  return FALSE;
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
