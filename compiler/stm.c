/*----------------------------------------------------------------------*\

                                STM.C
			   Statement Nodes

\*----------------------------------------------------------------------*/

#include "stm_x.h"


#include "alan.h"
#include "util.h"

#include "atr_x.h"
#include "cnt_x.h"
#include "exp_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "srcp_x.h"
#include "sym_x.h"
#include "whr_x.h"
#include "wht_x.h"

#include "lmList.h"

#include "adv.h"                /* ADV-node */
#include "scr.h"                /* SCR-nodes */
#include "sco.h"                /* SCORES */
#include "opt.h"                /* OPTIONS */

#include "emit.h"
#include "encode.h"

#include "dump.h"




/*======================================================================

  newstm()

  Allocates and initialises a stmnod.

  */
StmNod *newstm(Srcp *srcp,	/* IN - Source Position */
	       StmKind class)	/* IN - The statement class */
{
  StmNod *new;                  /* The newly allocated area */

  showProgress();

  new = NEW(StmNod);

  new->srcp = *srcp;
  new->class = class;

  return(new);
}



/*----------------------------------------------------------------------

  andescribe()

  Analyze a DESCRIBE statement.

  */
static void andescribe(StmNod *stm,
		       Context *context)
{
  Symbol *sym;

  switch (stm->fields.describe.wht->kind) {
  case WHAT_LOCATION:
    break;
  case WHAT_ACTOR:
    if (context->kind == EVENT_CONTEXT)
      lmLog(&stm->fields.describe.wht->srcp, 412, sevERR, "");
    break;
  case WHAT_ID:
    sym = symcheck(stm->fields.describe.wht->id, INSTANCE_SYMBOL, context);
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }
}



/*----------------------------------------------------------------------

  ansay()

  Analyze a SAY statement.

  */
static void ansay(StmNod *stm,
		  Context *context)
{
  anexp(stm->fields.say.exp, context);
}


/*----------------------------------------------------------------------

  analyzeList()

  Analyze a LIST statement.

  */
static void analyzeList(StmNod *stm,
			Context *context)	
{
  verifyContainer(stm->fields.list.wht, context);
}


/*----------------------------------------------------------------------

  anempty()

  Analyze an EMPTY statement.

  */
static void anempty(StmNod *stm,
		    Context *context)
{
  verifyContainer(stm->fields.list.wht, context);
  analyzeWhere(stm->fields.empty.whr, context);
}


/*----------------------------------------------------------------------

  analyzeLocate()

  */
static void analyzeLocate(StmNod *stm,
			  Context *context)
{
  Symbol *whtSymbol = NULL;

  switch (stm->fields.locate.wht->kind) {
  case WHAT_ACTOR:
    if (context->kind == EVENT_CONTEXT)
      lmLog(&stm->fields.locate.wht->srcp, 412, sevERR, "");
    break;
  case WHAT_LOCATION:
    lmLog(&stm->srcp, 311, sevERR, "an Object or an Actor");
    break;
  case WHAT_ID:
    whtSymbol = symcheck(stm->fields.locate.wht->id, INSTANCE_SYMBOL, context);
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }

  analyzeWhere(stm->fields.locate.whr, context);
  switch (stm->fields.locate.whr->kind) {
  case WHR_HERE:
  case WHR_AT:
    break;
  case WHR_IN:
    switch (stm->fields.locate.wht->kind) {
    case WHAT_LOCATION:
      lmLog(&stm->srcp, 402, sevERR, "A Location");
    case WHAT_ACTOR:
      lmLog(&stm->srcp, 402, sevERR, "An  Actor");
      break;
    case WHAT_ID:
      if (inheritsFrom(whtSymbol, actorSymbol))
	lmLog(&stm->srcp, 402, sevERR, "An Actor");
      if (inheritsFrom(whtSymbol, locationSymbol))
	lmLog(&stm->srcp, 402, sevERR, "A Location");
      break;
    default:
      syserr("Unexpected WhtKind in analyzeLocate()");
    }
    break;
  case WHR_NEAR:
    lmLog(&stm->srcp, 415, sevERR, "LOCATE");
    break;
  default:
    syserr("Unrecognized switch in analyzeLocate()");
    break;
  }
}



