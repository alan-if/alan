#ifndef _EMIT_H_
#define _EMIT_H_
/*----------------------------------------------------------------------*\

				EMIT.H
			ACODE Emit Header File

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "../interpreter/acode.h"


/* DATA */

ACodeHeader acodeHeader;


/* FUNCTIONS */

extern void initEmit(char acodeFileName[]);
extern Aword nextEmitAddress(void);
extern void emitString(char str[]);
extern void emit(Aword word);
extern void emitVariable(Aword word);
extern void emitConstant(Aword word);
extern void emitN(void *words, int noOfWords);
extern void emitEntry(void *entryToEmit, int noOfBytes);
extern void emit0(Aword op);
extern void emit1(Aword op, Aword arg1);
extern void emit2(Aword op, Aword arg1, Aword arg2);
extern void emit3(Aword op, Aword arg1, Aword arg2, Aword arg3);
extern void terminateEmit();
extern void emitTextDataToAcodeFile(char dataFileName[]);
extern void emitHeader();

#endif
