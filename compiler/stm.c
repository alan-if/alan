/*----------------------------------------------------------------------*\

                                STM.C
			   Statement Nodes

\*----------------------------------------------------------------------*/

#include "stm_x.h"


#include "alan.h"
#include "util.h"

#include "id_x.h"
#include "lst_x.h"
#include "srcp_x.h"
#include "context_x.h"
#include "adv_x.h"
#include "atr_x.h"
#include "cnt_x.h"
#include "exp_x.h"
#include "set_x.h"
#include "sym_x.h"
#include "whr_x.h"
#include "wht_x.h"
#include "form_x.h"

#include "lmList.h"

#include "scr.h"                /* SCR-nodes */
#include "sco.h"                /* SCORES */
#include "opt.h"                /* OPTIONS */

#include "emit.h"
#include "encode.h"

#include "dump.h"




/*======================================================================*/
Statement *newStatement(Srcp *srcp, StmKind class)
{
  Statement *new;                  /* The newly allocated area */

  showProgress();

  new = NEW(Statement);

  new->srcp = *srcp;
  new->kind = class;

  return(new);
}

/*======================================================================*/
Statement *newDescribeStatement(Srcp srcp, Expression *what)
{
  Statement *new;                  /* The newly allocated area */

  showProgress();

  new = newStatement(&srcp, DESCRIBE_STATEMENT);

  new->fields.describe.what = what;

  return(new);
}



/*======================================================================*/
Statement *newLocateStatement(Srcp srcp, Expression *what, Where *where)
{
  Statement *new;                  /* The newly allocated area */

  showProgress();

  new = newStatement(&srcp, LOCATE_STATEMENT);

  new->fields.locate.what = what;
  new->fields.locate.where = where;

  return(new);
}


/*======================================================================*/
Statement *newEmptyStatement(Srcp srcp, Expression *what, Where *where)
{
  Statement *new;                  /* The newly allocated area */

  showProgress();

  new = newStatement(&srcp, EMPTY_STATEMENT);

  new->fields.empty.what = what;
  new->fields.empty.where = where;

  return(new);
}


/*======================================================================*/
Statement *newIncludeStatement(Srcp srcp, Expression *what, Expression *set)
{
  Statement *new;                  /* The newly allocated area */

  showProgress();

  new = newStatement(&srcp, INCLUDE_STATEMENT);

  new->fields.include.what = what;
  new->fields.include.set = set;

  return(new);
}


/*======================================================================*/
Statement *newExcludeStatement(Srcp srcp, Expression *what, Expression *set)
{
  Statement *new;                  /* The newly allocated area */

  showProgress();

  new = newStatement(&srcp, EXCLUDE_STATEMENT);

  new->fields.include.what = what;
  new->fields.include.set = set;

  return(new);
}



/*======================================================================*/
Statement *newEachStatement(Srcp srcp, IdNode *loopId, List *filters, List *statements)
{
  Statement *new;                  /* The newly allocated area */

  showProgress();

  new = newStatement(&srcp, EACH_STATEMENT);

  new->fields.each.loopId = loopId;
  new->fields.each.filters = filters;
  new->fields.each.stms = statements;

  return(new);
}




/*----------------------------------------------------------------------*/
static void analyzeDescribe(Statement *stm, Context *context)
{
  analyzeExpression(stm->fields.describe.what, context);
}



/*----------------------------------------------------------------------*/
static void analyzeSay(Statement *stm, Context *context)
{
  analyzeExpression(stm->fields.say.exp, context);

  /* Can't say Boolean values or Sets */
  if (stm->fields.say.exp->type == BOOLEAN_TYPE || stm->fields.say.exp->type == SET_TYPE)
    lmLog(&stm->srcp, 337, sevERR, typeToString(stm->fields.say.exp->type));

  /* Can only say definite/indefinite forms of instances */
  if (stm->fields.say.form != SAY_SIMPLE
      && stm->fields.say.exp->type != INSTANCE_TYPE
      && stm->fields.say.exp->type != UNINITIALIZED_TYPE)
    lmLog(&stm->srcp, 339, sevERR, "");
}


/*----------------------------------------------------------------------*/
static void analyzeList(Statement *stm, Context *context)	
{
  analyzeExpression(stm->fields.list.wht, context);
  verifyContainerExpression(stm->fields.list.wht, context, "LIST statement");
}


/*----------------------------------------------------------------------*/
static void analyzeEmpty(Statement *stm, Context *context)
{
  analyzeExpression(stm->fields.empty.what, context);
  verifyContainerExpression(stm->fields.empty.what, context, "EMPTY statement");
  analyzeWhere(stm->fields.empty.where, context);
  if (stm->fields.empty.where->kind == WHERE_NEAR)
    lmLog(&stm->fields.empty.where->srcp, 415, sevERR, "LOCATE");
}


