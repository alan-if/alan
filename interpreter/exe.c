/*----------------------------------------------------------------------*\

  exe.c

  Amachine instruction execution unit of Alan interpreter

\*----------------------------------------------------------------------*/

#include "types.h"
#include "act.h"
#include "debug.h"
#include "syserr.h"

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


/* PRIVATE TYPES */
typedef struct GameState {
  /* Current data, can't use all of the CurVars (tick changes every move) */
  int score;

  /* Event queue */
  int eventQueueSize;
  EventQueueEntry *eventQueue;
  int eventQueueTop;		/* Event queue top pointer */

  /* Amachine data structures - Dynamic */
  AdminEntry *admin;		/* Administrative data about instances */
  AttributeEntry *attributes;	/* Attributes data area */
  Aword *scores;		/* Score table pointer */

} GameState;

/* PRIVATE DATA */
static int gameStateTop = 0;
static int gameStateSize = 0;
static GameState *gameState = NULL;

/*----------------------------------------------------------------------*/
static void ensureSpaceForGameState() {
  if (gameStateTop == gameStateSize) {
    gameState = realloc(gameState, (gameStateSize+2)*sizeof(GameState));
    if (gameState == NULL) syserr("Out of memory in pushGameState()");
    gameStateSize += 2;
  }
}


/*----------------------------------------------------------------------*/
static Boolean gameStateChanged(void)
{
  int i;
  Aword *previousEventQueue = (Aword *)gameState[gameStateTop-1].eventQueue;
  Aword *previousAdmin = (Aword *)gameState[gameStateTop-1].admin;
  Aword *previousAttributes = (Aword *)gameState[gameStateTop-1].attributes;
  Aword *previousScores = (Aword *)gameState[gameStateTop-1].scores;

  /* Compare current game state with last saved */
  if (gameState[gameStateTop-1].eventQueueSize != eventQueueSize) return TRUE;

  if (gameState[gameStateTop-1].score != current.score) return TRUE;

  for (i = 0; i < eventQueueTop*sizeof(EventQueueEntry)/sizeof(Aword); i++)
    if (((Aword*)eventQueue)[i] != previousEventQueue[i]) return TRUE;

  if (admin == NULL) syserr("admin[] == NULL in pushGameState()");
  for (i = 0; i < header->instanceMax*sizeof(AdminEntry)/sizeof(Aword); i++)
    if (((Aword*)admin)[i] != previousAdmin[i]) return TRUE;

  if (attributes == NULL) syserr("attributes[] == NULL in pushGameState()");
  for (i = 0; i < header->attributesAreaSize; i++)
    if (((Aword*)attributes)[i] != previousAttributes[i]) return TRUE;

  for (i = 0; i < header->scoresMax; i++)
    if (scores[i] != previousScores[i]) return TRUE;

  return FALSE;
}


/*======================================================================*/
void pushGameState(void) {

  ensureSpaceForGameState();

  if (gameStateChanged()) {
    gameState[gameStateTop].eventQueueSize = eventQueueSize;
    gameState[gameStateTop].eventQueue = duplicate(eventQueue, eventQueueSize*sizeof(EventQueueEntry));
    gameState[gameStateTop].eventQueueTop = eventQueueTop;

    gameState[gameStateTop].admin = duplicate(admin, header->instanceMax*sizeof(AdminEntry));
    gameState[gameStateTop].attributes = duplicate(attributes, header->attributesAreaSize*sizeof(Aword));
    gameState[gameStateTop].scores = duplicate(scores, header->scoresMax*sizeof(Aword));

    gameStateTop++;
  }
}
  
  
/*======================================================================*/
Boolean popGameState(void) {

  if (gameStateTop == 0) {
    return FALSE;
  }

  gameStateTop--;

  eventQueueSize = gameState[gameStateTop].eventQueueSize;
  eventQueueTop = gameState[gameStateTop].eventQueueTop;
  memcpy(eventQueue, gameState[gameStateTop].eventQueue,
	 eventQueueSize*sizeof(EventQueueEntry));
  free(gameState[gameStateTop].eventQueue);

  current.score = gameState[gameStateTop].score;

  if (admin == NULL) syserr("admin[] == NULL in popGameState()");
  memcpy(admin, gameState[gameStateTop].admin,
 	 header->instanceMax*sizeof(AdminEntry));
  free(gameState[gameStateTop].admin);

  if (attributes == NULL) syserr("attributes[] == NULL in pushGameState()");
  memcpy(attributes, gameState[gameStateTop].attributes,
	 header->attributesAreaSize*sizeof(Aword));
  free(gameState[gameStateTop].attributes);

  memcpy(scores, gameState[gameStateTop].scores,
	 header->scoresMax*sizeof(Aword));
  free(gameState[gameStateTop].scores);

  return TRUE;
}

