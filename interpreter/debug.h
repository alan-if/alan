#ifndef _DEBUG_H_
#define _DEBUG_H_
/*----------------------------------------------------------------------*\

  debug.h

  Header file for debug handler in Alan interpreter

\*----------------------------------------------------------------------*/

/* TYPES */


#ifdef _PROTOTYPES_

extern void saveInfo(void);
extern void restoreInfo(void);
extern void debug(void);

#else
extern void saveInfo();
extern void restoreInfo();
extern void debug();
#endif

#endif
