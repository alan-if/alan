#ifndef _ATR_X_H_
#define _ATR_X_H_
/*----------------------------------------------------------------------*\

				ATR.H
			   Attribute Nodes

\*----------------------------------------------------------------------*/

#include "atr.h"


/* USE other definitions */

#include "elm.h"


/* DATA: */

extern int atrmax;


/* FUNCTIONS: */


/* Create a new Attribute node */
extern AtrNod *newatr(Srcp *srcp,
		      TypeKind typ,
		      IdNode *id,
		      int val,
		      long fpos,
		      int len);

/* Find a particular attribute */
extern AtrNod *findatr(char atr[],
		       List *lst,
		       List *def);

/* Sort the attributes in a list */
extern void sortatr(List **alstp);

/* Verify parameter attribute */
extern AtrNod *paramatr(IdNode *id, ElmNod *elm);

/* Verify a symbol attribute */
extern AtrNod *symatr(IdNode *id, SymNod *sym);

/* Prepare all default attributes before analysis */
extern void prepatrs(void);

/* Analyze an list of attribute nodes */
extern void anatrs(List *atrs);

/* Generate an attribute table */
extern Aaddr geatrs(List *atrs,
		    List *datrs,
		    List *gatrs);

/* Generate the string initialisation table */
extern Aaddr generateStringInit(void);

/* Dump an Attribute node */
extern void duatr(AtrNod *atr);


#endif
