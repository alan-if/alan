#ifndef _EXE_H_
#define _EXE_H_
/*----------------------------------------------------------------------*\

  exe.h

  Header file for instruction execution unit in Alan interpreter

\*----------------------------------------------------------------------*/

/* The event queue */
extern Boolean looking;		/* LOOKING? flag */
extern int dscrstkp;		/* Point into describe stack */


/* Functions: */
extern Boolean undo(void);
extern void sys(Aword fpos, Aword len);
extern Boolean confirm(MsgKind msgno);
extern Aword attributeOf(Aword instance, Aword atr);
extern void say(Aword instance);
extern void sayForm(Aword instance, SayForm form);
extern void sayint(Aword val);
extern void saystr(char *str);
extern Aword strattr(Aword id, Aword atr);
extern Aword strip(Abool stripFromBeginningNotEnd, Aint count, Abool stripWordsNotChars, Aword id, Aword atr);
extern Aword concat(Aword s1, Aword s2);
extern void setStringAttribute(Aword id, Aword atr, char *str);
extern void getStringFromFile(Aword fpos, Aword len);
extern void print(Aword fpos, Aword len);
extern void look(void);
extern void showImage(Aword image, Aword align);
extern void make(Aword id, Aword atr, Aword val);
extern void set(Aword id, Aword atr, Aword val);
extern void increase(Aword id, Aword atr, Aword step);
extern void decrease(Aword id, Aword atr, Aword step);
extern void use(Aword act, Aword scr);
extern void stop(Aword act);
extern void describe(Aword id);
extern void list(Aword cnt);
extern void locate(Aword id, Aword whr);
extern void empty(Aword cnt, Aword whr);
extern void score(Aword sc);
extern void visits(Aword v);
extern void schedule(Aword evt, Aword whr, Aword aft);
extern void cancelEvent(Aword evt);
extern void increaseEventQueue(void);
extern void pushGameState(void);
extern Boolean popGameState(void);
extern void quitGame(void);
extern void restartGame(void);
extern void saveGame(void);
extern void restoreGame(void);
extern Aword rnd(Aword from, Aword to);
extern Abool btw(Aint val, Aint from, Aint to);
extern Aword contains(Aword string, Aword substring);
extern Abool streq(char a[], char b[]);
extern Abool in(Aword obj, Aword cnt);
extern Aword where(Aword instance);
extern Aint agrmax(Aword atr, Aword whr);
extern Aint agrsum(Aword atr, Aword whr);
extern Aint agrcount(Aword whr);
extern Abool isHere(Aword instance);
extern Abool isNear(Aword instance);
extern Abool isA(Aword instance, Aword class);

#endif
