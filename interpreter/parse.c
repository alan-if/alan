/*----------------------------------------------------------------------*\

  parse.c

  Command line parser unit for Alan interpreter ARUN

\*----------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#ifdef __sun__
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "types.h"
#include "arun.h"
#include "inter.h"
#include "exe.h"
#include "term.h"
#include "debug.h"
#include "params.h"

#include "parse.h"




#define LISTLEN 100


/* PUBLIC DATA */

int wrds[LISTLEN/2] = {EOF};	/* List of parsed words */
int wrdidx;			/* and an index into it */

Boolean plural = FALSE;


/* Syntax Parameters */
int paramidx;			/* Index in params */
ParamElem *params;		/* List of params */
static ParamElem *pparams;	/* Previous parameter list */
static ParamElem *mlst;		/* Multiple objects list */
static ParamElem *pmlst;	/* Previous multiple list */

/* Literals */
LitElem litValues[MAXPARAMS+1];
int litCount;

/* What did the user say? */
int vrbwrd;			/* The word he used */
int vrbcode;			/* The code for that verb */


/*----------------------------------------------------------------------*\

  SCAN DATA & PROCEDURES

  All procedures for getting a command and turning it into a list of
  dictionary entries are placed here.

  buf
  unknown()
  lookup()
  token
  getline()
  scan()

\*----------------------------------------------------------------------*/


/* PRIVATE DATA */

#ifdef __sun__
static char *buf;
#else
static char buf[LISTLEN+1];	/* The input buffer */
#endif


static Boolean eol = TRUE;	/* End of line? Yes, initially */



#ifdef _PROTOTYPES_
static void unknown(
     char token[]
)
#else
static void unknown(token)
     char token[];
#endif
{
  char *str = allocate((int)strlen(token)+4);

  str[0] = '\'';
  strcpy(&str[1], token);
  strcat(str, "'?");
#if ISO == 0
  fromIso(str, str);
#endif
  output(str);
  free(str);
  eol = TRUE;
  error(M_UNKNOWN_WORD);
}



static char *token;


#ifdef _PROTOTYPES_
static int lookup(
     char wrd[]
)
#else
static int lookup(wrd)
     char wrd[];
#endif
{
  int i;

  for (i = 0; !endOfTable(&dict[i]); i++) {
    if (strcmp(wrd, (char *) addrTo(dict[i].wrd)) == 0)
      return (i);
  }
  unknown(wrd);
  return(EOF);
}


#ifdef _PROTOTYPES_
static int number(
     char token[]		/* IN - The string to convert to a number */
)
#else
static int number(token)
     char token[];		/* IN - The string to convert to a number */
#endif
{
  int i;

  sscanf(token, "%d", &i);
  return i;
}

#ifdef _PROTOTYPES_
static char *gettoken(
     char *buf
)
#else
static char *gettoken(buf)
     char *buf;
#endif
{
  static char *marker;
  static char oldch;

  if (buf == NULL)
    *marker = oldch;
  else
    marker = buf;
  while (*marker != '\0' && isSpace(*marker) && *marker != '\n') marker++;
  buf = marker;
  if (isLetter(*marker))
    while (*marker&&(isLetter(*marker)||isdigit(*marker))) marker++;
  else if (isdigit(*marker))
    while (isdigit(*marker)) marker++;
  else if (*marker == '\"') {
    marker++;
    while (*marker != '\"') marker++;
    marker++;
  } else if (*marker == '\0' || *marker == '\n')
    return NULL;
  else
    marker++;
  oldch = *marker;
  *marker = '\0';
  return buf;
}


#ifdef _PROTOTYPES_
static void getline(void)
#else
static void getline()
#endif
{
  para();
  do {
    printf("> ");
#ifdef _READLINE_H_
    if (buf != NULL) {
      free(buf);
      buf = NULL;
    }
    if (!(buf = readline(""))) {
      newline();
      quit();
    }
#else
    if (fgets(buf, LISTLEN, stdin) == NULL) {
      newline();
      quit();
    }
#endif
    getPageSize();
    anyOutput = FALSE;
    if (logflg)
#ifdef __amiga__
      fprintf(logfil, "%s\n", buf);
#else
      fprintf(logfil, "%s", buf);
#endif
#if ISO == 0
    toIso(buf, buf);
#endif
    token = gettoken(buf);
    if (token != NULL && strcmp("debug", token) == 0 && header->debug) {
      dbgflg = TRUE;
      debug();
      token = NULL;
    }
  } while (token == NULL);
  eol = FALSE;
  lin = 1;
}


