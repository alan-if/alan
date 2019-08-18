#ifndef OUTPUT_H_
#define OUTPUT_H_

/* IMPORTS */
#include "types.h"


/* CONSTANTS */


/* TYPES */


/* DATA */
extern int col, lin; // TODO Move to current.column & current.line?
extern int pageLength, pageWidth;

extern bool anyOutput;
extern bool needSpace;
extern bool capitalize;

/* Log file */
#ifdef HAVE_GLK
#include "glk.h"
strid_t logFile;
#else
FILE *logFile;
#endif


/* FUNCTIONS */
extern void setSubHeaderStyle(void);
extern void setNormalStyle(void);
extern void newline(void);
extern void para(void);
extern void clear(void);
extern void printAndLog(char string[]);
extern void justify(char string[]); /* No expansion */
extern void output(char string[]);  /* Expand symbols */

#endif /* OUTPUT_H_ */
