#ifndef _CONTAINER_H_
#define _CONTAINER_H_
/*----------------------------------------------------------------------*\

			     Container.h
			   Container Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "acode.h"
#include "Srcp.h"
#include "List.h"


/* Types: */

typedef struct Container {	/* CONTAINER */
  Srcp srcp;			/* Source position */
  List *limits;			/* Limits */
  Aaddr limadr;			/* ACODE address to limit table */
  List *header;			/* Header statements */
  Aaddr hadr;			/* ACODE address to header statements */
  List *empty;			/* 'Empty' statements */
  Aaddr eadr;			/* ACODE address to 'empty' statements */  
} Container;



/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* Create a new Container node */
extern Container *newContainer(Srcp *srcp,
			       List *limits,
			       List *headerStatements,
			       List *emptyStatements);

/* Analyse one Container */
extern void analyseContainer(Container *container,
			     List *attributes,
			     List *inheritedAttributeLists);

/* Generate code for all Containers */
extern void generateContainer(Container *container);

/* Dump a Container node */
extern void dumpContainer(Container *container);

#else
extern Container *newContainer();
extern void analyseContainer();
extern void generateContainer();
extern void dumpContainer();
#endif

#endif
