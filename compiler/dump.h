#ifndef _DUMP_H
#define _DUMP_H
/*----------------------------------------------------------------------*\

				DUMP.H
			     Dump Support

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "sysdep.h"
#include "types.h"


/* Types: */

/* Dump kinds */
typedef enum dmpKd {
  DMPNON = 0,
  DMPSYN = 1L,
  DMPSTX = (long)DMPSYN<<1,
  DMPVRB = (long)DMPSTX<<1,
  DMPCLA = (long)DMPVRB<<1,
  DMPINS = (long)DMPCLA<<1,
  DMPCNT = (long)DMPINS<<1,
  DMPEVT = (long)DMPCNT<<1,
  DMPRUL = (long)DMPEVT<<1,
  DMPALL = (long)DMPRUL<<1
} DmpKind;


/* Data: */


/* Functions: */

extern void put(char str[]);
extern void nl(void);
extern void in(void);
extern void out(void);
extern void dumpString(char s[]);
extern void dumpAddress(int adr);
extern void dumpPointer(void *adr);
extern void dumpInt(int i);
extern void dumpBool(Bool b);


#endif
