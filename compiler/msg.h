#ifndef _MSG_H_
#define _MSG_H_
/*----------------------------------------------------------------------*\

				MSG.H
			       Messages
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "acode.h"
#include "id.h"


/* Types: */
typedef struct MsgNod {
  Srcp srcp;
  IdNod *id;			/* ID of message type */
  int msgno;			/* It's identity */
  List *stms;			/* List of statements */
  Aaddr stmadr;			/* Address to generated statements */
} MsgNod;


/* Data: */



/* Functions: */

/* Create a new node with a message declaration */
extern MsgNod *newmsg(Srcp *srcp, IdNod *id, List *stms);

/* Prepare all system messages depending on the choosen language */
extern void prepmsgs(void);

/* Analyze the system messages */
extern void anmsgs(void);

/* Generate the system messages */
extern Aword gemsgs(void);

/* Generate a text string as a encoded message in the data file */
extern void getxt(char txt[]);


#endif
