/*----------------------------------------------------------------------*\

  exe.c

  Amachine instruction execution unit of Alan interpreter

\*----------------------------------------------------------------------*/

#include "types.h"
#include "act.h"
#include "debug.h"

#ifdef USE_READLINE
#include "readline.h"
#endif


#ifdef GLK
#define MAP_STDIO_TO_GLK
#include "glkio.h"
#endif

#include "main.h"
#include "parse.h"
#include "inter.h"
#include "act.h"
#include "stack.h"
#include "decode.h"

#include "exe.h"

#define WIDTH 80


/* PUBLIC DATA */

Boolean looking = FALSE;        /* LOOKING? flag */

int dscrstkp = 0;               /* Describe-stack pointer */


void describeInstances(void);


/*======================================================================*/
void print(Aword fpos, Aword len)
{
  char str[2*WIDTH];            /* String buffer */
  int outlen = 0;               /* Current output length */
  int ch = 0;
  int i;
  long savfp = 0;		/* Temporary saved text file position */
  static Boolean printFlag = FALSE; /* Printing already? */
  Boolean savedPrintFlag = printFlag;
  void *info = NULL;		/* Saved decoding info */


  if (len == 0) return;

  if (isHere(HERO)) {           /* Check if the player will see it */
    if (printFlag) {            /* Already printing? */
      /* Save current text file position and/or decoding info */
      if (header->pack)
	info = pushDecode();
      else
	savfp = ftell(txtfil);
    }
    printFlag = TRUE;           /* We're printing now! */

    /* Position to start of text */
    fseek(txtfil, fpos+header->stringOffset, 0);

    if (header->pack)
      startDecoding();
    for (outlen = 0; outlen != len; outlen = outlen + strlen(str)) {
      /* Fill the buffer from the beginning */
      for (i = 0; i <= WIDTH || (i > WIDTH && ch != ' '); i++) {
	if (outlen + i == len)  /* No more characters? */
	  break;
	if (header->pack)
	  ch = decodeChar();
	else
	  ch = getc(txtfil);
	if (ch == EOFChar)      /* Or end of text? */
	  break;
	str[i] = ch;
      }
      str[i] = '\0';
#if ISO == 0
      fromIso(str, str);
#endif
      output(str);
    }

    /* And restore */
    printFlag = savedPrintFlag;
    if (printFlag) {
      if (header->pack)
	popDecode(info);
      else
	fseek(txtfil, savfp, 0);
    }
  }
}


/*======================================================================*/
void sys(Aword fpos, Aword len)
{
  char *command;

  getstr(fpos, len);            /* Returns address to string on stack */
  command = (char *)pop();
  system(command);
  free(command);
}


/*======================================================================*/
void getstr(Aword fpos, Aword len)
{
  char *buf = allocate(len+1);

  /* Push the address to the string */
  push((Aword) buf);

  /* Position to start of text */
  fseek(txtfil, fpos+header->stringOffset, 0);

  if (header->pack)
    startDecoding();
  while (len--)
    if (header->pack)
      *(buf++) = decodeChar();
    else
      *(buf++) = getc(txtfil);

  /* Terminate string with zero */
  *buf = '\0';
}



/*======================================================================*/
void score(Aword sc)
{
  char buf[80];

  if (sc == 0) {
    prmsg(M_SCORE1);
    sprintf(buf, "%d", current.score);
    output(buf);
    prmsg(M_SCORE2);
    sprintf(buf, "%ld.", header->maxscore);
    output(buf);
  } else {
    current.score += scores[sc-1];
    scores[sc-1] = 0;
  }
}


/*======================================================================*/
void visits(Aword v)
{
  current.visits = v;
}


/*======================================================================*/
Boolean confirm(MsgKind msgno)
{
  char buf[80];

  /* This is a bit of a hack since we really want to compare the input,
     it could be affirmative, but for now any input is NOT! */
  prmsg(msgno);

#ifdef USE_READLINE
  if (!readline(buf)) return TRUE;
#else
  if (gets(buf) == NULL) return TRUE;
#endif
  col = 1;

  return (buf[0] == '\0');
}


/*======================================================================*/
void quit(void)
{
  char buf[80];

  para();
  while (TRUE) {
    col = 1;
    statusline();
    prmsg(M_QUITACTION);
#ifdef USE_READLINE
    if (!readline(buf)) terminate(0);
#else
    if (gets(buf) == NULL) terminate(0);
#endif
    if (strcmp(buf, "restart") == 0)
      longjmp(restart_label, TRUE);
    else if (strcmp(buf, "restore") == 0) {
      restoreGame();
      return;
    } else if (strcmp(buf, "quit") == 0)
      terminate(0);
  }
  syserr("Fallthrough in QUIT");
}



