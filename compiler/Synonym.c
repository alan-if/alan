/*----------------------------------------------------------------------*\

				Synonym.c

			      Synonym Nodes

\*----------------------------------------------------------------------*/


#include "Synonym.h"

#include "dump.h"




/*======================================================================

  newSynonym()

  Allocates and initialises a Synonym.

 */
#ifdef _PROTOTYPES_
Synonym *newSynonym(Srcp *srcp,	/* IN - Source position */
		    List *synonyms, /* IN - Synonym words */
		    Id *id)	/* IN - Original word */
#else
Synonym *newSynonym(srcp, synonyms, id)
     Srcp *srcp;
     List *synonyms;
     Id *id;
#endif
{
  Synonym *new;			/* The newly allocated area */

  new = NEW(Synonym);

  new->srcp = *srcp;
  new->synonyms = synonyms;
  new->id = id;

  return(new);
}


/*======================================================================

  analyseSynonym()

  Analyze a SYNONYM.

 */
#ifdef _PROTOTYPES_
void analyseSynonym(Synonym *synonym)
#else
void analyseSynonym(synonym)
     Synonym *synonym;
#endif
{
  /* 4f - Analyse a synonym node */
}



/*======================================================================

  dumpSynonym()

  Dump a Synonym node.

 */
#ifdef _PROTOTYPES_
void dumpSynonym(Synonym *synonym)
#else
void dumpSynonym(synonym)
     Synonym *synonym;
#endif
{
  if (synonym == NULL) {
    put("NULL");
    return;
  }

  put("SYNONYM: "); dumpSrcp(&synonym->srcp); in();
  put("synonyms: "); dumpList(synonym->synonyms, ID_NODE); nl();
  put("id: "); dumpId(synonym->id); out();
}
