/*----------------------------------------------------------------------*\

				SRCP.C
			 Source Position Type

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "dump_x.h"
#include "srcp_x.h"


/*======================================================================

  equalSrcp()

  Compare two Source Positions.

  */
Bool equalSrcp(Srcp srcp1, Srcp srcp2)
{
  return (srcp1.col == srcp2.col && srcp1.file == srcp2.file && srcp1.line == srcp2.line);
}


/*======================================================================

  dumpSrcp()

  Dump a Source Position.

  */
void dumpSrcp(Srcp srcp)
{
  char str[20];

  sprintf(str, "(%d,%d,%d)", srcp.file, srcp.line, srcp.col);
  put(str);
}