/*======================================================================*/
void restartGame(void)
{
  para();
  if (confirm(M_REALLY)) {
    longjmp(restart_label, TRUE);
  } else
    return;
  syserr("Fallthrough in RESTART");
}



/*======================================================================*/
void cancelEvent(Aword evt)
{
  int i;

  for(i = etop-1; i>=0; i--)
    if (eventQueue[i].event == evt) {
      while (i < etop-1) {
	eventQueue[i].event = eventQueue[i+1].event;
	eventQueue[i].time = eventQueue[i+1].time;
	eventQueue[i].where = eventQueue[i+1].where;
	i++;
      }
      etop--;
      return;
    }
}


/*======================================================================*/
void increaseEventQueue(void)
{
  eventQueue = realloc(eventQueue, (etop+2)*sizeof(EventQueueEntry));
  if (eventQueue == NULL) syserr("Out of memory in increaseEventQueue()");

  EventQueueSize = etop + 2;
}


/*======================================================================*/
void schedule(Aword event, Aword where, Aword after)
{  int i;
   int time;
  
  cancelEvent(event);
  /* Check for overflow */
  if (etop == EventQueueSize)
    increaseEventQueue();
  
  time = current.tick+after;
  
  /* Bubble this event down */
  for (i = etop; i >= 1 && eventQueue[i-1].time <= time; i--) {
    eventQueue[i].event = eventQueue[i-1].event;
    eventQueue[i].time = eventQueue[i-1].time;
    eventQueue[i].where = eventQueue[i-1].where;
  }
  
  eventQueue[i].time = time;
  eventQueue[i].where = where;
  eventQueue[i].event = event;
  etop++;
}



/*======================================================================*/
AttributeEntry *findAttribute(Aaddr address, int code)
{
  AttributeEntry *attribute = (AttributeEntry *) pointerTo(address);
  while (attribute->code != code) {
    attribute++;
    if (*((Aword *)attribute) == EOF)
      syserr("Attribute not found.");
  }
  return attribute;
}




/*----------------------------------------------------------------------*/
static Aword getatr(
     Aaddr atradr,              /* IN - ACODE address to attribute table */
     Aaddr code                  /* IN - The attribute to read */
)
{
  AttributeEntry *attribute = findAttribute(atradr, code);

  return attribute->value;
}
  

/*----------------------------------------------------------------------*/
static void setatr(
     Aaddr atradr,              /* IN - ACODE address to attribute table */
     Aword code,                 /* IN - attribute code */
     Aword val                  /* IN - new value */
)
{
  AttributeEntry *at = findAttribute(atradr, code);

  at->value = val;
}


/*======================================================================*/
void make(Aword id, Aword code, Aword val)
{
  char str[80];

  if (id > 0 && id <= header->instanceMax) {
    setatr(instance[id].attributes, code, val);
    if (isA(id, LOCATION))	/* May have changed so describe next time */
      admin[id].visitsCount = 0;
  }  else {
    sprintf(str, "Can't MAKE instance attribute (%ld, %ld).", id, code);
    syserr(str);
  }
}


/*======================================================================*/
void set(Aword id, Aword atr, Aword val)
{
  char str[80];

  if (id > 0 && id <= header->instanceMax) {
    setatr(instance[id].attributes, atr, val);
    if (isA(id, LOCATION))	/* May have changed so describe next time */
      admin[id].visitsCount = 0;
  } else {
    sprintf(str, "Can't SET instance (%ld).", id);
    syserr(str);
  }
}


/*======================================================================*/
void setstr(Aword id, Aword atr, Aword str)
{
  free((char *)attributeOf(id, atr));
  set(id, atr, str);
}



/*----------------------------------------------------------------------*/
static void incratr(
	Aaddr atradr,           /* IN - ACODE address to attribute table */
	Aword code,		/* IN - attribute code */
	Aword step              /* IN - step to increment by */
)
{
  AttributeEntry *at = findAttribute(atradr, code);
  
  at->value += step;
}


/*======================================================================*/
void incr(Aword id, Aword atr, Aword step)
{
  char str[80];

  if (id > 0 && id <= header->instanceMax)
    incratr(instance[id].attributes, atr, step);
  else {
    sprintf(str, "Can't INCR instance attribute (%ld, %ld).", id, atr);
    syserr(str);
  }
}

