#ifndef _ALAN_H_
#define _ALAN_H_

#include "sysdep.h"
#include "types.h"

/* USE other definitions */

#include "srcp.h"
#include "lst.h"

/* Public data */

extern Srcp nulsrcp;		/* NULL source position */
extern int fileNo;		/* Number of last found file */
extern FILE *txtfil;		/* File for collected text */
extern FILE *datfil;		/* File for encoded text */
extern char advnam[];		/* The name of the adventure */
extern Bool verbose;		/* Verbose output */
extern long counter;		/* And counter for verbose mode */
extern List *includePaths;	/* The list of include paths to check */

/* Common procedures */

extern void unimpl(Srcp *srcp, char *phase);
extern void syserr(char str[]);
extern void *allocate(int len);
extern void terminate(int ecode);
extern void compile(void);

#endif
