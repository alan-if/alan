#ifndef _EMIT_H_
#define _EMIT_H_
/*----------------------------------------------------------------------*\

				EMIT.H
			ACODE Emit Header File

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "../interpreter/acode.h"


/* DATA */

AcdHdr acdHeader;


/* FUNCTIONS */

extern void initEmit(char acodeFileName[]);
extern Aword emadr(void);
extern void emitString(char str[]);
extern void emit(Aword word);
extern void emitN(void *words, int noOfWords);
extern void emitEntry(void *entryToEmit, int noOfBytes);
extern void emit0(OpClass class, Aword op);
extern void terminateEmit();
extern void emitTextDataToAcodeFile(char dataFileName[]);
extern void emitHeader();

#endif
