#ifndef _ATR_H_
#define _ATR_H_

/* USE other definitions */
#include "alan.h"

#include "srcp.h"
#include "sym.h"
#include "id.h"
#include "exp.h"
#include "elm.h"

#include "emit.h"


/* Types: */

typedef struct AtrNod {		/* ATTRIBUTE */
  Srcp srcp;			/* Source position of the attribute */
  TypeKind typ;			/* Type of this attribute */
  IdNod *id;			/* Id of the attribute */
  Aword stradr;			/* Acode address to the attribute name */
  Aword adr;			/* Acode address to the attribute value */
  int val;			/* The initial value */
  Bool encoded;			/* For string attributes... */
  long fpos;
  int len;
} AtrNod;


/* Data: */

extern int atrmax;


/* Functions: */


/* Create a new Attribute node */
extern AtrNod *newatr(Srcp *srcp,
		      TypeKind typ,
		      IdNod *id,
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
extern AtrNod *paramatr(IdNod *id, ElmNod *elm);

/* Verify a symbol attribute */
extern AtrNod *symatr(IdNod *id, SymNod *sym);

/* Prepare all default attributes before analysis */
extern void prepatrs(void);

/* Analyze an list of attribute nodes */
extern void anatrs(List *atrs);

/* Generate an attribute table */
extern Aaddr geatrs(List *atrs,
		    List *datrs,
		    List *gatrs);

/* Generate the string initialisation table */
extern Aaddr geinit(void);

/* Dump an Attribute node */
extern void duatr(AtrNod *atr);


#endif
