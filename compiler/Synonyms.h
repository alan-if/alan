#ifndef _SYNONYMS_H_
#define _SYNONYMS_H_
/*----------------------------------------------------------------------*\

			      Synonyms.h

			    Synonym Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "Srcp.h"
#include "Id.h"
#include "List.h"


/* Types: */

typedef struct Synonym {
  Srcp srcp;			/* Source position */
  List *synonyms;		/* List of synonyms */
  Id *id;			/* Target identifier */
} Synonym;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Synonym node */
extern Synonym *newSynonym(Srcp *srcp,
			   List *synonyms,
			   Id *id);

/* Analyse the Synonyms by finding the target word and inserting synonyms */
extern void analyseSynonyms(void);

extern void dumpSynonym(Synonym *synonym);

#else
extern SynonymNod *newSynonym();
extern void analyseSynonyms();
extern void dumpSynonym();
#endif

#endif