/*----------------------------------------------------------------------

  verifyMakeAttribute()

  Verify that a found attribute can be used in a MAKE statement.

*/
static void verifyMakeAttribute(IdNode *attributeId, Attribute *foundAttribute)
{
  if (foundAttribute != NULL) {
    if (foundAttribute->type != BOOLEAN_TYPE)
      lmLog(&attributeId->srcp, 408, sevERR, "MAKE statement");
    else
      attributeId->code = foundAttribute->id->code;
  }
}
      


/*----------------------------------------------------------------------

  anmake()

  Analyze a MAKE statement.

  */
static void anmake(StmNod *stm,
		   Context *context)
{
  Attribute *atr = NULL;

  switch (stm->fields.make.wht->kind) {

  case WHAT_ACTOR:
    if (context->kind == EVENT_CONTEXT)
      lmLog(&stm->fields.make.wht->srcp, 412, sevERR, "");
    break;

  case WHAT_LOCATION:
  case WHAT_ID:
    break;

  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }

  atr = resolveAttributeReference(stm->fields.make.wht, stm->fields.make.atr, context);
  verifyMakeAttribute(stm->fields.make.atr, atr);
}



/*----------------------------------------------------------------------

  verifySetTarget()

*/
static void verifySetTarget(IdNode *attributeId, Attribute  *foundAttribute)
{
  if (foundAttribute) {
    if (foundAttribute->type != INTEGER_TYPE && foundAttribute->type != STRING_TYPE)
      lmLog(&attributeId->srcp, 419, sevERR, "Target for");
    else
      attributeId->code = foundAttribute->id->code;
  }
}


/*----------------------------------------------------------------------

  anset()

  Analyze a SET statement

  */
static void anset(StmNod *stm,
		  Context *context)
{
  Attribute *atr;

  switch (stm->fields.set.wht->kind) {

  case WHAT_ACTOR:
    if (context->kind == EVENT_CONTEXT)
      lmLog(&stm->fields.set.wht->srcp, 412, sevERR, "");
    break;

  case WHAT_LOCATION:
  case WHAT_ID:
    break;

  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }
  atr = resolveAttributeReference(stm->fields.set.wht, stm->fields.set.atr, context);
  verifySetTarget(stm->fields.set.atr, atr);

  if (stm->fields.set.exp != NULL) {
    anexp(stm->fields.set.exp, context);
    if (stm->fields.set.exp->type != INTEGER_TYPE &&
        stm->fields.set.exp->type != STRING_TYPE)
      lmLog(&stm->fields.set.exp->srcp, 419, sevERR, "Expression in");
    if (atr && !equalTypes(stm->fields.set.exp->type, atr->type))
      lmLog(&stm->srcp, 331, sevERR, "SET statement");
  }
}


/*----------------------------------------------------------------------

  anincr()

  Analyze a INCR/DECR statement

  */
static void anincr(StmNod *stm,
		   Context *context)
{
  Attribute *atr;

  switch (stm->fields.incr.wht->kind) {

  case WHAT_ACTOR:
    if (context->kind == EVENT_CONTEXT)
      lmLog(&stm->fields.incr.wht->srcp, 412, sevERR, "");
    atr = findAttribute(NULL, stm->fields.incr.atr);
    if (atr == NULL)          /* attribute not found globally */
      lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "ACTOR");
    else
      stm->fields.incr.atr->code = atr->id->code;
    break;

  case WHAT_LOCATION:
    atr = findAttribute(NULL, stm->fields.incr.atr);
    if (atr == NULL)            /* attribute not found globally */
      lmLog(&stm->fields.incr.atr->srcp, 404, sevERR, "LOCATION");
    else
      stm->fields.incr.atr->code = atr->id->code;
    break;

  case WHAT_ID:
    atr = resolveAttributeReference(stm->fields.incr.wht, stm->fields.incr.atr, context);
    if (atr) {
      if (atr->type != INTEGER_TYPE)
        lmLog(&stm->fields.incr.atr->srcp, 413, sevERR, "INCREASE/DECREASE");
      else
        stm->fields.incr.atr->code = atr->id->code;
    }
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }

  if (stm->fields.incr.step != NULL) {
    anexp(stm->fields.incr.step, context);
    if (stm->fields.incr.step->type != INTEGER_TYPE)
      lmLog(&stm->fields.incr.step->srcp, 413, sevERR, "INCREASE/DECREASE");
  }
}


