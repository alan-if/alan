#ifndef _MSG_H_
#define _MSG_H_
/*----------------------------------------------------------------------*\

				MSG.H
			       Messages
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "acode.h"


/* Types: */
typedef struct MsgNod {
  long fpos;
  int len;
} MsgNod;


/* Data: */



/* Functions: */
#ifdef _PROTOTYPES_

/* Prepare all system messages depending on the choosen language */
extern void prepmsgs(void);

/* Generate the system messages */
extern Aword gemsgs(void);

/* Generate a text string as a encoded message in the data file */
extern void getxt(char txt[]);

#else
extern void prepmsgs();
extern Aword gemsgs();
extern void getxt();
#endif

#endif
