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

extern void eminit(char acdfnm[], Boolean rev);
extern Aword emadr(void);
extern void emitstr(char str[]);
extern void emit(Aword word);
extern void emit0(OpClass class, Aword op);
extern void emterm(AcdHdr *hdr);

#else
extern void eminit();
extern Aword emadr();
extern void emitstr();
extern void emit();
extern void emit0();
extern void emterm();
#endif

#endif
