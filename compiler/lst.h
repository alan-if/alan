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
  LIST_ADD,
  LIST_ALT,
  LIST_ATR,
  LIST_CASE,
  LIST_CHK,
  LIST_CLA,
  LIST_CNT,
  LIST_EENT,
  LIST_ELM,
  LIST_EVT,
  LIST_EXP,
  LIST_EXT,
  LIST_INS,
  LIST_ID,
  LIST_LIM,
  LIST_LST,
  LIST_MSG,
  LIST_NAM,
  LIST_REF,
  LIST_RES,
  LIST_RUL,
  LIST_SCR,
  LIST_STM,
  LIST_STP,
  LIST_STR,
  LIST_STX,
  LIST_SYN,
  LIST_SYM,
  LIST_VRB,
  LIST_LAST_KIND
} ListKind;


typedef struct List {		/* GENERIC LISTS */
  ListKind kind;
  union {
    struct AddNode *add;
    struct AltNod *alt;
    struct AtrNod *atr;
    struct ChkNod *chk;
    struct ClaNod *cla;
    struct InsNod *ins;
    struct ResNod *res;
    struct CntNod *cnt;
    struct ElmNod *elm;
    struct EvtNod *evt;
    struct ExpNod *exp;
    struct ExtNod *ext;
    struct LimNod *lim;
    struct MsgNod *msg;
    struct RulNod *rul;
    struct Script *scr;
    struct StmNod *stm;
    struct Step *stp;
    struct StxNod *stx;
    struct SynNod *syn;
    struct Symbol *sym;
    struct Verb *vrb;
    struct ElmEntry *eent;
    struct IdNode *id;
    struct List *lst;
    char *str;
  } element;			/* Pointer to any type of element */
  struct List *next;		/* Pointer to next list node */
} List;


#endif
