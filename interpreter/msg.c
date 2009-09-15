/*----------------------------------------------------------------------*\

	msg

\*----------------------------------------------------------------------*/
#include "msg.h"

/* IMPORTS */
#include "memory.h"
#include "inter.h"
#include "exe.h"
#include "lists.h"


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


static void (*errorHandler)(MsgKind msg) = NULL;

/*======================================================================*/
void setErrorHandler(void (*handler)(MsgKind msg))   /* IN - The error message number */
{
    errorHandler = handler;
    // TODO This must not return because the standard handler does not...
}


/*======================================================================*/
void error(MsgKind msgno)   /* IN - The error message number */
{
    if (errorHandler != NULL)
        errorHandler(msgno);
    else {
        /* Print an error message and longjmp to main loop. */
        if (msgno != NO_MSG)
            printMessage(msgno);
        longjmp(returnLabel, ERROR_RETURN);
    }
}


/*======================================================================*/
void abortPlayerCommand(void)
{
    error(NO_MSG);
}


/*======================================================================*/
void printMessageWithInstanceParameter(MsgKind message, int i) {
    Parameter *parameters = allocateParameterArray(NULL, MAXPARAMS);
    addParameterForInstance(parameters, i);
    printMessageWithParameters(message, parameters);
    free(parameters);
}


/*======================================================================*/
void printMessageUsing2InstanceParameters(MsgKind message, int instance1, int instance2) {
    Parameter *parameters = allocateParameterArray(NULL, MAXPARAMS);
    addParameterForInstance(parameters, instance1);
    addParameterForInstance(parameters, instance2);
    printMessageWithParameters(message, parameters);
    free(parameters);
}


/*======================================================================*/
void printMessageWithParameters(MsgKind msg, Parameter *messageParameters)
{
    Parameter *savedParameters = allocateParameterArray(NULL, MAXPARAMS);
    copyParameterList(savedParameters, globalParameters);
    copyParameterList(globalParameters, messageParameters);

    interpret(msgs[msg].stms);

    copyParameterList(globalParameters, savedParameters);
    free(savedParameters);
}
