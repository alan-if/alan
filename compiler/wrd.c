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


/*======================================================================*/
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
  if (class != SYNONYM_WORD)
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
    new->code = words[WRD_CLASSES];
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
    newWord("go", NOISE_WORD, 0, NULL);
    newWord("the", NOISE_WORD, 0, NULL);
    newWord("them", THEM_WORD, 0, NULL);
    newWord("except", BUT_WORD, 0, NULL);
    newWord("it", IT_WORD, 0, NULL);
    newWord("him", IT_WORD, 0, NULL);
    newWord("her", IT_WORD, 0, NULL);
    newWord("but", BUT_WORD, 0, NULL);
    newWord("and", CONJUNCTION_WORD, 0, NULL);
    newWord("all", ALL_WORD, 0, NULL);
    newWord("everything", ALL_WORD, 0, NULL);
    newWord("then", CONJUNCTION_WORD, 0, NULL);
    break;
  case L_SWEDISH:
    newWord("gå", NOISE_WORD, 0, NULL);
    newWord("dem", THEM_WORD, 0, NULL);
    newWord("utom", BUT_WORD, 0, NULL);
    newWord("den", IT_WORD, 0, NULL);
    newWord("det", IT_WORD, 0, NULL);
    newWord("henne", IT_WORD, 0, NULL);
    newWord("honom", IT_WORD, 0, NULL);
    newWord("förutom", BUT_WORD, 0, NULL);
    newWord("och", CONJUNCTION_WORD, 0, NULL);
    newWord("allt", ALL_WORD, 0, NULL);
    newWord("alla", ALL_WORD, 0, NULL);
    break;
  case L_GERMAN:
    newWord("gehen", NOISE_WORD, 0, NULL);
    newWord("sie", THEM_WORD, 0, NULL);
    newWord("ausser", BUT_WORD, 0, NULL);
    newWord("der", NOISE_WORD, 0, NULL);
    newWord("das", NOISE_WORD, 0, NULL);
    newWord("die", NOISE_WORD, 0, NULL);
    newWord("es", IT_WORD, 0, NULL);
    newWord("ihn", IT_WORD, 0, NULL);
    newWord("sie", IT_WORD, 0, NULL);
    newWord("und", CONJUNCTION_WORD, 0, NULL);
    newWord("alles", ALL_WORD, 0, NULL);
    break;
  default:
    syserr("Unrecognized language in '%s()'", __FUNCTION__);
    break;
  }
}


/*----------------------------------------------------------------------

  Analyze one word in the dictionary to find any words that are
  defined to be of multiple word classes that we want to warn about.

*/
void analyzeWord(WordNode *wrd)
{
  /* Analyze one word in the dictionary to find any words that are
     defined to be of multiple word classes that we want to warn
     about.
  */
#define HASBIT(b, w) (((1L<<(b))&w)==(1L<<(b)))
#define ISASYNONYM(w) HASBIT(SYNONYM_WORD, (w))
#define ISADIRECTION(w) HASBIT(DIRECTION_WORD, (w))
#define ISAVERB(w) HASBIT(VERB_WORD, (w))
#define ISAADJECTIVE(w) HASBIT(ADJECTIVE_WORD, (w))

  if (wrd == NULL) return;

  analyzeWord(wrd->low);

  if (ISASYNONYM(wrd->classbits) && (~(1L<<SYNONYM_WORD))&wrd->classbits)
    /* Synonyms can not be of any other class */
    lmLog(NULL, 333, sevERR, wrd->str);

  else if (ISADIRECTION(wrd->classbits) && ISAVERB(wrd->classbits))
    /* Directions and verbs don't work as expected */
    lmLogv(NULL, 320, sevWAR, wrd->str, "direction", "verb", NULL);

  else if (ISAADJECTIVE(wrd->classbits) && ISAVERB(wrd->classbits))
    /* Adjectives and verbs don't work as expected */
    lmLogv(NULL, 320, sevWAR, wrd->str, "adjective", "verb", NULL);

  analyzeWord(wrd->high);

}


/*======================================================================*/
void analyzeWords(void)
{
  /* Analyze the dictionary to find any words that are defined to be
  of multiple word classes that we want to warn about. */

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
  if (wrd->classbits&(1L<<NOUN_WORD)) {
    wrd->nounrefadr = nextEmitAddress();	/* Save address to noun reference table */
    for (lst = wrd->ref[NOUN_WORD]; lst != NULL; lst = lst->next)
      generateId(lst->element.ins->props->id);
    emit(EOF);
  } else
    wrd->nounrefadr = 0;

  if (wrd->classbits&(1L<<ADJECTIVE_WORD)) {
    wrd->adjrefadr = nextEmitAddress();	/* Save address to noun reference table */
    for (lst = wrd->ref[ADJECTIVE_WORD]; lst != NULL; lst = lst->next)
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
  wrd->stradr = nextEmitAddress();	/* Save address to string */
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
  if (wrd->classbits == (1L<<SYNONYM_WORD)) {
    /* If it is a synonym generate same as for original but mark as synonym */
    emit(((WordNode *)wrd->ref[SYNONYM_WORD])->classbits|(1L<<SYNONYM_WORD));
    emit(((WordNode *)wrd->ref[SYNONYM_WORD])->code);
    emit(((WordNode *)wrd->ref[SYNONYM_WORD])->adjrefadr);
    emit(((WordNode *)wrd->ref[SYNONYM_WORD])->nounrefadr);
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
  adr = nextEmitAddress();		/* Save ACODE address to dictionary */
  gewrdent(wrdtree);

  emit(EOF);

  return(adr);
}
