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

extern void action(ParamEntry *parameters, ParamEntry *multipleMatches);

#endif
