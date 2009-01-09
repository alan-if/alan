#ifndef _PARSE_H_
#define _PARSE_H_
/*----------------------------------------------------------------------*\

  PARSE.H

  Parse data for ALAN interpreter module.

\*----------------------------------------------------------------------*/

#include "types.h"

/* IMPORTS */
#include "params.h"

/* TYPES */


/* DATA */


/* FUNCTIONS */

extern void forceNewPlayerInput();
extern void parse(Parameter parameters[]);
extern void initParse(void);
extern int literalFromInstance(int instance);
extern int instanceFromLiteral(int literalIndex);
extern void addParameterForInstance(Parameter *parameter, int instance);
extern void addParameterForInteger(Parameter *parameter, int value);
extern void addParameterForString(Parameter *parameter, char *value);
extern void restoreParameters();

#endif
