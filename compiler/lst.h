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
  LIST_ACT,
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
  LIST_LOC,
  LIST_LST,
  LIST_MSG,
  LIST_NAM,
  LIST_OBJ,
  LIST_REF,
  LIST_RES,
  LIST_RUL,
  LIST_SCR,
  LIST_STM,
  LIST_STP,
  LIST_STR,
  LIST_STX,
  LIST_SYN,
  LIST_VRB,
  LIST_LAST_KIND
} ListKind;


typedef struct List {		/* GENERIC LISTS */
  ListKind kind;
  union {
    struct ActNod *act;
    struct AltNod *alt;
    struct AtrNod *atr;
    struct ChkNod *chk;
    struct ClaNod *cla;
    struct ResNod *res;
    struct CntNod *cnt;
    struct ElmNod *elm;
    struct EvtNod *evt;
    struct ExpNod *exp;
    struct ExtNod *ext;
    struct LimNod *lim;
    struct LocNod *loc;
    struct MsgNod *msg;
    struct NamNod *nam;
    struct ObjNod *obj;
    struct RulNod *rul;
    struct ScrNod *scr;
    struct StmNod *stm;
    struct StpNod *stp;
    struct StxNod *stx;
    struct SynNod *syn;
    struct VrbNod *vrb;
    struct ElmEntry *eent;
    struct IdNod *id;
    struct List *lst;
    char *str;
  } element;			/* Pointer to any type of element */
  struct List *tail;		/* Pointer to last list node */
  struct List *next;		/* Pointer to next list node */
} List;


/* Data: */

extern void (*(dumpNodeTable[]))();


/* Functions: */


/* Concat an element to a list */
extern List *concat(List *lst, void *elem, ListKind kind);

/* Combine two generic lists */
extern List *combine(List *lst1, List *lst2);

/* Dump a list of nodes */
extern void dulst(List *lst, ListKind nod);

/* Dump a list of lists of nodes */
extern void dulst2(List *lstlst, ListKind nod);


#endif