/*----------------------------------------------------------------------*/
static void analyzeLocate(Statement *stm, Context *context)
{
  Symbol *whtSymbol = NULL;
  Symbol *contentClass = NULL;
  Expression *what = stm->fields.locate.what;
  Where *whr = stm->fields.locate.where;

  analyzeExpression(what, context);
  if (what->type != ERROR_TYPE) {
    if (what->type != INSTANCE_TYPE)
      lmLogv(&what->srcp, 428, sevERR, "What-clause in Locate statement", "an instance", NULL);
    else if (inheritsFrom(what->class, locationSymbol))
      lmLog(&what->srcp, 405, sevERR, "be used in Locate statement");
    else
      whtSymbol = what->class;
  }
      
  analyzeWhere(whr, context);

  switch (whr->kind) {
  case WHERE_HERE:
  case WHERE_AT:
    break;
  case WHERE_IN:
    contentClass = contentOf(whr->what, context);
    if (contentClass != NULL && whtSymbol != NULL)
      if (!inheritsFrom(whtSymbol, contentClass))
	lmLog(&whr->srcp, 404, sevERR, contentClass->string);
    break;
  case WHERE_NEAR:
    lmLog(&stm->srcp, 415, sevERR, "LOCATE");
    break;
  default:
    syserr("Unexpected Where kind in '%s()'", __FUNCTION__);
    break;
  }
}



/*----------------------------------------------------------------------*/
static void verifyMakeAttribute(IdNode *attributeId, Attribute *foundAttribute)
{
  /* Verify that a found attribute can be used in a MAKE statement. */
  if (foundAttribute != NULL) {
    if (foundAttribute->type != BOOLEAN_TYPE && foundAttribute->type != ERROR_TYPE)
      lmLogv(&attributeId->srcp, 408, sevERR, "Attribute", "MAKE statement", "boolean", NULL);
    else
      attributeId->code = foundAttribute->id->code;
  }
}


/*----------------------------------------------------------------------*/
static void analyzeMake(Statement *stm, Context *context)
{
  Expression *wht = stm->fields.make.wht;
  Attribute *atr = NULL;

  analyzeExpression(wht, context);
  atr = resolveAttribute(wht, stm->fields.make.atr, context);
  verifyMakeAttribute(stm->fields.make.atr, atr);
}



/*----------------------------------------------------------------------*/
static void analyzeSet(Statement *stm, Context *context)
{
  Expression *exp = stm->fields.set.exp;
  Expression *wht = stm->fields.set.wht;

  analyzeExpression(wht, context);
  if (wht->type != ERROR_TYPE)
    if (wht->type == BOOLEAN_TYPE)
      lmLog(&wht->srcp, 419, sevERR, "Target for");

  analyzeExpression(exp, context);
  if (exp->type != ERROR_TYPE)
    if (exp->type == BOOLEAN_TYPE)
      lmLog(&exp->srcp, 419, sevERR, "Expression in");

  if (!equalTypes(exp->type, wht->type))
    lmLog(&stm->srcp, 331, sevERR, "target and expression in SET statement");
  else
    if (exp->type == INSTANCE_TYPE && exp->class != NULL && wht->class != NULL)
      if (!inheritsFrom(exp->class, wht->class))
	lmLog(&exp->srcp, 430, sevERR, wht->class->string);
}


/*----------------------------------------------------------------------*/
static void analyzeIncrease(Statement *stm, Context *context)
{
  analyzeExpression(stm->fields.incr.wht, context);

  if (stm->fields.incr.step != NULL) {
    analyzeExpression(stm->fields.incr.step, context);
    if (stm->fields.incr.step->type != INTEGER_TYPE
	&& stm->fields.incr.step->type != ERROR_TYPE)
      lmLogv(&stm->fields.incr.step->srcp, 408, sevERR, "Expression",
	     stm->kind==INCREASE_STATEMENT?"INCREASE statement":"DECREASE statement",
	     "integer", NULL);
  }
}


/*----------------------------------------------------------------------*/
static void analyzeIncludeAndRemove(Statement *stm, Context *context)
{
  Expression *what = stm->fields.include.what;
  Expression *set = stm->fields.include.set;
  char *message = stm->kind == INCLUDE_STATEMENT?"INCLUDE statement"
    :"REMOVE statement";

  analyzeExpression(what, context);
  analyzeExpression(set, context);
  if (set->type != ERROR_TYPE) {
    if (set->type != SET_TYPE)
      lmLogv(&set->srcp, 330, sevERR, "Set", message, NULL);
    else
      verifySetMember(set, what, message);
  }
}

