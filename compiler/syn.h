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

typedef struct Synonym {
  Srcp srcp;		/* Source position */
  List *ids;		/* List of synonyms */
  IdNode *id;		/* Target identifier */
} Synonym;


/* Data: */


/* Functions: */

extern Synonym *newSynonym(Srcp *srcp, List *ids, IdNode *id);
extern void analyzeSynonyms(void);
extern void dumpSynonyms(Synonym *syn);

#endif
