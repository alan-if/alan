/*----------------------------------------------------------------------*\

                                SET.C
			     Set Handling

\*----------------------------------------------------------------------*/

#include "set_x.h"

#include "util.h"
#include "types.h"
#include "atr.h"

#include "sym_x.h"

#include "lmList.h"


/*======================================================================*/
Symbol *classOfMembers(Expression *exp)
{
  /* Find what classes a Set contains */
  switch (exp->kind) {
  case ATTRIBUTE_EXPRESSION:
    return exp->fields.atr.atr->setClass;
    break;
  default:
    SYSERR("Unexpected Expression kind");
    break;
  }
  return NULL;
}


/*======================================================================*/
void verifySetMember(Expression *theSet, Expression *theMember, char contextMessage[]) {

  switch (theMember->type) {
  case INTEGER_TYPE: theMember->class = integerSymbol; break;
  case STRING_TYPE: theMember->class = stringSymbol; break;
  case INSTANCE_TYPE: break;
  case SET_TYPE: break;
  default: SYSERR("Unexpected member type");
  }
  if (theMember->class != NULL)
    if (!inheritsFrom(theMember->class, theSet->class)) {
      char *setContentsMessage;
      char buffer[1000];
      if (theSet->class != NULL) {
	if (theSet->class->fields.entity.isBasicType)
	  setContentsMessage = theSet->class->string;
	else {
	  sprintf(buffer, "instances of '%s' and its subclasses", theSet->class->string);
	  setContentsMessage = buffer;
	}
      } else
	setContentsMessage = "unknown class";
      lmLogv(&theMember->srcp, 410, sevERR, contextMessage, setContentsMessage, NULL);
    }
}