/*======================================================================*/
void decr(Aword id, Aword atr, Aword step)
{
  char str[80];

  if (id > 0 && id <= header->instanceMax)
    incratr(instance[id].attributes, atr, -step);
  else {
    sprintf(str, "Can't DECR instance attribute (%ld, %ld).", id, atr);
    syserr(str);
  }
}



/*----------------------------------------------------------------------*/
static Aword litatr(Aword lit, Aword atr)
{
  char str[80];

  if (atr == 1)
    return literal[lit-header->instanceMax].value;
  else {
    sprintf(str, "Unknown attribute for literal (%ld).", atr);
    syserr(str);
  }
  return(EOF);
}


/*======================================================================*/
Aword attributeOf(Aword id, Aword atr)
{
  char str[80];

  if (isLit(id))
    return litatr(id, atr);
  else {
    if (id > 0 && id <= header->instanceMax)
      return getatr(instance[id].attributes, atr);
    else {
      sprintf(str, "Can't ATTRIBUTE item (%ld).", id);
      syserr(str);
    }
  }
  return(EOF);
}


/*======================================================================*/
Aword strattr(Aword id, Aword atr)
{
  return (Aword) strdup((char *)attributeOf(id, atr));
}


/*----------------------------------------------------------------------*/
#ifdef FIXME
static Aword objloc(Aword obj)
{
  /* Keep this for container handling */
  if (isCnt(objs[obj-OBJMIN].loc)) /* In something ? */
    if (isObj(objs[obj-OBJMIN].loc) || isAct(objs[obj-OBJMIN].loc))
      return(where(objs[obj-OBJMIN].loc));
    else /* Containers not anywhere is where the hero is! */
      return(where(HERO));
  else
    return(objs[obj-OBJMIN].loc);
}

#endif


/*======================================================================*/
Aword where(Aword id)
{
  char str[80];

  if (id == 0) {
    sprintf(str, "Can't WHERE item (%ld).", id);
    syserr(str);
  } else if (id > header->instanceMax) {
    sprintf(str, "Can't WHERE item (%ld > instanceMax).", id);
    syserr(str);
  } else
    return instance[id].location;
  syserr("Fall through to end in where()");
  return 0;
}



/*======================================================================*/
Aint agrmax(Aword atr, Aword whr)
{
  Aword i;
  Aint max = 0;

  for (i = 1; i <= header->instanceMax; i++) {
    if (isObj(i)) {
      if (isLoc(whr)) {
	if (where(i) == whr && attributeOf(i, atr) > max)
	  max = attributeOf(i, atr);
      } else if (instance[i].location == whr && attributeOf(i, atr) > max)
	max = attributeOf(i, atr);
    }
  }
  return(max);
}


/*======================================================================*/
Aint agrsum(Aword atr, Aword whr)
{
  Aword i;
  Aint sum = 0;

  for (i = 1; i <= header->instanceMax; i++) {
    if (isObj(i)) {
      if (isLoc(whr)) {
	if (where(i) == whr)
	  sum += attributeOf(i, atr);
      } else if (instance[i].location == whr)
	sum += attributeOf(i, atr);
    }
  }
  return(sum);
}


/*======================================================================*/
Aint agrcount(Aword whr)
{
  Aword i;
  Aword count = 0;

  for (i = 1; i <= header->instanceMax; i++) {
    if (isObj(i)) {
      if (isLoc(whr)) {
	if (where(i) == whr)
	  count++;
      } else if (instance[i].location == whr)
	count++;
    }
  }
  return(count);
}


/*----------------------------------------------------------------------*/
static void locateObject(Aword obj, Aword whr)
{
  if (isCnt(whr)) { /* Into a container */
    if (whr == obj)
      syserr("Locating something inside itself.");
    if (checklim(whr, obj))
      return;
    else
      instance[obj].location = whr;
  } else {
    instance[obj].location = whr;
    /* Make sure the location is described since it's changed */
    admin[whr].visitsCount = 0;
  }
}


