/*----------------------------------------------------------------------*\

				SRCP.C
			 Source Position Type

\*----------------------------------------------------------------------*/

#define SRCP


#include "srcp.h"

#include "alan.h"

#include "dump.h"



/*======================================================================

  dusrcp()

  Dump a Source Position.

  */
#ifdef _PROTOTYPES_
void dusrcp(Srcp *srcp)
#else
void dusrcp(srcp)
     Srcp *srcp;
#endif
{
  char str[20];

  sprintf(str, "(%d,%d,%d)", srcp->file, srcp->line, srcp->col);
  put(str);
}



