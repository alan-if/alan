#ifndef _ENCODE_H_
#define _ENCODE_H_
/*----------------------------------------------------------------------*\

			       ENCODE.H
			    Text encoding

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "../interpreter/acode.h"


/* DATA */
extern int txtlen;		/* Number of bytes of text data */

/* FUNCTIONS */

extern void eninit(void);
extern void incFreq(int ch);
extern void encode(long *fpos, int *len);
extern void enterm(void);
extern Aaddr gefreq(void);


#endif
