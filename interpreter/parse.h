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
Pronoun *pronouns2;


/* FUNCTIONS */

extern void parse(void);
extern void initParsing(void);

#endif
