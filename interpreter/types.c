/*
  Acode functions
*/

#include "acode.h"
#include "lists.h"

/*======================================================================*/
Aaddr addressAfterParameterMap(Aaddr adr) {
    ParameterMapEntry *e = (ParameterMapEntry *) &memory[adr];
    
    while (!isEndOfArray(e)) {
        e++;
        adr += AwordSizeOf(ParameterMapEntry);
    }
    return adr+1;
}