/*----------------------------------------------------------------------

  anschedule()

  Analyze a SCHEDULE statement.

  */
static void anschedule(StmNod *stm,
		       Context *context)
{
  Symbol *sym;

  sym = symcheck(stm->fields.schedule.id, EVENT_SYMBOL, NULL);

  /* Now lookup where */
  analyzeWhere(stm->fields.schedule.whr, context);
  switch (stm->fields.schedule.whr->kind) {
  case WHR_DEFAULT:
    stm->fields.schedule.whr->kind = WHR_HERE;
    break;
  case WHR_HERE:
  case WHR_AT:
    break;
  case WHR_IN:
  case WHR_NEAR:
    lmLog(&stm->fields.schedule.whr->srcp, 415, sevERR, "SCHEDULE");
    break;
  default:
    syserr("Unrecognized switch in anschedule()");
    break;
  }

  /* Analyze the when expression */
  anexp(stm->fields.schedule.when, context);
  if (stm->fields.schedule.when->type != INTEGER_TYPE)
    lmLog(&stm->fields.schedule.when->srcp, 413, sevERR, "when-clause of SCHEDULE statement");

}


/*----------------------------------------------------------------------

  ancancel()

  Analyze a CANCEL statement.

  */
static void ancancel(StmNod *stm) /* IN - The statement to analyze */
{
  Symbol *sym;

  sym = symcheck(stm->fields.cancel.id, EVENT_SYMBOL, NULL);
}


/*----------------------------------------------------------------------

  anif()

  Analyze an IF statement.

  */
static void anif(StmNod *stm,
		 Context *context)
{
  anexp(stm->fields.iff.exp, context);
  if (!equalTypes(stm->fields.iff.exp->type, BOOLEAN_TYPE))
    lmLogv(&stm->fields.iff.exp->srcp, 330, sevERR, "boolean", "'IF'", NULL);
  analyzeStatements(stm->fields.iff.thn, context);
  if (stm->fields.iff.els != NULL)
    analyzeStatements(stm->fields.iff.els, context);
}


/*----------------------------------------------------------------------

  analyzeUse()

  Analyze a USE statement. It must refer a script that is defined
  within the mentioned actor. If the actor is not specified the
  actor is assumed to be the one we are in (it is an error if we are
  not).

  */
static void analyzeUse(StmNod *stm,
		       Context *context)
{
  Symbol *sym;
  List *lst;
  List *scripts = NULL;
  IdNode *id = NULL;

  if (stm->fields.use.actor == NULL && context->instance != NULL)
    lmLog(&stm->srcp, 401, sevERR, "");
  else {
    if (stm->fields.use.actor != NULL) {
      /* Lookup specified actors symbol */
      sym = symcheck(stm->fields.use.actor, INSTANCE_SYMBOL, context);
      if (sym) {
	if (sym->kind == PARAMETER_SYMBOL)
	  lmLog(&stm->fields.use.actor->srcp, 410, sevERR, "USE statement");
	else {
	  scripts = sym->fields.claOrIns.slots->scripts;
	  id = sym->fields.claOrIns.slots->id;
	}
      }
    } else {
      if (context->instance == NULL && context->instance->slots == NULL)
	syserr("Unexpected context in analyzeUse()");
      scripts = context->instance->slots->scripts;
      id = context->instance->slots->id;
    }

    /* Loop over actors scripts to check if script is defined */
    for (lst = scripts; lst != NULL; lst = lst->next) {
      if (equalId(lst->element.scr->id, stm->fields.use.script)) {
	stm->fields.use.scriptno = lst->element.scr->id->code;
	break;		/* Found it so break loop */
      }
    }
    if (lst == NULL)
      lmLog(&stm->fields.use.script->srcp, 400, sevERR, id->string);

  }
}  


/*----------------------------------------------------------------------

  andep()

  Analyze a DEPENDING statement. It has partial expressions in the
  cases which must be connected to the depend expression.

  */
