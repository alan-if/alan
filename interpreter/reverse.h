#ifndef _REVERSE_H_
#define _REVERSE_H_
/*----------------------------------------------------------------------*\

  reverse.h

  Header file for reverse-module

\*----------------------------------------------------------------------*/

/* TYPES */


#ifdef _PROTOTYPES_

extern void reverseACD(void);
extern void reverse(Aword *word);

#else
extern void reverseACD();
extern void reverse();
#endif

#endif