/*----------------------------------------------------------------------*/
static void locateActor(Aword act, Aword whr)
{
  Aword prevloc = current.location;

  current.location = whr;
  instance[act].location = whr;
  if (act == HERO) {
    if (admin[instance[act].location].visitsCount % (current.visits+1) == 0)
      look();
    else {
      if (anyOutput)
	para();
      say(where(HERO));
      prmsg(M_AGAIN);
      newline();
      describeInstances();
    }
    admin[where(HERO)].visitsCount++;
    admin[where(HERO)].visitsCount %= (current.visits+1);
  } else
    admin[whr].visitsCount = 0;
#ifdef IMPLEMENTED_DOES
  if (instance[cur.loc].does != 0) {
    cur.act = act;
    interpret(instance[cur.loc].does);
    cur.act = prevact;
  }
#endif

  if (current.actor != act)
    current.location = prevloc;
}


/*======================================================================*/
void locate(Aword id, Aword whr)
{
  char str[80];
  Aword containerId;
  ContainerEntry *theContainer;
  Aword previousInstance = current.instance;

  if (id == 0) {
    sprintf(str, "Can't LOCATE instance (%ld).", id);
    syserr(str);
  } else if (id > header->instanceMax) {
    sprintf(str, "Can't LOCATE instance (%ld > instanceMax).", id);
    syserr(str);
  } else if (whr == 0) {
    sprintf(str, "Can't LOCATE instance at (%ld).", whr);
    syserr(str);
  } else if (whr > header->instanceMax) {
    sprintf(str, "Can't LOCATE instance at (%ld > instanceMax).", whr);
    syserr(str);
  }

  /* First check if the instance is in a container, if so run extract checks */
  if (isCnt(instance[id].location)) {    /* In something? */
    current.instance = instance[id].location;
    containerId = instance[instance[id].location].container;
    theContainer = &container[containerId];

    if (theContainer->extractChecks != 0) {
      if (traceOption) {
	printf("\n<EXTRACT from ");
	traceSay(id);
	printf("(%ld, container %ld), Checking:>\n", id, containerId);
      }
      if (!trycheck(theContainer->extractChecks, EXECUTE)) {
	fail = TRUE;
	return;
      }
      current.instance = previousInstance;
    }
    if (theContainer->extractStatements != 0) {
      if (traceOption) {
	printf("\n<EXTRACT from ");
	traceSay(id);
	printf("(%ld, container %ld), Executing:>\n", id, containerId);
      }
      interpret(theContainer->extractStatements);
    }
  }
    
  if (isAct(id))
    locateActor(id, whr);
  else
    locateObject(id, whr);
}


/*----------------------------------------------------------------------*/
static Abool instanceHere(Aword id)
{
  Aword owner;

  if (isCnt(instance[id].location)) {    /* In something? */
    owner = instance[id].location;
    if (instance[owner].location != 0)
      return(isHere(owner));
    else /* If the container wasn't anywhere, assume where HERO is! */
      return(where(HERO) == current.location);
  } else
    return(instance[id].location == current.location);
}


/*======================================================================*/
Aword isHere(Aword id)
{
  char str[80];

  if (id == 0) {
    sprintf(str, "Can't HERE instance (%ld).", id);
    syserr(str);
  } else if (id > header->instanceMax) {
    sprintf(str, "Can't HERE instance (%ld > instanceMax).", id);
    syserr(str);
  } else
    return instanceHere(id);
  syserr("Fall through to end in isHere()");
  return EOF;
}


/*----------------------------------------------------------------------*/
static Aword objnear(Aword obj)
{
  if (isCnt(instance[obj].location)) {    /* In something? */
    if (isObj(instance[obj].location) || isAct(instance[obj].location))
      return(isNear(instance[obj].location));
    else  /* If the container wasn't anywhere, assume here, so not nearby! */
      return(FALSE);
  } else
    return(exitto(where(obj), current.location));
}


/*----------------------------------------------------------------------*/
static Aword actnear(Aword act)
{
  return(exitto(where(act), current.location));
}


/*======================================================================*/
Abool isNear(Aword id)
{
  char str[80];

  if (isObj(id))
    return objnear(id);
  else if (isAct(id))
    return actnear(id);
  else {
    sprintf(str, "Can't NEAR instance (%ld).", id);
    syserr(str);
  }
  return(EOF);
}



/*======================================================================*/
Abool isA(Aword instanceId, Aword ancestor)
{
  int parent;

  if (isLit(instanceId))
    parent = literal[instanceId-header->instanceMax].class;
  else
    parent = instance[instanceId].parent;
  while (parent != 0 && parent != ancestor)
    parent = class[parent].parent;

  return (parent != 0);
}



/*======================================================================*/
Abool in(Aword theInstance, Aword cnt)
{
  if (!isCnt(cnt))
    syserr("IN in a non-container.");

  return (instance[theInstance].location == cnt);
}