#ifdef _PROTOTYPES_
static void scan(void)
#else
static void scan()
#endif
{
  int i;
  int w;
  char *str;

  getline();
  wrds[0] = 0;
  for (i = 0; i < litCount; i++)
    if (litValues[i].type == TYPSTR)
      free((char *) litValues[i].value);
  i = 0;
  litCount = 0;
  do {
    if (isLetter(token[0])) {
      (void) strlow(token);
      w = lookup(token);
      if (!isNoise(w))
	wrds[i++] = w;
    } else if (isdigit(token[0])) {
      if (litCount > MAXPARAMS)
	syserr("Too many parameters.");
      wrds[i++] = dictsize+litCount; /* Word outside dictionary = literal */
      litValues[litCount].type = TYPNUM;
      litValues[litCount++].value = number(token);
    } else if (token[0] == '\"') {
      if (litCount > MAXPARAMS)
	syserr("Too many parameters.");
      wrds[i++] = dictsize+litCount; /* Word outside dictionary = literal */
      litValues[litCount].type = TYPSTR;
      /* Remove the string quotes while copying */
      str = strdup(&token[1]);
      str[strlen(token)-2] = '\0';
      litValues[litCount++].value = (Aword) str;
    } else
      unknown(token);
    wrds[i] = EOF;
    eol = (token = gettoken(NULL)) == NULL;
  } while (!eol);
#ifdef _READLINE_H_
  add_history(buf);
#endif
}



/*----------------------------------------------------------------------*\

  PARSE DATA & PROCEDURES

  All procedures and data for getting a command and parsing it

  nonverb()	- search for a non-verb command
  buildall()	- build a list of objects matching 'all'
  unambig()	- match an unambigous object reference
  simple()	- match a simple verb command
  complex()	- match a complex -"-
  try()		- to match a verb command
  match()	- find the verb class (not used currently) and 'try()'

\*---------------------------------------------------------------------- */

static int allLength;		/* No. of objects matching 'all' */


#ifdef _PROTOTYPES_
static void nonverb(void)
#else
static void nonverb()
#endif
{
  if (isDir(wrds[wrdidx])) {
    wrdidx++;
    if (wrds[wrdidx] != EOF && !isConj(wrds[wrdidx]))
      error(M_WHAT);
    else
      go(dict[wrds[wrdidx-1]].code);
    if (wrds[wrdidx] != EOF)
      wrdidx++;
  } else
    error(M_WHAT);
}


#ifdef _PROTOTYPES_
static void buildall(
     ParamElem list[]
)
#else
static void buildall(list)
     ParamElem list[];
#endif
{
  int o, i = 0;
  Boolean found = FALSE;
  
  for (o = OBJMIN; o <= OBJMAX; o++)
    if (isHere(o)) {
      found = TRUE;
      list[i].code = o;
      list[i++].firstWord = EOF;
    }
  if (!found)
    error(M_WHAT_ALL);
  else
    list[i].code = EOF;
}


#ifdef _PROTOTYPES_
static void unambig(
     ParamElem plst[]
)
#else
static void unambig(plst)
     ParamElem plst[];
