#ifndef _STATEMENT_H_
#define _STATEMENT_H_
/*----------------------------------------------------------------------*\

			     Statement.h

			   Statement Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "alan.h"

#include "What.h"
#include "Where.h"
#include "Event.h"
#include "Expression.h"
#include "Id.h"
#include "List.h"



/* Types: */

typedef enum StatementKind {		/* STATEMENT kinds */
  NOP_STATEMENT,			/* No operation */
  PRINT_STATEMENT,
  QUIT_STATEMENT,
  LOOK_STATEMENT,
  SAVE_STATEMENT,
  RESTORE_STATEMENT,
  SCORE_STATEMENT,
  VISITS_STATEMENT,
  DESCRIBE_STATEMENT,
  SAY_STATEMENT,
  LIST_STATEMENT,
  EMPTY_STATEMENT,
  LOCATE_STATEMENT,
  MAKE_STATEMENT,
  SET_STATEMENT,
  INCR_STATEMENT,
  DECR_STATEMENT,
  SCHEDULE_STATEMENT,
  CANCEL_STATEMENT,
  IF_STATEMENT,
  USE_STATEMENT,
  SYSTEM_STATEMENT
} StatementKind;
    

typedef struct Statement {	/* STATEMENT */
  Srcp srcp;			/* Source position */
  StatementKind kind;		/* Class tag */
  union {			/* Class dependent fields */

    struct {			/* for PRINT */
      long fpos;		/* Position in the text file */
      int len;			/* and length */
    } print;

    struct {			/* for SCORE */
      int count;		/* Number of this SCORE */
      int score;		/* Its value */
    } score;

    struct {			/* for VISITS */
      int count;		/* How many times not to show long descriptions */
    } visits;

    struct {			/* for DESCRIBE */
      What *what;		/* What? */
    } describe;

    struct {			/* for SAY */
      Expression *expression;	/* What? */
    } say;

    struct {			/* for LIST */
      What *what;		/* What? */
    } list;

    struct {			/* for EMPTY */
      What *what;		/* What? */
      Where *where;		/* Where? */
    } empty;

    struct {			/* for LOCATE */
      What *what;		/* What? */
      Where *where;		/* Where? */
    } locate;

    struct {			/* for MAKE */
      What *what;		/* What? */
      Bool not;			/* Was there a NOT ? */
      Id *attribute;		/* The attribute */
    } make;

    struct {			/* for SET */
      What *what;		/* What? */
      Id *attribute;
      Expression *expression;	/* The new value */
    } set;

    struct {			/* for INCR/DECR */
      What *what;		/* What? */
      Id *attribute;
      Expression *by;		/* By what? */
    } incr;

    struct {			/* for SCHEDULE */
      Id *id;			/* Name of the event */
      Where *where;		/* Where? */
      Expression *after;
    } schedule;

    struct {			/* for CANCEL */
      Id *id;			/* Name of the event */
    } cancel;

    struct {			/* for IF */
      Expression *expression;
      List *thn;
      List *els;
    } iff;

    struct {			/* USE */
      Id *script;		/* Which script to use */
      Id *actor;		/* For which actor */
    } use;

    struct {			/* SYSTEM */
      long fpos;		/* Position to string to execute */
      int len;			/* Length of the string */
    } system;

  } fields;
} Statement;



/* Data: */



/* Methods: */

#ifdef _PROTOTYPES_

/* Create a new Statement node */
extern Statement *newStatement(Srcp *srcp,
			       StatementKind kind);

/* Analyze a list of statements */
extern void analyseStatements(List *statements,
			      Event *event,
			      List *parameters);

/* Generate code for a list of statements */
extern void generateStatements(List *statements);

/* Dump a statement node */
extern void dumpStatement(Statement *statement);

#else
extern Statement *newStatement();
extern void analyseStatements();
extern void generateStatements();
extern void dumpStatement();
#endif

#endif
