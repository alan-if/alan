#ifndef _EMIT_H_
#define _EMIT_H_
/*----------------------------------------------------------------------*\

				EMIT.H
			ACODE Emit Header File

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "acode.h"


/* DATA */


/* FUNCTIONS */

extern void initEmit(char acdfnm[]);
extern Aword emadr(void);
extern void emitstr(char str[]);
extern void emit(Aword word);
extern void emit0(OpClass class, Aword op);
extern void terminateEmit(AcdHdr *hdr);


#endif
