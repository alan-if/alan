/*----------------------------------------------------------------------*\

				WRD.C
			Dictionary Word Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "lst.h"                /* LST-nodes */
#include "nam.h"                /* NAM-nodes */
#include "wrd.h"                /* WRD-nodes */
#include "opt.h"		/* OPTIONS */

#include "emit.h"
#include "dump.h"



/* PUBLIC: */

int words[WRD_CLASSES+1];


/* Private: */
static WrdNod *wrdtree = NULL;
static WrdNod *lwrd;	/* The last word found by findwrd() */


/*======================================================================

  findwrd()

  Look for a word in the dictionary.

  */
#ifdef _PROTOTYPES_
WrdNod *findwrd(char *str)
                		/* IN - The string */
#else
WrdNod *findwrd(str)
     char str[];		/* IN - The string */
#endif
{
  WrdNod *wrd;			/* Traversal pointers */
  int comp = 1;			/* Result of comparison */

  wrd = wrdtree;
  while (wrd != NULL) {
    lwrd = wrd;			/* Set last word found */
    comp = strcmp(str, lwrd->str);
    if (comp == 0)
      return(lwrd);
    if (comp < 0)
      wrd = lwrd->low;
    else
      wrd = lwrd->high;
  }
  return(NULL);
}



/*======================================================================

  newwrd()

  Creates a new wrdnod and links it in the wrdtree.

  */
#ifdef _PROTOTYPES_
int newwrd(char *str, WrdKind class, int code, NamNod *ref)
                		/* IN - Name of the new word */
                   		/* IN - and its class */
              			/* IN - and code */
                 		/* IN - What it refers to */
#else
int newwrd(str, class, code, ref)
     char str[];		/* IN - Name of the new word */
     WrdKind class;		/* IN - and its class */
     int code;			/* IN - and code */
     NamNod *ref;		/* IN - What it refers to */
#endif
{
  WrdNod *new;			/* The newly created wrdnod */
  WrdNod *wrd;			/* The wrdnod found in dictionary */

  if (str == NULL)
    return 0;

  /* Find the word if it exists */
  wrd = findwrd(str);
  if (wrd != NULL) {
    if ((1L<<class)&(~wrd->class)) { /* Multiple word classes */
      if (class==WRD_SYN || ((1L<<WRD_SYN)&wrd->class))
	lmLog(NULL, 333, sevERR, str);
      else
	lmLog(NULL, 320, sevWAR, str);
      wrd->class |= 1L<<class;
    } else
      wrd->ref = concat(wrd->ref, ref); /* Add another reference */
    return wrd->code;
  }

  new = NEW(WrdNod);

  new->class = 1L<<class;
  new->str = str;
  new->code = code;
  if (class != WRD_SYN)
    new->ref = concat(NULL, ref);
  else
    new->ref = (List *) ref;

  new->low = NULL;
  new->high = NULL;

  if (wrdtree == NULL)
    wrdtree = new;
  else if (strcmp(str, lwrd->str) < 0) /* Use last word found by findwrd() */
    lwrd->low = new;
  else
    lwrd->high = new;

  words[class]++;
  words[WRD_CLASSES]++;

  if (new->code == 0)
    new->code = words[class];
  return new->code;
}



/*======================================================================

  prepwrds()

  Prepare the dictionary by inserting some words dependent on the
  selected language, for example.
 
  */
