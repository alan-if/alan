#ifndef _RULES_H_
#define _RULES_H_
/*----------------------------------------------------------------------*\

  rules.h

  Header file for rules handler in Alan interpreter

\*----------------------------------------------------------------------*/

/* IMPORTS */
#include "acode.h"

/* TYPES */

typedef struct RulEntry {   /* RULE TABLE */
  Abool run;            /* Is rule already run? */
  Aaddr exp;            /* Address to expression code */
  Aaddr stms;           /* Address to run */
} RulEntry;


/* DATA */
extern RulEntry *ruls;      /* Rule table pointer */


/* FUNCTIONS */
extern void rules(void);

#endif