/*----------------------------------------------------------------------*/
static void saylit(Aword lit)
{
  char *str;

  if (isNum(lit))
    sayint(literal[lit-header->instanceMax].value);
  else {
    str = (char *)strdup((char *)literal[lit-header->instanceMax].value);
    saystr(str);
  }    
}

	
/*----------------------------------------------------------------------*/
static void sayInstance(Aword id)
{
  int p, i;

  if (params != NULL)
    for (p = 0; params[p].code != EOF; p++)
      if (params[p].code == id) {
	if (params[p].firstWord == EOF) /* Any words he used? */
	  break;
	else				/* Yes, so use them... */
	  for (i = params[p].firstWord; i <= params[p].lastWord; i++)
	    output((char *)pointerTo(dict[wrds[i]].wrd));
	return;
      }
  interpret(instance[id].mentioned);
}


/*======================================================================*/
void sayint(Aword val)
{
  char buf[25];

  if (isHere(HERO)) {
    sprintf(buf, "%ld", val);
    output(buf);
  }
}


/*======================================================================*/
void saystr(char *str)
{
  if (isHere(HERO))
    output(str);
  free(str);
}


/*======================================================================*/
void sayarticle(Aword id)
{
  if (instance[id].article != 0)
    interpret(instance[id].article);
  else
    prmsg(M_ARTICLE);
}


/*======================================================================*/
void say(Aword id)
{
  char str[80];

  if (isHere(HERO)) {
    if (isLit(id))
      saylit(id);
    else {
      if (id == 0) {
	sprintf(str, "Can't SAY instance (%ld).", id);
	syserr(str);
      } else if (id > header->instanceMax) {
	sprintf(str, "Can't SAY instance (%ld > instanceMax).", id);
	syserr(str);
      } else
	sayInstance(id);
    }
  }
}


/***********************************************************************\

  Description Handling

\***********************************************************************/


FORWARD void list(Aword cnt);

/*----------------------------------------------------------------------*/
static Boolean inheritedDescriptionCheck(Aint classId)
{
  if (classId == 0) return TRUE;
  if (!inheritedDescriptionCheck(class[classId].parent)) return FALSE;
  if (class[classId].checks == 0) return TRUE;
  return trycheck(class[classId].checks, TRUE);
}

/*----------------------------------------------------------------------*/
static Boolean descriptionCheck(Aint instanceId)
{
  if (inheritedDescriptionCheck(instance[instanceId].parent)) {
    if (instance[instanceId].checks == 0) return TRUE;
    return trycheck(instance[instanceId].checks, TRUE);
  } else
    return FALSE;
}

/*----------------------------------------------------------------------*/
static Abool inheritsDescriptionFrom(Aword classId)
{
  if (class[classId].description != 0)
    return TRUE;
  else if (class[classId].parent != 0)
    return inheritsDescriptionFrom(class[classId].parent);
  else
    return FALSE;
}  

/*----------------------------------------------------------------------*/
static Abool haveDescription(Aword instanceId)
{
  if (instance[instanceId].description != 0)
    return TRUE;
  else if (instance[instanceId].parent != 0)
    return inheritsDescriptionFrom(instance[instanceId].parent);
  else
    return FALSE;
}

/*----------------------------------------------------------------------*/
static void describeClass(Aword id)
{
  if (class[id].description != 0) {
    /* This class has a description, run it */
    interpret(class[id].description);
  } else {
    /* Search up the inheritance tree, if any, to find a description */
    if (class[id].parent != 0)
      describeClass(class[id].parent);
  }
}


/*----------------------------------------------------------------------*/
static void describeAnything(Aword id)
{
  if (instance[id].description != 0) {
    /* This instance has its own description, run it */
    interpret(instance[id].description);
  } else {
    /* Search up the inheritance tree to find a description */
    if (instance[id].parent != 0)
      describeClass(instance[id].parent);
  }
  admin[id].alreadyDescribed = TRUE;
}


/*----------------------------------------------------------------------*/
static void describeObject(Aword obj)
{
if (haveDescription(obj))
    describeAnything(obj);
  else {
    prmsg(M_SEEOBJ1);
    sayarticle(obj);
    say(obj);
    prmsg(M_SEEOBJ4);
    if (instance[obj].container != 0)
      list(obj);
  }
  admin[obj].alreadyDescribed = TRUE;
}


