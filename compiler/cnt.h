#ifndef _CNT_H_
#define _CNT_H_
/*----------------------------------------------------------------------*\

				CNT.H
			   Container Nodes

\*----------------------------------------------------------------------*/

/* USE: */
#include "acode.h"
#include "srcp.h"
#include "wht.h"
#include "id.h"
#include "lst.h"


/* Types: */

typedef struct CntNod {		/* CONTAINER */
  Srcp srcp;			/* Source position */
  struct IdNod *nam;		/* Name of the container */
  List *namstms;		/* Name printing statements */
  Aaddr namadr;			/* ACODE address to name statement */
  int code;			/* Code for this container */
  struct IdNod *parent;		/* Pointer to parent name */
  List *lims;			/* Limits */
  Aaddr limadr;			/* ACODE address to limit table */
  List *hstms;			/* Header statements */
  Aaddr hadr;			/* ACODE address to header statements */
  List *estms;			/* 'Empty' statements */
  Aaddr eadr;			/* ACODE address to 'empty' statements */  
} CntNod;



/* Data: */

extern int cntmin, cntmax;
extern int cntcount;



/* Functions: */

/* Initialise container data */
extern void initcnt(void);

/* Create a new Container node */
extern CntNod *newcnt(Srcp *srcp,
		      struct IdNod *nam,
		      List *lims,
		      List *hstms,
		      List *estms);

/* Check a possible containers code */
extern void cntcheck(WhtNod *wht, List *pars);

/* Analyse one Container */
extern void ancnt(CntNod *cnt);

/* Analyse all Containers */
extern void ancnts(void);

/* Generate code for all Containers */
extern Aaddr gecnts(void);

/* Dump a Container node */
extern void ducnt(CntNod *cnt);


#endif