#endif
{
  int i;
  Boolean found = FALSE;	/* Adjective or noun found ? */
  static ParamElem *refs;	/* Entities referenced by word */
  static ParamElem *savlst;	/* Saved list for backup at EOF */
  int firstWord, lastWord;

  if (refs == NULL)
    refs = (ParamElem *)allocate((MAXENTITY+1)*sizeof(ParamElem));

  if (savlst == NULL)
    savlst = (ParamElem *)allocate((MAXENTITY+1)*sizeof(ParamElem));

  if (isLiteral(wrds[wrdidx])) {
    /* Transform the word into a reference to the literal value */
    plst[0].code = wrds[wrdidx++]-dictsize+LITMIN;
    plst[0].firstWord = EOF;	/* No words used! */
    plst[1].code = EOF;
    return;
  }

  plst[0].code = EOF;		/* Make empty */
  if (isIt(wrds[wrdidx])) {
    wrdidx++;
    /* Use last object in previous command! */
    for (i = lstlen(pparams)-1; i >= 0 && (pparams[i].code == 0 || pparams[i].code >= LITMIN); i--);
    if (i < 0)
      error(M_WHAT_IT);
    if (!isHere(pparams[i].code)) {
      params[0].code = pparams[i].code;
      params[0].firstWord = EOF;
      params[1].code = EOF;
      error(M_NO_SUCH);
    }
    plst[0] = pparams[i];
    plst[0].firstWord = EOF;	/* No words used! */
    plst[1].code = EOF;
    return;
  }

  firstWord = wrdidx;
  while (wrds[wrdidx] != EOF && isAdj(wrds[wrdidx])) {
    cpyrefs(refs, (Aword *)addrTo(dict[wrds[wrdidx]].adjrefs));
    lstcpy(savlst, plst);	/* To save it for backtracking */
    if (found)
      isect(plst, refs);
    else {
      lstcpy(plst, refs);
      found = TRUE;
    }
    wrdidx++;
  }
  if (wrds[wrdidx] != EOF) {
    if (isNoun(wrds[wrdidx])) {
      cpyrefs(refs, (Aword *)addrTo(dict[wrds[wrdidx]].nounrefs));
      if (found)
	isect(plst, refs);
      else {
	lstcpy(plst, refs);
	found = TRUE;
      }
      wrdidx++;
    } else
      error(M_NOUN);
  } else if (found)
    if (isNoun(wrds[wrdidx-1])) {
      /* Perhaps the last word was also a noun? */
      lstcpy(plst, savlst);	/* Restore to before last adjective */
      cpyrefs(refs, (Aword *)addrTo(dict[wrds[wrdidx-1]].nounrefs));
      if (plst[0].code == EOF)
	lstcpy(plst, refs);
      else
	isect(plst, refs);
    } else
      error(M_NOUN);
  lastWord = wrdidx-1;

  /* Resolve ambiguities by presence */
  for (i=0; plst[i].code != EOF; i++)
    if (!isHere(plst[i].code))
      plst[i].code = 0;
  compress(plst);
    
  if (lstlen(plst) > 1 || (found && lstlen(plst) == 0)) {
    params[0].code = 0;		/* Just make it anything != EOF */
    params[0].firstWord = firstWord; /* Remember words for errors below */
    params[0].lastWord = lastWord;
    params[1].code = EOF;	/* But be sure to terminate */
    if (lstlen(plst) > 1)
      error(M_WHICH_ONE);
    else if (found && lstlen(plst) == 0)
      error(M_NO_SUCH);
  } else {
    plst[0].firstWord = firstWord;
    plst[0].lastWord = lastWord;
  }
}
  
  
#ifdef _PROTOTYPES_
static void simple(
     ParamElem olst[]
)
#else
static void simple(olst)
     ParamElem olst[];
#endif
{
  static ParamElem *tlst = NULL;
  int savidx = wrdidx;
  Boolean savplur = FALSE;
  int i;

  if (tlst == NULL)
    tlst = (ParamElem *) allocate(sizeof(ParamElem)*(MAXENTITY+1));
  tlst[0].code = EOF;

  for (;;) {
    if (isThem(wrds[wrdidx])) {
      plural = TRUE;
      for (i = 0; pmlst[i].code != EOF; i++)
	if (!isHere(pmlst[i].code))
	  pmlst[i].code = 0;
      compress(pmlst);
      if (lstlen(pmlst) == 0)
	error(M_WHAT_THEM);
      lstcpy(olst, pmlst);
      olst[0].firstWord = EOF;	/* No words used */
      wrdidx++;
    } else {
      unambig(olst);		/* Look for unambigous noun phrase */
      if (lstlen(olst) == 0) {	/* Failed! */
	lstcpy(olst, tlst);
	wrdidx = savidx;
	plural = savplur;
	return;
      }
    }
    mrglst(tlst, olst);
    if (wrds[wrdidx] != EOF
	&& (isConj(wrds[wrdidx]) &&
	    (isAdj(wrds[wrdidx+1]) || isNoun(wrds[wrdidx+1])))) {
      /* More parameters in a conjunction separated list ? */
      savplur = plural;
      savidx = wrdidx;
      wrdidx++;
      plural = TRUE;
    } else {
      lstcpy(olst, tlst);
      return;
    }
  }
}
  
  
  
#ifdef _PROTOTYPES_
static void complex(
     ParamElem olst[]
)
#else
static void complex(olst)
     ParamElem olst[];
