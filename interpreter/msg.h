#ifndef MSG_H_
#define MSG_H_
/*----------------------------------------------------------------------*\

	msg

\*----------------------------------------------------------------------*/

/* IMPORTS */
#include "types.h"
#include "params.h"

/* CONSTANTS */


/* TYPES */


/* DATA */
extern MessageEntry *msgs;  /* Message table pointer */


/* FUNCTIONS */
extern void error(MsgKind msg);
extern Bool confirm(MsgKind msgno);
extern void printMessage(MsgKind msg);
extern void printMessageWithParameters(MsgKind msg, Parameter *messageParameters);

#endif /* MSG_H_ */
