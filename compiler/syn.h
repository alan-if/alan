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
  IdNode *id;		/* Target identifier */
} Synonym;


/* Data: */


/* Functions: */

/* Create a new Synonym node */
extern Synonym *newsyn(Srcp *srcp,
		      List *ids,
		      IdNode *id);

/* Analyse the Synonyms by finding the target word and inserting synonyms */
extern void analyzeSynonyms(void);

extern void dumpSynonyms(Synonym *syn);


#endif