#ifdef _PROTOTYPES_
void prepwrds(void)
#else
void prepwrds()
#endif
{

  /* Some words in the dictionary */
  switch (opts[OPTLANG].value) {
  case L_ENGLISH:
    newwrd("go", WRD_NOISE, 0, NULL);
    newwrd("the", WRD_NOISE, 0, NULL);
    newwrd("them", WRD_THEM, 0, NULL);
    newwrd("except", WRD_BUT, 0, NULL);
    newwrd("it", WRD_IT, 0, NULL);
    newwrd("but", WRD_BUT, 0, NULL);
    newwrd("and", WRD_CONJ, 0, NULL);
    newwrd("all", WRD_ALL, 0, NULL);
    newwrd("everything", WRD_ALL, 0, NULL);
    newwrd("then", WRD_CONJ, 0, NULL);
    break;
  case L_SWEDISH7:
    newwrd("g}", WRD_NOISE, 0, NULL);
    newwrd("dem", WRD_THEM, 0, NULL);
    newwrd("utom", WRD_BUT, 0, NULL);
    newwrd("den", WRD_IT, 0, NULL);
    newwrd("det", WRD_IT, 0, NULL);
    newwrd("f|rutom", WRD_BUT, 0, NULL);
    newwrd("och", WRD_CONJ, 0, NULL);
    newwrd("allt", WRD_ALL, 0, NULL);
    newwrd("alla", WRD_ALL, 0, NULL);
    break;
  case L_SWEDISH:
    newwrd("gå", WRD_NOISE, 0, NULL);
    newwrd("dem", WRD_THEM, 0, NULL);
    newwrd("utom", WRD_BUT, 0, NULL);
    newwrd("den", WRD_IT, 0, NULL);
    newwrd("det", WRD_IT, 0, NULL);
    newwrd("förutom", WRD_BUT, 0, NULL);
    newwrd("och", WRD_CONJ, 0, NULL);
    newwrd("allt", WRD_ALL, 0, NULL);
    newwrd("alla", WRD_ALL, 0, NULL);
    break;
  default:
    syserr("unrecognized language in prepwrds()");
    break;
  }
}



static int refidx;

/*----------------------------------------------------------------------

  gewrdref()

  Generate reference lists for all entries in the Dictionary.

  */
#ifdef _PROTOTYPES_
static void gewrdref(WrdNod *wrd)
                          /* IN - Word to generate for */
#else
static void gewrdref(wrd)
     WrdNod *wrd;         /* IN - Word to generate for */
#endif
{
  List *lst;
  
  if (wrd == NULL)
    return;
  
  /* First generate for lower */
  gewrdref(wrd->low);
  
  /* Then this node */
  if (((wrd->class&(1L<<WRD_NOUN)) || (wrd->class&(1L<<WRD_ADJ))) && wrd->ref != NULL) {
    wrd->refadr = emadr();	/* Save address to reference table */
    for (lst = wrd->ref; lst != NULL; lst = lst->next)
      genam(lst->element.nam);
    emit(EOF);
  } else
    wrd->refadr = 0;
  
  /* Then for higher */
  gewrdref(wrd->high);

}



/*----------------------------------------------------------------------

  gewrdstr()

  Generate strings for all entries in the Wrdionary.

  */
#ifdef _PROTOTYPES_
static void gewrdstr(WrdNod *wrd)
                          /* IN - Word to generate for */
#else
static void gewrdstr(wrd)
     WrdNod *wrd;         /* IN - Word to generate for */
#endif
{
  if (wrd == NULL)
    return;
  
  /* First generate for lower */
  gewrdstr(wrd->low);
  
  /* Then this node */
  wrd->stradr = emadr();	/* Save address to string */
  emitstr(wrd->str);
  
  /* Then for higher */
  gewrdstr(wrd->high);

}



/*----------------------------------------------------------------------

  gewrdent()

  Generate a dictionary entry, recursively calls itself to generate all of
  the dictionary.

  */
#ifdef _PROTOTYPES_
static void gewrdent(WrdNod *wrd)
                          /* IN - The word to generate an entry for */
#else
static void gewrdent(wrd)
     WrdNod *wrd;         /* IN - The word to generate an entry for */
#endif
{
  if (wrd->low != NULL)
    gewrdent(wrd->low);
  
  /* Generate for this word */
  emit(wrd->stradr);
  if (wrd->class&WRD_SYN) {
    emit(1L<<((WrdNod *)wrd->ref)->class);
    emit(((WrdNod *)wrd->ref)->code);
    emit(((WrdNod *)wrd->ref)->refadr);
  } else {
    emit(1L<<wrd->class);
    emit(wrd->code);
    emit(wrd->refadr);
  }
  
  if (wrd->high != NULL) gewrdent(wrd->high);
}



/*======================================================================

  gewrds()

  Generates the words in the dictionary of course.

  */
#ifdef _PROTOTYPES_
Aaddr gewrds(void)
#else
Aaddr gewrds()
#endif
{
  Aaddr adr;

  /* First generate reference lists */
  refidx = 0;
  gewrdref(wrdtree);

  /* and strings */
  gewrdstr(wrdtree);

  /* Now traverse the wrdtree and generate dictionary entries */
  refidx = 0;
  adr = emadr();		/* Save ACODE address to dictionary */
  gewrdent(wrdtree);

  emit(EOF);

  return(adr);
}



