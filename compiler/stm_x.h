#ifndef _STM_X_H_
#define _STM_X_H_
/*----------------------------------------------------------------------*\

				STM.H
			   Statement Nodes

\*----------------------------------------------------------------------*/

#include "stm.h"


/* USE: */
#include "context.h"

#include "srcp.h"
#include "ins.h"
#include "lst.h"


/* DATA: */


/* METHODS: */


/* Create a new Statement node */
extern StmNod *newstm(Srcp *srcp, StmKind class);

/* Analyze a list of statements */
extern void analyzeStatements(List *stms,
		   Context *context);

/* Generate code for a list of statements */
extern void generateStatements(List *stms);

/* Dump a statement node */
extern void dumpStatement(StmNod *stm);


#endif