/*----------------------------------------------------------------------*/
static void describeActor(Aword act)
{
  ScriptEntry *scr = NULL;

  if (admin[act].script != 0) {
    for (scr = (ScriptEntry *) pointerTo(header->scriptTableAddress); !endOfTable(scr); scr++)
      if (scr->code == admin[act].script)
	break;
    if (endOfTable(scr)) scr = NULL;
  }
  if (scr != NULL && scr->description != 0)
    interpret(scr->description);
  else if (haveDescription(act))
    describeAnything(act);
  else {
    interpret(instance[act].mentioned);
    prmsg(M_SEEACT);
    if (instance[act].container != 0)
      list(act);
  }
  admin[act].alreadyDescribed = TRUE;
}



static Boolean descriptionOk;
static Aword dscrstk[255];

/*======================================================================*/
void describe(Aword id)
{
  int i;
  char str[80];
  Aword previousInstance = current.instance;

  for (i = 0; i < dscrstkp; i++)
    if (dscrstk[i] == id)
      syserr("Recursive DESCRIBE.");
  dscrstk[dscrstkp++] = id;
  if (dscrstkp == 255)
    syserr("To deep recursion of DESCRIBE.");

  current.instance = id;
  if (id == 0) {
    sprintf(str, "Can't DESCRIBE item (%ld).", id);
    syserr(str);
  } else if (id > header->instanceMax) {
    sprintf(str, "Can't DESCRIBE item (%ld > instanceMax).", id);
    syserr(str);
  } else if (descriptionCheck(id)) {
    descriptionOk = TRUE;
    if (isObj(id)) {
      describeObject(id);
    } else if (isAct(id)) {
      describeActor(id);
    } else
      describeAnything(id);
  } else
    descriptionOk = FALSE;
  dscrstkp--;
  current.instance = previousInstance;
}


/*======================================================================*/
void describeInstances(void)
{
  int i;
  int prevobj = 0;
  Boolean found = FALSE;
  Boolean multiple = FALSE;

  /* First describe every object here with its own description */
  for (i = 1; i <= header->instanceMax; i++)
    if (instance[i].location == current.location && isA(i, OBJECT) &&
	!admin[i].alreadyDescribed && haveDescription(i))
      describe(i);

  /* Then list all other objects here */
  for (i = 1; i <= header->instanceMax; i++)
    if (instance[i].location == current.location && isA(i, OBJECT) &&
	!admin[i].alreadyDescribed) {
      if (!found) {
	prmsg(M_SEEOBJ1);
	sayarticle(i);
	say(i);
	found = TRUE;
      } else {
	if (multiple) {
	  needsp = FALSE;
	  prmsg(M_SEEOBJ2);
	  sayarticle(prevobj);
	  say(prevobj);
	}
	multiple = TRUE;
      }
      prevobj = i;
    }

  if (found) {
    if (multiple) {
      prmsg(M_SEEOBJ3);
      sayarticle(prevobj);
      say(prevobj);
    }
    prmsg(M_SEEOBJ4);
  }
  
  /* Now for all actors */
  for (i = 1; i <= header->instanceMax; i++)
    if (instance[i].location == current.location && i != HERO && isA(i, ACTOR)
	&& !admin[i].alreadyDescribed)
      describe(i);

  /* Clear the describe flag for all objects */
  for (i = 1; i <= header->instanceMax; i++)
    admin[i].alreadyDescribed = FALSE;
}


/*======================================================================*/
void look(void)
{
  int i;

  if (looking)
    syserr("Recursive LOOK.");

  looking = TRUE;
  /* Set describe flag for all objects and actors */
  for (i = 1; i <= header->instanceMax; i++)
    admin[i].alreadyDescribed = FALSE;

  if (anyOutput)
    para();

#ifdef GLK
  glk_set_style(style_Subheader);
#endif

  interpret(instance[current.location].mentioned);

#ifdef GLK
  glk_set_style(style_Normal);
#endif

  newline();
  describe(current.location);
  if (descriptionOk)
    describeInstances();
  looking = FALSE;
}


