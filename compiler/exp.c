/*----------------------------------------------------------------------*\

				EXP.C
			   Expression Nodes

\*----------------------------------------------------------------------*/

#include "exp_x.h"

#include "util.h"

#include "srcp_x.h"
#include "whr_x.h"
#include "id_x.h"
#include "atr_x.h"
#include "sym_x.h"
#include "wht_x.h"
#include "cnt_x.h"

#include "lmList.h"

#include "adv.h"		/* ADV-node */
#include "lst.h"		/* LST-nodes */
#include "elm.h"		/* ELM-nodes */
#include "ins.h"		/* INS-nodes */


#include "emit.h"

#include "acode.h"
#include "encode.h"

#include "dump.h"




/*======================================================================

  equalTypes()

  Check if two types are equal. If one is TYPUNK they are.

  */
Bool equalTypes(TypeKind typ1,	/* IN - types to compare */
		TypeKind typ2)
{
  if (typ1 == ERROR_TYPE || typ2 == ERROR_TYPE) syserr("Unintialised type in equalTypes()");
  return (typ1 == UNKNOWN_TYPE || typ2 == UNKNOWN_TYPE || typ1 == typ2);
}



/*======================================================================

  newexp()

  Allocates and initialises an expnod.

 */
ExpNod *newexp(Srcp *srcp,	/* IN - Source Position */
	       ExpKind class)	/* IN - The expression class */
{
  ExpNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ExpNod);

  new->srcp = *srcp;
  new->class = class;
  new->not = FALSE;

  return(new);
}




/*----------------------------------------------------------------------

  anexpwhr()

  Analyze a WHR expression.

 */
static void anexpwhr(ExpNod *exp,
		     Context *context)
{
  anexp(exp->fields.whr.wht, context);
  if (exp->fields.whr.wht->class != EXPWHT)
    lmLog(&exp->fields.whr.wht->srcp, 311, sevERR, "an instance");
  else {
    switch (exp->fields.whr.wht->fields.wht.wht->kind) {
    case WHT_OBJ:
      if (context->verb->fields.verb.parameterSymbols == NULL)
	lmLog(&exp->fields.whr.wht->srcp, 409, sevERR, "");
      break;
    case WHT_ACT:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&exp->fields.whr.wht->srcp, 412, sevERR, "");
      break;
    case WHT_LOC:
      lmLog(&exp->fields.whr.wht->srcp, 311, sevERR, "an Object or an Actor");
      break;
    case WHT_ID:
      break;
    default:
      syserr("Unrecognized switch in anexpwhr()");
      break;
    }
  }

  switch (exp->fields.whr.whr->kind) {
  case WHR_HERE:
  case WHR_NEAR:
    break;
  case WHR_AT:
    switch (exp->fields.whr.whr->wht->kind) {
    case WHT_ID:
      symcheck(exp->fields.whr.whr->wht->id, INSTANCE_SYMBOL, context);
      break;
    case WHT_LOC:
      exp->fields.whr.whr->kind = WHR_HERE;
      break;
    case WHT_OBJ:
      if (context->kind != VERB_CONTEXT || context->verb->fields.verb.parameterSymbols == NULL)
	lmLog(&exp->fields.whr.whr->wht->srcp, 409, sevERR, "");
      break;
    case WHT_ACT:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&exp->fields.whr.whr->srcp, 412, sevERR, "");
      break;
    default:
      syserr("Unrecognized switch in anexpwhr()");
      break;
    }
    break;
  case WHR_IN:
    verifyContainer(exp->fields.whr.whr->wht, context);
    break;
  default:
    syserr("Unrecognized switch in anexpwhr()");
    break;
  }

  exp->type = BOOLEAN_TYPE;
}



/*----------------------------------------------------------------------

  anexpatr()

  Analyze an ATR expression.

  */
