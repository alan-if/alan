#ifndef _DEBUG_H_
#define _DEBUG_H_
/*----------------------------------------------------------------------*\

  debug.h

  Header file for debug handler in Alan interpreter

\*----------------------------------------------------------------------*/

/* TYPES: */


/* DATA: */
#define BREAKPOINTMAX 50
extern int breakpointCount;
extern int breakpoint[BREAKPOINTMAX];

/* FUNCTIONS: */
extern void saveInfo(void);
extern void restoreInfo(void);
extern Boolean breakpointIndex(int line);
extern void debug(Boolean calledFromBreakpoint, int line, int fileNumber);
extern void traceSay(int item);

#endif
