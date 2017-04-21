/*----------------------------------------------------------------------*\

    The Amachine memory

\*----------------------------------------------------------------------*/
#include "memory.h"

/* Imports */
#include "types.h"
#include "syserr.h"


/* PUBLIC DATA */

Aword *memory = NULL;
static ACodeHeader dummyHeader; /* Dummy to use until memory allocated */
ACodeHeader *header = &dummyHeader;
int memTop = 0;         /* Top of load memory */


/*======================================================================*/
void *allocate(unsigned long lengthInBytes)
{
    void *p = (void *)calloc((size_t)lengthInBytes, 1);

    if (p == NULL)
        syserr("Out of memory.");

    return p;
}


/*======================================================================*/
void deallocate(void *memory)
{
    free(memory);
}


/*======================================================================*/
void *duplicate(void *original, unsigned long len)
{
  void *p = allocate(len+1);

  memcpy(p, original, len);
  return p;
}


typedef struct {
    Aptr aptr;
    void *voidp;
} PointerMapEntry;

static PointerMapEntry *pointerMap = NULL;
static int pointerMapSize = 0;

/*======================================================================*/
void resetPointerMap(void) {
    if (pointerMap != NULL) free(pointerMap);
    pointerMap = NULL;
    pointerMapSize = 0;
}

/*======================================================================*/
void *fromAptr(Aptr aptr) {
    int index;

    for (index=0; index < pointerMapSize && pointerMap[index].aptr != aptr; index++)
        ;

    if (index == pointerMapSize)
        syserr("Could not find any Aptr");

    return pointerMap[index].voidp;
}


/*======================================================================*/
Aptr toAptr(void *ptr) {
    int index;

    if (pointerMap == NULL) {
        pointerMap = (PointerMapEntry *)allocate(sizeof(PointerMapEntry));
        pointerMapSize = 1;
    }

    for (index=0; pointerMap[index].voidp != NULL && index < pointerMapSize; index++)
        ;
    if (index == pointerMapSize) {
        pointerMap = realloc(pointerMap, (index+1)*sizeof(PointerMapEntry));
        pointerMapSize++;
    }

    pointerMap[index].voidp = ptr;
    pointerMap[index].aptr = rand();
    for (int i=0; i < index; i++)
        if (pointerMap[i].aptr == pointerMap[index].aptr) {
            char message[100];
            sprintf(message, "Same aptr allocated, aptr[%d]=%d, aptr[%d]=%d\n",
                    i, pointerMap[i].aptr,
                    index, pointerMap[index].aptr);
            syserr(message);
        }
    return pointerMap[index].aptr;
}
