#ifndef _ALAN_H_
#define _ALAN_H_

#include "sysdep.h"
#include "types.h"

/* USE other definitions */
#include "Srcp.h"


/* Public data */

extern Srcp nullSrcp;		/* NULL source position */
extern int fileNumber;		/* Number of last found file */
extern FILE *textFile;		/* File for collected text */
extern FILE *dataFile;		/* File for encoded text */
extern char adventureName[];	/* The name of the adventure */
extern Bool verbose;		/* Verbose output */


/* Common procedures */

#ifdef _PROTOTYPES_

extern void unimplemented(Srcp *srcp, char *phase);
extern void syserr(char str[]);
extern void *allocate(int len);

#else
extern void unimplemented();
extern void syserr();
extern void *allocate();
#endif

#endif
