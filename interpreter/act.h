#ifndef _ACT_H_
#define _ACT_H_
/*----------------------------------------------------------------------*\

  ACT.H

  Header file for action unit of ARUN Alan System interpreter

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"
#include <setjmp.h>


/* DATA */

/* trycheck() execute flags */
#define EXECUTE TRUE
#define DONT_EXECUTE FALSE

/* FUNCTIONS */

#ifdef _PROTOTYPES_

extern Boolean checklim(Aword cnt, Aword obj);
extern Boolean trycheck(Aaddr adr, Boolean act);
extern Boolean possible(void);
extern Boolean exitto(int to, int from);
extern void action(ParamEntry *plst);
extern void go(int dir);

#else
extern Boolean checklim();
extern Boolean trycheck();
extern Boolean possible();
#endif

#endif
