#ifndef _SRCP_H
#define _SRCP_H
/*----------------------------------------------------------------------*\

				SRCP.H
			 Source Position Type

\*----------------------------------------------------------------------*/


/* Types: */

typedef struct Srcp {
  int file;
  int line;
  int col;
} Srcp;



/* Data: */


/* Functions: */
extern void dusrcp(Srcp *srcp);

#endif
