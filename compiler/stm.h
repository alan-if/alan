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
      IdNode *atr;		/* Id of the attribute */
    } make;

    struct {			/* for SET */
      WhtNod *wht;		/* What? */
      IdNode *atr;
      ExpNod *exp;		/* The new value */
    } set;

    struct {			/* for INCR/DECR */
      WhtNod *wht;		/* What? */
      IdNode *atr;
      ExpNod *step;		/* By what? */
    } incr;

    struct {			/* for SCHEDULE */
      IdNode *id;		/* Name of the event */
      WhrNod *whr;		/* Where? */
      ExpNod *when;
    } schedule;

    struct {			/* for CANCEL */
      IdNode *id;		/* Name of the event */
    } cancel;

    struct {			/* for IF */
      ExpNod *exp;
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
      ExpNod *exp;		/* Depending expression */
      List *cases;		/* The cases */
    } depend;

    struct {			/* DEPCASE */
      ExpNod *exp;		/* The partial right hand expression */
      List *stms;
    } depcase;

  } fields;
} StmNod;



#endif
