#ifndef _CLA_X_H_
#define _CLA_X_H_
/*----------------------------------------------------------------------*\

			     Class Nodes Export
                                                                     
\*----------------------------------------------------------------------*/

#include "cla.h"

/* Use: */


/* Data: */
extern ClaNod *entity, *thing, *object, *location, *actor, *string, *integer;


/* Methods: */
extern void initClasses();
extern ClaNod *newClass(Srcp *srcp,
			IdNode *id,
			IdNode *parent,
			Properties *props);
extern void symbolizeClasses(void);
extern void analyzeAllClassAttributes();
extern void analyzeClasses(void);
extern Aaddr generateClasses(void);
extern void dumpClasses(void);

#endif
