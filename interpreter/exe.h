#ifndef _EXE_H_
#define _EXE_H_
/*----------------------------------------------------------------------*\

  exe.h

  Header file for instruction execution unit in Alan interpreter

\*----------------------------------------------------------------------*/

/* The event queue */
extern int dscrstkp;		/* Point into describe stack */


/* Functions: */
extern Bool undo(void);
extern void sys(Aword fpos, Aword len);
extern Bool confirm(MsgKind msgno);
extern Aword attributeOf(Aword instance, Aword atr);
extern void say(Aword instance);
extern void sayForm(Aword instance, SayForm form);
extern void sayInteger(Aword val);
extern void sayString(char *str);
extern Aword strattr(Aword id, Aword atr);
extern Aword strip(Abool stripFromBeginningNotEnd, Aint count, Abool stripWordsNotChars, Aword id, Aword atr);
extern Aword concat(Aword s1, Aword s2);
extern void setStringAttribute(Aword id, Aword atr, char *str);
extern void clearSetAttribute(Aword id, Aword atr);
extern void getStringFromFile(Aword fpos, Aword len);
extern void print(Aword fpos, Aword len);
extern void setStyle(Aint style);
extern void look(void);
extern void showImage(Aword image, Aword align);
extern void playSound(Aword sound);
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
extern Bool popGameState(void);
extern void quitGame(void);
extern void restartGame(void);
extern void save(void);
extern void restore(void);
extern Aword randomInteger(Aword from, Aword to);
extern Aword randomInContainer(Aint cont);
extern Aword randomInSet(Aset set);
extern Abool btw(Aint val, Aint from, Aint to);
extern Aword contains(Aword string, Aword substring);
extern Abool streq(char a[], char b[]);
extern Abool at(Aint theInstance, Aint other, Abool directly);
extern Abool in(Aint theInstance, Aint theContainer, Abool directly);
extern Aword where(Aint instance, Abool directly);
extern Aword location(Aint instance);
extern Aint agrmax(Aint atr, Aint whr);
extern Aint agrsum(Aint atr, Aint whr);
extern Aint agrcount(Aint where);
extern Abool isHere(Aword instance, Abool directly);
extern Abool isNearby(Aint instance, Abool directly);
extern Abool isNear(Aint instance, Aint other, Abool directly);
extern Abool isA(Aint instance, Aint class);

#endif
