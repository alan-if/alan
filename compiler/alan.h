#ifndef _ALAN_H_
#define _ALAN_H_

#include "sysdep.h"
#include "types.h"

/* USE other definitions */

#include "lst.h"
#include "options.h"
#include "charset.h"


/* Public data */
extern int fileNo;          /* Number of last found file */
extern FILE *txtfil;        /* File for collected text */
extern FILE *datfil;        /* File for encoded text */
extern CharSet input_encoding;		/* Which character set the source is in */
extern List *importPaths;   /* The list of include paths to check */
#ifdef WINGUI
extern bool guiMode;        /* Using the GUI? Or console? */
#endif

/* Common procedures */

extern void compile(void);

#endif
