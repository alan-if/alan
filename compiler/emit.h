#ifndef _EMIT_H_
#define _EMIT_H_
/*----------------------------------------------------------------------*\

				EMIT.H
			ACODE Emit Header File

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "acode.h"


/* DATA */

AcdHdr acdHeader;


/* FUNCTIONS */

extern void initEmit(char acdfnm[]);
extern Aword emadr(void);
extern void emitstr(char str[]);
extern void emit(Aword word);
extern void emitN(void *words, int noOfWords);
extern void emit0(OpClass class, Aword op);
extern void terminateEmit(void);


#endif
