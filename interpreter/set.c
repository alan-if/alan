/*----------------------------------------------------------------------*\

	Abstract datatype Set for Alan interpreter
	
\*----------------------------------------------------------------------*/

#include "set.h"
#include "main.h"


/*======================================================================*/
void initSets(SetInitEntry *initTable)
{
  SetInitEntry *init;
  AttributeEntry *attribute;
  int i;
  
  for (init = initTable; !endOfTable(init); init++) {
    Set *set = (Set *)allocate(sizeof(Set));
    attribute = pointerTo(init->adr);
    attribute->value = (Aword)set;
    set->size = init->size;
    set->allocated = set->size+5;
    set->members = (Aword*)allocate(sizeof(Aword)*set->allocated);
    for (i = 0; i < init->size; i++)
    	set->members[i] = ((Aword *)pointerTo(init->setAddress))[i];
  }
}

/*----------------------------------------------------------------------*/
static void copyMembers(Aword *source, Aword *destination, int size)
{
  memcpy(destination, source, size*sizeof(Aword));
}	

/*======================================================================*/
Bool inSet(Set *theSet, Aword member)
{
  int i;
	
  for (i = 0; i < theSet->size; i++)
    if (theSet->members[i] == member)
      return TRUE;
  return FALSE;
}

/*=======================================================================*/
void addToSet(Set *theSet, Aword newMember)
{
#define EXTENT 5

  if (inSet(theSet, newMember)) return;
  if (theSet->size == theSet->allocated) {
    Aword *members = theSet->members;
    theSet->members = (Aword *)allocate((theSet->allocated+EXTENT)*sizeof(Aword));
    theSet->allocated += EXTENT;
    if (members != NULL) {
      copyMembers(members, theSet->members, theSet->size);
      free(members);
    }
  }
  theSet->members[theSet->size++] = newMember;
}