/*======================================================================*/
void list(Aword cnt)
{
  int i;
  Aword props;
  Aword prevobj = 0;
  Boolean found = FALSE;
  Boolean multiple = FALSE;

  /* Find container table entry */
  props = instance[cnt].container;
  if (props == 0) syserr("Trying to list something not a container.");

  for (i = 1; i <= header->instanceMax; i++) {
    if (in(i, cnt)) { /* Yes, it's in this container */
      if (!found) {
	found = TRUE;
	if (container[props].header != 0)
	  interpret(container[props].header);
	else {
	  if (isA(container[props].owner, ACTOR)) {
	    say(container[props].owner);
	    prmsg(M_CARRIES);
	  } else {
	    prmsg(M_CONTAINS0);
	    say(container[props].owner);
	    prmsg(M_CONTAINS);
	  }
	}
      } else {
	if (multiple) {
	  needsp = FALSE;
	  prmsg(M_CONTAINSCOMMA);
	}
	multiple = TRUE;
	sayarticle(prevobj);
	say(prevobj);
      }
      prevobj = i;
    }
  }

  if (found) {
    if (multiple)
      prmsg(M_CONTAINSAND);
    sayarticle(prevobj);
    say(prevobj);
    prmsg(M_CONTAINSEND);
  } else {
    if (container[props].empty != 0)
      interpret(container[props].empty);
    else {
      if (isA(container[props].owner, ACTOR)) {
	say(container[props].owner);
	prmsg(M_EMPTYHANDED);
      } else {
	prmsg(M_CONTAINS0);
	say(container[props].owner);
	prmsg(M_EMPTY);
      }
    }
  }
  needsp = TRUE;
}


/*======================================================================*/
void showImage(Aword image, Aword align)
{
#ifdef GLK
  if ((glk_gestalt(gestalt_Graphics, 0) == 1) &&
      (glk_gestalt(gestalt_DrawImage, wintype_TextBuffer) == 1)) {
    glk_window_flow_break(glkMainWin);
    printf("\n");
    (void)glk_image_draw(glkMainWin, image, imagealign_MarginLeft, 0);
  }
#endif
}    
 


/*======================================================================*/
void empty(Aword cnt, Aword whr)
{
  int i;

  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt))
      locate(i, whr);
}



/*======================================================================*/
void use(Aword act, Aword scr)
{
  char str[80];

  if (!isAct(act)) {
    sprintf(str, "Instance is not an Actor (%ld).", act);
    syserr(str);
  }

  admin[act].script = scr;
  admin[act].step = 0;
}

/*======================================================================*/
void stop(Aword act)
{
  char str[80];

  if (!isAct(act)) {
    sprintf(str, "Instance is not an Actor (%ld).", act);
    syserr(str);
  }

  admin[act].script = 0;
  admin[act].step = 0;
}




#ifndef GLK
static char saveFileName[256];
#endif


/*----------------------------------------------------------------------*/
void saveGame(void)
{
  int i;
  AttributeEntry *atr;

#ifdef GLK
  frefid_t saveFileRef;
  strid_t saveFile;
  saveFileRef = glk_fileref_create_by_prompt(fileusage_SavedGame, filemode_Write, 0);
  if (saveFileRef == NULL) return;
  saveFile = glk_stream_open_file(saveFileRef, filemode_Write, 0);

#else

  FILE *saveFile;
  char str[256];

  /* First save ? */
  if (saveFileName[0] == '\0') {
    strcpy(saveFileName, adventureName);
    strcat(saveFileName, ".sav");
  }
  prmsg(M_SAVEWHERE);
  sprintf(str, "(%s) : ", saveFileName);
  output(str);
#ifdef USE_READLINE
  readline(str);
#else
  gets(str);
#endif
  if (str[0] == '\0')
    strcpy(str, saveFileName);
  col = 1;
  if ((saveFile = fopen(str, READ_MODE)) != NULL)
    /* It already existed */
    if (!confirm(M_SAVEOVERWRITE))
      error(MSGMAX);            /* Return to player without saying anything */
  if ((saveFile = fopen(str, WRITE_MODE)) == NULL)
    error(M_SAVEFAILED);
  strcpy(saveFileName, str);
#endif

  /* Save version of interpreter and name of game */
  fwrite((void *)"ASAV", 1, 4, saveFile);
  fwrite((void *)&header->vers, 1, sizeof(Aword), saveFile);
  fwrite((void *)adventureName, 1, strlen(adventureName)+1, saveFile);
  /* Save current values */
  fwrite((void *)&current, sizeof(current), 1, saveFile);

  /* Save admin about each instance and its location & attributes */
  for (i = 1; i <= header->instanceMax; i++) {
    fwrite((void *)&admin[i], sizeof(AdminEntry), 1, saveFile);
    fwrite((void *)&instance[i].location, sizeof(Aword), 1, saveFile);
    if (instance[i].attributes != 0)
      for (atr = (AttributeEntry *) pointerTo(instance[i].attributes); !endOfTable(atr); atr++)
	fwrite((void *)&atr->value, sizeof(Aword), 1, saveFile);
  }

  /* Save the event queue */
  eventQueue[etop].time = 0;        /* Mark the top */
  fwrite((void *)&eventQueue[0], sizeof(eventQueue[0]), etop+1, saveFile);

  /* Save scores */
  for (i = 0; scores[i] != EOF; i++)
    fwrite((void *)&scores[i], sizeof(Aword), 1, saveFile);

  fclose(saveFile);
}


