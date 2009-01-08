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
extern void error(MsgKind msg);
extern void newline(void);
extern void para(void);
extern void clear(void);
extern void printAndLog(char string[]);
extern void output(char string[]);
extern void printMessage(MsgKind msg);
extern void printMessageWithParameters(MsgKind msg, Parameter *messageParameters);

#endif /* OUTPUT_H_ */
