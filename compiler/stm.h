#ifndef _STM_H_
#define _STM_H_
/*----------------------------------------------------------------------*\

				STM.H
			   Statement Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "alan.h"
#include "srcp.h"
#include "wht.h"
#include "whr.h"
#include "id.h"
#include "ins.h"
#include "exp.h"
#include "lst.h"
#include "evt.h"


/* Types: */

typedef enum StmKind {		/* STATEMENT kinds */
  STM_NOP,			/* No operation */
  STM_PRINT,
  STM_QUIT,
  STM_LOOK,
  STM_SAVE,
  STM_RESTORE,
  STM_RESTART,
  STM_SCORE,
  STM_VISITS,
  STM_DESCRIBE,
  STM_SAY,
  STM_LIST,
  STM_EMPTY,
  STM_LOCATE,
  STM_MAKE,
  STM_SET,
  STM_INCR,
  STM_DECR,
  STM_SCHEDULE,
  STM_CANCEL,
  STM_IF,
  STM_USE,
  STM_SYSTEM,
  STM_DEPEND,
  STM_DEPCASE
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
      WhtNod *wht;		/* What? */
    } describe;

    struct {			/* for SAY */
      ExpNod *exp;		/* What? */
    } say;

    struct {			/* for LIST */
      WhtNod *wht;		/* What? */
    } list;

    struct {			/* for EMPTY */
      WhtNod *wht;		/* What? */
      WhrNod *whr;		/* Where? */
    } empty;

    struct {			/* for LOCATE */
      WhtNod *wht;		/* What? */
      WhrNod *whr;		/* Where? */
    } locate;

    struct {			/* for MAKE */
      WhtNod *wht;		/* What? */
      Bool not;			/* Was there a NOT ? */
      IdNod *atr;		/* Id of the attribute */
    } make;

    struct {			/* for SET */
      WhtNod *wht;		/* What? */
      IdNod *atr;
      ExpNod *exp;		/* The new value */
    } set;

    struct {			/* for INCR/DECR */
      WhtNod *wht;		/* What? */
      IdNod *atr;
      ExpNod *step;		/* By what? */
    } incr;

    struct {			/* for SCHEDULE */
      IdNod *id;		/* Name of the event */
      WhrNod *whr;		/* Where? */
      ExpNod *when;
    } schedule;

    struct {			/* for CANCEL */
      IdNod *id;		/* Name of the event */
    } cancel;

    struct {			/* for IF */
      ExpNod *exp;
      List *thn;
      List *els;
    } iff;

    struct {			/* USE */
      int scriptno;		/* Which script to use (number) */
      IdNod *script;		/* Which script to use (name) */
      IdNod *actor;		/* For which actor */
    } use;

    struct {			/* SYSTEM */
      long fpos;		/* Position to string to execute */
      int len;			/* Length of the string */
    } system;

    struct {			/* DEPEND */
      ExpNod *exp;		/* Depending expression */
      List *cases;		/* The cases */
    } depend;

    struct {			/* DEPCASE */
      ExpNod *exp;		/* The partial right hand expression */
      List *stms;
    } depcase;

  } fields;
} StmNod;



/* Data: */



/* Methods: */


/* Create a new Statement node */
extern StmNod *newstm(Srcp *srcp,
		      StmKind class);

/* Analyze a list of statements */
extern void anstms(List *stms,
		   InsNod *ins,	/* Instance context */
		   EvtNod *evt,	/* Event context */
		   List *pars);

/* Generate code for a list of statements */
extern void gestms(List *stms, InsNod *ins);

/* Dump a statement node */
extern void dustm(StmNod *stm);


#endif
