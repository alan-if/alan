/*----------------------------------------------------------------------*\

				SRCP.C
			 Source Position Type

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "dump.h"

#include "srcp.h"


/*======================================================================

  dusrcp()

  Dump a Source Position.

  */
void dusrcp(Srcp *srcp)
{
  char str[20];

  sprintf(str, "(%d,%d,%d)", srcp->file, srcp->line, srcp->col);
  put(str);
}



