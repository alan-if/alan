#ifndef _ENCODE_H_
#define _ENCODE_H_
/*----------------------------------------------------------------------*\

			       ENCODE.H
			    Text encoding

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "acode.h"


/* DATA */
extern int txtlen;		/* Number of bytes of text data */

/* FUNCTIONS */
#ifdef _PROTOTYPES_

extern void eninit(void);
extern void incFreq(int ch);
extern void encode(long *fpos, int *len);
extern void enterm(void);
extern Aaddr gefreq(void);

#else
extern void eninit();
extern void incFreq();
extern void encode();
extern void enterm();
extern Aaddr gefreq();
#endif

#endif
