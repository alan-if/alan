#ifndef _ATR_X_H_
#define _ATR_X_H_
/*----------------------------------------------------------------------*\

				ATR_X.H
			Attribute Nodes Export

\*----------------------------------------------------------------------*/

#include "atr.h"


/* USE other definitions */

#include "elm.h"
#include "context.h"


/* DATA: */

extern int attributeAreaSize;


/* FUNCTIONS: */

/* Create a new Attribute node */
extern Attribute *newAttribute(Srcp *srcp,
			       TypeKind typ,
			       IdNode *id,
			       int val,
			       long fpos,
			       int len,
			       IdNode *instance);

extern void symbolizeAttributes(List *attributeList);
extern List *sortAttributes(List *attributeList);
extern Attribute *findAttribute(List *attributeList, IdNode *id);
extern List *combineAttributes(List *ownAttributes, List *inheritedAttributes);
extern void analyzeAttributes(List *attributeList);
extern Attribute *resolveAttribute(Expression *exp, IdNode *attribute, Context *context);
extern Aaddr generateAttributes(List *attributeList);
extern Aaddr generateStringInit(void);
extern void dumpAttribute(Attribute *atr);

#endif
