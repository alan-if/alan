#ifndef _SYSERR_H_
#define _SYSERR_H_
/*----------------------------------------------------------------------*\

  SYSERR.H

  Header file for syserr unit of ARUN Alan System interpreter

\*----------------------------------------------------------------------*/

/* Data: */

/* Functions: */
#define SYSERR(m) syserr(m, __FUNCTION__)
extern void syserr(char *msg);

#endif
