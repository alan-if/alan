#ifndef _CNT_H_
#define _CNT_H_
/*----------------------------------------------------------------------*\

				CNT.H
			   Container Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "acode.h"
#include "srcp.h"
#include "id.h"
#include "lst.h"


/* Types: */

typedef struct CntNod {		/* CONTAINER */
  Srcp srcp;			/* Source position */
  IdNode *nam;			/* Name of the container */
  List *namstms;		/* Name printing statements */
  Aaddr namadr;			/* ACODE address to name statement */
  int code;			/* Code for this container */
  struct SlotsNode *ownerSlots;	/* Pointer to parent slots */
  List *lims;			/* Limits */
  Aaddr limadr;			/* ACODE address to limit table */
  List *hstms;			/* Header statements */
  Aaddr hadr;			/* ACODE address to header statements */
  List *estms;			/* 'Empty' statements */
  Aaddr eadr;			/* ACODE address to 'empty' statements */  
} CntNod;


#endif
