#ifndef _LST_H_
#define _LST_H_
/*----------------------------------------------------------------------*\

				LST.H
			      List Nodes

\*----------------------------------------------------------------------*/

/* Use: */
#include "dump.h"

/* Types */

typedef struct List {		/* GENERIC LISTS */
  NodKind kind;
  union {
    struct ActNod *act;
    struct AltNod *alt;
    struct AtrNod *atr;
    struct ChkNod *chk;
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
    struct List *lst;
    char *str;
  } element;			/* Pointer to any type of element */
  struct List *tail;		/* Pointer to last list node */
  struct List *next;		/* Pointer to next list node */
} List;

/* Data: */



/* Functions: */


/* Concat an element to a list */
extern List *concat(List *lst, void *elem, NodKind kind);

/* Combine two generic lists */
extern List *combine(List *lst1, List *lst2);

/* Dump a list of nodes */
extern void dulst(List *lst, NodKind nod);

/* Dump a list of lists of nodes */
extern void dulst2(List *lstlst, NodKind nod);


#endif
