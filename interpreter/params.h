/*----------------------------------------------------------------------*\

  params.h

  Various utility functions for handling parameters

\*----------------------------------------------------------------------*/
#ifndef PARAMS_H
#include "types.h"


#ifdef _PROTOTYPES_
extern void compress(ParamEntry *a);
extern int lstlen(ParamEntry *a);
extern Boolean inlst(ParamEntry *l, Aword e);
extern void lstcpy(ParamEntry *a, ParamEntry *b);
extern void sublst(ParamEntry *a, ParamEntry *b);
extern void mrglst(ParamEntry *a, ParamEntry *b);
extern void isect(ParamEntry *a, ParamEntry *b);
extern void cpyrefs(ParamEntry *p, Aword *r);
#else
extern void compress();
extern int lstlen();
extern Boolean inlst();
extern void lstcpy();
extern void sublst();
extern void mrglst();
extern void isect();
extern void cpyrefs();
#endif

#endif