static void andep(StmNod *stm, Context *context)
{
  List *cases;

 /* The expression will be analysed once for each case so no need to
    do this separately, is there?

    FIXME - performance may be somewhat improved by not re-analyze the
    expression for every case => some indication of an anlyzed
    expression must be available (the type?) in the expressions nodes.
 */

  for (cases = stm->fields.depend.cases; cases != NULL; cases =
	 cases->next) {

    if (cases->element.stm->fields.depcase.exp != NULL) {
      /* Unless it is an ELSE clause set left hand of case expression
         to be the depend expression */
      switch (cases->element.stm->fields.depcase.exp->kind) {
      case BINARY_EXPRESSION:
	cases->element.stm->fields.depcase.exp->fields.bin.left =
	  stm->fields.depend.exp;
	break;
      case WHERE_EXPRESSION:
	cases->element.stm->fields.depcase.exp->fields.whr.wht =
	  stm->fields.depend.exp;
	break;
      case ATTRIBUTE_EXPRESSION:
	cases->element.stm->fields.depcase.exp->fields.atr.wht =
	  stm->fields.depend.exp;
	break;
      case BETWEEN_EXPRESSION:
	cases->element.stm->fields.depcase.exp->fields.btw.val =
	  stm->fields.depend.exp;
	break;
      default:
	syserr("andep(): Unrecognized switch case on expkd.");
      }
    } else
      /* If this is an ELSE-case there can not be any other afterwards */
      if (cases->next != NULL)
	lmLog(&cases->element.stm->srcp, 335, sevERR, "");	

    /* Analyze the expression and the statements */
    anexp(cases->element.stm->fields.depcase.exp, context);
    analyzeStatements(cases->element.stm->fields.depcase.stms, context);

  }
}



/*----------------------------------------------------------------------

  anstm()

  Analyze one statement.

  */
static void anstm(StmNod *stm,
		  Context *context)
{
  switch (stm->class) {
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
      scotot += stm->fields.score.score;
    }
    break;
  case DESCRIBE_STATEMENT:
    andescribe(stm, context);
    break;
  case SAY_STATEMENT:
    ansay(stm, context);
    break;
  case LIST_STATEMENT:
    analyzeList(stm, context);
    break;
  case EMPTY_STATEMENT:
    anempty(stm, context);
    break;
  case LOCATE_STATEMENT:
    analyzeLocate(stm, context);
    break;
  case MAKE_STATEMENT:
    anmake(stm, context);
    break;
  case SET_STATEMENT:
    anset(stm, context);
    break;
  case INCREASE_STATEMENT:
  case DECREASE_STATEMENT:
    anincr(stm, context);
    break;
  case SCHEDULE_STATEMENT:
    anschedule(stm, context);
    break;
  case CANCEL_STATEMENT:
    ancancel(stm);
    break;
  case IF_STATEMENT:
    anif(stm, context);
    break;
  case USE_STATEMENT:
    analyzeUse(stm, context);
    break;
  case DEPEND_STATEMENT:
    andep(stm, context);
    break;
  default:
    unimpl(&stm->srcp, "Analyzer");
    break;
  }
}



/*======================================================================*/
void analyzeStatements(List *stms,
		       Context *context)
{
  while (stms != NULL) {
    anstm(stms->element.stm, context);
    stms = stms->next;
  }
}


/*----------------------------------------------------------------------

  Generate the code for a PRINT-stm. The text is found and copied to the
  data file (and encoded if requested!).

  */
static void geprint(StmNod *stm)
{
  encode(&stm->fields.print.fpos, &stm->fields.print.len);
  emit0(C_CONST, stm->fields.print.len);
  emit0(C_CONST, stm->fields.print.fpos);
  emit0(C_STMOP, I_PRINT);
}



/*----------------------------------------------------------------------

  Generate a SCORE statement

  */
static void generateScore(StmNod *stm)
{
  emit0(C_CONST, stm->fields.score.count);
  emit0(C_STMOP, I_SCORE);
}



/*----------------------------------------------------------------------

  Generate code to implement a DESCRIBE statement.

  */
static void generateDescribe(StmNod *stm)
{
  switch (stm->fields.describe.wht->kind) {

  case WHAT_LOCATION:
    emit0(C_CURVAR, V_CURLOC);
    break;

  case WHAT_ACTOR:
    emit0(C_CURVAR, V_CURACT);
    break;

  case WHAT_ID:
    generateId(stm->fields.describe.wht->id);
    break;

  default:
    unimpl(&stm->srcp, "Code Generator");
    return;
  }
  emit0(C_STMOP, I_DESCRIBE);
}


