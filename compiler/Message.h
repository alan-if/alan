#ifndef _MESSAGE_H_
#define _MESSAGE_H_
/*----------------------------------------------------------------------*\

			      Message.h

			 Handling of Messages
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "acode.h"


/* Types: */
typedef struct Message {
  long fpos;
  int len;
} Message;


/* Data: */



/* Functions: */
#ifdef _PROTOTYPES_

/* Prepare all system messages depending on the choosen language */
extern void prepareMessages(void);

/* Generate the system messages */
extern Aword generateMessages(void);

/* Generate a text string as a encoded message in the data file */
extern void generateText(char text[]);

#else
extern void prepareMessages();
extern Aword generateMessages();
extern void generateText();
#endif

#endif
