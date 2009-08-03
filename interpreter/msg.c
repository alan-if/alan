/*----------------------------------------------------------------------*\

	msg

\*----------------------------------------------------------------------*/
#include "msg.h"

/* IMPORTS */
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


/*======================================================================*/
void error(MsgKind msgno)   /* IN - The error message number */
{
  /* Print an error message and longjmp to main loop. */
  if (msgno != NO_MSG)
    printMessage(msgno);
  longjmp(returnLabel, ERROR_RETURN);
}


/*======================================================================*/
void abortPlayerCommand(void)
{
    error(NO_MSG);
}


/*======================================================================*/
void addParameterForInstance(Parameter *parameters, int instance) {
    Parameter *parameter = findEndOfList(parameters);

    parameter->instance = instance;
    parameter->useWords = FALSE;

    setEndOfList(parameter+1);
}


/*======================================================================*/
void printMessageWithInstanceParameter(MsgKind message, int i) {
    Parameter *parameters = allocateParameterArray(NULL);
    addParameterForInstance(parameters, i);
    printMessageWithParameters(message, parameters);
    free(parameters);
}


/*======================================================================*/
void printMessageUsing2InstanceParameters(MsgKind message, int instance1, int instance2) {
    Parameter *parameters = allocateParameterArray(NULL);
    addParameterForInstance(parameters, instance1);
    addParameterForInstance(parameters, instance2);
    printMessageWithParameters(message, parameters);
    free(parameters);
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