/*----------------------------------------------------------------------

  gesay()

  Generate code for a SAY statement.

  */
static void gesay(StmNod *stm, int currentInstance)
{
  geexp(stm->fields.say.exp, currentInstance);
  switch (stm->fields.say.exp->type) {
  case INTEGER_TYPE:
    emit0(C_STMOP, I_SAYINT);
    break;
  case STRING_TYPE:
    emit0(C_STMOP, I_SAYSTR);
    break;
  case INSTANCE_TYPE:
    emit0(C_STMOP, I_SAY);
    break;
  default:
    unimpl(&stm->srcp, "Code Generator");
    break;
  }
}


/*----------------------------------------------------------------------

  gelist()

  Generate code to implement a LIST statement.

  */
static void gelist(StmNod *stm)	/* IN - Statement */
{
  if (stm->fields.list.wht->kind == WHAT_ID) {
    generateId(stm->fields.list.wht->id);
    emit0(C_STMOP, I_LIST);
  } else
    unimpl(&stm->srcp, "Code Generator");
}



/*----------------------------------------------------------------------

  geempty()

  Generate code to implement the EMPTY statement.

  */
static void geempty(StmNod *stm, int currentInstance)
{
  if (stm->fields.empty.wht->kind == WHAT_ID) {
    gewhr(stm->fields.empty.whr, currentInstance);
    generateId(stm->fields.empty.wht->id);
    emit0(C_STMOP, I_EMPTY);
  } else
    unimpl(&stm->srcp, "Code Generator");
}



/*----------------------------------------------------------------------

  gelocate()

  Generate code to implement a LOCATE statement.

  */
static void gelocate(StmNod *stm, int currentInstance)
{
  gewhr(stm->fields.locate.whr, currentInstance);
  generateWhat(stm->fields.locate.wht, currentInstance);
  emit0(C_STMOP, I_LOCATE);
}



/*----------------------------------------------------------------------

  gemake()

  Generate code to implement a MAKE statement.

  */
static void gemake(StmNod *stm, int currentInstance)
{
  emit0(C_CONST, !stm->fields.make.not);
  emit0(C_CONST, stm->fields.make.atr->code);
  generateWhat(stm->fields.make.wht, currentInstance);
  emit0(C_STMOP, I_MAKE);
}




/*----------------------------------------------------------------------

  geset()

  Generate code to implement a SET statement.

  */
static void geset(StmNod *stm, int currentInstance)
{
  geexp(stm->fields.set.exp, currentInstance);

  emit0(C_CONST, stm->fields.set.atr->code);
  generateWhat(stm->fields.set.wht, currentInstance);
  if (stm->fields.set.exp->type == STRING_TYPE)
    emit0(C_STMOP, I_STRSET);
  else
    emit0(C_STMOP, I_SET);
}



/*----------------------------------------------------------------------

  geincr()

  Generate code to implement a INCR/DECR statement.

  */
static void geincr(StmNod *stm, int currentInstance)
{
  if (stm->fields.incr.step != NULL)
    geexp(stm->fields.incr.step, currentInstance);
  else
    emit0(C_CONST, 1);

  emit0(C_CONST, stm->fields.incr.atr->code);
  generateWhat(stm->fields.incr.wht, currentInstance);
  if (stm->class == INCREASE_STATEMENT)
    emit0(C_STMOP, I_INCR);
  else
    emit0(C_STMOP, I_DECR);
}



/*----------------------------------------------------------------------

  geschedule()

  Generate code to implement a SCHEDULE statement.

  */
static void geschedule(StmNod *stm, int currentInstance)
{
  geexp(stm->fields.schedule.when, currentInstance);

  /* NOTE: we can't use gewhr() because the semantics of the schedule */
  /* statement is such that at scheduling AT something does not mean */
  /* where that something is now but where it is when the event is run! */
  switch (stm->fields.schedule.whr->kind) {
  case WHR_DEFAULT:
  case WHR_HERE:
    emit0(C_CURVAR, V_CURLOC);
    break;
    
  case WHR_AT:
    generateWhat(stm->fields.schedule.whr->wht, currentInstance);
    break;

  default:
    unimpl(&stm->srcp, "Code Generator");
    return;
  }
  generateId(stm->fields.schedule.id);
  emit0(C_STMOP, I_SCHEDULE);
}


