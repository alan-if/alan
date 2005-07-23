#ifndef _LST_H_
#define _LST_H_
/*----------------------------------------------------------------------*\

				LST.H
			      List Nodes

\*----------------------------------------------------------------------*/

/* Use: */

/* Types */

typedef enum ListKind {
  UNKNOWN_LIST,
  ADD_LIST,
  ALTERNATIVE_LIST,
  ATTRIBUTE_LIST,
  CASE_LIST,
  CHECK_LIST,
  CLASS_LIST,
  CONTAINER_LIST,
  ELEMENT_LIST,
  EVENT_LIST,
  EXIT_LIST,
  EXPRESSION_LIST,
  ID_LIST,
  INSTANCE_LIST,
  LIMIT_LIST,
  LIST_EENT,
  LIST_LIST,
  MESSAGE_LIST,
  NAME_LIST,
  REFERENCE_LIST,
  RESTRICTION_LIST,
  RESOURCE_LIST,
  RULE_LIST,
  SCRIPT_LIST,
  STATEMENT_LIST,
  STEP_LIST,
  STRING_LIST,
  SYMBOL_LIST,
  SYNONYM_LIST,
  SYNTAX_LIST,
  SRCP_LIST,
  VERB_LIST,
  LAST_LIST_KIND
} ListKind;


typedef struct List {		/* GENERIC LISTS */
  ListKind kind;
  union {
    struct AddNode *add;
    struct AltNod *alt;
    struct Attribute *atr;
    struct ChkNod *chk;
    struct Class *cla;
    struct Container *cnt;
    struct Element *elm;
    struct ElmEntry *eent;
    struct Event *evt;
    struct Expression *exp;
    struct Exit *ext;
    struct IdNode *id;
    struct Instance *ins;
    struct LimNod *lim;
    struct List *lst;
    struct MsgNod *msg;
    struct Resource *resource;
    struct ResNod *res;
    struct RulNod *rul;
    struct Script *script;
    struct Step *stp;
    struct Statement *stm;
    struct Symbol *sym;
    struct Synonym *syn;
    struct Syntax *stx;
    struct Srcp *srcp;
    struct Verb *vrb;
    struct Word *word;
    char *str;
  } element;			/* Pointer to any type of element */
  struct List *next;		/* Pointer to next list node */
} List;


#endif