#endif
{
  static ParamElem *alst = NULL;

  if (alst == NULL)
    alst = (ParamElem *) allocate((MAXENTITY+1)*sizeof(ParamElem));

  if (isAll(wrds[wrdidx])) {
    plural = TRUE;
    buildall(alst);		/* Build list of all objects */
    wrdidx++;
    if (wrds[wrdidx] != EOF && isBut(wrds[wrdidx])) {
      wrdidx++;
      simple(olst);
      if (lstlen(olst) == 0)
	error(M_AFTER_BUT);
      sublst(alst, olst);
      if (lstlen(alst) == 0)
	error(M_NOT_MUCH);
    }
    lstcpy(olst, alst);
    allLength = lstlen(olst);
  } else
    simple(olst);		/* Look for simple noun group */
}


#ifdef _PROTOTYPES_
static Boolean claCheck(
     ClaElem *cla		/* IN - The cla elem to check */
)
#else
static Boolean claCheck(cla)
     ClaElem *cla;		/* IN - The cla elem to check */
#endif
{
  Boolean ok = FALSE;

  if ((cla->classes&(Aword)CLA_OBJ) != 0)
    ok = ok || isObj(params[cla->code-1].code);
  if ((cla->classes&(Aword)CLA_CNT) != 0)
    ok = ok || isCnt(params[cla->code-1].code);
  if ((cla->classes&(Aword)CLA_ACT) != 0)
    ok = ok || isAct(params[cla->code-1].code);
  if ((cla->classes&(Aword)CLA_NUM) != 0)
    ok = ok || isNum(params[cla->code-1].code);
  if ((cla->classes&(Aword)CLA_STR) != 0)
    ok = ok || isStr(params[cla->code-1].code);
  if ((cla->classes&(Aword)CLA_COBJ) != 0)
    ok = ok || (isCnt(params[cla->code-1].code) && isObj(params[cla->code-1].code));
  if ((cla->classes&(Aword)CLA_CACT) != 0)
    ok = ok || (isCnt(params[cla->code-1].code) && isAct(params[cla->code-1].code));
  return ok;
}


#ifdef _PROTOTYPES_
static void try(
  ParamElem mlst[]		/* OUT - List of params allowed by multiple */
)
#else
static void try(mlst)
  ParamElem mlst[];		/* OUT - List of params allowed by multiple */
