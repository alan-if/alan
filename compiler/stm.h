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
#include "nam.h"
#include "exp.h"
#include "lst.h"
#include "obj.h"
#include "act.h"
#include "loc.h"
#include "evt.h"


/* Types: */

typedef enum StmKind {		/* STATEMENT kinds */
  STM_NOP,			/* No operation */
  STM_PRINT,
  STM_QUIT,
  STM_LOOK,
  STM_SAVE,
  STM_RESTORE,
  STM_SCORE,
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
  STM_USE
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
      Boolean not;		/* Was there a NOT ? */
      NamNod *atr;
    } make;

    struct {			/* for SET */
      WhtNod *wht;		/* What? */
      NamNod *atr;
      ExpNod *exp;		/* The new value */
    } set;

    struct {			/* for INCR/DECR */
      WhtNod *wht;		/* What? */
      NamNod *atr;
      ExpNod *step;		/* By what? */
    } incr;

    struct {			/* for SCHEDULE */
      NamNod *nam;		/* Name of the event */
      WhrNod *whr;		/* Where? */
      int when;
    } schedule;

    struct {			/* for CANCEL */
      NamNod *nam;		/* Name of the event */
    } cancel;

    struct {			/* for IF */
      ExpNod *exp;
      List *thn;
      List *els;
    } iff;

    struct {
      int script;		/* Which script to use */
      NamNod *actor;		/* For which actor */
    } use;

  } fields;
} StmNod;



/* Data: */



/* Methods: */

#ifdef _PROTOTYPES_

/* Create a new Statement node */
extern StmNod *newstm(Srcp *srcp,
		      StmKind class);

/* Analyze a list of statements */
extern void anstms(List *stms,
		   ActNod *act,
		   EvtNod *evt,
		   List *pars);

/* Generate code for a list of statements */
extern void gestms(List *stms, ActNod *act);

/* Dump a statement node */
extern void dustm(StmNod *stm);

#else
extern StmNod *newstm();
extern void anstms();
extern void gestms();
extern void dustm();
#endif

#endif
