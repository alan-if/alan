#ifndef _DUMP_H
#define _DUMP_H
/*----------------------------------------------------------------------*\

				DUMP.H
			     Dump Support

\*----------------------------------------------------------------------*/

/* USE other definitions */
#include "types.h"


/* Types: */

/* Dump kinds */
typedef enum dmpkd {
    DMPALL = 1,
    DMPSYN = (long)DMPALL<<1,
    DMPSTX = (long)DMPSYN<<1,
    DMPVRB = (long)DMPSTX<<1,
    DMPLOC = (long)DMPVRB<<1,
    DMPOBJ = (long)DMPLOC<<1,
    DMPCNT = (long)DMPOBJ<<1,
    DMPEVT = (long)DMPCNT<<1,
    DMPACT = (long)DMPEVT<<1,
    DMPRUL = (long)DMPACT<<1
} DmpKind;


typedef enum nodkd {
  ACTNOD,
  ALTNOD,
  ATRNOD,
  CHKNOD,
  CLANOD,
  CNTNOD,
  ELMNOD,
  EVTNOD,
  EXPNOD,
  EXTNOD,
  LIMNOD,
  LOCNOD,
  NAMNOD,
  OBJNOD,
  RULNOD,
  SCRNOD,
  STMNOD,
  STPNOD,
  STXNOD,
  SYNNOD,
  VRBNOD
} NodKind;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

extern void put(char str[]);
extern void nl(void);
extern void in(void);
extern void out(void);
extern void dustr(char s[]);
extern void duint(int i);
extern void duBoolean(Boolean b);

#else
extern void put();
extern void nl();
extern void in();
extern void out();
extern void dusrcp();
extern void dustr();
extern void duint();
extern void duBoolean();
#endif

#endif
