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
  Srcp srcp;
  NamNod *nam;			/* ID of message type */
  int msgno;			/* It's identity */
  long fpos;			/* Message string position */
  int len;			/*  -"- length */
} MsgNod;


/* Data: */



/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new node with a message declaration */
extern MsgNod *newmsg(Srcp *srcp, NamNod *nam, long int fpos, int len);

/* Prepare all system messages depending on the choosen language */
extern void prepmsgs(void);

/* Generate the system messages */
extern Aword gemsgs(void);

/* Generate a text string as a encoded message in the data file */
extern void getxt(char txt[]);

#else
extern MsgNod *newmsg();
extern void prepmsgs();
extern Aword gemsgs();
extern void getxt();
#endif

#endif
