/*----------------------------------------------------------------------*\

				Name.c

			      Name Nodes

\*----------------------------------------------------------------------*/


#include "Name.h"

#include "Id.h"
#include "Word.h"


/*======================================================================

  analyseName()

  Analyze a NAME by putting the words into the dictionary.

 */
#ifdef _PROTOTYPES_
void analyseName(List *name, void *reference)
#else
void analyseName(name, reference)
     List *name;
     void *reference;
#endif
{
  List *list;

  /* Analyse a name by entering adjectives and the noun in the dictionary */
  for (list = name; list; list = list->next)
    if (list->next)
      newWord(list->the.id->string, ADJECTIVE_WORD, reference);
    else
      newWord(list->the.id->string, NOUN_WORD, reference);
}
