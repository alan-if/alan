#ifndef _PARSE_H_
#define _PARSE_H_
/*----------------------------------------------------------------------*\

  PARSE.H

  Parse data for ALAN interpreter module.

\*----------------------------------------------------------------------*/

#include "types.h"


/* TYPES */


/* DATA */


/* FUNCTIONS */

extern void forceNewPlayerInput();
extern void parse(Parameter parameters[]);
extern void initParse(void);
extern int literalFromInstance(Aint instance);
extern Aint instanceFromLiteral(int literalIndex);
extern void addParameterForInstance(Parameter *parameter, Aint instance);
extern void addParameterForInteger(Parameter *parameter, Aint value);
extern void addParameterForString(Parameter *parameter, char *value);
extern void restoreParameters();

#endif