/*----------------------------------------------------------------------*/
static void analyzeSchedule(Statement *stm, Context *context)
{
  Symbol *sym;

  sym = symcheck(stm->fields.schedule.id, EVENT_SYMBOL, NULL);

  /* Now lookup where */
  analyzeWhere(stm->fields.schedule.whr, context);
  switch (stm->fields.schedule.whr->kind) {
  case WHERE_DEFAULT:
    stm->fields.schedule.whr->kind = WHERE_HERE;
    break;
  case WHERE_HERE:
  case WHERE_AT:
    break;
  case WHERE_IN:
  case WHERE_NEAR:
    lmLog(&stm->fields.schedule.whr->srcp, 415, sevERR, "SCHEDULE");
    break;
  default:
    syserr("Unrecognized switch in '%s()'", __FUNCTION__);
    break;
  }

  /* Analyze the when (AFTER) expression */
  analyzeExpression(stm->fields.schedule.when, context);
  if (stm->fields.schedule.when->type != INTEGER_TYPE)
    lmLog(&stm->fields.schedule.when->srcp, 413, sevERR, "When-clause of SCHEDULE statement");

}


/*----------------------------------------------------------------------*/
static void analyzeCancel(Statement *stm) /* IN - The statement to analyze */
{
  Symbol *sym;

  sym = symcheck(stm->fields.cancel.id, EVENT_SYMBOL, NULL);
}


/*----------------------------------------------------------------------*/
static void analyzeIf(Statement *stm, Context *context)
{
  analyzeExpression(stm->fields.iff.exp, context);
  if (!equalTypes(stm->fields.iff.exp->type, BOOLEAN_TYPE))
    lmLogv(&stm->fields.iff.exp->srcp, 330, sevERR, "boolean", "'IF'", NULL);
  analyzeStatements(stm->fields.iff.thn, context);
  if (stm->fields.iff.els != NULL)
    analyzeStatements(stm->fields.iff.els, context);
}


/*----------------------------------------------------------------------*/
static void analyzeUse(Statement *stm, Context *context)
{
  /* Analyze a USE statement. It must refer to a script that is
  defined within the mentioned actor. If the actor is not specified
  the actor is assumed to be the one we are in (it is an error if we
  are not). */

  Symbol *symbol;
  Script *script;

  if (stm->fields.use.actorExp == NULL && context->instance == NULL)
    lmLog(&stm->srcp, 401, sevERR, "");
  else {
    if (stm->fields.use.actorExp != NULL) {
      Expression *exp = stm->fields.use.actorExp;
      analyzeExpression(exp, context);
      if (exp->type != ERROR_TYPE)
	if (exp->type != INSTANCE_TYPE || !inheritsFrom(exp->class, actorSymbol)) {
	  lmLogv(&exp->srcp, 351, sevERR, "USE statement", "an instance", "actor", NULL);
	  return;
	}
      symbol = symbolOfExpression(exp, context);
    } else {
      if (context->instance == NULL && context->instance->props == NULL)
	syserr("Unexpected context in '%s()'", __FUNCTION__);
      symbol = context->instance->props->id->symbol;
    }

    /* Find the script */
    if (symbol != NULL) {
      script = lookupScript(symbol, stm->fields.use.script);
      if (script != NULL)
	stm->fields.use.script->code = script->id->code;
      else {
	char *str;
	switch (symbol->kind) {
	case CLASS_SYMBOL: str = "class"; break;
	case INSTANCE_SYMBOL: str = "actor"; break;
	case PARAMETER_SYMBOL: str = "parameter"; break;
	default: syserr("Unexpected symbol kind in '%s()'", __FUNCTION__);
	}
	lmLogv(&stm->fields.use.script->srcp, 400, sevERR,
	       str, symbol->string, NULL);
      }
    }
  }  
}


/*----------------------------------------------------------------------*/
static void analyzeStop(Statement *stm, Context *context)
{
  Symbol *sym;
  Expression *exp = stm->fields.stop.actor;

  analyzeExpression(exp, context);
  if (exp->type != ERROR_TYPE) {
    sym = symbolOfExpression(exp, context);
    if (sym) {
      if (!inheritsFrom(sym, actorSymbol))
	lmLogv(&stm->fields.stop.actor->srcp, 351, sevERR, "STOP statement", "an instance", "actor", NULL);
    }
  }
}


/*----------------------------------------------------------------------*/
static void analyzeDepend(Statement *stm, Context *context)
{
  /* Analyze a DEPENDING statement. It has partial expressions in the
     cases which must be connected to the depend expression. */

  List *cases;

 /* The expression will be analysed once for each case so no need to
    do this separately, is there?

    FIXME - performance may be somewhat improved by not re-analyze the
    expression for every case => some indication of an anlyzed
    expression must be available (the type?) in the expressions nodes.
 */

  for (cases = stm->fields.depend.cases; cases != NULL; cases = cases->next) {
    if (cases->element.stm->fields.depcase.exp != NULL) {
      Expression *exp = cases->element.stm->fields.depcase.exp;
      /* Unless it is an ELSE clause, set left hand of case expression
	 to be the depend expression */
      switch (exp->kind) {
      case BINARY_EXPRESSION:
	exp->fields.bin.left = stm->fields.depend.exp;
	break;
      case WHERE_EXPRESSION:
	exp->fields.whr.wht = stm->fields.depend.exp;
	break;
      case ATTRIBUTE_EXPRESSION:
	exp->fields.atr.wht = stm->fields.depend.exp;
	break;
      case BETWEEN_EXPRESSION:
	exp->fields.btw.val = stm->fields.depend.exp;
	break;
      default:
	SYSERR("Unrecognized switch case on Expression kind");
      }
    } else
      /* If this is an ELSE-case there can not be any other afterwards */
      if (cases->next != NULL)
	lmLog(&cases->element.stm->srcp, 335, sevERR, "");	
    
    /* Analyze the expression and the statements */
    analyzeExpression(cases->element.stm->fields.depcase.exp, context);
    analyzeStatements(cases->element.stm->fields.depcase.stms, context);
  }
}