static void anexpatr(ExpNod *exp, /* IN - The expression to analyze */
		     Context *context)
{
  AtrNod *atr;

  if (exp->fields.atr.wht->class == EXPWHT) {
    switch (exp->fields.atr.wht->fields.wht.wht->kind) {
      
    case WHT_ACT:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 412, sevERR, "");
      else {
	atr = findAttribute(NULL, exp->fields.atr.atr);
	if (atr == NULL) {		/* attribute not found globally */
	  lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "ACTOR");
	  exp->type = UNKNOWN_TYPE;
	} else {
	  exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	  exp->type = atr->type;
	}
      }
      break;

    case WHT_LOC:
      atr = findAttribute(NULL, exp->fields.atr.atr);
      if (atr == NULL) {		/* attribute not found globally */
	lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "LOCATION");
	exp->type = UNKNOWN_TYPE;
      } else {
	exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	exp->type = atr->type;
      }
      break;

    case WHT_OBJ:
      if (context->kind != VERB_CONTEXT || context->verb->fields.verb.parameterSymbols == NULL)
	lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 409, sevERR, "");
      atr = findAttribute(NULL, exp->fields.atr.atr);
      if (atr == NULL) {		/* attribute not found globally */
	lmLog(&exp->fields.atr.atr->srcp, 404, sevERR, "OBJECT");
	exp->type = UNKNOWN_TYPE;
      } else {
	exp->fields.atr.atr->symbol->code = atr->id->symbol->code;
	exp->type = atr->type;
      }
      break;

    case WHT_ID:
      atr = resolveAttributeReference(exp->fields.atr.wht->fields.wht.wht,
				      exp->fields.atr.atr, context);
      if (atr == NULL) {	/* Attribute not found */
	exp->type = UNKNOWN_TYPE;
      } else if (exp->fields.atr.atr->symbol == NULL) {
	exp->fields.atr.atr->code = atr->id->code;
	exp->type = atr->type;
      } else
	syserr("Attribute with symbol in anexpatr()");
      break;

    default:
      syserr("Unrecognized switch in anexpatr()");
      break;
    }
  } else
    lmLog(&exp->srcp, 420, sevERR, "attribute reference");
}


/*----------------------------------------------------------------------

  anbin()

  Analyze a binary expression and find out its type.

 */
