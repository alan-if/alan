/*----------------------------------------------------------------------*\

				WRD.C
			Dictionary Word Nodes

\*----------------------------------------------------------------------*/

#include "wrd_x.h"

#include "alan.h"
#include "srcp_x.h"
#include "id_x.h"
#include "lst_x.h"

#include "lmList.h"
#include "../interpreter/acode.h"
#include "util.h"
#include "opt.h"
#include "emit.h"



/* PUBLIC: */

int words[WRD_CLASSES+1];


/* Private: */
static WordNode *wrdtree = NULL;
static WordNode *lwrd;	/* The last word found by findwrd() */


/*======================================================================*/
WordNode *findWord(char *str)	/* IN - The string */
{
  WordNode *wrd;			/* Traversal pointers */
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


/*----------------------------------------------------------------------

  findReference()

  Find a reference in a list.

*/
static Bool findReference(Instance *ref, List *referenceList)
{
  List *l;

  for (l = referenceList; l != NULL; l = l->next)
    if (l->element.ins == ref)
      return TRUE;
  return FALSE;
}


/*======================================================================

  newWord()

  Creates a new WordNode and links it in the wrdtree.

  */
int newWord(char *str,		/* IN - Name of the new word */
	    WrdKind class,	/* IN - and its class */
	    int code,		/* IN - and code */
	    Instance *ref)	/* IN - The entity nodes it refers to */
{
  WordNode *new;			/* The newly created wrdnod */
  WordNode *wrd;			/* The wrdnod found in dictionary */

  if (str == NULL)
    return 0;

  /* Find the word if it exists */
  wrd = findWord(str);
  if (wrd != NULL) {
    if (!findReference(ref, wrd->ref[class])) {
      /* Add another reference */
      wrd->classbits |= 1L<<class;
      wrd->ref[class] = concat(wrd->ref[class], ref, REFERENCE_LIST);
      if (wrd->code == -1)
	/* It was previously without a code */
	wrd->code = code;
    }
    return wrd->code;
  }

  new = NEW(WordNode);

  new->classbits = 1L<<class;
  new->str = str;
  new->code = code;
  memset(new->ref, 0, sizeof(new->ref));
  if (class != WRD_SYN)
    new->ref[class] = concat(NULL, ref, REFERENCE_LIST);
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
    new->code = words[class];
  return new->code;
}



/*======================================================================

  prepareWords()

  Prepare the dictionary by inserting some words dependent on the
  selected language, for example.
 
  */
void prepareWords(void)
{

  /* Some words in the dictionary */
  switch (opts[OPTLANG].value) {
  case L_ENGLISH:
    newWord("go", WRD_NOISE, 0, NULL);
    newWord("the", WRD_NOISE, 0, NULL);
    newWord("them", WRD_THEM, 0, NULL);
    newWord("except", WRD_BUT, 0, NULL);
    newWord("it", WRD_IT, 0, NULL);
    newWord("but", WRD_BUT, 0, NULL);
    newWord("and", WRD_CONJ, 0, NULL);
    newWord("all", WRD_ALL, 0, NULL);
    newWord("everything", WRD_ALL, 0, NULL);
    newWord("then", WRD_CONJ, 0, NULL);
    break;
  case L_SWEDISH:
    newWord("gå", WRD_NOISE, 0, NULL);
    newWord("dem", WRD_THEM, 0, NULL);
    newWord("utom", WRD_BUT, 0, NULL);
    newWord("den", WRD_IT, 0, NULL);
    newWord("det", WRD_IT, 0, NULL);
    newWord("förutom", WRD_BUT, 0, NULL);
    newWord("och", WRD_CONJ, 0, NULL);
    newWord("allt", WRD_ALL, 0, NULL);
    newWord("alla", WRD_ALL, 0, NULL);
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
void analyzeWord(WordNode *wrd)
{
#define HASBIT(b, w) (((1L<<(b))&w)==(1L<<(b)))
#define ISASYNONYM(w) HASBIT(WRD_SYN, (w))
#define ISADIRECTION(w) HASBIT(WRD_DIR, (w))
#define ISAVERB(w) HASBIT(WRD_VRB, (w))
#define ISAADJECTIVE(w) HASBIT(WRD_ADJ, (w))

  if (wrd == NULL) return;

  analyzeWord(wrd->low);

  if (ISASYNONYM(wrd->classbits) && (~(1L<<WRD_SYN))&wrd->classbits)
    /* Synonyms can not be of any other class */
    lmLog(NULL, 333, sevERR, wrd->str);

  else if (ISADIRECTION(wrd->classbits) && ISAVERB(wrd->classbits))
    /* Directions and verbs don't work as expected */
    lmLogv(NULL, 320, sevWAR, wrd->str, "direction", "verb", NULL);

  else if (ISAADJECTIVE(wrd->classbits) && ISAVERB(wrd->classbits))
    /* Directions and verbs don't work as expected */
    lmLogv(NULL, 320, sevWAR, wrd->str, "adjective", "verb", NULL);

  analyzeWord(wrd->high);

}


/*======================================================================

  anwrds()

  Analyze the dictionary to find any words that are defined to be of
  multiple word classes that we want to warn about.

*/
void analyzeWords(void)
{
  analyzeWord(wrdtree);
}



static int refidx;

/*----------------------------------------------------------------------

  gewrdref()

  Generate reference lists for all entries in the Dictionary.

  */
static void gewrdref(WordNode *wrd) /* IN - Word to generate for */
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
      generateId(lst->element.ins->props->id);
    emit(EOF);
  } else
    wrd->nounrefadr = 0;

  if (wrd->classbits&(1L<<WRD_ADJ)) {
    wrd->adjrefadr = emadr();	/* Save address to noun reference table */
    for (lst = wrd->ref[WRD_ADJ]; lst != NULL; lst = lst->next)
      generateId(lst->element.ins->props->id);
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
static void gewrdstr(WordNode *wrd) /* IN - Word to generate for */
{
  if (wrd == NULL)
    return;
  
  /* First generate for lower */
  gewrdstr(wrd->low);
  
  /* Then this node */
  wrd->stradr = emadr();	/* Save address to string */
  emitString(wrd->str);
  
  /* Then for higher */
  gewrdstr(wrd->high);

}



/*----------------------------------------------------------------------

  gewrdent()

  Generate a dictionary entry, recursively calls itself to generate all of
  the dictionary.

  */
static void gewrdent(WordNode *wrd) /* IN - The word to generate an entry for */
{
  if (wrd->low != NULL)
    gewrdent(wrd->low);
  
  /* Generate for this word */
  showProgress();

  emit(wrd->stradr);
  if (wrd->classbits == (1L<<WRD_SYN)) {
    /* If it is a synonym generate same as for original but mark as synonym */
    emit(((WordNode *)wrd->ref[WRD_SYN])->classbits|(1L<<WRD_SYN));
    emit(((WordNode *)wrd->ref[WRD_SYN])->code);
    emit(((WordNode *)wrd->ref[WRD_SYN])->adjrefadr);
    emit(((WordNode *)wrd->ref[WRD_SYN])->nounrefadr);
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
Aaddr generateAllWords(void)
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
