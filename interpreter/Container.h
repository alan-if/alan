#ifndef _CONTAINER_H_
#define _CONTAINER_H_
/*----------------------------------------------------------------------*\

  Containers

\*----------------------------------------------------------------------*/

#include "types.h"
#include "acode.h"


/* CONSTANTS */


/* TYPES */


/* DATA */
extern ContainerEntry *containers; /* Container table pointer */


/* FUNCTIONS */
extern Bool passesContainerLimits(Aint container, Aint addedInstance);

#endif