static void anbin(ExpNod *exp,
		  Context *context)
{
  anexp(exp->fields.bin.left, context);
  anexp(exp->fields.bin.right, context);

  switch (exp->fields.bin.op) {
  case OP_AND:
  case OP_OR:
    if (!equalTypes(exp->fields.bin.left->type, BOOLEAN_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "boolean", "AND/OR", NULL);
    if (!equalTypes(exp->fields.bin.right->type, BOOLEAN_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "boolean", "AND/OR", NULL);
    exp->type = BOOLEAN_TYPE;
    break;

  case OP_NE:
  case OP_EQ:
    if (!equalTypes(exp->fields.bin.left->type, exp->fields.bin.right->type))
      lmLog(&exp->srcp, 331, sevERR, "expression");
    else if (exp->fields.bin.left->type != UNKNOWN_TYPE && exp->fields.bin.right->type != UNKNOWN_TYPE)
      if (exp->fields.bin.left->type == INSTANCE_TYPE) {
	WhtNod *leftWhat = exp->fields.bin.left->fields.wht.wht;
	WhtNod *rightWhat = exp->fields.bin.right->fields.wht.wht;
	if (leftWhat->kind == WHT_ID && rightWhat->kind == WHT_ID)
	  if (leftWhat->id->symbol->kind != PARAMETER_SYMBOL
	      && rightWhat->id->symbol->kind != PARAMETER_SYMBOL)
	    lmLog(&exp->srcp, 417, sevINF, NULL);
      }
    exp->type = BOOLEAN_TYPE;
    break;

  case OP_EXACT:
    if (!equalTypes(exp->fields.bin.left->type, STRING_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "string", "'=='", NULL);
    if (!equalTypes(exp->fields.bin.right->type, STRING_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "string", "'=='", NULL);
    break;
	    
  case OP_LE:
  case OP_GE:
  case OP_LT:
  case OP_GT:
    if (!equalTypes(exp->fields.bin.left->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "integer", "relational", NULL);
    if (!equalTypes(exp->fields.bin.right->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "integer", "relational", NULL);
    exp->type = BOOLEAN_TYPE;
    break;

  case OP_PLUS:
  case OP_MINUS:
  case OP_MULT:
  case OP_DIV:
    if (!equalTypes(exp->fields.bin.left->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "integer", "arithmetic", NULL);
    if (!equalTypes(exp->fields.bin.right->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "integer", "arithmetic", NULL);
    exp->type = INTEGER_TYPE;
    break;

  case OP_CONTAINS:
    if (!equalTypes(exp->fields.bin.left->type, STRING_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "string", "'CONTAINS'", NULL);
    if (!equalTypes(exp->fields.bin.right->type, STRING_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "string", "'CONTAINS'", NULL);
    exp->type = BOOLEAN_TYPE;
    break;

  default:
    syserr("unrecognized binary operator in anbin()");
    break;    
  }
}



/*----------------------------------------------------------------------

  anagr()

  Analyze an aggregate expression.

 */
static void anagr(ExpNod *exp,
		  Context *context)
{
  AtrNod *atr = NULL;

  exp->type = INTEGER_TYPE;
  if (exp->fields.agr.agr != AGR_COUNT) {
    atr = findAttribute(NULL, exp->fields.agr.atr);
    if (atr == NULL) {		/* attribute not found globally */
      lmLog(&exp->fields.agr.atr->srcp, 404, sevERR,
	    "OBJECT in aggregate expression");
      exp->type = UNKNOWN_TYPE;
    } else if (!equalTypes(INTEGER_TYPE, atr->type)) {
      lmLog(&exp->fields.agr.atr->srcp, 418, sevERR, "");
      exp->type = UNKNOWN_TYPE;
    } else
      exp->fields.agr.atr->symbol->code = atr->id->symbol->code;
  }

  analyzeWhere(exp->fields.agr.whr, context);
}


/*----------------------------------------------------------------------

  anrnd()

  Analyse a random expression.

  */
static void anrnd(ExpNod *exp,
		  Context *context)
{
  exp->type = INTEGER_TYPE;
  anexp(exp->fields.rnd.from, context);
  if (!equalTypes(INTEGER_TYPE, exp->fields.rnd.from->type)) {
    lmLog(&exp->fields.rnd.from->srcp, 413, sevERR, "RANDOM");
    exp->type = UNKNOWN_TYPE;
  }

  anexp(exp->fields.rnd.to, context);
  if (!equalTypes(INTEGER_TYPE, exp->fields.rnd.to->type)) {
    lmLog(&exp->fields.rnd.to->srcp, 413, sevERR, "RANDOM");
    exp->type = UNKNOWN_TYPE;
  }
}


/*----------------------------------------------------------------------

  anexpwht()

  Analyse a WHT expression.

  */
static void anexpwht(ExpNod *exp,
		     Context *context)
{
  Symbol *symbol;

  switch (exp->fields.wht.wht->kind) {
  case WHT_OBJ:
    if (context->kind != VERB_CONTEXT || context->verb->fields.verb.parameterSymbols == NULL)
      lmLog(&exp->fields.wht.wht->srcp, 409, sevERR, "");
    exp->type = INSTANCE_TYPE;
    break;

  case WHT_LOC:
    exp->type = INSTANCE_TYPE;
    break;

  case WHT_ACT:
    if (context->kind == EVENT_CONTEXT)
      lmLog(&exp->fields.wht.wht->srcp, 412, sevERR, "");
    exp->type = INSTANCE_TYPE;
    break;

  case WHT_ID:
    symbol = symcheck(exp->fields.wht.wht->id, INSTANCE_SYMBOL, context);
    if (symbol != NULL) {
      switch (symbol->kind) {
      case PARAMETER_SYMBOL:
	exp->type = symbol->fields.parameter.type;
	break;
      case INSTANCE_SYMBOL:
	exp->type = INSTANCE_TYPE;
	break;
      default:
	syserr("Unexpected symbolKind in anexpwht()");
	break;
      }
    } else
      exp->type = UNKNOWN_TYPE;
    break;

  default:
    syserr("Unrecognized switch in anexpwht()");
    break;
  }
}


/*----------------------------------------------------------------------

  anexpbtw()

  Analyse a BTW expression.

  */
static void anexpbtw(ExpNod *exp,
		     Context *context)
{
  anexp(exp->fields.btw.val, context);
  if (!equalTypes(exp->fields.btw.val->type, INTEGER_TYPE))
    lmLogv(&exp->fields.btw.val->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  anexp(exp->fields.btw.low, context);
  if (!equalTypes(exp->fields.btw.low->type, INTEGER_TYPE))
    lmLogv(&exp->fields.btw.low->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  anexp(exp->fields.btw.high, context);
  if (!equalTypes(exp->fields.btw.high->type, INTEGER_TYPE))
    lmLogv(&exp->fields.btw.high->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  exp->type = BOOLEAN_TYPE;
}


/*======================================================================

  anexp()

  Analyze one expression.

  */
void anexp(ExpNod *exp,
	   Context *context)
{
  if (exp == NULL) return;	/* Ignore empty expressions (syntax error) */
  
  switch (exp->class) {
    
  case EXPWHR:
    anexpwhr(exp, context);
    break;
    
  case EXPATR:
    anexpatr(exp, context);
    break;

  case EXPBIN:
    anbin(exp, context);
    break;
    
  case EXPINT:
    exp->type = INTEGER_TYPE;
    break;
    
  case EXPSTR:
    exp->type = STRING_TYPE;
    break;
    
  case EXPAGR:
    anagr(exp, context);
    break;
    
  case EXPRND:
    anrnd(exp, context);
    break;

  case EXPSCORE:
    exp->type = INTEGER_TYPE;
    break;

  case EXPWHT:
    anexpwht(exp, context);
    break;

  case EXPBTW:
    anexpbtw(exp, context);
    break;

  case EXPISA:
    /* FIXME unimplemented */
    unimpl(&exp->srcp, "analyzer");
    break;

  default:
    syserr("Unrecognized switch in anexp()");
    break;
  }
}


/*======================================================================

  genererateBinaryOperator()

*/
void generateBinaryOperator(ExpNod *exp)
{
  switch (exp->fields.bin.op) {
  case OP_AND:
    emit0(C_STMOP, I_AND);
    break;
  case OP_OR:
    emit0(C_STMOP, I_OR);
    break;
  case OP_NE:
    emit0(C_STMOP, I_NE);
    break;
  case OP_EQ:
    if (exp->fields.bin.right->type == STRING_TYPE)
      emit0(C_STMOP, I_STREQ);
    else
      emit0(C_STMOP, I_EQ);
    break;
  case OP_EXACT:
    emit0(C_STMOP, I_STREXACT);
    break;
  case OP_LE:
    emit0(C_STMOP, I_LE);
    break;
  case OP_GE:
    emit0(C_STMOP, I_GE);
    break;
  case OP_LT:
    emit0(C_STMOP, I_LT);
    break;
  case OP_GT:
    emit0(C_STMOP, I_GT);
    break;
  case OP_PLUS:
    emit0(C_STMOP, I_PLUS);
    break;
  case OP_MINUS:
    emit0(C_STMOP, I_MINUS);
    break;
  case OP_MULT:
    emit0(C_STMOP, I_MULT);
    break;
  case OP_DIV:
    emit0(C_STMOP, I_DIV);
    break;
  case OP_CONTAINS:
    emit0(C_STMOP, I_CONTAINS);
    break;
  }
}


/*----------------------------------------------------------------------

  geexpbin()

  Generate a binary expression.

  */
static void geexpbin(ExpNod *exp) /* IN - Expression node */
{
  geexp(exp->fields.bin.left);
  geexp(exp->fields.bin.right);
  generateBinaryOperator(exp);
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*----------------------------------------------------------------------
  geexpwhr()

  Generate a where-expression.

  */
static void geexpwhr(ExpNod *exp) /* IN - Expression node */
{
  switch(exp->fields.whr.wht->fields.wht.wht->kind) {
    
  case WHT_OBJ:
    switch (exp->fields.whr.whr->kind) {
    case WHR_NEAR:
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_NEAR);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_HERE:
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_HERE);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_IN:
      generateId(exp->fields.whr.whr->wht->id);
      emit0(C_CONST, 1);
      emit0(C_CURVAR, V_PARAM);
      emit0(C_STMOP, I_IN);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_AT:
      generateId(exp->fields.whr.whr->wht->id);
      emit0(C_STMOP, I_WHERE);
      break;
    default:
      unimpl(&exp->srcp, "Code Generator");
      emit0(C_CONST, 0);
      return;
    }
    break;
    
  case WHT_ID:
    switch (exp->fields.whr.whr->kind) {
    case WHR_HERE:
      generateId(exp->fields.whr.wht->fields.wht.wht->id);
      emit0(C_STMOP, I_HERE);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_NEAR:
      gewht(exp->fields.whr.wht->fields.wht.wht);
      emit0(C_STMOP, I_NEAR);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_IN:
      generateId(exp->fields.whr.whr->wht->id);
      gewht(exp->fields.whr.wht->fields.wht.wht);
      emit0(C_STMOP, I_IN);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_AT:
      gewht(exp->fields.whr.wht->fields.wht.wht);
      emit0(C_STMOP, I_WHERE);
      break;
    default:
      unimpl(&exp->srcp, "Code Generator");
      emit0(C_CONST, 0);
      return;
    }
    break;
    
  default:
    unimpl(&exp->srcp, "Code Generator");
    emit0(C_CONST, 0);
    return;
  }
  
  gewhr(exp->fields.whr.whr);
  emit0(C_STMOP, I_EQ);
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*======================================================================

  generateAttributeAccess()

*/
void generateAttributeAccess(ExpNod *exp)
{
  if (exp->type == STRING_TYPE)
    emit0(C_STMOP, I_STRATTR);
  else
    emit0(C_STMOP, I_ATTRIBUTE);
}


/*----------------------------------------------------------------------
  geexpatr()

  Generate an attribute-expression.

 */
static void geexpatr(ExpNod *exp) /* IN - Expression node */
{
  generateId(exp->fields.atr.atr);
  gewht(exp->fields.atr.wht->fields.wht.wht);
  generateAttributeAccess(exp);
  if (exp->not) emit0(C_STMOP, I_NOT);
}




/*----------------------------------------------------------------------
  geexpagr()

  Generate the code for an aggregate expression.

  */
static void geexpagr(ExpNod *exp) /* IN - The expression to generate */
{
  gewhr(exp->fields.agr.whr);

  if (exp->fields.agr.agr != AGR_COUNT)
    emit0(C_CONST, exp->fields.agr.atr->symbol->code);

  switch (exp->fields.agr.agr) {
  case AGR_SUM: emit0(C_STMOP, I_SUM); break;
  case AGR_MAX: emit0(C_STMOP, I_MAX); break;
  case AGR_COUNT: emit0(C_STMOP, I_COUNT); break;
  default: syserr("Unrecognized switch in geexpagr()");
  }
}



/*----------------------------------------------------------------------
  geexprnd()

  Generate code for a random expression.

  */
static void geexprnd(ExpNod *exp) /* IN - The expression to generate */
{
  geexp(exp->fields.rnd.from);
  geexp(exp->fields.rnd.to);
  emit0(C_STMOP, I_RND);
}



/*----------------------------------------------------------------------
  geexpscore()

  Generate the code for a SCORE expression.

  */
static void geexpscore(ExpNod *exp) /* IN - The expression to generate */
{
  emit0(C_CURVAR, V_SCORE);
}



/*----------------------------------------------------------------------
  geexpwht()

  Generate the code for a WHAT expression.

  */
static void geexpwht(ExpNod *exp) /* IN - The expression to generate */
{
  gewht(exp->fields.wht.wht);
}



/*======================================================================

  generateBetweenCheck()

*/
void generateBetweenCheck(ExpNod *exp)
{
  geexp(exp->fields.btw.low);
  geexp(exp->fields.btw.high);
  emit0(C_STMOP, I_BTW);
}


/*----------------------------------------------------------------------
  geexpbtw()

  Generate code for a random expression.

  */
static void geexpbtw(ExpNod *exp) /* IN - The expression to generate */
{
  geexp(exp->fields.btw.val);
  generateBetweenCheck(exp);
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*======================================================================

  geexp()

  Generate the code for an expression.

  */
void geexp(ExpNod *exp)		/* IN - The expression to generate */
{
  if (exp == NULL) {
    emit0(C_CONST, 0);
    return;
  }
  
  switch (exp->class) {
    
  case EXPBIN:
    geexpbin(exp);
    break;
    
  case EXPWHR:
    geexpwhr(exp);
    break;
    
  case EXPATR:
    geexpatr(exp);
    break;
    
  case EXPINT:
    emit0(C_CONST, exp->fields.val.val);
    break;
    
  case EXPSTR:
    encode(&exp->fields.str.fpos, &exp->fields.str.len);
    emit0(C_CONST, exp->fields.str.len);
    emit0(C_CONST, exp->fields.str.fpos);
    emit0(C_STMOP, I_GETSTR);
    break;
    
  case EXPAGR:
    geexpagr(exp);
    break;
    
  case EXPRND:
    geexprnd(exp);
    break;
    
  case EXPSCORE:
    geexpscore(exp);
    break;

  case EXPWHT:
    geexpwht(exp);
    break;

  case EXPBTW:
    geexpbtw(exp);
    break;
    
  default:
    unimpl(&exp->srcp, "Code Generator");
    emit0(C_CONST, 0);
    return;
  }
}



/*----------------------------------------------------------------------
  dumpOperator()

  Dump an operator

  */
static void dumpOperator(OpKind op)
{
  switch (op) {
  case OP_AND:
    put("AND");
    break;
  case OP_OR:
    put("OR");
    break;
  case OP_NE:
    put("<>");
    break;
  case OP_EQ:
    put("=");
    break;
  case OP_EXACT:
    put("==");
    break;
  case OP_LE:
    put("<=");
    break;
  case OP_GE:
    put(">=");
    break;
  case OP_LT:
    put("LT");
    break;
  case OP_GT:
    put("GT");
    break;
  case OP_PLUS:
    put("PLUS");
    break;
  case OP_MINUS:
    put("MINUS");
    break;
  case OP_MULT:
    put("MULT");
    break;
  case OP_DIV:
    put("DIV");
    break;
  case OP_CONTAINS:
    put("CONTAINS");
    break;
  }
}


/*----------------------------------------------------------------------
  duagr()

  Dump an aggregate kind.

  */
static void duagr(AgrKind agr)
{
  switch (agr) {
  case AGR_SUM:
    put("SUM");
    break;
  case AGR_MAX:
    put("MAX");
    break;
  case AGR_COUNT:
    put("COUNT");
    break;
  }
}



/*======================================================================

  dumpType()

  Dump a type indication.

  */
void dumpType(TypeKind typ)
{
  switch (typ) {
  case BOOLEAN_TYPE:
    put("BOOLEAN");
    break;
  case INTEGER_TYPE:
    put("INTEGER");
    break;
  case STRING_TYPE:
    put("STRING");
    break;
  case INSTANCE_TYPE:
    put("INSTANCE");
    break;
  case UNKNOWN_TYPE:
    put("UNKNOWN");
    break;
  case ERROR_TYPE:
    put("ERROR");
    break;
  }
}



/*======================================================================

  dumpExpression()

  Dump an expression node.

 */
void dumpExpression(ExpNod *exp)
{
  if (exp == NULL) {
    put("NULL");
    return;
  }

  put("EXP: ");
  switch (exp->class) {
  case EXPBIN:
    put("BIN ");
    break;
  case EXPWHR:
    put("WHR ");
    if (exp->not) put("NOT ");
    break;
  case EXPATR:
    if (exp->not) put("NOT ");
    put("ATR ");
    break;
  case EXPINT:
    put("INT ");
    break;
  case EXPSTR:
    put("STR ");
    break;
  case EXPAGR:
    put("AGR ");
    break;
  case EXPRND:
    put("RND ");
    break;
  case EXPSCORE:
    put("SCORE ");
    break;
  case EXPWHT:
    put("WHT ");
    break; 
  case EXPBTW:
    if (exp->not) put("NOT ");
    put("BTW ");
    break;
  default:
    put("*** ERROR *** ");
    break;
  }
  dumpSrcp(&exp->srcp);
  in();

  switch (exp->class) {
  case EXPWHR:
    put("wht: "); dumpExpression(exp->fields.whr.wht); nl();
    put("whr: "); duwhr(exp->fields.whr.whr);
    break;
  case EXPATR:
    put("wht: "); dumpExpression(exp->fields.atr.wht); nl();
    put("atr: "); dumpId(exp->fields.atr.atr);
    break;
  case EXPINT:
    put("val: "); dumpInt(exp->fields.val.val);
    break;
  case EXPSTR:
    put("fpos: "); dumpInt(exp->fields.str.fpos); nl();
    put("len: "); dumpInt(exp->fields.str.len);
    break;
  case EXPBIN:
    put("operator: "); dumpOperator(exp->fields.bin.op); nl();
    put("left: "); dumpExpression(exp->fields.bin.left); nl();
    put("right: "); dumpExpression(exp->fields.bin.right);
    break;
  case EXPAGR:
    put("agr: "); duagr(exp->fields.agr.agr); nl();
    put("atr: "); dumpId(exp->fields.agr.atr); nl();
    put("whr: "); duwhr(exp->fields.agr.whr);
    break;
  case EXPRND:
    put("from: "); dumpExpression(exp->fields.rnd.from); nl();
    put("to: "); dumpExpression(exp->fields.rnd.to);
    break;
  case EXPWHT:
    put("wht: "); duwht(exp->fields.wht.wht);
    break;
  case EXPSCORE:
    break;
  case EXPBTW:
    put("val: "); dumpExpression(exp->fields.btw.val); nl();
    put("low: "); dumpExpression(exp->fields.btw.low); nl();
    put("high: "); dumpExpression(exp->fields.btw.high);
    break;
  case EXPISA:
    /* FIXME */
    break;
  }
  out();
}
