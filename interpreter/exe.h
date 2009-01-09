#ifndef _EXE_H_
#define _EXE_H_
/*----------------------------------------------------------------------*\

  exe.h

  Header file for instruction execution unit in Alan interpreter

\*----------------------------------------------------------------------*/

/* IMPORTS */
#include <setjmp.h>
#include "acode.h"
#include "types.h"
#include "set.h"


/* CONSTANTS */
#define NO_JUMP_RETURN 0
#define ERROR_RETURN 1
#define UNDO_RETURN 2


/* DATA */

/* The text and message file */
extern FILE *textFile;

/* Global failure flag */
extern Bool fail;

/* Long jump buffer for restart, errors and undo */
extern jmp_buf restartLabel;
extern jmp_buf returnLabel;
extern jmp_buf forfeitLabel;


/* FUNCTIONS */
extern void sys(Aword fpos, Aword len);
extern void sayInstance(int id);
extern void say(int instance);
extern void sayForm(int instance, SayForm form);
extern void sayInteger(int val);
extern void sayString(char *str);
extern Aword strip(Bool stripFromBeginningNotEnd, int count, Bool stripWordsNotChars, int id, int atr);
extern Aword concat(Aword s1, Aword s2);
extern void include(int id, int atr, Aword member);
extern void exclude(int id, int atr, Aword member);
extern char *getStringFromFile(Aword fpos, Aword len);
extern void print(Aword fpos, Aword len);
extern void setStyle(int style);
extern void look(void);
extern void showImage(int image, int align);
extern void playSound(int sound);
extern void increase(int id, int atr, Aword step);
extern void decrease(int id, int atr, Aword step);
extern void use(int act, int scr);
extern void stop(int act);
extern void describe(int id);
extern void list(int cnt);
extern void locate(int id, int whr);
extern void empty(int cnt, int whr);
extern void score(Aword sc);
extern void visits(Aword v);
extern void schedule(Aword evt, Aword whr, Aword aft);
extern void cancelEvent(Aword evt);
extern void undo(void);
extern void quitGame(void);
extern void restartGame(void);
extern int randomInteger(int from, int to);
extern int randomInContainer(int cont);
extern Bool btw(int val, int from, int to);
extern Bool contains(Aword string, Aword substring);
extern Bool streq(char a[], char b[]);
extern Bool at(int theInstance, int other,Bool directly);
extern Bool in(int theInstance, int theContainer, Bool directly);
extern int where(int instance, Bool directly);
extern int locationOf(int instance);
extern int containerSize(int where, Bool directly);
extern int getContainerMember(int container, int index, Bool directly);
extern Bool isHere(int instance, Bool directly);
extern Bool isNearby(int instance, Bool directly);
extern Bool isNear(int instance, int other, Bool directly);
extern Bool isA(int instance, int class);

#endif
