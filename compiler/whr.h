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
  WHERE_DEFAULT,		/* None */
  WHERE_HERE,			/* HERE */
  WHERE_NEAR,			/* NEARBY */
  WHERE_AT,			/* AT what */
  WHERE_IN,			/* IN what */
  WHERE_INSET			/* IN <set> */
} WhereKind;


typedef struct Where {		/* WHERE node */
  Srcp srcp;			/* Source position */
  WhereKind kind;		/* Where kind */
  Bool directly;		/* DIRECTLY? (Non-transitive) */
  struct Expression *what;	/* What */
} Where;


#endif
