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
typedef enum dmpkd {
    DMPSYN = 1L,
    DMPSTX = (long)DMPSYN<<1,
    DMPVRB = (long)DMPSTX<<1,
    DMPLOC = (long)DMPVRB<<1,
    DMPOBJ = (long)DMPLOC<<1,
    DMPCNT = (long)DMPOBJ<<1,
    DMPEVT = (long)DMPCNT<<1,
    DMPACT = (long)DMPEVT<<1,
    DMPRUL = (long)DMPACT<<1,
    DMPALL = (long)DMPRUL<<1
} DmpKind;


typedef enum nodkd {
  UNKNOD,
  ACTNOD,
  ALTNOD,
  ATRNOD,
  CHKNOD,
  CNTNOD,
  ELMNOD,
  EVTNOD,
  EXPNOD,
  EXTNOD,
  LIMNOD,
  LOCNOD,
  NAMNOD,
  OBJNOD,
  RESNOD,
  RULNOD,
  SCRNOD,
  STMNOD,
  STPNOD,
  STXNOD,
  SYNNOD,
  VRBNOD,
  MSGNOD,
  STRNOD,
  REFNOD,
  LSTNOD,
  EENTNOD
} NodKind;


/* Data: */


/* Functions: */

extern void put(char str[]);
extern void nl(void);
extern void in(void);
extern void out(void);
extern void dustr(char s[]);
extern void duadr(int adr);
extern void duptr(void *adr);
extern void duint(int i);
extern void duBool(Bool b);


#endif
