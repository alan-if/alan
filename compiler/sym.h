#ifndef _SYM_H_
#define _SYM_H_
/*----------------------------------------------------------------------*\

				SYM.H
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/


/* USE: */
#include "srcp.h"
#include "nam.h"
#include "elm.h"
#include "sym.h"


/* Types: */

typedef struct SymNod {		/* SYMBOL TABLE ENTRY */
  NamKind class;		/* Which kind of name */
  char *str;			/* Name of this entry */
  int code;			/* Internal code for this class */
  struct SymNod *low, *high;	/* Links */
  void *ref;			/* Reference to element */
} SymNod;


/* Data: */


/* Functions: */
#ifdef _PROTOTYPES_

/* List a message indicating a redefined symbol */
extern void redefined(Srcp *srcp,
		      SymNod *sym,
		      char str[]);

/* Create a new symbol node */
extern int newsym(char str[],
		  NamKind class,
		  void *ref);

/* Lookup a symbol */
extern SymNod *lookup(char symnam[]);

/* Check a symbol against legal types */
extern void symcheck(SymNod **sym,
		     ElmNod **elm,
		     NamNod *nam,
		     NamKind classes,
		     NamKind props,
		     List *pars);

#else
extern void redefined();
extern int newsym();
extern SymNod *lookup();
extern void symcheck();
#endif

#endif