#endif
{
  ElmElem *elms;		/* Pointer to element list */
  StxElem *stx;			/* Pointer to syntax list */
  ClaElem *cla;			/* Pointer to class definitions */
  Boolean anyPlural = FALSE;	/* Any parameter that was plural? */
  int i, p;
  static ParamElem *tlst = NULL; /* List of params found by complex() */
  static Boolean *checked = NULL; /* Corresponding parameter checked? */

  if (tlst == NULL) {
    tlst = (ParamElem *) allocate((MAXENTITY+1)*sizeof(ParamElem));
    checked = (Boolean *) allocate((MAXENTITY+1)*sizeof(Boolean));
  }

  for (stx = stxs; !endOfTable(stx); stx++)
    if (stx->code == vrbcode)
      break;
  if (endOfTable(stx))
    error(M_WHAT);

  elms = (ElmElem *) addrTo(stx->elms);

  while (TRUE) {
    /* End of input? */
    if (wrds[wrdidx] == EOF || isConj(wrds[wrdidx])) {
	while (!endOfTable(elms) && elms->code != EOS)
	  elms++;
	if (endOfTable(elms))
	  error(M_WHAT);
	else
	  break;
    } else {
      /* A preposition? */
      if (isPrep(wrds[wrdidx])) {
	while (!endOfTable(elms) && elms->code != dict[wrds[wrdidx]].code)
	  elms++;
	if (endOfTable(elms))
	  error(M_WHAT);
	else
	  wrdidx++;
      } else {
	/* Must be a parameter! */
	while (!endOfTable(elms) && elms->code != 0)
	  elms++;
	if (endOfTable(elms))
	  error(M_WHAT);
	/* Get it! */
	plural = FALSE;
	complex(tlst);
	if (lstlen(tlst) == 0) /* No object!? */
	  error(M_WHAT);
	if (plural)
	  if (!elms->multiple)
	    error(M_MULTIPLE);
	  else {
	    params[paramidx++].code = 0;
	    lstcpy(mlst, tlst);
	    anyPlural = TRUE;
	  }
	else
	  params[paramidx++] = tlst[0];
	params[paramidx].code = EOF;
      }
      elms = (ElmElem *) addrTo(elms->next);
    }
  }
  
  /* Now perform class checks */
  if (elms->next == 0)	/* No verb code, verb not declared! */
    error(M_CANT0);

  for (p = 0; params[p].code != EOF; p++) /* Mark all parameters unchecked */
    checked[p] = FALSE;
  for (cla = (ClaElem *) addrTo(elms->next); !endOfTable(cla); cla++) {
    if (params[cla->code-1].code == 0) {
      /* This was a multiple parameter, so check all and remove failing */
      for (i = 0; mlst[i].code != EOF; i++) {
	params[cla->code-1] = mlst[i];
	if (!claCheck(cla))
	  mlst[i].code = 0;
      }
      params[cla->code-1].code = 0;
    } else {
      if (!claCheck(cla)) {
	interpret(cla->stms);
	error(M_NOMSG);		/* Return to player */
      }
    }
    checked[cla->code-1] = TRUE; /* Remember that it's already checked */
  }
  /* Now check the rest of the parameters, must be objects */
  for (p = 0; params[p].code != EOF; p++)
    if (!checked[p])
      if (params[p].code == 0) {
	/* This was a multiple parameter, check all and remove failing */
	for (i = 0; mlst[i].code != EOF; i++)
	  if (mlst[i].code != 0) /* Skip any empty slots */
	    if (!isObj(mlst[i].code))
	      mlst[i].code = 0;
      } else if (!isObj(params[p].code))
	error(M_CANT0);

  /* Set verb code */
  cur.vrb = ((Aword *) cla)[1];	/* Take first word after end of table! */

  /* Finally, if ALL was used, try to find out what was applicable */
  if (allLength > 0) {
    for (p = 0; params[p].code != 0; p++); /* Find multiple marker */
    for (i = 0; i < allLength; i++) {
      if (mlst[i].code != 0) {	/* Already empty? */
	params[p] = mlst[i];
	if (!possible())
	  mlst[i].code = 0;	/* Remove this from list */
      }
    }
    params[p].code = 0;		/* Restore multiple marker */
    compress(mlst);
    if (lstlen(mlst) == 0) {
      params[0].code = EOF;
      error(M_WHAT_ALL);
    }
  }
  plural = anyPlural;		/* Remember that we found plural objects */
}

  
#ifdef _PROTOTYPES_
static void match(
     ParamElem *mlst		/* OUT - List of params allowed by multiple */
)
#else
static void match(mlst)
     ParamElem *mlst;		/* OUT - List of params allowed by multiple */
#endif
{
  try(mlst);			/* ... to understand what he said */
  if (wrds[wrdidx] != EOF && !isConj(wrds[wrdidx]))
    error(M_WHAT);
  if (wrds[wrdidx] != EOF)	/* More on this line? */
    wrdidx++;			/* If so skip the AND */
}


#ifdef _PROTOTYPES_
void parse(void)
#else
void parse()
#endif
{
  if (mlst == NULL) {		/* Allocate large enough paramlists */
    mlst = (ParamElem *) allocate(sizeof(ParamElem)*(MAXENTITY+1));
    mlst[0].code = EOF;
    pmlst = (ParamElem *) allocate(sizeof(ParamElem)*(MAXENTITY+1));
    params = (ParamElem *) allocate(sizeof(ParamElem)*(MAXENTITY+1));
    params[0].code = EOF;
    pparams = (ParamElem *) allocate(sizeof(ParamElem)*(MAXENTITY+1));
  }

  if (wrds[wrdidx] == EOF) {
    wrdidx = 0;
    scan();
  } else if (anyOutput)
    para();

  allLength = 0;
  paramidx = 0;
  lstcpy(pparams, params);
  params[0].code = EOF;
  lstcpy(pmlst, mlst);
  mlst[0].code = EOF;
  if (isVerb(wrds[wrdidx])) {
    vrbwrd = wrds[wrdidx];
    vrbcode = dict[vrbwrd].code;
    wrdidx++;
    match(mlst);
    action(mlst);		/* mlst contains possible multiple params */
  } else {
    params[0].code = EOF;
    pmlst[0].code = EOF;
    nonverb();
  }
}