/*----------------------------------------------------------------------*/
static void analyzeEach(Statement *stm, Context *context)
{
  Symbol *loopSymbol;
  IdNode *classId = NULL;
  Bool error;

  /* Analyze the partial filter expressions */
  if (stm->fields.each.filters != NULL)
    analyzeFilterExpressions("EACH statement", stm->fields.each.filters,
			     context, &classId, &error);

  /* Create a new frame and register the loop variable */
  newFrame();
  loopSymbol = newSymbol(stm->fields.each.loopId, LOCAL_SYMBOL);
  if (classId != NULL && classId->symbol != NULL)
    loopSymbol->fields.local.class = classId->symbol;
  else
    loopSymbol->fields.local.class = entitySymbol;
  /* Can only loop over instances */
  loopSymbol->fields.local.type = INSTANCE_TYPE;

  /* Analyze the statements in the loop body */
  analyzeStatements(stm->fields.each.stms, context);

  deleteFrame();
}


/*----------------------------------------------------------------------*/
static void analyzeShow(Statement *stm, Context *context)
{
  FILE *imagefile;

  imagefile = fopen(stm->fields.show.filename->string, READ_MODE);
  if (!imagefile)
    lmLog(&stm->fields.show.filename->srcp, 153, sevERR, "");
  else
    adv.images = concat(adv.images, stm->fields.show.filename, ID_LIST);
}


/*----------------------------------------------------------------------*/
static void analyzeStrip(Statement *stm, Context *context)
{
  if (stm->fields.strip.count != NULL) {
    analyzeExpression(stm->fields.strip.count, context);
    if (!equalTypes(stm->fields.strip.count->type, INTEGER_TYPE))
      lmLogv(&stm->fields.strip.count->srcp, 330, sevERR, "integer", "STRIP statement", NULL);
  }

  analyzeExpression(stm->fields.strip.from, context);
  if (!equalTypes(stm->fields.strip.from->type, STRING_TYPE))
    lmLogv(&stm->fields.strip.from->srcp, 330, sevERR, "string", "STRIP statement", NULL);
  if (stm->fields.strip.from->kind != ATTRIBUTE_EXPRESSION)
    lmLogv(&stm->fields.strip.from->srcp, 428, sevERR, "Expression", "an attribute", NULL);

  if (stm->fields.strip.into != NULL) {
    analyzeExpression(stm->fields.strip.into, context);
    if (!equalTypes(stm->fields.strip.into->type, STRING_TYPE))
      lmLogv(&stm->fields.strip.into->srcp, 330, sevERR, "string", "STRIP statement", NULL);
  if (stm->fields.strip.into->kind != ATTRIBUTE_EXPRESSION)
    lmLogv(&stm->fields.strip.into->srcp, 428, sevERR, "Expression", "an attribute", NULL);
  }
}



/*----------------------------------------------------------------------*/
static void analyzeStatement(Statement *stm, Context *context)
{
  switch (stm->kind) {
  case NOP_STATEMENT:
  case PRINT_STATEMENT:
  case QUIT_STATEMENT:
  case LOOK_STATEMENT:
  case SAVE_STATEMENT:
  case RESTORE_STATEMENT:
  case RESTART_STATEMENT:
  case VISITS_STATEMENT:
  case SYSTEM_STATEMENT:
    /* Nothing to analyse */
    break;
  case SCORE_STATEMENT:
    if (stm->fields.score.count != 0) {
      adv.scores[stm->fields.score.count] = stm->fields.score.score;
      totalScore += stm->fields.score.score;
    }
    break;
  case DESCRIBE_STATEMENT:
    analyzeDescribe(stm, context);
    break;
  case SAY_STATEMENT:
    analyzeSay(stm, context);
    break;
  case LIST_STATEMENT:
    analyzeList(stm, context);
    break;
  case EMPTY_STATEMENT:
    analyzeEmpty(stm, context);
    break;
  case LOCATE_STATEMENT:
    analyzeLocate(stm, context);
    break;
  case MAKE_STATEMENT:
    analyzeMake(stm, context);
    break;
  case SET_STATEMENT:
    analyzeSet(stm, context);
    break;
  case INCREASE_STATEMENT:
  case DECREASE_STATEMENT:
    analyzeIncrease(stm, context);
    break;
  case INCLUDE_STATEMENT:
  case EXCLUDE_STATEMENT:
    analyzeIncludeAndRemove(stm, context);
    break;
  case SCHEDULE_STATEMENT:
    analyzeSchedule(stm, context);
    break;
  case CANCEL_STATEMENT:
    analyzeCancel(stm);
    break;
  case IF_STATEMENT:
    analyzeIf(stm, context);
    break;
  case USE_STATEMENT:
    analyzeUse(stm, context);
    break;
  case STOP_STATEMENT:
    analyzeStop(stm, context);
    break;
  case DEPEND_STATEMENT:
    analyzeDepend(stm, context);
    break;
  case EACH_STATEMENT:
    analyzeEach(stm, context);
    break;
  case SHOW_STATEMENT:
    analyzeShow(stm, context);
    break;
  case STRIP_STATEMENT:
    analyzeStrip(stm, context);
    break;
  default:
    unimpl(stm->srcp, "Analyzer");
    break;
  }
}