/* Forward: */
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
Boolean undo(void) {
  if (gameStateTop != 0) {
    gameStateTop--;
    return popGameState();
  } else
    return FALSE;
}


/*======================================================================*/
void quitGame(void)
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

  for(i = eventQueueTop-1; i>=0; i--)
    if (eventQueue[i].event == evt) {
      while (i < eventQueueTop-1) {
	eventQueue[i].event = eventQueue[i+1].event;
	eventQueue[i].time = eventQueue[i+1].time;
	eventQueue[i].where = eventQueue[i+1].where;
	i++;
      }
      eventQueueTop--;
      return;
    }
}


/*======================================================================*/
void increaseEventQueue(void)
{
  eventQueue = realloc(eventQueue, (eventQueueTop+2)*sizeof(EventQueueEntry));
  if (eventQueue == NULL) syserr("Out of memory in increaseEventQueue()");

  eventQueueSize = eventQueueTop + 2;
}


/*======================================================================*/
void schedule(Aword event, Aword where, Aword after)
{  int i;
   int time;
  
  cancelEvent(event);
  /* Check for overflow */
  if (eventQueueTop == eventQueueSize)
    increaseEventQueue();
  
  time = current.tick+after;
  
  /* Bubble this event down */
  for (i = eventQueueTop; i >= 1 && eventQueue[i-1].time <= time; i--) {
    eventQueue[i].event = eventQueue[i-1].event;
    eventQueue[i].time = eventQueue[i-1].time;
    eventQueue[i].where = eventQueue[i-1].where;
  }
  
  eventQueue[i].time = time;
  eventQueue[i].where = where;
  eventQueue[i].event = event;
  eventQueueTop++;
}



/*======================================================================*/
AttributeEntry *findAttribute(AttributeEntry *attributeTable,
			      Aint attributeCode)
{
  AttributeEntry *attribute = attributeTable;
  while (attribute->code != attributeCode) {
    attribute++;
    if (*((Aword *)attribute) == EOF)
      syserr("Attribute not found.");
  }
  return attribute;
}




/*----------------------------------------------------------------------*/
static Aword getAttribute(AttributeEntry *attributeTable,
			  Aint attributeCode)
{
  AttributeEntry *attribute = findAttribute(attributeTable, attributeCode);

  return attribute->value;
}
  

/*----------------------------------------------------------------------*/
static void setAttribute(AttributeEntry *attributeTable,
			 Aword attributeCode,
			 Aword newValue)
{
  AttributeEntry *attribute = findAttribute(attributeTable, attributeCode);

  attribute->value = newValue;
}


/*======================================================================*/
void make(Aword id, Aword code, Aword val)
{
  char str[80];

  if (id > 0 && id <= header->instanceMax) {
    setAttribute(admin[id].attributes, code, val);
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
    setAttribute(admin[id].attributes, atr, val);
    if (isA(id, LOCATION))	/* May have changed so describe next time */
      admin[id].visitsCount = 0;
  } else {
    sprintf(str, "Can't SET instance (%ld).", id);
    syserr(str);
  }
}


/*======================================================================*/
void setStringAttribute(Aword id, Aword atr, Aword str)
{
  free((char *)attributeOf(id, atr));
  set(id, atr, str);
}



/*----------------------------------------------------------------------*/
static void incrementAttribute(AttributeEntry *attributeTable,
			       Aint attributeCode,
			       Aint step)
{
  AttributeEntry *attribute = findAttribute(attributeTable, attributeCode);
  
  attribute->value += step;
}


/*======================================================================*/
void increase(Aword id, Aword attributeCode, Aword step)
{
  char str[80];

  if (id > 0 && id <= header->instanceMax)
    incrementAttribute(admin[id].attributes, attributeCode, step);
  else {
    sprintf(str, "Can't INCR instance attribute (%ld, %ld).", id, attributeCode);
    syserr(str);
  }
}

/*======================================================================*/
void decrease(Aword id, Aword atr, Aword step)
{
  char str[80];

  if (id > 0 && id <= header->instanceMax)
    incrementAttribute(admin[id].attributes, atr, -step);
  else {
    sprintf(str, "Can't DECR instance attribute (%ld, %ld).", id, atr);
    syserr(str);
  }
}



