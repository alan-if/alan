/*----------------------------------------------------------------------*\

  params.c

  Various utility functions for handling parameters:

  compress()		Compact a list, i.e remove any NULL elements
  listlength()		Count number of elements
  inList()		Check if an element is in the list
  subtractList()	Subract two lists
  listCopy()		Copy one list onto another
  mergeLists()		Merge the paramElems of one list into the first
  intersect()		Take the intersection of two lists
  copyReferences()	Copy the refs (in dictionary) to a paramList

\*----------------------------------------------------------------------*/

#include "types.h"

#include "params.h"
#include "main.h"


/*======================================================================*/
ParamEntry *createParameterList(ParamEntry *currentList) {
	ParamEntry *list;
	if (currentList != NULL)
		return currentList;
	list = allocate(sizeof(ParamEntry)*(MAXPARAMS+1));
	setEndOfList(list);
	return list;
}


/*======================================================================*/
ParamEntry *findEndOfList(ParamEntry *parameters) {
	ParamEntry *parameter;
	for (parameter = parameters; !isEndOfList(parameter); parameter++);
	return parameter;
}


/*======================================================================*/
void compress(ParamEntry theList[])
{
  int i, j;

  for (i = 0, j = 0; theList[j].instance != EOF; j++)
    if (theList[j].instance != 0)
      theList[i++] = theList[j];
  setEndOfList(&theList[i]);
}


/*======================================================================*/
int listLength(ParamEntry theList[])
{
  int i = 0;

  while (!isEndOfList(&theList[i]))
    i++;
  return i;
}


/*======================================================================*/
Bool inList(ParamEntry theList[], Aword theCode)
{
  int i;

  for (i = 0; !isEndOfList(&theList[i]) && theList[i].instance != theCode; i++);
  return (theList[i].instance == theCode);
}


/*======================================================================*/
void copyParameterList(ParamEntry to[], ParamEntry from[])
{
  int i;

  for (i = 0; !isEndOfList(&from[i]); i++)
    to[i] = from[i];
  setEndOfList(&to[i]);
}


/*======================================================================*/
void subtractListFromList(ParamEntry theList[], ParamEntry remove[])
{
  int i;

  for (i = 0; !isEndOfList(&theList[i]); i++)
    if (inList(remove, theList[i].instance))
      theList[i].instance = 0;		/* Mark empty */
  compress(theList);
}


/*======================================================================*/
void mergeLists(ParamEntry one[], ParamEntry other[])
{
  int i,last;

  for (last = 0; !isEndOfList(&one[last]); last++); /* Find end of list */
  for (i = 0; !isEndOfList(&other[i]); i++)
    if (!inList(one, other[i].instance)) {
      one[last++] = other[i];
      setEndOfList(&one[last]);
    }
}


/*======================================================================*/
void intersect(ParamEntry one[], ParamEntry other[])
{
  int i, last = 0;

  for (i = 0; !isEndOfList(&one[i]); i++)
    if (inList(other, one[i].instance))
      one[last++] = one[i];
  setEndOfList(&one[last]);
}


/*======================================================================*/
void copyReferences(ParamEntry parameterList[], Aword references[])
{
  int i;

  for (i = 0; !isEndOfList(&references[i]); i++) {
    parameterList[i].instance = references[i];
    parameterList[i].firstWord = EOF;
  }
  setEndOfList(&parameterList[i]);
}
