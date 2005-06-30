/*----------------------------------------------------------------------*\

  ID.C
  Identifier Nodes handling

\*----------------------------------------------------------------------*/

#include "id_x.h"

#include <stdio.h>

#include "srcp_x.h"
#include "sym_x.h"
#include "dump_x.h"

#include "emit.h"
#include "elm.h"
#include "str.h"
#include "util.h"
#include "lmList.h"


/*======================================================================*/
IdNode *newId(Srcp srcp,	/* IN - Source Position */
	      char *str)	/* IN - The string */
{
  IdNode *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(IdNode);
  
  new->srcp = srcp;
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
    return (compareStrings(id1->string, id2->string) == 0);
  else
    SYSERR("ID == NULL");
  return FALSE;
}


/*======================================================================*/
void symbolizeId(IdNode *id)
{
  id->symbol = lookup(id->string);
  if (id->symbol == NULL) {
    if (id->string[0] != '$')
      /* Generated identifiers start with '$', don't report errors on those */
      lmLog(&id->srcp, 310, sevERR, id->string);
    id->symbol = newSymbol(id, ERROR_SYMBOL);
  } else
    id->code = id->symbol->code;
}


/*======================================================================*/
IdNode *findIdInList(IdNode *theId, List *theList) {
  List *here;

  for (here = theList; here != NULL; here = here->next)
    if (equalId(here->element.id, theId))
      return here->element.id;
  return NULL;
}


/*======================================================================*/
void generateId(IdNode *id)
{
  if (id->symbol != NULL) {
    if (id->symbol->kind == PARAMETER_SYMBOL) {
      switch (id->symbol->fields.parameter.type) {
	/* If it is a literal we have to fetch its value */
	/* which is the first attribute */
      case STRING_TYPE:
	emitConstant(1);
	emitConstant(id->symbol->code);
	emitVariable(V_PARAM);
	emit0(I_ATTRSTR);
	break;
      case INTEGER_TYPE:
	emitConstant(1);
	emitConstant(id->symbol->code);
	emitVariable(V_PARAM);
	emit0(I_ATTRIBUTE);
	break;
      case INSTANCE_TYPE:
	emitConstant(id->symbol->code);
	emitVariable(V_PARAM);
	break;
      default:
	SYSERR("Unexpected type");
      }
    } else if (id->symbol->kind == LOCAL_SYMBOL) {
      /* Calculate the variable number and frame depth */
      emit2(I_GETLOCAL, frameLevel - id->symbol->fields.local.level,
	    id->symbol->fields.local.number);
    } else
      emitConstant(id->symbol->code);
  } else if (id->code == 0)
    SYSERR("Generating a symbol-less id with code == 0");
  else
    emit(id->code);
}



/*======================================================================*/
void dumpId(IdNode *id)
{
  if (id == NULL) {
    put("NULL");
    return;
  }

  put("ID: "); dumpSrcp(id->srcp);
  put(", "); dumpString(id->string);
  put(":"); dumpInt(id->code);
  put(", "); dumpSymbol(id->symbol);
}
