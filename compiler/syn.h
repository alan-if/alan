#ifndef _SYN_H_
#define _SYN_H_
/*----------------------------------------------------------------------*\

				SYN.H
			    Synonym Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "nam.h"
#include "lst.h"


/* Types: */

typedef struct SynNod {
  Srcp srcp;		/* Source position */
  List *nams;		/* List of synonyms */
  NamNod *nam;		/* Target identifier */
} SynNod;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Synonym node */
extern SynNod *newsyn(Srcp *srcp,
		      List *nams,
		      NamNod *nam);

/* Analyse the Synonyms by finding the target word and inserting synonyms */
extern void ansyns(void);

extern void dusyn(SynNod *syn);

#else
extern SynNod *newsyn();
extern void ansyns();
extern void dusyn();
#endif

#endif
