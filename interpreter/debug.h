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
extern Breakpoint breakpoint[BREAKPOINTMAX];

/* FUNCTIONS: */
extern void saveInfo(void);
extern void restoreInfo(void);
extern Bool breakpointIndex(int line);
extern char *sourceFileName(int file);
extern char *readSourceLine(int line, int file);
extern void showSourceLine(int line, int fileNumber);
extern void debug(Bool calledFromBreakpoint, int line, int fileNumber);
extern void traceSay(int item);

#endif
