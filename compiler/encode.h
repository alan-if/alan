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

extern void initEncoding(void);
extern void incrementFrequency(int ch);
extern void encode(long *fpos, int *len);
extern void terminateEncoding(void);
extern Aaddr generateFrequencies(void);

#else
extern void initEncoding();
extern void incrementFrequency();
extern void encode();
extern void terminateEncoding();
extern Aaddr generateFrequencies();
#endif

#endif
