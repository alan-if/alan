/*----------------------------------------------------------------------*\

                                SET.C
			     Set Handling

\*----------------------------------------------------------------------*/

#include "set_x.h"

#include "util.h"
#include "types.h"
#include "atr.h"
#include "exp_x.h"
#include "lst_x.h"

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


/*----------------------------------------------------------------------*/
static Symbol *commonAncestor(Symbol *inferedClass, Expression *exp) {
  while (!inheritsFrom(inferedClass, exp->class) && !inheritsFrom(exp->class, inferedClass)) {
    /* They are not of the same class so we need to find a common ancestor */
    inferedClass = inferedClass->fields.entity.parent;
    if (inferedClass == NULL)
      SYSERR("No common ancestor found for Set members");
  }
  return(inferedClass);
}


/*======================================================================*/
void analyzeSetMembers(List *set, TypeKind *_inferedType, Symbol **_inferedClass) {
  List *elements;
  TypeKind inferedType = UNINITIALIZED_TYPE;
  Symbol *inferedClass = NULL;

  if (length(set) == 0) {
    /* If the set is empty it could match anything */
    inferedType = INSTANCE_TYPE;
    inferedClass = entitySymbol;
  } else
    TRAVERSE(elements, set) {
      Expression *exp = elements->element.exp;
      analyzeExpression(exp, NULL);
      if (inferedType == UNINITIALIZED_TYPE)
	inferedType = exp->type;
      if (!equalTypes(inferedType, exp->type)) {
	lmLogv(&exp->srcp, 408, sevERR, "Expressions", "a Set", "the same", NULL);
	inferedType = ERROR_TYPE;
      } else if (exp->type == ERROR_TYPE)
	inferedType = ERROR_TYPE;
      else
	switch (exp->type) {
	case INSTANCE_TYPE:
	  if (inferedClass == NULL)
	    inferedClass = exp->class;
	  else
	    inferedClass = commonAncestor(inferedClass, exp);
	  break;
	case INTEGER_TYPE:
	  inferedClass = integerSymbol;
	  break;
	case STRING_TYPE:
	case SET_TYPE:
	case BOOLEAN_TYPE:
	case EVENT_TYPE:
	  lmLogv(&exp->srcp, 410, sevERR, "Set literal expression", "integers or instance references", NULL);
	  break;
	case UNINITIALIZED_TYPE:
	case REFERENCE_TYPE:
	  SYSERR("Unexpected type kind");
	  break;
	case ERROR_TYPE:
	  ;
	}
    }

  *_inferedType = inferedType;
  *_inferedClass = inferedClass;
}