/*======================================================================*/
void analyzeStatements(List *stms,
		       Context *context)
{
  while (stms != NULL) {
    analyzeStatement(stms->element.stm, context);
    stms = stms->next;
  }
}


/*----------------------------------------------------------------------*/
static void generatePrint(Statement *stm)
{
  /* Generate the code for a PRINT-stm. The text is found and copied
     to the data file (and encoded if requested!). */

  if (!stm->fields.print.encoded)
    encode(&stm->fields.print.fpos, &stm->fields.print.len);
  stm->fields.print.encoded = TRUE;
  emit2(I_PRINT, stm->fields.print.fpos, stm->fields.print.len);
}



/*----------------------------------------------------------------------*/
static void generateScore(Statement *stm)
{
  emitConstant(stm->fields.score.count);
  emit0(I_SCORE);
}



/*----------------------------------------------------------------------*/
static void generateDescribe(Statement *stm)
{
  generateExpression(stm->fields.describe.what);
  emit0(I_DESCRIBE);
}


/*----------------------------------------------------------------------*/
static void generateSay(Statement *stm)
{
  generateExpression(stm->fields.say.exp);
  switch (stm->fields.say.exp->type) {
  case INTEGER_TYPE:
    emit0(I_SAYINT);
    break;
  case STRING_TYPE:
    emit0(I_SAYSTR);
    break;
  case INSTANCE_TYPE:
    emit1(I_SAY, stm->fields.say.form);
    break;
  default:
    unimpl(stm->srcp, "Code Generator");
    break;
  }
}


/*----------------------------------------------------------------------*/
static void generateList(Statement *stm)
{
  generateExpression(stm->fields.list.wht);
  emit0(I_LIST);
}


/*----------------------------------------------------------------------*/
static void generateShow(Statement *stm)
{
  emit2(I_SHOW, stm->fields.show.filename->code, 0);
}


/*----------------------------------------------------------------------*/
static void generateEmpty(Statement *stm)
{
  generateWhere(stm->fields.empty.where);
  generateExpression(stm->fields.empty.what);
  emit0(I_EMPTY);
}



/*----------------------------------------------------------------------*/
static void generateLocate(Statement *stm)
{
  generateWhere(stm->fields.locate.where);
  generateExpression(stm->fields.locate.what);
  emit0(I_LOCATE);
}


/*----------------------------------------------------------------------*/
static void generateLvalue(Expression *exp) {
  switch (exp->kind) {
  case WHAT_EXPRESSION:
    generateWhat(exp->fields.wht.wht);
    break;
  case ATTRIBUTE_EXPRESSION:
    generateId(exp->fields.atr.id);
    generateExpression(exp->fields.atr.wht);
    break;
  default: syserr("Unexpected expression in '%s()'", __FUNCTION__);
  }
}


/*----------------------------------------------------------------------*/
static void generateMake(Statement *stm)
{
  emitConstant(!stm->fields.make.not);
  emitConstant(stm->fields.make.atr->code);
  generateExpression(stm->fields.make.wht);
  emit0(I_MAKE);
}


/*----------------------------------------------------------------------*/
static void generateSet(Statement *stm)
{
  generateLvalue(stm->fields.set.wht);

  generateExpression(stm->fields.set.exp);

  if (stm->fields.set.exp->type == STRING_TYPE)
    emit0(I_STRSET);
  else
    emit0(I_SET);
}


/*----------------------------------------------------------------------*/
static void generateIncrease(Statement *stm)
{
  if (stm->fields.incr.step != NULL)
    generateExpression(stm->fields.incr.step);
  else
    emitConstant(1);

  generateLvalue(stm->fields.incr.wht);
  if (stm->kind == INCREASE_STATEMENT)
    emit0(I_INCR);
  else
    emit0(I_DECR);
}


/*----------------------------------------------------------------------*/
static void generateIncludeAndRemove(Statement *stm)
{
  generateExpression(stm->fields.include.what);
  generateExpression(stm->fields.include.set);
  if (stm->kind == INCLUDE_STATEMENT)
    emit0(I_INCLUDE);
  else
    emit0(I_EXCLUDE);
}


