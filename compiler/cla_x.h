#ifndef _CLA_X_H_
#define _CLA_X_H_
/*----------------------------------------------------------------------*\

			     Class Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "cla.h"

/* USE: */


/* DATA: */
extern ClaNod *entity, *thing, *object, *location, *actor, *string, *integer;


/* METHODS: */
extern void initClasses();

/* Create a new Class node */
extern ClaNod *newClass(Srcp *srcp,
			IdNode *id,
			IdNode *parent,
			Properties *props);

/* Symbolize all Classes */
extern void symbolizeClasses(void);

/* Analyze all Classes */
extern void analyzeClasses(void);

/* Generate code for all Classes */
extern Aaddr generateClasses(void);

/* Dump all Class node */
extern void dumpClasses(void);


#endif
