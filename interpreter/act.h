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


/* FUNCTIONS */

extern Bool exitto(int to, int from);
extern void action(ParamEntry *plst);
extern void go(int location, int dir);

#endif