/*----------------------------------------------------------------------

  gecancel()

  Generate code to implement the CANCEL statement.

  */
static void gecancel(StmNod *stm) /* IN - Statement to generate */
{
  generateId(stm->fields.schedule.id);
  emit0(C_STMOP, I_CANCEL);
}

/*----------------------------------------------------------------------

  geif()

  Generate code to implement a IF statement.

  */
static void geif(StmNod *stm, int currentInstance)
{
  geexp(stm->fields.iff.exp, currentInstance);
  emit0(C_STMOP, I_IF);
  gestms(stm->fields.iff.thn, currentInstance);
  if (stm->fields.iff.els != NULL) {
    emit0(C_STMOP, I_ELSE);
    gestms(stm->fields.iff.els, currentInstance);
  }
  emit0(C_STMOP, I_ENDIF);
}



/*----------------------------------------------------------------------

  geuse()

  Generate USE statement.

  */
static void geuse(StmNod *stm, int currentInstance)
{
  if (stm->fields.use.actor == NULL) { /* No actor specified, use current */
    emit0(C_CONST, stm->fields.use.scriptno);
    emit0(C_CONST, currentInstance);
    emit0(C_STMOP, I_USE);
  } else {
    emit0(C_CONST, stm->fields.use.scriptno);
    generateId(stm->fields.use.actor);
    emit0(C_STMOP, I_USE);
  }
}



/*----------------------------------------------------------------------

  gedepcase()

  Will generate just the right hand part of the expression and the
  operator.

*/
static void generateDependCase(ExpNod *exp, int currentInstance)
{
  switch (exp->kind) {
  case BINARY_EXPRESSION:
    geexp(exp->fields.bin.right, currentInstance);
    generateBinaryOperator(exp);
    break;
  case ATTRIBUTE_EXPRESSION:
    generateId(exp->fields.atr.atr);
    generateAttributeAccess(exp);
    break;
  case BETWEEN_EXPRESSION:
    generateBetweenCheck(exp, currentInstance);
    break;
  default:
    syserr("generateDependingCase(): Unrecognized switch case on expression kind.");
  }
}


/*----------------------------------------------------------------------

  gedep()

  Generate DEPENDING statement.

  Code generation principle:				Stack:

      DEPSTART

      depend expression					d-exp

      DEPCASE ----------+ (not present for first case)	d-exp
      DUP               |				d-exp	d-exp  
      case1 expression  |				c-exp	d-exp	d-exp
      case1 operator    |				case?	d-exp
      DEPEXEC           > repeat for each case		d-exp
      stms1 ------------+

      DEPELSE--+ optional
      stmsn----+

      DEPEND

  DEPSTART does nothing but must be there to indicate start of a new
  level for skipping over statements.

  Executing a DEPCASE or DEPELSE indicates the end of executing a
  matching case so skip to the DEPEND (on this level).

  After the DEPCASE is a DUP to duplicate the depend expression, then
  comes the case expression and then the operator which does the
  compare.

  DEPEXEC inspects the results on the stack top and if true continues
  else skips to the instruction after next DEPCASE, DEPELSE or to the DEPEND.

  DEPEND just pops off the initially pushed depend expression.

  */
static void gedep(StmNod *stm, int currentInstance)
{
  List *cases;

  emit0(C_STMOP, I_DEPSTART);
  geexp(stm->fields.depend.exp, currentInstance);
  /* For each case: */
  for (cases = stm->fields.depend.cases; cases != NULL; cases = cases->next) {
    /* If it is not the ELSE clause ... */
    if (cases->element.stm->fields.depcase.exp != NULL) {
      /* Generate a DEPCASE (if not first case) and a DUP */
      if (cases != stm->fields.depend.cases)
	emit0(C_STMOP, I_DEPCASE);
      emit0(C_STMOP, I_DUP);
      /* ...and the case expression (right hand + operator) */
      generateDependCase(cases->element.stm->fields.depcase.exp, currentInstance);
      emit0(C_STMOP, I_DEPEXEC);
    } else
      emit0(C_STMOP, I_DEPELSE);
    /* ...and then the statements */
    gestms(cases->element.stm->fields.depcase.stms, currentInstance);
  }
  emit0(C_STMOP, I_DEPEND);
}