/*----------------------------------------------------------------------*/
static Aword literalAttribute(Aword lit, Aword atr)
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

  if (isLiteral(id))
    return literalAttribute(id, atr);
  else {
    if (id > 0 && id <= header->instanceMax)
      return getAttribute(admin[id].attributes, atr);
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

/*======================================================================*/
Aword concat(Aword s1, Aword s2)
{
  char *result = allocate(strlen((char*)s1)+strlen((char*)s2)+1);
  strcpy(result, (char*)s1);
  strcat(result, (char*)s2);
  free((char*)s1);
  free((char*)s2);
  return (Aword)result;
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

/*----------------------------------------------------------------------*/
static void verifyId(Aword id, char action[]) {
  char message[200];

  if (id == 0) {
    sprintf(message, "Can't %s instance (%ld).", action, id);
    syserr(message);
  } else if (id > header->instanceMax) {
    sprintf(message, "Can't %s instance (%ld > instanceMax).", action, id);
    syserr(message);
  }
}


/*======================================================================*/
Aword where(Aword id)
{
  verifyId(id, "WHERE");
  return admin[id].location;
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
      } else if (admin[i].location == whr && attributeOf(i, atr) > max)
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
      } else if (admin[i].location == whr)
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
      } else if (admin[i].location == whr)
	count++;
    }
  }
  return(count);
}

/*----------------------------------------------------------------------*/
static void locateIntoContainer(Aword ins, Aword whr) {
  if (!isA(ins, container[instance[whr].container].class))
    error(M_CANNOTCONTAIN);
  else if (checklim(whr, ins))
    error(MSGMAX);		/* Return to player without any message */
  else
    admin[ins].location = whr;
}


/*----------------------------------------------------------------------*/
static void locateObject(Aword obj, Aword whr)
{
  if (isCnt(whr)) { /* Into a container */
    locateIntoContainer(obj, whr);
  } else {
    admin[obj].location = whr;
    /* Make sure the location is described since it's changed */
    admin[whr].visitsCount = 0;
  }
}


/*----------------------------------------------------------------------*/
static void executeInheritedEntered(Aint theClass) {
  if (theClass == 0) return;
  if (class[theClass].entered)
    interpret(class[theClass].entered);
  else
    executeInheritedEntered(class[theClass].parent);
}


