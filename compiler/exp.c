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

#include "../interpreter/acode.h"
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
Expression *newexp(Srcp *srcp, ExpressionKind kind)
{
  Expression *new;			/* The newly allocated area */

  showProgress();

  new = NEW(Expression);

  new->srcp = *srcp;
  new->kind = kind;
  new->not = FALSE;

  return(new);
}




/*----------------------------------------------------------------------*/
static void analyzeWhereExpression(Expression *exp, Context *context)
{
  analyzeExpression(exp->fields.whr.wht, context);
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
  case WHERE_AT:
    switch (exp->fields.whr.whr->what->kind) {
    case WHAT_ID:
      symcheck(exp->fields.whr.whr->what->id, INSTANCE_SYMBOL, context);
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
    verifyContainer(exp->fields.whr.whr->what, context);
    break;
  default:
    syserr("Unrecognized switch in anexpwhr()");
    break;
  }

  exp->type = BOOLEAN_TYPE;
}



/*----------------------------------------------------------------------*/
static TypeKind verifyExpressionAttribute(IdNode *attributeId,
					  Attribute *foundAttribute)
{
  if (foundAttribute == NULL) {	/* Attribute not found */
    return UNKNOWN_TYPE;
  } else if (foundAttribute->id->symbol == NULL) {
    attributeId->code = foundAttribute->id->code;
    return foundAttribute->type;
  } else
    syserr("Attribute with symbol in verifyExpressionAttribute()");
  return ERROR_TYPE;
}