/*----------------------------------------------------------------------*/
static void generateSchedule(Statement *stm)
{
  generateExpression(stm->fields.schedule.when);

  /* NOTE: we can't use gewhr() because the semantics of the schedule */
  /* statement is such that at scheduling AT something does not mean */
  /* where that something is now but where it is when the event is run! */
  switch (stm->fields.schedule.whr->kind) {
  case WHERE_DEFAULT:
  case WHERE_HERE:
    emitVariable(V_CURLOC);
    break;
    
  case WHERE_AT:
    generateWhat(stm->fields.schedule.whr->what->fields.wht.wht);
    break;

  default:
    unimpl(stm->srcp, "Code Generator");
    return;
  }
  generateId(stm->fields.schedule.id);
  emit0(I_SCHEDULE);
}


/*----------------------------------------------------------------------*/
static void generateCancel(Statement *stm) /* IN - Statement to generate */
{
  generateId(stm->fields.schedule.id);
  emit0(I_CANCEL);
}


/*----------------------------------------------------------------------*/
static void generateIf(Statement *stm)
{
  generateExpression(stm->fields.iff.exp);
  emit0(I_IF);
  generateStatements(stm->fields.iff.thn);
  if (stm->fields.iff.els != NULL) {
    emit0(I_ELSE);
    generateStatements(stm->fields.iff.els);
  }
  emit0(I_ENDIF);
}


/*----------------------------------------------------------------------*/
static void generateUse(Statement *stm)
{
  if (stm->fields.use.actorExp == NULL) { /* No actor specified, use current */
    emitConstant(stm->fields.use.script->code);
    emitVariable(V_CURRENT_INSTANCE);
    emit0(I_USE);
  } else {
    emitConstant(stm->fields.use.script->code);
    generateExpression(stm->fields.use.actorExp);
    emit0(I_USE);
  }
}


/*----------------------------------------------------------------------*/
static void generateStop(Statement *stm)
{
  generateExpression(stm->fields.stop.actor);
  emit0(I_STOP);
}


/*----------------------------------------------------------------------*/
static void generateDepend(Statement *stm)
{
  /* Generate DEPENDING statement.

  Code generation principle:				Stack:

      DEPEND

      depend expression					d-exp

      DEPCASE ----------+ (not present for first case)	d-exp
      DUP               |				d-exp	d-exp  
      case1 expression  |				c-exp	d-exp	d-exp
      case1 operator    |				case?	d-exp
      DEPEXEC           > repeat for each case		d-exp
      stms1 ------------+

      DEPELSE--+ optional
      stmsn----+

      ENDDEP

  DEPSTART does nothing but must be there to indicate start of a new
  level for skipping over statements.

  Executing a DEPCASE or DEPELSE indicates the end of executing a
  matching case so skip to the ENDDEP (on this level).

  After the DEPCASE is a DUP to duplicate the depend value, then
  comes the case expression and then the operator which does the
  compare.

  DEPEXEC inspects the results on the stack top and if true continues
  else skips to the instruction after next DEPCASE, DEPELSE or to the ENDDEP.

  ENDDEP just pops off the initially pushed depend expression.

  */

  List *cases;

  emit0(I_DEPEND);
  generateExpression(stm->fields.depend.exp);
  /* For each case: */
  for (cases = stm->fields.depend.cases; cases != NULL; cases = cases->next) {
    /* If it is not the ELSE clause ... */
    if (cases->element.stm->fields.depcase.exp != NULL) {
      /* Generate a DEPCASE (if not first case) and a DUP */
      if (cases != stm->fields.depend.cases)
	emit0(I_DEPCASE);
      emit0(I_DUP);
      /* ...and the case expression (right hand + operator) */
      generateRightHandExpression(cases->element.stm->fields.depcase.exp);
      emit0(I_DEPEXEC);
    } else
      emit0(I_DEPELSE);
    /* ...and then the statements */
    generateStatements(cases->element.stm->fields.depcase.stms);
  }
  emit0(I_ENDDEP);
}


/*----------------------------------------------------------------------*/
static void generateEach(Statement *statement)
{
  List *filter;

  /* Generate a new FRAME */
  emit1(I_FRAME, 1);		/* One local variable in this block */
  frameLevel++;

  /* Loop variable is initialised to 0
     which works since the EACH statement will
     increment it to 1 (first instance number) */

  /* Start of loop */
  emit1(I_EACH, 1);

  /* Generate filters */
  TRAVERSE(filter, statement->fields.each.filters) {
    emit2(I_GETLOCAL, 0, 1);
    generateRightHandExpression(filter->element.exp);
    emit0(I_NOT);
    emit0(I_IF);
    emit0(I_NEXTEACH);
    emit0(I_ENDIF);
  }

  generateStatements(statement->fields.each.stms);

  /* End of loop */
  emit0(I_ENDEACH);

  /* End of block */
  emit0(I_ENDFRAME);
  frameLevel--;
}

