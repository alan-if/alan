#ifndef _SYNTAX_H_
#define _SYNTAX_H_
/*----------------------------------------------------------------------*\

			       Syntax.h

			     Syntax Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "List.h"
#include "Id.h"

#include "acode.h"


/* Types: */

typedef struct Syntax {		/* SYNTAX */
  Srcp srcp;			/* Source position of this syntax */
  Id *id;			/* Name of the verb */
  List *elements;		/* List of elements */
  List *restrictions;		/* List of class restrictions */
  List *parameters;		/* AN - List of parameters */
  Bool generated;		/* AN - Already generated? */
  Aaddr elmsadr;		/* GE - Address to element table for */
				/* all syntaxs with the same first word */
  Aaddr resadr;			/* GE - Address to class restriction checks */
} Syntax;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Syntax node */
extern Syntax *newSyntax(Srcp *srcp,
			 Id *id,
			 List *elements,
			 List *restrictions);

/* Create a deafult syntax node */
extern Syntax *defaultSyntax(char verbString[]);

/* Compare parameter lists of two syntaxes */
extern Bool equalParameters(Syntax *syntax1,
			    Syntax *syntax2);

/* Analyze a list of Syntaxs */
extern void analyseSyntaxs(void);

/* Generate code for a list of Syntaxs */
extern Aaddr generateSyntaxs(void);

/* Dump a Syntax node */
extern void dumpSyntax(Syntax *syntax);

#else
extern Syntax *newSyntax();
extern Syntax *defaultSyntax();
extern Bool equalParameters();
extern void analyseSyntaxs();
extern Aaddr generateSyntaxs();
extern void dumpSyntax();
#endif

#endif