/*----------------------------------------------------------------------*/
static void analyzeAttributeExpression(Expression *exp,
				       Context *context)
{
  Attribute *atr;

  if (exp->fields.atr.wht->kind == WHAT_EXPRESSION) {
    switch (exp->fields.atr.wht->fields.wht.wht->kind) {
      
    case WHAT_ACTOR:
      if (context->kind == EVENT_CONTEXT)
	lmLog(&exp->fields.atr.wht->fields.wht.wht->srcp, 412, sevERR, "");
      break;

    case WHAT_LOCATION:
    case WHAT_ID:
    case WHAT_THIS:
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

/*----------------------------------------------------------------------*/
static Bool isConstantIdentifier(IdNode *id)
{
  return id->symbol->kind != PARAMETER_SYMBOL
    && id->symbol->kind != LOCAL_SYMBOL;
}



/*----------------------------------------------------------------------*/
static void analyzeBinaryExpression(Expression *exp,
				    Context *context)
{
  analyzeExpression(exp->fields.bin.left, context);
  analyzeExpression(exp->fields.bin.right, context);

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
	  if (isConstantIdentifier(leftWhat->id)
	      && isConstantIdentifier(rightWhat->id))
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
static void anagr(Expression *exp,
		  Context *context)
{
  Attribute *atr = NULL;

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


/*----------------------------------------------------------------------*/
static void anrnd(Expression *exp,
		  Context *context)
{
  exp->type = INTEGER_TYPE;
  analyzeExpression(exp->fields.rnd.from, context);
  if (!equalTypes(INTEGER_TYPE, exp->fields.rnd.from->type)) {
    lmLog(&exp->fields.rnd.from->srcp, 413, sevERR, "RANDOM");
    exp->type = UNKNOWN_TYPE;
  }

  analyzeExpression(exp->fields.rnd.to, context);
  if (!equalTypes(INTEGER_TYPE, exp->fields.rnd.to->type)) {
    lmLog(&exp->fields.rnd.to->srcp, 413, sevERR, "RANDOM");
    exp->type = UNKNOWN_TYPE;
  }
}


/*----------------------------------------------------------------------

  anexpwht()

  Analyse a WHT expression.

  */
static void anexpwht(Expression *exp,
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
      case LOCAL_SYMBOL:
	exp->type = symbol->fields.local.type;
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
    if (context->kind == INSTANCE_CONTEXT || context->kind == CLASS_CONTEXT ||
	(context->kind == VERB_CONTEXT && (context->instance != NULL || context->class != NULL)))
      exp->type = INSTANCE_TYPE;
    else
      lmLog(&exp->fields.wht.wht->srcp, 421, sevERR, "");
    break;

  default:
    syserr("Unrecognized switch in anexpwht()");
    break;
  }
}


/*----------------------------------------------------------------------*/
static void anexpbtw(Expression *exp,
		     Context *context)
{
  analyzeExpression(exp->fields.btw.val, context);
  if (!equalTypes(exp->fields.btw.val->type, INTEGER_TYPE))
    lmLogv(&exp->fields.btw.val->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  analyzeExpression(exp->fields.btw.low, context);
  if (!equalTypes(exp->fields.btw.low->type, INTEGER_TYPE))
    lmLogv(&exp->fields.btw.low->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  analyzeExpression(exp->fields.btw.high, context);
  if (!equalTypes(exp->fields.btw.high->type, INTEGER_TYPE))
    lmLogv(&exp->fields.btw.high->srcp, 330, sevERR, "integer", "'BETWEEN'", NULL);

  exp->type = BOOLEAN_TYPE;
}


/*----------------------------------------------------------------------*/
static void analyzeIsaExpression(Expression *expression,
				 Context *context)
{
  switch (expression->fields.isa.wht->kind) {
  case WHAT_EXPRESSION:
    switch (expression->fields.isa.wht->fields.wht.wht->kind) {
    case WHAT_ID:
      symcheck(expression->fields.isa.wht->fields.wht.wht->id,
	       INSTANCE_SYMBOL, context);
      break;
    case WHAT_LOCATION:
    case WHAT_ACTOR:
      break;
    default:
      unimpl(&expression->srcp, "Analyzer");
      break;
    }
    break;
  default:
      unimpl(&expression->srcp, "Analyzer");
      break;
  }

  symcheck(expression->fields.isa.id, CLASS_SYMBOL, context);

  expression->type = BOOLEAN_TYPE;
}

/*======================================================================*/
void analyzeExpression(Expression *expression,
		       Context *context)
{
  if (expression == NULL) 	/* Ignore empty expressions (syntax error) */
    return;

  switch (expression->kind) {
    
  case WHERE_EXPRESSION:
    analyzeWhereExpression(expression, context);
    break;
    
  case ATTRIBUTE_EXPRESSION:
    analyzeAttributeExpression(expression, context);
    break;

  case BINARY_EXPRESSION:
    analyzeBinaryExpression(expression, context);
    break;
    
  case INTEGER_EXPRESSION:
    expression->type = INTEGER_TYPE;
    break;
    
  case STRING_EXPRESSION:
    expression->type = STRING_TYPE;
    break;
    
  case AGGREGATE_EXPRESSION:
    anagr(expression, context);
    break;
    
  case RANDOM_EXPRESSION:
    anrnd(expression, context);
    break;

  case SCORE_EXPRESSION:
    expression->type = INTEGER_TYPE;
    break;

  case WHAT_EXPRESSION:
    anexpwht(expression, context);
    break;

  case BETWEEN_EXPRESSION:
    anexpbtw(expression, context);
    break;

  case ISA_EXPRESSION:
    analyzeIsaExpression(expression, context);
    break;

  default:
    syserr("Unrecognized switch in analyzeExpression()");
    break;
  }
}


/*======================================================================

  genererateBinaryOperator()

*/
void generateBinaryOperator(Expression *exp)
{
  switch (exp->fields.bin.op) {
  case AND_OPERATOR:
    emit0(I_AND);
    break;
  case OR_OPERATOR:
    emit0(I_OR);
    break;
  case NE_OPERATOR:
    emit0(I_NE);
    break;
  case EQ_OPERATOR:
    if (exp->fields.bin.right->type == STRING_TYPE)
      emit0(I_STREQ);
    else
      emit0(I_EQ);
    break;
  case EXACT_OPERATOR:
    emit0(I_STREXACT);
    break;
  case LE_OPERATOR:
    emit0(I_LE);
    break;
  case GE_OPERATOR:
    emit0(I_GE);
    break;
  case LT_OPERATOR:
    emit0(I_LT);
    break;
  case GT_OPERATOR:
    emit0(I_GT);
    break;
  case PLUS_OPERATOR:
    emit0(I_PLUS);
    break;
  case MINUS_OPERATOR:
    emit0(I_MINUS);
    break;
  case MULT_OPERATOR:
    emit0(I_MULT);
    break;
  case DIV_OPERATOR:
    emit0(I_DIV);
    break;
  case CONTAINS_OPERATOR:
    emit0(I_CONTAINS);
    break;
  }
}


/*----------------------------------------------------------------------

  geexpbin()

  Generate a binary expression.

  */
static void generateBinaryExpression(Expression *exp)
{
  generateExpression(exp->fields.bin.left);
  generateExpression(exp->fields.bin.right);
  generateBinaryOperator(exp);
  if (exp->not) emit0(I_NOT);
}



/*----------------------------------------------------------------------
  geexpwhr()

  Generate a where-expression.

  */
static void generateWhereExpression(Expression *exp)
{
  switch(exp->fields.whr.wht->fields.wht.wht->kind) {
    
  case WHAT_ID:
    switch (exp->fields.whr.whr->kind) {
    case WHR_HERE:
      generateId(exp->fields.whr.wht->fields.wht.wht->id);
      emit0(I_HERE);
      if (exp->not) emit0(I_NOT);
      return;
    case WHR_NEAR:
      generateWhat(exp->fields.whr.wht->fields.wht.wht);
      emit0(I_NEAR);
      if (exp->not) emit0(I_NOT);
      return;
    case WHR_IN:
      generateId(exp->fields.whr.whr->what->id);
      generateWhat(exp->fields.whr.wht->fields.wht.wht);
      emit0(I_IN);
      if (exp->not) emit0(I_NOT);
      return;
    case WHERE_AT:
      generateWhat(exp->fields.whr.wht->fields.wht.wht);
      emit0(I_WHERE);
      break;
    default:
      unimpl(&exp->srcp, "Code Generator");
      emitConstant(0);
      return;
    }
    break;
    
  default:
    unimpl(&exp->srcp, "Code Generator");
    emitConstant(0);
    return;
  }
  
  generateWhere(exp->fields.whr.whr);
  emit0(I_EQ);
  if (exp->not) emit0(I_NOT);
}



/*======================================================================

  generateAttributeAccess()

*/
void generateAttributeAccess(Expression *exp)
{
  if (exp->type == STRING_TYPE)
    emit0(I_STRATTR);
  else
    emit0(I_ATTRIBUTE);
}


/*----------------------------------------------------------------------
  geexpatr()

  Generate an attribute-expression.

 */
static void generateAttributeExpression(Expression *exp)
{
  generateId(exp->fields.atr.atr);
  generateWhat(exp->fields.atr.wht->fields.wht.wht);
  generateAttributeAccess(exp);
  if (exp->not) emit0(I_NOT);
}




/*----------------------------------------------------------------------
  geexpagr()

  Generate the code for an aggregate expression.

  */
static void generateAggregateExpression(Expression *exp)
{
  generateWhere(exp->fields.agr.whr);

  if (exp->fields.agr.kind != COUNT_AGGREGATE)
    emitConstant(exp->fields.agr.atr->symbol->code);

  switch (exp->fields.agr.kind) {
  case SUM_AGGREGATE: emit0(I_SUM); break;
  case MAX_AGGREGATE: emit0(I_MAX); break;
  case COUNT_AGGREGATE: emit0(I_COUNT); break;
  default: syserr("Unrecognized switch in geexpagr()");
  }
}



/*----------------------------------------------------------------------
  geexprnd()

  Generate code for a random expression.

  */
static void generateRandomExpression(Expression *exp)
{
  generateExpression(exp->fields.rnd.to);
  generateExpression(exp->fields.rnd.from);
  emit0(I_RND);
}



/*----------------------------------------------------------------------
  geexpscore()

  Generate the code for a SCORE expression.

  */
static void geexpscore(Expression *exp) /* IN - The expression to generate */
{
  emitVariable(V_SCORE);
}



/*----------------------------------------------------------------------
  geexpwht()

  Generate the code for a WHAT expression.

  */
static void generateWhatExpression(Expression *exp)
{
  generateWhat(exp->fields.wht.wht);
}



/*======================================================================

  generateBetweenCheck()

*/
void generateBetweenCheck(Expression *exp)
{
  generateExpression(exp->fields.btw.low);
  generateExpression(exp->fields.btw.high);
  emit0(I_BTW);
}


/*----------------------------------------------------------------------
  geexpbtw()

  Generate code for a random expression.

  */
static void generateBetweenExpression(Expression *exp)
{
  generateExpression(exp->fields.btw.val);
  generateBetweenCheck(exp);
  if (exp->not) emit0(I_NOT);
}


/*----------------------------------------------------------------------*/
static void generateIsaExpression(Expression *exp)
{
  generateExpression(exp->fields.isa.wht);
  generateId(exp->fields.isa.id);
  emit0(I_ISA);
  if (exp->not) emit0(I_NOT);
}



/*======================================================================*/
void generateExpression(Expression *exp)
{
  if (exp == NULL) {
    syserr("Generating a NULL expression");
    emitConstant(0);
    return;
  }
  
  switch (exp->kind) {
    
  case BINARY_EXPRESSION:
    generateBinaryExpression(exp);
    break;
    
  case WHERE_EXPRESSION:
    generateWhereExpression(exp);
    break;
    
  case ATTRIBUTE_EXPRESSION:
    generateAttributeExpression(exp);
    break;
    
  case INTEGER_EXPRESSION:
    emitConstant(exp->fields.val.val);
    break;
    
  case STRING_EXPRESSION:
    encode(&exp->fields.str.fpos, &exp->fields.str.len);
    emit2(I_GETSTR, exp->fields.str.fpos, exp->fields.str.len);
    break;
    
  case AGGREGATE_EXPRESSION:
    generateAggregateExpression(exp);
    break;
    
  case RANDOM_EXPRESSION:
    generateRandomExpression(exp);
    break;
    
  case SCORE_EXPRESSION:
    geexpscore(exp);
    break;

  case WHAT_EXPRESSION:
    generateWhatExpression(exp);
    break;

  case BETWEEN_EXPRESSION:
    generateBetweenExpression(exp);
    break;

  case ISA_EXPRESSION:
    generateIsaExpression(exp);
    break;
    
  default:
    unimpl(&exp->srcp, "Code Generator");
    emitConstant(0);
    return;
  }
}



/*----------------------------------------------------------------------*/
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
void dumpExpression(Expression *exp)
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
