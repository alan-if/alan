#ifndef _CHK_H_
#define _CHK_H_

/* USE: */
#include "exp.h"
#include "ins.h"
#include "lst.h"


/* Types: */

typedef struct ChkNod {		/* CHECK */
  ExpNod *exp;			/* Expression to check */
  Aaddr expadr;			/* ACODE address to code for expression */
  List *stms;			/* Statements for false check */
  Aaddr stmadr;			/* ACODE address to statement code */
} ChkNod;


/* Data: */


/* Functions: */


/* Create a new Check node */
extern ChkNod *newchk(ExpNod *exp, List *stms);

/* Analyze a list of Checks */
extern void anchks(List *chks, InsNod *ins, List *pars);

/* Generate code for a list of Checks */
extern Aaddr gechks(List *chks, InsNod *ins);

/* Dump a Check node */
extern void duchk(ChkNod *chk);


#endif
