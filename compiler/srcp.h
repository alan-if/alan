#ifndef _SRCP_H
#define _SRCP_H
/*----------------------------------------------------------------------*\

				SRCP.H
			 Source Position Type

\*----------------------------------------------------------------------*/


#include "types.h"


/* Types: */

typedef struct Srcp {
  int file;
  int line;
  int col;
} Srcp;


/* Data: */


/* Functions: */
extern void dumpSrcp(Srcp *srcp);
extern Bool equalSrcp(Srcp srcp1, Srcp srcp2);

#endif
