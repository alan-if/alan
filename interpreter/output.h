#ifndef OUTPUT_H_
#define OUTPUT_H_

/* IMPORTS */
#include "types.h"

/* CONSTANTS */


/* TYPES */


/* DATA */

extern int col, lin; // TODO Move to current.column & current.line?
extern int pageLength, pageWidth;

extern Bool anyOutput;
extern Bool needSpace;
extern Bool capitalize;


/* FUNCTIONS */
extern void newline(void);
extern void para(void);
extern void clear(void);
extern void output(char string[]);

#endif /* OUTPUT_H_ */
