#ifndef _STR_H_
#define _STR_H_
/*----------------------------------------------------------------------*\

				STR.H
			   Dynamic Strings
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */


/* Types: */


/* Data: */



/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Dynamic String */
extern char *newstr(char str[]);

#else
extern char *newstr();
#endif

#endif
