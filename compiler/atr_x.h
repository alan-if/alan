#ifndef _ATR_X_H_
#define _ATR_X_H_
/*----------------------------------------------------------------------*\

				ATR_X.H
			Attribute Nodes Export

\*----------------------------------------------------------------------*/

#include "atr.h"


/* Use other definitions */

#include "elm.h"
#include "context.h"


/* Data: */

extern int attributeAreaSize;


/* Functions: */

/* Create a new Attribute node */
extern Attribute *newBooleanAttribute(Srcp srcp, IdNode *id, Bool value);
extern Attribute *newStringAttribute(Srcp srcp, IdNode *id, long fpos, int len);
extern Attribute *newIntegerAttribute(Srcp srcp, IdNode *id, int value);
extern Attribute *newReferenceAttribute(Srcp srcp, IdNode *id, IdNode *instance);
extern Attribute *newSetAttribute(Srcp srcp, IdNode *id, Expression *set);

extern void symbolizeAttributes(List *attributeList);
extern List *sortAttributes(List *attributeList);
extern Attribute *findAttribute(List *attributeList, IdNode *id);
extern List *combineAttributes(List *ownAttributes, List *inheritedAttributes);
extern void analyzeAllAttributes(void);
extern void analyzeAttributes(List *attributeList, Symbol *symbol);
extern Attribute *resolveAttribute(Expression *exp, IdNode *attribute, Context *context);
extern Aaddr generateAttributes(List *attributeList, int instanceCode);
extern Aaddr generateStringInit(void);
extern Aaddr generateSetInit(void);
extern void dumpAttribute(Attribute *atr);

#endif