/*----------------------------------------------------------------------*/
static void generateStrip(Statement *stm)
{
  /* First generate the attribute reference for any INTO clause */
  if (stm->fields.strip.into != NULL)
    generateAttributeReference(stm->fields.strip.into);

  /* Push First/Last indicator */
  emitConstant(stm->fields.strip.first);

  /* Push count, implicit = 1 */
  if (stm->fields.strip.count != NULL)
    generateExpression(stm->fields.strip.count);
  else
    emitConstant(1);

  /* Push words or character indicator */
  emitConstant(stm->fields.strip.wordOrChar);

  /* Push attribute reference */
  generateAttributeReference(stm->fields.strip.from);

  emit0(I_STRIP);		/* Will modify the FROM and leave rest on stack */

  /* If there was an INTO clause we set the string attribute */
  if (stm->fields.strip.into != NULL)
    emit0(I_STRSET);
  else				/* Pop of the rest produced above */
    emit0(I_POP);
}


/*----------------------------------------------------------------------*/
static void generateSystem(Statement *stm)
{
  encode(&stm->fields.system.fpos, &stm->fields.system.len);
  emitConstant(stm->fields.system.len);
  emitConstant(stm->fields.system.fpos);
  emit0(I_SYSTEM);
}



/*----------------------------------------------------------------------*/
static void generateStatement(Statement *stm)
{
  if ((Bool)opts[OPTDEBUG].value)
    emitLine(stm->srcp);

  switch (stm->kind) {

  case NOP_STATEMENT:
    break;

  case PRINT_STATEMENT:
    generatePrint(stm);
    break;

  case QUIT_STATEMENT:
    emit0(I_QUIT);
    break;

  case LOOK_STATEMENT:
    emit0(I_LOOK);
    break;

  case SAVE_STATEMENT:
    emit0(I_SAVE);
    break;

  case RESTORE_STATEMENT:
    emit0(I_RESTORE);
    break;

  case RESTART_STATEMENT:
    emit0(I_RESTART);
    break;

  case VISITS_STATEMENT:
    emitConstant(stm->fields.visits.count);
    emit0(I_VISITS);
    break;

  case SCORE_STATEMENT:
    generateScore(stm);
    break;

  case DESCRIBE_STATEMENT:
    generateDescribe(stm);
    break;

  case SAY_STATEMENT:
    generateSay(stm);
    break;

  case LIST_STATEMENT:
    generateList(stm);
    break;

  case SHOW_STATEMENT:
    generateShow(stm);
    break;

  case EMPTY_STATEMENT:
    generateEmpty(stm);
    break;

  case LOCATE_STATEMENT:
    generateLocate(stm);
    break;

  case MAKE_STATEMENT:
    generateMake(stm);
    break;

  case SET_STATEMENT:
    generateSet(stm);
    break;

  case INCREASE_STATEMENT:
  case DECREASE_STATEMENT:
    generateIncrease(stm);
    break;

  case INCLUDE_STATEMENT:
  case EXCLUDE_STATEMENT:
    generateIncludeAndRemove(stm);
    break;

  case SCHEDULE_STATEMENT:
    generateSchedule(stm);
    break;

  case CANCEL_STATEMENT:
    generateCancel(stm);
    break;

  case IF_STATEMENT:
    generateIf(stm);
    break;

  case USE_STATEMENT:
    generateUse(stm);
    break;

  case STOP_STATEMENT:
    generateStop(stm);
    break;

  case DEPEND_STATEMENT:
    generateDepend(stm);
    break;

  case SYSTEM_STATEMENT:
    generateSystem(stm);
    break;

  case EACH_STATEMENT:
    generateEach(stm);
    break;

  case STRIP_STATEMENT:
    generateStrip(stm);
    break;

  default:
    unimpl(stm->srcp, "Code Generator");
    break;
  }
}


/*======================================================================*/
void generateStatements(List *stms)
{
  List *current = stms;

  for (current = stms; current != NULL; current = current->next) {
    generateStatement(current->element.stm);
  }
}



