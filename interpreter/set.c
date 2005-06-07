/*----------------------------------------------------------------------*\

	Abstract datatype Set for Alan interpreter

	A set is implemented as a struct holding a size and a
	dynamically allocated array of members. Members can be
	integers or instance numbers. Attributes of Set type is
	allocated and the pointer to it is used as the attribute
	value. As members are only references, clearing a set can
	simply be done by setting the size to zero.

\*----------------------------------------------------------------------*/

#include "set.h"
#include "main.h"
#include "syserr.h"


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
int sizeOfSet(Set *theSet) {
  return theSet->size;
}


/*======================================================================*/
void clearSet(Set *theSet) {
  theSet->size = 0;
}


/*======================================================================*/
Aword getMember(Set *theSet, Aint theMember) {
  if (theMember > theSet->size) syserr("Accessing nonexistion member in a set");
  return theSet->members[theMember];
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

/*=======================================================================*/
void removeFromSet(Set *theSet, Aword member)
{
  int i, j;

  if (!inSet(theSet, member)) return;

  for (i = 0; i < theSet->size; i++) {
    if ((Aword)theSet->members[i] == member) {
      for (j = i; j < theSet->size-1; j++)
	theSet->members[j] = theSet->members[j+1];
      theSet->size--;
      break;
    }
  }
}
