#ifndef _ATR_H_
#define _ATR_H_

/* USE other definitions */
#include "alan.h"

#include "srcp.h"
#include "nam.h"
#include "exp.h"

#include "emit.h"
#include "dump.h"


/* Types: */

typedef struct AtrNod {		/* ATTRIBUTE */
  Srcp srcp;			/* Source position of the attribute */
  TypeKind typ;			/* Type of this attribute */
  NamNod *nam;			/* Name of the attribute */
  Aword stradr;			/* Acode address to the attribute name */
  Aword adr;			/* Acode address to the attribute value */
  int val;			/* The initial value */
  Boolean encoded;		/* For string attributes... */
  long fpos;
  int len;
} AtrNod;


/* Data: */


/* Functions: */

#ifdef _PROTOTYPES_

/* Create a new Attribute node */
extern AtrNod *newatr(Srcp *srcp,
		      TypeKind typ,
		      NamNod *nam,
		      int val,
		      long fpos,
		      int len);

/* Find a particular attribute */
extern AtrNod *findatr(char atr[],
		       List *lst);

/* Sort the attributes in a list */
extern void sortatr(List **alstp);

/* Prepare all default attributes before analysis */
extern void prepatrs(void);

/* Analyze an list of attribute nodes */
extern void anatrs(List *atrs);

/* Generate an attribute table */
extern Aaddr geatrs(List *atrs,
		    List *datrs);

/* Generate the string initialisation table */
extern Aaddr geinit(void);

/* Dump an Attribute node */
extern void duatr(AtrNod *atr);

#else
extern AtrNod *newatr();
extern AtrNod *findatr();
extern void sortatr();
extern void anatrs();
extern void prepatrs();
extern Aaddr geatrs();
extern Aaddr geinit();
extern void duatr();
#endif

#endif