/*----------------------------------------------------------------------

  gesystem()

  Generate SYSTEM statement.

  */
static void gesystem(StmNod *stm)
{
  encode(&stm->fields.system.fpos, &stm->fields.system.len);
  emit0(C_CONST, stm->fields.system.len);
  emit0(C_CONST, stm->fields.system.fpos);
  emit0(C_STMOP, I_SYSTEM);
}



/*----------------------------------------------------------------------

  gestm()

  Generate code for one statement.

  */
static void gestm(StmNod *stm, int currentInstance)
{
  switch (stm->class) {

  case NOP_STATEMENT:
    break;

  case PRINT_STATEMENT:
    geprint(stm);
    break;

  case QUIT_STATEMENT:
    emit0(C_STMOP, I_QUIT);
    break;

  case LOOK_STATEMENT:
    emit0(C_STMOP, I_LOOK);
    break;

  case SAVE_STATEMENT:
    emit0(C_STMOP, I_SAVE);
    break;

  case RESTORE_STATEMENT:
    emit0(C_STMOP, I_RESTORE);
    break;

  case RESTART_STATEMENT:
    emit0(C_STMOP, I_RESTART);
    break;

  case VISITS_STATEMENT:
    emit0(C_CONST, stm->fields.visits.count);
    emit0(C_STMOP, I_VISITS);
    break;

  case SCORE_STATEMENT:
    generateScore(stm);
    break;

  case DESCRIBE_STATEMENT:
    generateDescribe(stm);
    break;

  case SAY_STATEMENT:
    gesay(stm, currentInstance);
    break;

  case LIST_STATEMENT:
    gelist(stm);
    break;

  case EMPTY_STATEMENT:
    geempty(stm, currentInstance);
    break;

  case LOCATE_STATEMENT:
    gelocate(stm, currentInstance);
    break;

  case MAKE_STATEMENT:
    gemake(stm, currentInstance);
    break;

  case SET_STATEMENT:
    geset(stm, currentInstance);
    break;

  case INCREASE_STATEMENT:
  case DECREASE_STATEMENT:
    geincr(stm, currentInstance);
    break;

  case SCHEDULE_STATEMENT:
    geschedule(stm, currentInstance);
    break;

  case CANCEL_STATEMENT:
    gecancel(stm);
    break;

  case IF_STATEMENT:
    geif(stm, currentInstance);
    break;

  case USE_STATEMENT:
    geuse(stm, currentInstance);
    break;

  case DEPEND_STATEMENT:
    gedep(stm, currentInstance);
    break;

  case SYSTEM_STATEMENT:
    gesystem(stm);
    break;

  default:
    unimpl(&stm->srcp, "Code Generator");
    break;
  }
}


/*======================================================================

  gestms()

  Generate code for all the statements in a list.

  */
void gestms(List *stms, int currentInstance)
{
  List *current = stms;

  for (current = stms; current != NULL; current = current->next) {
    gestm(current->element.stm, currentInstance);
  }
}



/*======================================================================

  dustm()

  Dump a statement node.

  */