/*----------------------------------------------------------------------*/
static void locateActor(Aword movingActor, Aword whr)
{
  Aword previousLocation = current.location;
  Aword previousActor = current.actor;

  /* FIXME: Actors locating into containers is dubious, anyway as it
   is now it allows the hero to be located into a container. And what
   happens with current location if so... */
  if (isCnt(whr))
    locateIntoContainer(movingActor, whr);
  else {
    current.location = whr;
    admin[movingActor].location = whr;
  }
  if (movingActor == HERO) {
    if (admin[admin[movingActor].location].visitsCount % (current.visits+1) == 0)
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
  current.actor = movingActor;
  if (instance[current.location].entered != 0) {
    interpret(instance[current.location].entered);
  } else
    executeInheritedEntered(instance[current.location].parent);
  current.actor = previousActor;

  if (current.actor != movingActor)
    current.location = previousLocation;
}


/*======================================================================*/
void locate(Aword id, Aword whr)
{
  Aword containerId;
  ContainerEntry *theContainer;
  Aword previousInstance = current.instance;

  verifyId(id, "LOCATE");
  verifyId(whr, "LOCATE AT");

  /* First check if the instance is in a container, if so run extract checks */
  if (isCnt(admin[id].location)) {    /* In something? */
    current.instance = admin[id].location;
    containerId = instance[admin[id].location].container;
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

  if (isCnt(admin[id].location)) {    /* In something? */
    owner = admin[id].location;
    if (admin[owner].location != 0)
      return(isHere(owner));
    else /* If the container wasn't anywhere, assume where HERO is! */
      return(where(HERO) == current.location);
  } else
    return(admin[id].location == current.location);
}


/*======================================================================*/
Aword isHere(Aword id)
{
  verifyId(id, "HERE");
  return instanceHere(id);
}


/*----------------------------------------------------------------------*/
static Aword objnear(Aword obj)
{
  if (isCnt(admin[obj].location)) {    /* In something? */
    if (isObj(admin[obj].location) || isAct(admin[obj].location))
      return(isNear(admin[obj].location));
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

  if (isLiteral(instanceId))
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

  return (admin[theInstance].location == cnt);
}



/*----------------------------------------------------------------------*/
static void executeInheritedMentioned(Aword theClass) {
  if (theClass == 0) return;

  if (class[theClass].mentioned)
    interpret(class[theClass].mentioned);
  else
    executeInheritedMentioned(class[theClass].parent);
}


/*----------------------------------------------------------------------*/
static void mention(Aword id) {
  if (instance[id].mentioned)
    interpret(instance[id].mentioned);
  else
    executeInheritedMentioned(instance[id].parent);
}


/*----------------------------------------------------------------------*/
static void sayLiteral(Aword lit)
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
#ifdef SAY_INSTANCE_WITH_PLAYER_WORDS_IF_PARAMETER
  int p, i;

  /* Find the id in the parameters... */
  if (params != NULL)
    for (p = 0; params[p].code != EOF; p++)
      if (params[p].code == id) {
	/* Found it so.. */
	if (params[p].firstWord == EOF) /* Any words he used? */
	  break;		/* No... */
	else {				/* Yes, so use them... */ 
	  char *capitalized;
	  /* Assuming the noun is the last word we can simply output the adjectives... */
	  for (i = params[p].firstWord; i <= params[p].lastWord-1; i++)
	    output((char *)pointerTo(dict[wrds[i]].wrd));
	  /* ... and then the noun, capitalized if necessary */
	  if (header->capitalizeNouns) {
	    capitalized = strdup((char *)pointerTo(dict[wrds[params[p].lastWord]].wrd));
	    capitalized[0] = IsoToUpperCase(capitalized[0]);
	    output(capitalized);
	    free(capitalized);
	  } else
	    output((char *)pointerTo(dict[wrds[params[p].lastWord]].wrd));
	}
	return;
      }
#endif
  mention(id);
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


/*----------------------------------------------------------------------*/
static Boolean sayInheritedDefiniteForm(Aword theClass) {
  if (theClass == 0) {
    prmsg(M_DEFINITE);
    return FALSE;
  } else {
    if (class[theClass].definite) {
      interpret(class[theClass].definite);
      return class[theClass].definiteIsForm;
    } else
      return sayInheritedDefiniteForm(class[theClass].parent);
  }
}


/*----------------------------------------------------------------------*/
static void sayDefinite(Aint id) {
  if (instance[id].definite) {
    interpret(instance[id].definite);
    if (!instance[id].definiteIsForm)
      sayInstance(id);
  } else
    if (!sayInheritedDefiniteForm(instance[id].parent))
      mention(id);
}

/*----------------------------------------------------------------------*/
static Boolean sayInheritedIndefiniteForm(Aword theClass) {
  if (theClass == 0) {
    prmsg(M_INDEFINITE);
    return FALSE;
  } else {
    if (class[theClass].indefinite) {
      interpret(class[theClass].indefinite);
      return class[theClass].indefiniteIsForm;
    } else
      return sayInheritedIndefiniteForm(class[theClass].parent);
  }
}


/*----------------------------------------------------------------------*/
static void sayIndefinite(Aint id) {
  if (instance[id].indefinite) {
    interpret(instance[id].indefinite);
    if (!instance[id].indefiniteIsForm)
      sayInstance(id);
  } else
    if (!sayInheritedIndefiniteForm(instance[id].parent))
      mention(id);
}


/*----------------------------------------------------------------------*/
static void sayArticleOrForm(Aint id, SayForm form)
{
  switch (form) {
  case SAY_DEFINITE:
    sayDefinite(id);
    break;
  case SAY_INDEFINITE:
    sayIndefinite(id);
    break;
  case SAY_SIMPLE:
    say(id);
    break;
  default:
    syserr("Unexpected form in 'sayArticleOrForm()'");
  }
}


/*======================================================================*/
void say(Aword id)
{
  Aword previousInstance = current.instance;
  current.instance = id;

  if (isHere(HERO)) {
    if (isLiteral(id))
      sayLiteral(id);
    else {
      verifyId(id, "SAY");
      sayInstance(id);
    }
  }
  current.instance = previousInstance;
}


/*======================================================================*/
void sayForm(Aword id, SayForm form)
{
  Aword previousInstance = current.instance;
  current.instance = id;

  sayArticleOrForm(id, form);

  current.instance = previousInstance;
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
  if (class[classId].descriptionChecks == 0) return TRUE;
  return trycheck(class[classId].descriptionChecks, TRUE);
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
static Boolean containerIsEmpty(Aword cnt)
{
  int i;

  for (i = 1; i <= header->instanceMax; i++)
    if (instance[i].location == cnt)
      return FALSE;
  return TRUE;
}


/*----------------------------------------------------------------------*/
static void describeContainer(Aword id)
{
  if (!containerIsEmpty(id) && !attributeOf(id, OPAQUEATTRIBUTE))
    list(id);
}    


/*----------------------------------------------------------------------*/
static void describeObject(Aword obj)
{
  if (haveDescription(obj))
    describeAnything(obj);
  else {
    prmsg(M_SEEOBJ1);
    sayForm(obj, SAY_INDEFINITE);
    prmsg(M_SEEOBJ4);
    if (instance[obj].container != 0)
      describeContainer(obj);
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
    mention(act);
    prmsg(M_SEEACT);
    if (instance[act].container != 0)
      describeContainer(act);
  }
  admin[act].alreadyDescribed = TRUE;
}



static Boolean descriptionOk;
static Aword dscrstk[255];

/*======================================================================*/
void describe(Aword id)
{
  int i;
  Aword previousInstance = current.instance;

  for (i = 0; i < dscrstkp; i++)
    if (dscrstk[i] == id)
      syserr("Recursive DESCRIBE.");
  dscrstk[dscrstkp++] = id;
  if (dscrstkp == 255)
    syserr("To deep recursion of DESCRIBE.");

  current.instance = id;
  verifyId(id, "DESCRIBE");
  if (descriptionCheck(id)) {
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
    if (admin[i].location == current.location && isA(i, OBJECT) &&
	!admin[i].alreadyDescribed && haveDescription(i))
      describe(i);

  /* Then list all other objects here */
  for (i = 1; i <= header->instanceMax; i++)
    if (admin[i].location == current.location && isA(i, OBJECT) &&
	!admin[i].alreadyDescribed) {
      if (!found) {
	prmsg(M_SEEOBJ1);
	sayForm(i, SAY_INDEFINITE);
	found = TRUE;
      } else {
	if (multiple) {
	  needSpace = FALSE;
	  prmsg(M_SEEOBJ2);
	  sayForm(prevobj, SAY_INDEFINITE);
	}
	multiple = TRUE;
      }
      prevobj = i;
    }

  if (found) {
    if (multiple) {
      prmsg(M_SEEOBJ3);
      sayForm(prevobj, SAY_INDEFINITE);
    }
    prmsg(M_SEEOBJ4);
  }
  
  /* Now for all actors */
  for (i = 1; i <= header->instanceMax; i++)
    if (admin[i].location == current.location && i != HERO && isA(i, ACTOR)
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

  mention(current.location);

#ifdef GLK
  glk_set_style(style_Normal);
#endif

  newline();
  capitalize = TRUE;
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
  Aword previouslyFoundInstance = 0;
  Boolean found = FALSE;
  Boolean multiple = FALSE;
  Aint previousThis = current.instance;

  current.instance = cnt;

  /* Find container table entry */
  props = instance[cnt].container;
  if (props == 0) syserr("Trying to list something not a container.");

  for (i = 1; i <= header->instanceMax; i++) {
    if (in(i, cnt)) {		/* Yes, it's in this container */
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
	  needSpace = FALSE;
	  prmsg(M_CONTAINSCOMMA);
	}
	multiple = TRUE;
	sayForm(previouslyFoundInstance, SAY_INDEFINITE);
      }
      previouslyFoundInstance = i;
    }
  }

  if (found) {
    if (multiple)
      prmsg(M_CONTAINSAND);
    sayForm(previouslyFoundInstance, SAY_INDEFINITE);
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
  needSpace = TRUE;
  current.instance = previousThis;
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

  /* Save admin about each instance and its attributes */
  for (i = 1; i <= header->instanceMax; i++) {
    fwrite((void *)&admin[i], sizeof(AdminEntry), 1, saveFile);
    if (instance[i].attributes != 0)
      for (atr = admin[i].attributes; !endOfTable(atr); atr++)
	fwrite((void *)&atr->value, sizeof(AttributeEntry), 1, saveFile);
  }

  /* Save the event queue */
  eventQueue[eventQueueTop].time = 0;        /* Mark the top */
  fwrite((void *)&eventQueue[0], sizeof(eventQueue[0]), eventQueueTop+1, saveFile);

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
    if (instance[i].attributes != 0)
      for (atr = admin[i].attributes; !endOfTable(atr); atr++)
	fread((void *)&atr->value, sizeof(AttributeEntry), 1, saveFile);
  }

  /* Restore the eventQueue */
  eventQueueTop = 0;
  do {
    fread((void *)&eventQueue[eventQueueTop], sizeof(eventQueue[0]), 1, saveFile);
    eventQueueTop++;
  } while (eventQueue[eventQueueTop-1].time != 0);
  eventQueueTop--;

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
