#ifndef _SYM_H_
#define _SYM_H_
/*----------------------------------------------------------------------*\

				SYM.H
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "types.h"
#include "lst.h"
#include "type.h"


/* TYPES: */

typedef enum SymbolKind {
  CLASS_SYMBOL,
  INSTANCE_SYMBOL,
  FUNCTION_SYMBOL,
  VERB_SYMBOL,
  DIRECTION_SYMBOL,
  PARAMETER_SYMBOL,
  EVENT_SYMBOL,
  LOCAL_SYMBOL
} SymbolKind;


typedef struct Symbol {		/* SYMBOL TABLE ENTRY */
  SymbolKind kind;		/* What kind of symbol? */
  char *string;			/* Name of this entry */
  int code;			/* Internal code for this symbol in its kind */
  struct Symbol *lower, *higher;	/* Links to build a binary search tree */
  union {

    struct {
      struct Symbol *parent;
      Bool attributesNumbered;
      Bool replicated;
      struct Properties *props;
      Bool prohibitedSubclassing;
      Bool isBasicType;
    } entity;

    struct {
      List *parameterSymbols;
      struct Syntax *firstSyntax;
    } verb;

    struct {
      struct Element *element;
      struct Symbol *class;
      Bool restrictedToContainer;
      TypeKind type;
    } parameter;

    struct {
      int number;
      int level;
      TypeKind type;
      struct Symbol *class;
    } local;

  } fields;
} Symbol;

#endif
