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
WrdNod *findwrd(char *str)	/* IN - The string */
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
int newwrd(char *str,		/* IN - Name of the new word */
	   WrdKind class,	/* IN - and its class */
	   int code,		/* IN - and code */
	   NamNod *ref)		/* IN - The entity nodes it refers to,
				   NamNod is generic */
{
  WrdNod *new;			/* The newly created wrdnod */
  WrdNod *wrd;			/* The wrdnod found in dictionary */

  if (str == NULL)
    return 0;

  /* Find the word if it exists */
  wrd = findwrd(str);
  if (wrd != NULL) {
    /* Add another reference */
    wrd->classbits |= 1L<<class;
    wrd->ref[class] = concat(wrd->ref[class], ref, REFNOD);
    if (wrd->code == -1)
      /* It was previously without a code */
      wrd->code = code;
    return wrd->code;
  }

  new = NEW(WrdNod);

  new->classbits = 1L<<class;
  new->str = str;
  new->code = code;
  memset(new->ref, 0, sizeof(new->ref));
  if (class != WRD_SYN)
    new->ref[class] = concat(NULL, ref, REFNOD);
  else
    new->ref[class] = (List *) ref;

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
    new->code = words[WRD_CLASSES];
  return new->code;
}



/*======================================================================

  prepwrds()

  Prepare the dictionary by inserting some words dependent on the
  selected language, for example.
 
  */
void prepwrds(void)
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


/*----------------------------------------------------------------------

  anwrd()

  Analyze one word in the dictionary to find any words that are
  defined to be of multiple word classes that we want to warn about.

*/
void anwrd(WrdNod *wrd)
{
#define HASBIT(b, w) (((1L<<(b))&w)==(1L<<(b)))
#define ISASYNONYM(w) HASBIT(WRD_SYN, (w))
#define ISADIRECTION(w) HASBIT(WRD_DIR, (w))
#define ISAVERB(w) HASBIT(WRD_VRB, (w))
#define ISAADJECTIVE(w) HASBIT(WRD_ADJ, (w))

  if (wrd == NULL) return;

  anwrd(wrd->low);

  if (ISASYNONYM(wrd->classbits) && (~(1L<<WRD_SYN))&wrd->classbits)
    /* Synonyms can not be of any other class */
    lmLog(NULL, 333, sevERR, wrd->str);

  else if (ISADIRECTION(wrd->classbits) && ISAVERB(wrd->classbits))
    /* Directions and verbs don't work as expected */
    lmLogv(NULL, 320, sevWAR, wrd->str, "direction", "verb", NULL);

  else if (ISAADJECTIVE(wrd->classbits) && ISAVERB(wrd->classbits))
    /* Directions and verbs don't work as expected */
    lmLogv(NULL, 320, sevWAR, wrd->str, "adjective", "verb", NULL);

  anwrd(wrd->high);

}


/*======================================================================

  anwrds()

  Analyze the dictionary to find any words that are defined to be of
  multiple word classes that we want to warn about.

*/
void anwrds(void)
{
  anwrd(wrdtree);
}



static int refidx;

/*----------------------------------------------------------------------

  gewrdref()

  Generate reference lists for all entries in the Dictionary.

  */
static void gewrdref(WrdNod *wrd) /* IN - Word to generate for */
{
  List *lst;
  
  if (wrd == NULL)
    return;
  
  /* First generate for lower */
  gewrdref(wrd->low);
  
  /* Then this node */
  if (wrd->classbits&(1L<<WRD_NOUN)) {
    wrd->nounrefadr = emadr();	/* Save address to noun reference table */
    for (lst = wrd->ref[WRD_NOUN]; lst != NULL; lst = lst->next)
      genam(lst->element.nam);
    emit(EOF);
  } else
    wrd->nounrefadr = 0;

  if (wrd->classbits&(1L<<WRD_ADJ)) {
    wrd->adjrefadr = emadr();	/* Save address to noun reference table */
    for (lst = wrd->ref[WRD_ADJ]; lst != NULL; lst = lst->next)
      genam(lst->element.nam);
    emit(EOF);
  } else
    wrd->adjrefadr = 0;
  
  /* Then for higher */
  gewrdref(wrd->high);

}



/*----------------------------------------------------------------------

  gewrdstr()

  Generate strings for all entries in the dictionary.

  */
static void gewrdstr(WrdNod *wrd) /* IN - Word to generate for */
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
static void gewrdent(WrdNod *wrd) /* IN - The word to generate an entry for */
{
  if (wrd->low != NULL)
    gewrdent(wrd->low);
  
  /* Generate for this word */
  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  emit(wrd->stradr);
  if (wrd->classbits == (1L<<WRD_SYN)) {
    /* If it is a synonym generate same as for original but mark as synonym */
    emit(((WrdNod *)wrd->ref[WRD_SYN])->classbits|(1L<<WRD_SYN));
    emit(((WrdNod *)wrd->ref[WRD_SYN])->code);
    emit(((WrdNod *)wrd->ref[WRD_SYN])->adjrefadr);
    emit(((WrdNod *)wrd->ref[WRD_SYN])->nounrefadr);
  } else {
    emit(wrd->classbits);
    emit(wrd->code);
    emit(wrd->adjrefadr);
    emit(wrd->nounrefadr);
  }
  
  if (wrd->high != NULL) gewrdent(wrd->high);
}



/*======================================================================

  gewrds()

  Generates the words in the dictionary of course.

  */
Aaddr gewrds(void)
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


/*----------------------------------------------------------------------*/
void duwrdclasses(WrdKind classes)
{
  if ((classes&(1<<WRD_SYN)) != 0) put(" Synonym");
  if ((classes&(1<<WRD_ADJ)) != 0) put(" Adjective");
  if ((classes&(1<<WRD_ALL)) != 0) put(" All");
  if ((classes&(1<<WRD_BUT)) != 0) put("But ");
  if ((classes&(1<<WRD_CONJ)) != 0) put(" Conjunction");
  if ((classes&(1<<WRD_PREP)) != 0) put(" Preposition");
  if ((classes&(1<<WRD_DIR)) != 0) put(" Direction");
  if ((classes&(1<<WRD_IT)) != 0) put(" It");
  if ((classes&(1<<WRD_NOISE)) != 0) put(" Noise");
  if ((classes&(1<<WRD_NOUN)) != 0) put(" Noun");
  if ((classes&(1<<WRD_ACT)) != 0) put(" Actor");
  if ((classes&(1<<WRD_THEM)) != 0) put(" Them");
  if ((classes&(1<<WRD_VRB)) != 0) put(" Verb");
}


/*----------------------------------------------------------------------*/
void duwrd(WrdNod *wrd)
{
  if (wrd == NULL) return;

  duwrd(wrd->low);

  nl();
  put("WRD: "); dustr(wrd->str); in();
  put("classbits: "); duadr(wrd->classbits); duwrdclasses(wrd->classbits); nl();
  put("code: "); duint(wrd->code); out();

  duwrd(wrd->high);
}


/*======================================================================*/
void duwrds(void)
{
  in();
  duwrd(wrdtree);
  out();
}
