#ifndef _WHR_H_
#define _WHR_H_
/*----------------------------------------------------------------------*\

				WHR.H
			     Where Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE other definitions */
#include "wht.h"


/* TYPES: */

typedef enum WhrKind {		/* WHERE kinds */
  WHR_DEFAULT,			/* None */
  WHR_HERE,			/* HERE */
  WHR_NEAR,			/* NEARBY */
  WHR_AT,			/* AT what */
  WHR_IN			/* IN what */
} WhrKind;


typedef struct WhrNod {		/* WHERE node */
  Srcp srcp;			/* Source position */
  WhrKind kind;			/* Where kind */
  What *wht;			/* What */
} WhrNod;


#endif
