#ifndef _LST_H_
#define _LST_H_
/*----------------------------------------------------------------------*\

				LST.H
			      List Nodes

\*----------------------------------------------------------------------*/

/* Use: */

/* Types */

typedef enum ListKind {
  LIST_UNK,
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
    struct ScrNod *scr;
    struct StmNod *stm;
    struct StpNod *stp;
    struct StxNod *stx;
    struct SynNod *syn;
    struct SymNod *sym;
    struct VrbNod *vrb;
    struct ElmEntry *eent;
    struct IdNode *id;
    struct List *lst;
    char *str;
  } element;			/* Pointer to any type of element */
  struct List *tail;		/* Pointer to last list node */
  struct List *next;		/* Pointer to next list node */
} List;


#endif