/*======================================================================*/
void dumpStatement(Statement *stm)
{
  if (stm == NULL) {
    put("NULL");
    return;
  }

  put("STM: ");
  switch(stm->kind) {
  case PRINT_STATEMENT: put("PRINT "); break;
  case DESCRIBE_STATEMENT: put("DESCRIBE "); break;
  case SAY_STATEMENT: put("SAY "); break;
  case LIST_STATEMENT: put("LIST "); break;
  case IF_STATEMENT: put("IF "); break;
  case MAKE_STATEMENT: put("MAKE "); break;
  case SET_STATEMENT: put("SET "); break;
  case INCREASE_STATEMENT: put("INCR "); break;
  case DECREASE_STATEMENT: put("DECR "); break;
  case LOCATE_STATEMENT: put("LOCATE "); break;
  case EMPTY_STATEMENT: put("EMPTY "); break;
  case INCLUDE_STATEMENT: put("INCLUDE "); break;
  case EXCLUDE_STATEMENT: put("REMOVE "); break;
  case SCHEDULE_STATEMENT: put("SCHEDULE "); break;
  case CANCEL_STATEMENT: put("CANCEL "); break;
  case LOOK_STATEMENT: put("LOOK "); break;
  case QUIT_STATEMENT: put("QUIT "); break;
  case SCORE_STATEMENT: put("SCORE "); break;
  case USE_STATEMENT: put("USE "); break;
  case STRIP_STATEMENT: put("STRIP "); break;
  case STOP_STATEMENT: put("STOP "); break;
  case SAVE_STATEMENT: put("SAVE "); break;
  case RESTORE_STATEMENT: put("RESTORE "); break;
  case RESTART_STATEMENT: put("RESTART "); break;
  case VISITS_STATEMENT: put("VISITS "); break;
  case NOP_STATEMENT: put("NOP "); break;
  case SHOW_STATEMENT: put("SHOW "); break;
  case SYSTEM_STATEMENT: put("SYSTEM "); break;
  case DEPEND_STATEMENT: put("DEPEND "); break;
  case DEPENDCASE_STATEMENT: put("DEPENDCASE "); break;
  case EACH_STATEMENT: put("EACH "); break;
  }
  dumpSrcp(&stm->srcp);

  switch(stm->kind) {
  case LOOK_STATEMENT:
  case QUIT_STATEMENT:
  case SAVE_STATEMENT:
  case RESTORE_STATEMENT:
  case RESTART_STATEMENT:
    break;
  default:
    indent();
    switch(stm->kind) {
    case PRINT_STATEMENT:
      put("fpos: "); dumpInt(stm->fields.print.fpos); nl();
      put("len: "); dumpInt(stm->fields.print.len);
      break;
    case SCORE_STATEMENT:
      put("count: "); dumpInt(stm->fields.score.count); nl();
      put("score: "); dumpInt(stm->fields.score.score);
      break;
    case DESCRIBE_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.describe.what);
      break;
    case SAY_STATEMENT:
      put("exp: "); dumpExpression(stm->fields.say.exp); nl();
      put("form: "); dumpForm(stm->fields.say.form);
      break;
    case LIST_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.list.wht);
      break;
    case EMPTY_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.empty.what); nl();
      put("whr: "); dumpWhere(stm->fields.empty.where);
      break;
    case LOCATE_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.locate.what); nl();
      put("whr: "); dumpWhere(stm->fields.locate.where);
      break;
    case INCLUDE_STATEMENT:
    case EXCLUDE_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.include.what); nl();
      put("set: "); dumpExpression(stm->fields.include.set);
      break;
    case MAKE_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.make.wht); nl();
      put("not: "); dumpBool(stm->fields.make.not); nl();
      put("atr: "); dumpId(stm->fields.make.atr);
      break;
    case SET_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.set.wht); nl();
      put("exp: "); dumpExpression(stm->fields.set.exp);
      break;
    case INCREASE_STATEMENT:
    case DECREASE_STATEMENT:
      put("wht: "); dumpExpression(stm->fields.incr.wht); nl();
      put("step: "); dumpExpression(stm->fields.incr.step);
      break;
    case SCHEDULE_STATEMENT:
      put("id: "); dumpId(stm->fields.schedule.id); nl();
      put("whr: "); dumpWhere(stm->fields.schedule.whr); nl();
      put("when: "); dumpExpression(stm->fields.schedule.when);
      break;
    case CANCEL_STATEMENT:
      put("id: "); dumpId(stm->fields.cancel.id);
      break;
    case IF_STATEMENT:
      put("exp: "); dumpExpression(stm->fields.iff.exp); nl();
      put("thn: "); dumpList(stm->fields.iff.thn, STATEMENT_LIST); nl();
      put("els: "); dumpList(stm->fields.iff.els, STATEMENT_LIST);
      break;
    case USE_STATEMENT:
      put("script: "); dumpId(stm->fields.use.script); nl();
      put("actor: "); dumpExpression(stm->fields.use.actorExp);
      break;
    case STOP_STATEMENT:
      put("actor: "); dumpExpression(stm->fields.stop.actor);
      break;
    case EACH_STATEMENT:
      put("loopId: "); dumpId(stm->fields.each.loopId); nl();
      put("classId: "); dumpId(stm->fields.each.classId); nl();
      put("filters: "); dumpList(stm->fields.each.filters, EXPRESSION_LIST); nl();
      put("stms: "); dumpList(stm->fields.each.stms, STATEMENT_LIST);
      break;
    case VISITS_STATEMENT:
      put("count: "); dumpInt(stm->fields.visits.count);
      break;
    case STRIP_STATEMENT:
      put("first: "); dumpBool(stm->fields.strip.first); nl();
      put("count: "); dumpExpression(stm->fields.strip.count); nl();
      put("word?: "); dumpBool(stm->fields.strip.wordOrChar); nl();
      put("from: "); dumpExpression(stm->fields.strip.from); nl();
      put("into: "); dumpExpression(stm->fields.strip.into);
      break;
    default:
      break;
    }
    out();
  }
}
