/*----------------------------------------------------------------------*\

			     Statement.c

			 Statements handling

\*----------------------------------------------------------------------*/

#include "Statement.h"

#include "dump.h"


/*======================================================================

  newStatement()

  Create a new statement node.

  */
#ifdef _PROTOTYPES_
Statement *newStatement(Srcp *srcp,	/* IN - Source position */
	      StatementKind kind)	/* IN - Kind of Statement */
#else
Statement *newStatement(srcp, kind)
     Srcp *srcp;
     StatementKind kind;
#endif
{
  Statement *new;			/* The newly allocated area */

  new = NEW(Statement);

  new->srcp = *srcp;
  new->kind = kind;

  return(new);
}



/*======================================================================

  analyseStatements()

  Analyze a list of statements.

 */
#ifdef _PROTOTYPES_
void analyseStatements(List *statements,
		       Event *event,
		       List *parameters)
#else
void analyseStatements(statements, event, parameters)
     List *statements;
     Event *event;
     List *parameters;
#endif
{
  /* 4f - analyse statements */
}



/*----------------------------------------------------------------------

  dumpStatementKind()

  Dump a STATEMENT StatementKind.

  */
#ifdef _PROTOTYPES_
static void dumpStatementKind(StatementKind kind)
#else
static void dumpStatementKind(kind)
     StatementKind kind;
#endif
{
  switch(kind) {
  case PRINT_STATEMENT: put("PRINT "); break;
  case DESCRIBE_STATEMENT: put("DESCRIBE "); break;
  case SAY_STATEMENT: put("SAY "); break;
  case LIST_STATEMENT: put("LIST "); break;
  case IF_STATEMENT: put("IF "); break;
  case MAKE_STATEMENT: put("MAKE "); break;
  case SET_STATEMENT: put("SET "); break;
  case INCR_STATEMENT: put("INCR "); break;
  case DECR_STATEMENT: put("DECR "); break;
  case LOCATE_STATEMENT: put("LOCATE "); break;
  case EMPTY_STATEMENT: put("EMPTY "); break;
  case SCHEDULE_STATEMENT: put("SCHEDULE "); break;
  case CANCEL_STATEMENT: put("CANCEL "); break;
  case LOOK_STATEMENT: put("LOOK "); break;
  case QUIT_STATEMENT: put("QUIT "); break;
  case SCORE_STATEMENT: put("SCORE "); break;
  case USE_STATEMENT: put("USE "); break;
  case SAVE_STATEMENT: put("SAVE "); break;
  case RESTORE_STATEMENT: put("RESTORE "); break;
  case VISITS_STATEMENT: put("VISITS "); break;
  default: put("*** UNKNOWN Statement Kind ***"); break;
  }
}



/*======================================================================

  dumpStatement()

  Dump a Statement node.

 */
#ifdef _PROTOTYPES_
void dumpStatement(Statement *statement)
#else
void dumpStatement(statement)
     Statement *statement;
#endif
{
  if (statement == NULL) {
    put("NULL");
    return;
  }

  put("STATEMENT: "); dumpSrcp(&statement->srcp); in();
  dumpStatementKind(statement->kind);

  switch(statement->kind) {
  case LOOK_STATEMENT:
  case QUIT_STATEMENT:
  case SAVE_STATEMENT:
  case RESTORE_STATEMENT:
    break;
  default:
    in();
    switch(statement->kind) {
    case PRINT_STATEMENT:
    case SYSTEM_STATEMENT:
      put("fpos: "); dumpInteger(statement->fields.print.fpos); nl();
      put("len: "); dumpInteger(statement->fields.print.len);
      break;
    case SCORE_STATEMENT:
      put("count: "); dumpInteger(statement->fields.score.count); nl();
      put("score: "); dumpInteger(statement->fields.score.score);
      break;
    case VISITS_STATEMENT:
      put("count: "); dumpInteger(statement->fields.visits.count);
      break;
    case DESCRIBE_STATEMENT:
      put("what: "); dumpWhat(statement->fields.describe.what);
      break;
    case SAY_STATEMENT:
      put("expression: "); dumpExpression(statement->fields.say.expression);
      break;
    case LIST_STATEMENT:
      put("what: "); dumpWhat(statement->fields.list.what);
      break;
    case EMPTY_STATEMENT:
      put("what: "); dumpWhat(statement->fields.empty.what); nl();
      put("where: "); dumpWhere(statement->fields.empty.where);
      break;
    case LOCATE_STATEMENT:
      put("what: "); dumpWhat(statement->fields.locate.what); nl();
      put("where: "); dumpWhere(statement->fields.locate.where);
      break;
    case MAKE_STATEMENT:
      put("what: "); dumpWhat(statement->fields.list.what); nl();
      put("not: "); dumpBoolean(statement->fields.make.not); nl();
      put("attribute: "); dumpId(statement->fields.make.attribute);
      break;
    case SET_STATEMENT:
      put("what: "); dumpWhat(statement->fields.set.what); nl();
      put("attribute: "); dumpId(statement->fields.set.attribute); nl();
      put("expression: "); dumpExpression(statement->fields.set.expression);
      break;
    case INCR_STATEMENT:
    case DECR_STATEMENT:
      put("what: "); dumpWhat(statement->fields.incr.what); nl();
      put("attribute: "); dumpId(statement->fields.incr.attribute); nl();
      put("by: "); dumpExpression(statement->fields.incr.by);
      break;
    case SCHEDULE_STATEMENT:
      put("id: "); dumpId(statement->fields.schedule.id); nl();
      put("where: "); dumpWhere(statement->fields.locate.where); nl();
      put("after: "); dumpExpression(statement->fields.schedule.after);
      break;
    case CANCEL_STATEMENT:
      put("id: "); dumpId(statement->fields.cancel.id);
      break;
    case IF_STATEMENT:
      put("expression: "); dumpExpression(statement->fields.iff.expression); nl();
      put("thn: "); dumpList(statement->fields.iff.thn, STATEMENT_NODE); nl();
      put("els: "); dumpList(statement->fields.iff.els, STATEMENT_NODE);
      break;
    case USE_STATEMENT:
      put("script: "); dumpId(statement->fields.use.script); nl();
      put("actor: "); dumpId(statement->fields.use.actor);
      break;
    default:
      break;
    }
    out();
  }
}


