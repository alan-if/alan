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
ExpNod *newexp(Srcp *srcp, ExpressionKind kind)
{
  ExpNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(ExpNod);

  new->srcp = *srcp;
  new->kind = kind;
  new->not = FALSE;

  return(new);
}




/*----------------------------------------------------------------------

  anexpwhr()

  Analyze a WHR expression.

 */
static void anexpwhr(ExpNod *exp, Context *context)
{
  anexp(exp->fields.whr.wht, context);
  if (exp->fields.whr.wht->kind != WHAT_EXPRESSION)
    lmLog(&exp->fields.whr.wht->srcp, 311, sevERR, "an instance");
  else {
    switch (exp->fields.whr.wht->fields.wht.wht->kind) {
    case WHAT_ACTOR:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&exp->fields.whr.wht->srcp, 412, sevERR, "");
      break;
    case WHAT_LOCATION:
      lmLog(&exp->fields.whr.wht->srcp, 311, sevERR, "an Object or an Actor");
      break;
    case WHAT_ID:
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
    case WHAT_ID:
      symcheck(exp->fields.whr.whr->wht->id, INSTANCE_SYMBOL, context);
      break;
    case WHAT_LOCATION:
      exp->fields.whr.whr->kind = WHR_HERE;
      break;
    case WHAT_ACTOR:
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

  verifyExpressionAttribute()

*/
static TypeKind verifyExpressionAttribute(IdNode *attributeId, AtrNod *foundAttribute)
{
  if (foundAttribute == NULL) {	/* Attribute not found */
    return UNKNOWN_TYPE;
  } else if (foundAttribute->id->symbol == NULL) {
    attributeId->code = foundAttribute->id->code;
    return foundAttribute->type;
  } else
    syserr("Attribute with symbol in anexpatr()");
  return ERROR_TYPE;
}


/*----------------------------------------------------------------------

  anexpatr()

  Analyze an ATR expression.

  */
static void anexpatr(ExpNod *exp, /* IN - The expression to analyze */
		     Context *context)
{
  AtrNod *atr;

  if (exp->fields.atr.wht->kind == WHAT_EXPRESSION) {
    switch (exp->fields.atr.wht->fields.wht.wht->kind) {
      
    case WHAT_ACTOR:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 412, sevERR, "");
      break;

    case WHAT_LOCATION:
    case WHAT_ID:
      break;

    default:
      syserr("Unrecognized switch in anexpatr()");
      break;
    }

    atr = resolveAttributeReference(exp->fields.atr.wht->fields.wht.wht,
				    exp->fields.atr.atr, context);
    exp->type = verifyExpressionAttribute(exp->fields.atr.atr, atr);

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
  case AND_OPERATOR:
  case OR_OPERATOR:
    if (!equalTypes(exp->fields.bin.left->type, BOOLEAN_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "boolean", "AND/OR", NULL);
    if (!equalTypes(exp->fields.bin.right->type, BOOLEAN_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "boolean", "AND/OR", NULL);
    exp->type = BOOLEAN_TYPE;
    break;

  case NE_OPERATOR:
  case EQ_OPERATOR:
    if (!equalTypes(exp->fields.bin.left->type, exp->fields.bin.right->type))
      lmLog(&exp->srcp, 331, sevERR, "expression");
    else if (exp->fields.bin.left->type != UNKNOWN_TYPE && exp->fields.bin.right->type != UNKNOWN_TYPE)
      if (exp->fields.bin.left->type == INSTANCE_TYPE) {
	What *leftWhat = exp->fields.bin.left->fields.wht.wht;
	What *rightWhat = exp->fields.bin.right->fields.wht.wht;
	if (leftWhat->kind == WHAT_ID && rightWhat->kind == WHAT_ID)
	  if (leftWhat->id->symbol->kind != PARAMETER_SYMBOL
	      && rightWhat->id->symbol->kind != PARAMETER_SYMBOL)
	    lmLog(&exp->srcp, 417, sevINF, NULL);
      }
    exp->type = BOOLEAN_TYPE;
    break;

  case EXACT_OPERATOR:
    if (!equalTypes(exp->fields.bin.left->type, STRING_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "string", "'=='", NULL);
    if (!equalTypes(exp->fields.bin.right->type, STRING_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "string", "'=='", NULL);
    break;
	    
  case LE_OPERATOR:
  case GE_OPERATOR:
  case LT_OPERATOR:
  case GT_OPERATOR:
    if (!equalTypes(exp->fields.bin.left->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "integer", "relational", NULL);
    if (!equalTypes(exp->fields.bin.right->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "integer", "relational", NULL);
    exp->type = BOOLEAN_TYPE;
    break;

  case PLUS_OPERATOR:
  case MINUS_OPERATOR:
  case MULT_OPERATOR:
  case DIV_OPERATOR:
    if (!equalTypes(exp->fields.bin.left->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.left->srcp, 330, sevERR, "integer", "arithmetic", NULL);
    if (!equalTypes(exp->fields.bin.right->type, INTEGER_TYPE))
      lmLogv(&exp->fields.bin.right->srcp, 330, sevERR, "integer", "arithmetic", NULL);
    exp->type = INTEGER_TYPE;
    break;

  case CONTAINS_OPERATOR:
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
  if (exp->fields.agr.kind != COUNT_AGGREGATE) {
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

  case WHAT_LOCATION:
    exp->type = INSTANCE_TYPE;
    break;

  case WHAT_ACTOR:
    if (context->kind == EVENT_CONTEXT)
      lmLog(&exp->fields.wht.wht->srcp, 412, sevERR, "");
    exp->type = INSTANCE_TYPE;
    break;

  case WHAT_ID:
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

  case WHAT_THIS:
    if (context->instance == NULL)
      lmLog(&exp->fields.wht.wht->srcp, 421, sevERR, "");
    else
      exp->type = INSTANCE_TYPE;
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
  
  switch (exp->kind) {
    
  case WHERE_EXPRESSION:
    anexpwhr(exp, context);
    break;
    
  case ATTRIBUTE_EXPRESSION:
    anexpatr(exp, context);
    break;

  case BINARY_EXPRESSION:
    anbin(exp, context);
    break;
    
  case INTEGER_EXPRESSION:
    exp->type = INTEGER_TYPE;
    break;
    
  case STRING_EXPRESSION:
    exp->type = STRING_TYPE;
    break;
    
  case AGGREGATE_EXPRESSION:
    anagr(exp, context);
    break;
    
  case RANDOM_EXPRESSION:
    anrnd(exp, context);
    break;

  case SCORE_EXPRESSION:
    exp->type = INTEGER_TYPE;
    break;

  case WHAT_EXPRESSION:
    anexpwht(exp, context);
    break;

  case BETWEEN_EXPRESSION:
    anexpbtw(exp, context);
    break;

  case ISA_EXPRESSION:
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
  case AND_OPERATOR:
    emit0(C_STMOP, I_AND);
    break;
  case OR_OPERATOR:
    emit0(C_STMOP, I_OR);
    break;
  case NE_OPERATOR:
    emit0(C_STMOP, I_NE);
    break;
  case EQ_OPERATOR:
    if (exp->fields.bin.right->type == STRING_TYPE)
      emit0(C_STMOP, I_STREQ);
    else
      emit0(C_STMOP, I_EQ);
    break;
  case EXACT_OPERATOR:
    emit0(C_STMOP, I_STREXACT);
    break;
  case LE_OPERATOR:
    emit0(C_STMOP, I_LE);
    break;
  case GE_OPERATOR:
    emit0(C_STMOP, I_GE);
    break;
  case LT_OPERATOR:
    emit0(C_STMOP, I_LT);
    break;
  case GT_OPERATOR:
    emit0(C_STMOP, I_GT);
    break;
  case PLUS_OPERATOR:
    emit0(C_STMOP, I_PLUS);
    break;
  case MINUS_OPERATOR:
    emit0(C_STMOP, I_MINUS);
    break;
  case MULT_OPERATOR:
    emit0(C_STMOP, I_MULT);
    break;
  case DIV_OPERATOR:
    emit0(C_STMOP, I_DIV);
    break;
  case CONTAINS_OPERATOR:
    emit0(C_STMOP, I_CONTAINS);
    break;
  }
}


/*----------------------------------------------------------------------

  geexpbin()

  Generate a binary expression.

  */
static void geexpbin(ExpNod *exp, int currentInstance)
{
  geexp(exp->fields.bin.left, currentInstance);
  geexp(exp->fields.bin.right, currentInstance);
  generateBinaryOperator(exp);
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*----------------------------------------------------------------------
  geexpwhr()

  Generate a where-expression.

  */
static void geexpwhr(ExpNod *exp, int currentInstance)
{
  switch(exp->fields.whr.wht->fields.wht.wht->kind) {
    
  case WHAT_ID:
    switch (exp->fields.whr.whr->kind) {
    case WHR_HERE:
      generateId(exp->fields.whr.wht->fields.wht.wht->id);
      emit0(C_STMOP, I_HERE);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_NEAR:
      generateWhat(exp->fields.whr.wht->fields.wht.wht, currentInstance);
      emit0(C_STMOP, I_NEAR);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_IN:
      generateId(exp->fields.whr.whr->wht->id);
      generateWhat(exp->fields.whr.wht->fields.wht.wht, currentInstance);
      emit0(C_STMOP, I_IN);
      if (exp->not) emit0(C_STMOP, I_NOT);
      return;
    case WHR_AT:
      generateWhat(exp->fields.whr.wht->fields.wht.wht, currentInstance);
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
  
  gewhr(exp->fields.whr.whr, currentInstance);
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
static void geexpatr(ExpNod *exp, int currentInstance)
{
  generateId(exp->fields.atr.atr);
  generateWhat(exp->fields.atr.wht->fields.wht.wht, currentInstance);
  generateAttributeAccess(exp);
  if (exp->not) emit0(C_STMOP, I_NOT);
}




/*----------------------------------------------------------------------
  geexpagr()

  Generate the code for an aggregate expression.

  */
static void geexpagr(ExpNod *exp, int currentInstance)
{
  gewhr(exp->fields.agr.whr, currentInstance);

  if (exp->fields.agr.kind != COUNT_AGGREGATE)
    emit0(C_CONST, exp->fields.agr.atr->symbol->code);

  switch (exp->fields.agr.kind) {
  case SUM_AGGREGATE: emit0(C_STMOP, I_SUM); break;
  case MAX_AGGREGATE: emit0(C_STMOP, I_MAX); break;
  case COUNT_AGGREGATE: emit0(C_STMOP, I_COUNT); break;
  default: syserr("Unrecognized switch in geexpagr()");
  }
}



/*----------------------------------------------------------------------
  geexprnd()

  Generate code for a random expression.

  */
static void geexprnd(ExpNod *exp, int currentInstance)
{
  geexp(exp->fields.rnd.from, currentInstance);
  geexp(exp->fields.rnd.to, currentInstance);
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
static void geexpwht(ExpNod *exp, int currentInstance)
{
  generateWhat(exp->fields.wht.wht, currentInstance);
}



/*======================================================================

  generateBetweenCheck()

*/
void generateBetweenCheck(ExpNod *exp, int currentInstance)
{
  geexp(exp->fields.btw.low, currentInstance);
  geexp(exp->fields.btw.high, currentInstance);
  emit0(C_STMOP, I_BTW);
}


/*----------------------------------------------------------------------
  geexpbtw()

  Generate code for a random expression.

  */
static void geexpbtw(ExpNod *exp, int currentInstance)
{
  geexp(exp->fields.btw.val, currentInstance);
  generateBetweenCheck(exp, currentInstance);
  if (exp->not) emit0(C_STMOP, I_NOT);
}



/*======================================================================

  geexp()

  Generate the code for an expression.

  */
void geexp(ExpNod *exp, int currentInstance)
{
  if (exp == NULL) {
    emit0(C_CONST, 0);
    return;
  }
  
  switch (exp->kind) {
    
  case BINARY_EXPRESSION:
    geexpbin(exp, currentInstance);
    break;
    
  case WHERE_EXPRESSION:
    geexpwhr(exp, currentInstance);
    break;
    
  case ATTRIBUTE_EXPRESSION:
    geexpatr(exp, currentInstance);
    break;
    
  case INTEGER_EXPRESSION:
    emit0(C_CONST, exp->fields.val.val);
    break;
    
  case STRING_EXPRESSION:
    encode(&exp->fields.str.fpos, &exp->fields.str.len);
    emit0(C_CONST, exp->fields.str.len);
    emit0(C_CONST, exp->fields.str.fpos);
    emit0(C_STMOP, I_GETSTR);
    break;
    
  case AGGREGATE_EXPRESSION:
    geexpagr(exp, currentInstance);
    break;
    
  case RANDOM_EXPRESSION:
    geexprnd(exp, currentInstance);
    break;
    
  case SCORE_EXPRESSION:
    geexpscore(exp);
    break;

  case WHAT_EXPRESSION:
    geexpwht(exp, currentInstance);
    break;

  case BETWEEN_EXPRESSION:
    geexpbtw(exp, currentInstance);
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
static void dumpOperator(OperatorKind op)
{
  switch (op) {
  case AND_OPERATOR:
    put("AND");
    break;
  case OR_OPERATOR:
    put("OR");
    break;
  case NE_OPERATOR:
    put("<>");
    break;
  case EQ_OPERATOR:
    put("=");
    break;
  case EXACT_OPERATOR:
    put("==");
    break;
  case LE_OPERATOR:
    put("<=");
    break;
  case GE_OPERATOR:
    put(">=");
    break;
  case LT_OPERATOR:
    put("LT");
    break;
  case GT_OPERATOR:
    put("GT");
    break;
  case PLUS_OPERATOR:
    put("PLUS");
    break;
  case MINUS_OPERATOR:
    put("MINUS");
    break;
  case MULT_OPERATOR:
    put("MULT");
    break;
  case DIV_OPERATOR:
    put("DIV");
    break;
  case CONTAINS_OPERATOR:
    put("CONTAINS");
    break;
  }
}


/*----------------------------------------------------------------------

  dumpAggregateKind()

  */
static void dumpAggregateKind(AggregateKind agr)
{
  switch (agr) {
  case SUM_AGGREGATE: put("SUM"); break;
  case MAX_AGGREGATE: put("MAX"); break;
  case COUNT_AGGREGATE: put("COUNT"); break;
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
  switch (exp->kind) {
  case BINARY_EXPRESSION:
    put("BIN ");
    break;
  case WHERE_EXPRESSION:
    put("WHR ");
    if (exp->not) put("NOT ");
    break;
  case ATTRIBUTE_EXPRESSION:
    if (exp->not) put("NOT ");
    put("ATR ");
    break;
  case INTEGER_EXPRESSION:
    put("INT ");
    break;
  case STRING_EXPRESSION:
    put("STR ");
    break;
  case AGGREGATE_EXPRESSION:
    put("AGR ");
    break;
  case RANDOM_EXPRESSION:
    put("RND ");
    break;
  case SCORE_EXPRESSION:
    put("SCORE ");
    break;
  case WHAT_EXPRESSION:
    put("WHT ");
    break; 
  case BETWEEN_EXPRESSION:
    if (exp->not) put("NOT ");
    put("BTW ");
    break;
  default:
    put("*** ERROR *** ");
    break;
  }
  dumpSrcp(&exp->srcp);
  in();

  switch (exp->kind) {
  case WHERE_EXPRESSION:
    put("wht: "); dumpExpression(exp->fields.whr.wht); nl();
    put("whr: "); duwhr(exp->fields.whr.whr);
    break;
  case ATTRIBUTE_EXPRESSION:
    put("wht: "); dumpExpression(exp->fields.atr.wht); nl();
    put("atr: "); dumpId(exp->fields.atr.atr);
    break;
  case INTEGER_EXPRESSION:
    put("val: "); dumpInt(exp->fields.val.val);
    break;
  case STRING_EXPRESSION:
    put("fpos: "); dumpInt(exp->fields.str.fpos); nl();
    put("len: "); dumpInt(exp->fields.str.len);
    break;
  case BINARY_EXPRESSION:
    put("operator: "); dumpOperator(exp->fields.bin.op); nl();
    put("left: "); dumpExpression(exp->fields.bin.left); nl();
    put("right: "); dumpExpression(exp->fields.bin.right);
    break;
  case AGGREGATE_EXPRESSION:
    put("kind: "); dumpAggregateKind(exp->fields.agr.kind); nl();
    put("atr: "); dumpId(exp->fields.agr.atr); nl();
    put("whr: "); duwhr(exp->fields.agr.whr);
    break;
  case RANDOM_EXPRESSION:
    put("from: "); dumpExpression(exp->fields.rnd.from); nl();
    put("to: "); dumpExpression(exp->fields.rnd.to);
    break;
  case WHAT_EXPRESSION:
    put("wht: "); dumpWhat(exp->fields.wht.wht);
    break;
  case SCORE_EXPRESSION:
    break;
  case BETWEEN_EXPRESSION:
    put("val: "); dumpExpression(exp->fields.btw.val); nl();
    put("low: "); dumpExpression(exp->fields.btw.low); nl();
    put("high: "); dumpExpression(exp->fields.btw.high);
    break;
  case ISA_EXPRESSION:
    /* FIXME */
    break;
  }
  out();
}
