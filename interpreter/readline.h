#ifndef _READLINE_H_
#define _READLINE_H_
/*----------------------------------------------------------------------*\

  readline.h

  Header file for user input, history and editing support

\*----------------------------------------------------------------------*/

#ifdef _PROTOTYPES_
extern void readline(char usrbuf[]);

#else
extern void readline();
#endif

#endif