/*----------------------------------------------------------------------*/
void restoreGame(void)
{
  int i;
  AttributeEntry *atr;
  char savedVersion[4];
  char savedName[256];
  char str[256];

#ifdef GLK
  frefid_t saveFileRef;
  strid_t saveFile;
  saveFileRef = glk_fileref_create_by_prompt(fileusage_SavedGame, filemode_Read, 0);
  if (saveFileRef == NULL) return;
  saveFile = glk_stream_open_file(saveFileRef, filemode_Read, 0);

#else

  FILE *saveFile;

  /* First save ? */
  if (saveFileName[0] == '\0') {
    strcpy(saveFileName, adventureName);
    strcat(saveFileName, ".sav");
  }
  prmsg(M_RESTOREFROM);
  sprintf(str, "(%s) : ", saveFileName);
  output(str);
#ifdef USE_READLINE
  readline(str);
#else
  gets(str);
#endif

  if (str[0] == '\0')
    strcpy(str, saveFileName);
  col = 1;
  if (str[0] == '\0')
    strcpy(str, saveFileName);        /* Use the name temporarily */
  if ((saveFile = fopen(str, READ_MODE)) == NULL)
    error(M_SAVEMISSING);
  strcpy(saveFileName, str);          /* Save it for future use */

#endif

  fread((void *)&str, 1, 4, saveFile);
  str[4] = '\0';
  if (strcmp(str, "ASAV") != 0)
    error(M_NOTASAVEFILE);
  fread((void *)&savedVersion, sizeof(Aword), 1, saveFile);
  if (strncmp(savedVersion, header->vers, 4)) {
    error(M_SAVEVERS);
    goto close;
  }

  i = 0;
  while ((savedName[i++] = fgetc(saveFile)) != '\0');
  if (strcmp(savedName, adventureName) != 0) {
    error(M_SAVENAME);
    goto close;
  }

  /* Restore current values */
  fread((void *)&current, sizeof(current), 1, saveFile);

  /* Restore admin and attributes for instances */
  for (i = 1; i <= header->instanceMax; i++) {
    fread((void *)&admin[i], sizeof(AdminEntry), 1, saveFile);
    fread((void *)&instance[i].location, sizeof(Aword), 1, saveFile);
    if (instance[i].attributes != 0)
      for (atr = (AttributeEntry *) pointerTo(instance[i].attributes); !endOfTable(atr); atr++)
	fread((void *)&atr->value, sizeof(Aword), 1, saveFile);
  }

  /* Restore the eventQueue */
  etop = 0;
  do {
    fread((void *)&eventQueue[etop], sizeof(eventQueue[0]), 1, saveFile);
    etop++;
  } while (eventQueue[etop-1].time != 0);
  etop--;

  /* Restore scores */
  for (i = 0; scores[i] != EOF; i++)
    fread((void *)&scores[i], sizeof(Aword), 1, saveFile);

 close:
  fclose(saveFile);
}



/*----------------------------------------------------------------------*/
Aword rnd(Aword from, Aword to)
{
  if (to == from)
    return to;
  else if (to > from)
    return (rand()/10)%(to-from+1)+from;
  else
    return (rand()/10)%(from-to+1)+to;
}



/*----------------------------------------------------------------------*/
Abool btw(Aint val, Aint low, Aint high)
{
  if (high > low)
    return low <= val && val <= high;
  else
    return high <= val && val <= low;
}



/*----------------------------------------------------------------------*/
Aword contains(Aword string, Aword substring)
{
  Abool found;

  strlow((char *)string);
  strlow((char *)substring);

  found = (strstr((char *)string, (char *)substring) != 0);

  free((char *)string);
  free((char *)substring);

  return(found);
}



/*======================================================================

  streq()

  Compare two strings approximately, ignore case

  */
Abool streq(char a[], char b[])
{
  Boolean eq;

  strlow(a);
  strlow(b);

  eq = (strcmp(a, b) == 0);

  free(a);
  free(b);

  return(eq);
}
