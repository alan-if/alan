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
#ifdef _PROTOTYPES_

extern void initEmit(char acdfnm[]);
extern Aword emitAddress(void);
extern void emitString(char str[]);
extern void emit(Aword word);
extern void emit0(OpClass class, Aword op);
extern void terminateEmit(AcdHdr *hdr);

#else
extern void initEmit();
extern Aword emitAddress();
extern void emitString();
extern void emit();
extern void emit0();
extern void terminateEmit();
#endif

#endif
