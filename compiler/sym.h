#ifndef _SYM_H_
#define _SYM_H_
/*----------------------------------------------------------------------*\

				SYM.H
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/


/* USE: */


/* TYPES: */

typedef enum SymbolKind {
  CLASS_SYMBOL,
  INSTANCE_SYMBOL,
  VERB_SYMBOL
} SymbolKind;


typedef struct SymNod {		/* SYMBOL TABLE ENTRY */
  SymbolKind kind;		/* What kind of symbol? */
  char *string;			/* Name of this entry */
  int code;			/* Internal code for this symbol in its kind */
  struct SymNod *low, *high;	/* Links to build a binary search tree */
  union {
    struct {
      struct SymNod *parent;
    } cla;
    struct {
      struct SymNod *parent;
    } ins;
  } fields;
} SymNod;

#endif
