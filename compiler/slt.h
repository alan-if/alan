#ifndef _SLT_H_
#define _SLT_H_
/*----------------------------------------------------------------------*\

				SLT.H
			     Slots Nodes
                                                                     
\*----------------------------------------------------------------------*/


/* USE: */
#include "types.h"
#include "lst.h"
#include "whr.h"
#include "cnt.h"

#include "acode.h"


/* Types */

typedef struct SlotsNode {	/* SLOTS  */
  IdNode *id;			/* Identifier */
  SymNod *symbol;		/* Pointer to symbol table node */
  Aaddr idAddress;
  IdNode *parent;		/* The instances parent */
  List *names;			/* List of lists of adjectives and a noun (ids)*/
  WhrNod *whr;			/* Where is it initially? */
  CntNod *cnt;			/* Container properties */
  List *atrs;			/* Attributes ... */
  Aword atradr;			/* ACODE address to attribute list */
  List *description;		/* Long description statements */
  Aword descriptionAddress;	/* ACODE address to long description code */
  List *art;			/* Article printing statements */
  Aword artadr;			/* ACODE address to article statements */
  List *mentioned;		/* Short (inventory like) statements */
  Aword mentionedAddress;	/* ACODE address to short description (mentioned) code */
  List *vrbs;			/* List of verbs */
  Aword vrbadr;			/* ACODE address to local verb table */
  List *scrs;			/* List of scripts */
  Aaddr scradr;			/* ACODE address to scripts */
  List *exts;			/* List of exits */
  Aaddr extadr;			/* ACODE address to exits */
} SlotsNode;


#endif
