/*----------------------------------------------------------------------*\

  ID.C
  Identifier Nodes handling

\*----------------------------------------------------------------------*/

#include "id_x.h"

#include <stdio.h>

#include "srcp_x.h"
#include "sym_x.h"

#include "emit.h"
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
	      char *str)	/* IN - The string */
{
  IdNode *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(IdNode);
  
  new->srcp = *srcp;
  new->string  = newString(str);
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

  symbolizeId()

  Symbolize an ID-node.

  */
void symbolizeId(IdNode *id)
{
  id->symbol = lookup(id->string);
  if (id->symbol == NULL) 
    lmLog(&id->srcp, 310, sevERR, id->string);
  else
    id->code = id->symbol->code;
}


/*======================================================================

  generateId()

  */
void generateId(IdNode *id)
{
  if (id->symbol != NULL) {
    if (id->symbol->kind == PARAMETER_SYMBOL) {
      switch (id->symbol->fields.parameter.type) {
	/* If it is a literal we have to fetch its value */
	/* which is the first attribute */
      case STRING_TYPE:
	emit0(C_CONST, 1);
	emit0(C_CONST, id->symbol->code);
	emit0(C_CURVAR, V_PARAM);
	emit0(C_STMOP, I_STRATTR);
	break;
      case INTEGER_TYPE:
	emit0(C_CONST, 1);
	emit0(C_CONST, id->symbol->code);
	emit0(C_CURVAR, V_PARAM);
	emit0(C_STMOP, I_ATTRIBUTE);
	break;
      case INSTANCE_TYPE:
	emit0(C_CONST, id->symbol->code);
	emit0(C_CURVAR, V_PARAM);
	break;
      default:
	syserr("Unexpected type in generateId()");
      }
    } else
      emit0(C_CONST, id->symbol->code);
  } else if (id->code == 0)
    syserr("Generating a symbol-less id with code == 0");
  else
    emit(id->code);
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

  put("ID: "); dumpSrcp(&id->srcp);
  put(", "); dumpString(id->string);
  put(", code: "); dumpInt(id->code);
  put(", symbol: "); dumpPointer(id->symbol);
}
