#ifndef _STM_H_
#define _STM_H_
/*----------------------------------------------------------------------*\

				STM.H
			   Statement Nodes

\*----------------------------------------------------------------------*/

/* USE: */

#include "srcp.h"
#include "wht.h"
#include "whr.h"
#include "id.h"
#include "exp.h"
#include "lst.h"


/* Types: */

typedef enum StmKind {		/* STATEMENT kinds */
  NOP_STATEMENT,
  PRINT_STATEMENT,
  QUIT_STATEMENT,
  LOOK_STATEMENT,
  SAVE_STATEMENT,
  RESTORE_STATEMENT,
  RESTART_STATEMENT,
  SCORE_STATEMENT,
  VISITS_STATEMENT,
  DESCRIBE_STATEMENT,
  SAY_STATEMENT,
  LIST_STATEMENT,
  EMPTY_STATEMENT,
  LOCATE_STATEMENT,
  MAKE_STATEMENT,
  SET_STATEMENT,
  INCREASE_STATEMENT,
  DECREASE_STATEMENT,
  SCHEDULE_STATEMENT,
  CANCEL_STATEMENT,
  IF_STATEMENT,
  USE_STATEMENT,
  SYSTEM_STATEMENT,
  DEPEND_STATEMENT,
  DEPENDCASE_STATEMENT
} StmKind;
    

typedef struct StmNod {		/* STATEMENT */
  Srcp srcp;			/* Source position */
  StmKind class;		/* Class tag */
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
      What *wht;		/* What? */
    } describe;

    struct {			/* for SAY */
      Expression *exp;		/* What? */
    } say;

    struct {			/* for LIST */
      What *wht;		/* What? */
    } list;

    struct {			/* for EMPTY */
      What *wht;		/* What? */
      Where *where;		/* Where? */
    } empty;

    struct {			/* for LOCATE */
      What *wht;		/* What? */
      Where *whr;		/* Where? */
    } locate;

    struct {			/* for MAKE */
      What *wht;		/* What? */
      Bool not;			/* Was there a NOT ? */
      IdNode *atr;		/* Id of the attribute */
    } make;

    struct {			/* for SET */
      What *wht;		/* What? */
      IdNode *atr;
      Expression *exp;		/* The new value */
    } set;

    struct {			/* for INCR/DECR */
      What *wht;		/* What? */
      IdNode *atr;
      Expression *step;		/* By what? */
    } incr;

    struct {			/* for SCHEDULE */
      IdNode *id;		/* Name of the event */
      Where *whr;		/* Where? */
      Expression *when;
    } schedule;

    struct {			/* for CANCEL */
      IdNode *id;		/* Name of the event */
    } cancel;

    struct {			/* for IF */
      Expression *exp;
      List *thn;
      List *els;
    } iff;

    struct {			/* USE */
      int scriptno;		/* Which script to use (number) */
      IdNode *script;		/* Which script to use (name) */
      IdNode *actor;		/* For which actor */
    } use;

    struct {			/* SYSTEM */
      long fpos;		/* Position to string to execute */
      int len;			/* Length of the string */
    } system;

    struct {			/* DEPEND */
      Expression *exp;		/* Depending expression */
      List *cases;		/* The cases */
    } depend;

    struct {			/* DEPCASE */
      Expression *exp;		/* The partial right hand expression */
      List *stms;
    } depcase;

  } fields;
} StmNod;



#endif