void dustm(StmNod *stm)
{
  if (stm == NULL) {
    put("NULL");
    return;
  }

  put("STM: ");
  switch(stm->class) {
  case PRINT_STATEMENT:
    put("PRINT ");
    break;
  case DESCRIBE_STATEMENT:
    put("DESCRIBE ");
    break;
  case SAY_STATEMENT:
    put("SAY ");
    break;
  case LIST_STATEMENT:
    put("LIST ");
    break;
  case IF_STATEMENT:
    put("IF ");
    break;
  case MAKE_STATEMENT:
    put("MAKE ");
    break;
  case SET_STATEMENT:
    put("SET ");
    break;
  case INCREASE_STATEMENT:
    put("INCR ");
    break;
  case DECREASE_STATEMENT:
    put("DECR ");
    break;
  case LOCATE_STATEMENT:
    put("LOCATE ");
    break;
  case EMPTY_STATEMENT:
    put("EMPTY ");
    break;
  case SCHEDULE_STATEMENT:
    put("SCHEDULE ");
    break;
  case CANCEL_STATEMENT:
    put("CANCEL ");
    break;
  case LOOK_STATEMENT:
    put("LOOK ");
    break;
  case QUIT_STATEMENT:
    put("QUIT ");
    break;
  case SCORE_STATEMENT:
    put("SCORE ");
    break;
  case USE_STATEMENT:
    put("USE ");
    break;
  case SAVE_STATEMENT:
    put("SAVE ");
    break;
  case RESTORE_STATEMENT:
    put("RESTORE ");
    break;
  case RESTART_STATEMENT:
    put("RESTART ");
    break;
  case VISITS_STATEMENT:
    put("VISITS ");
    break;
  default:
    put("*** UNKNOWN ***");
    break;
  }
  dumpSrcp(&stm->srcp);

  switch(stm->class) {
  case LOOK_STATEMENT:
  case QUIT_STATEMENT:
  case SAVE_STATEMENT:
  case RESTORE_STATEMENT:
  case RESTART_STATEMENT:
    break;
  default:
    in();
    switch(stm->class) {
    case PRINT_STATEMENT:
      put("fpos: "); dumpInt(stm->fields.print.fpos); nl();
      put("len: "); dumpInt(stm->fields.print.len);
      break;
    case SCORE_STATEMENT:
      put("count: "); dumpInt(stm->fields.score.count); nl();
      put("score: "); dumpInt(stm->fields.score.score);
      break;
    case DESCRIBE_STATEMENT:
      put("wht: "); dumpWhat(stm->fields.describe.wht);
      break;
    case SAY_STATEMENT:
      put("exp: "); dumpExpression(stm->fields.say.exp);
      break;
    case LIST_STATEMENT:
      put("wht: "); dumpWhat(stm->fields.list.wht);
      break;
    case EMPTY_STATEMENT:
      put("wht: "); dumpWhat(stm->fields.empty.wht); nl();
      put("whr: "); duwhr(stm->fields.empty.whr);
      break;
    case LOCATE_STATEMENT:
      put("wht: "); dumpWhat(stm->fields.locate.wht); nl();
      put("whr: "); duwhr(stm->fields.locate.whr);
      break;
    case MAKE_STATEMENT:
      put("wht: "); dumpWhat(stm->fields.list.wht); nl();
      put("not: "); dumpBool(stm->fields.make.not); nl();
      put("atr: "); dumpId(stm->fields.make.atr);
      break;
    case SET_STATEMENT:
      put("wht: "); dumpWhat(stm->fields.set.wht); nl();
      put("atr: "); dumpId(stm->fields.set.atr); nl();
      put("exp: "); dumpExpression(stm->fields.set.exp);
      break;
    case INCREASE_STATEMENT:
    case DECREASE_STATEMENT:
      put("wht: "); dumpWhat(stm->fields.incr.wht); nl();
      put("atr: "); dumpId(stm->fields.incr.atr); nl();
      put("step: "); dumpExpression(stm->fields.incr.step);
      break;
    case SCHEDULE_STATEMENT:
      put("id: "); dumpId(stm->fields.schedule.id); nl();
      put("whr: "); duwhr(stm->fields.locate.whr); nl();
      put("when: "); dumpExpression(stm->fields.schedule.when);
      break;
    case CANCEL_STATEMENT:
      put("id: "); dumpId(stm->fields.cancel.id);
      break;
    case IF_STATEMENT:
      put("exp: "); dumpExpression(stm->fields.iff.exp); nl();
      put("thn: "); dumpList(stm->fields.iff.thn, LIST_STM); nl();
      put("els: "); dumpList(stm->fields.iff.els, LIST_STM);
      break;
    case USE_STATEMENT:
      put("script: "); dumpId(stm->fields.use.script); nl();
      put("scriptno: "); dumpInt(stm->fields.use.scriptno); nl();
      put("actor: "); dumpId(stm->fields.use.actor);
      break;
    case VISITS_STATEMENT:
      put("count: "); dumpInt(stm->fields.visits.count);
      break;
    default:
      break;
    }
    out();
  }
}
