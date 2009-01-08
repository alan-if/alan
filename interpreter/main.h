#ifndef _MAIN_H_
#define _MAIN_H_
/*----------------------------------------------------------------------*\

  Header file for main unit of ARUN Alan System interpreter

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"
#include <setjmp.h>


/* DATA */

/* Amachine data structures - Dynamic */

/* Amachine data structures - Static */

extern VerbEntry *vrbs;		/* Verb table pointer */
extern SyntaxEntry *stxs;	/* Syntax table pointer */
extern RulEntry *ruls;		/* Rule table pointer */

/* Long jump buffer for restart, errors and undo */
extern jmp_buf restartLabel;
extern jmp_buf returnLabel;
extern jmp_buf forfeitLabel;
#define NO_JUMP_RETURN 0
#define ERROR_RETURN 1
#define UNDO_RETURN 2


/* FUNCTIONS: */

/* Run the game! */
extern void run(void);

#endif
