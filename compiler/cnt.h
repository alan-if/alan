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
#include "nam.h"
#include "lst.h"


/* Types: */

typedef struct CntNod {		/* CONTAINER */
  Srcp srcp;			/* Source position */
  NamNod *nam;			/* Name of the container */
  List *namstms;		/* Name printing statements */
  Aaddr namadr;			/* ACODE address to name statement */
  int code;			/* Code for this container */
  NamNod *parent;		/* Pointer to parent name */
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
#ifdef _PROTOTYPES_

/* Initialise container data */
extern void initcnt(void);

/* Create a new Container node */
extern CntNod *newcnt(Srcp *srcp,
		      NamNod *nam,
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

#else
extern void initcnt();
extern CntNod *newcnt();
extern void ancnts();
extern void cntcheck();
extern Aaddr gecnts();
extern void ducnt();
#endif

#endif
