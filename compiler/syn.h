#ifndef _SYN_H_
#define _SYN_H_
/*----------------------------------------------------------------------*\

				SYN.H
			    Synonym Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "id.h"
#include "lst.h"


/* Types: */

typedef struct SynNod {
  Srcp srcp;		/* Source position */
  List *ids;		/* List of synonyms */
  IdNod *id;		/* Target identifier */
} SynNod;


/* Data: */


/* Functions: */

/* Create a new Synonym node */
extern SynNod *newsyn(Srcp *srcp,
		      List *ids,
		      IdNod *id);

/* Analyse the Synonyms by finding the target word and inserting synonyms */
extern void ansyns(void);

extern void dusyn(SynNod *syn);


#endif
