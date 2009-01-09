/*----------------------------------------------------------------------*\

	msg

\*----------------------------------------------------------------------*/
#include "msg.h"

/* IMPORTS */
#include "params.h"
#include "inter.h"
#include "exe.h"


/* CONSTANTS */


/* PUBLIC DATA */
MessageEntry *msgs;         /* Message table pointer */


/* PRIVATE TYPES & DATA */


/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*======================================================================*/
void printMessage(MsgKind msg)      /* IN - message number */
{
  interpret(msgs[msg].stms);
}


/*======================================================================*/
void error(MsgKind msgno)   /* IN - The error message number */
{
  /* Print an error message and longjmp to main loop. */
  if (msgno != NO_MSG)
    printMessage(msgno);
  longjmp(returnLabel, ERROR_RETURN);
}


/*======================================================================*/
void printMessageWithParameters(MsgKind msg, Parameter *messageParameters)
{
    Parameter *savedParameters = allocateParameterArray(NULL);
    copyParameterList(savedParameters, parameters);
    if (messageParameters != NULL)
        copyParameterList(parameters, messageParameters);

    interpret(msgs[msg].stms);

    copyParameterList(parameters, savedParameters);
    free(savedParameters);
}


