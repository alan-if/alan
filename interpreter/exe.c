/*----------------------------------------------------------------------*\

  exe.c

  Amachine instruction execution unit of Alan interpreter

\*----------------------------------------------------------------------*/

#include "types.h"

#ifdef USE_READLINE
#include "readline.h"
#endif

#ifdef GLK
#include "glk.h"
#endif

#include "main.h"
#include "parse.h"
#include "inter.h"
#include "act.h"
#include "stack.h"
#include "decode.h"

#include "exe.h"

#define WIDTH 80

#define N_EVTS 100


/* PUBLIC DATA */

/* The event queue */
EventQueueEntry eventQueue[N_EVTS];        /* Event queue */
int etop = 0;                   /* Event queue top pointer */

Boolean looking = FALSE;        /* LOOKING? flag */

int dscrstkp = 0;               /* Describe-stack pointer */


#ifdef _PROTOTYPES_
void describeInstances(void);
#else
void describeInstances();
#endif



#ifdef _PROTOTYPES_
void print(Aword fpos, Aword len)
#else
void print(fpos, len)
     Aword fpos, len;
#endif
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


#ifdef _PROTOTYPES_
void sys(Aword fpos, Aword len)
#else
void sys(fpos, len)
     Aword fpos, len;
#endif
{
  char *command;

  getstr(fpos, len);            /* Returns address to string on stack */
  command = (char *)pop();
  system(command);
  free(command);
}


#ifdef _PROTOTYPES_
void getstr(Aword fpos, Aword len)
#else
void getstr(fpos, len)
     Aword fpos, len;
#endif
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



#ifdef _PROTOTYPES_
void score(Aword sc)
#else
void score(sc)
     Aword sc;
#endif
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

#ifdef _PROTOTYPES_
void visits(Aword v)
#else
void visits(v)
     Aword v;
#endif
{
  current.visits = v;
}


#ifdef _PROTOTYPES_
Boolean confirm(MsgKind msgno)
#else
Boolean confirm(msgno)
     MsgKind msgno;
#endif
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


#ifdef _PROTOTYPES_
void quit(void)
#else
void quit()
#endif
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
      restore();
      return;
    } else if (strcmp(buf, "quit") == 0)
      terminate(0);
  }
  syserr("Fallthrough in QUIT");
}



#ifdef _PROTOTYPES_
void restart(void)
#else
void restart()
#endif
{
  para();
  if (confirm(M_REALLY)) {
    longjmp(restart_label, TRUE);
  } else
    return;
  syserr("Fallthrough in RESTART");
}



#ifdef _PROTOTYPES_
void cancl(Aword evt)
#else
void cancl(evt)
     Aword evt;
#endif
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


#ifdef _PROTOTYPES_
void schedule(Aword evt, Aword whr, Aword aft)
#else
void schedule(evt, whr, aft)
     Aword evt, whr, aft;
#endif
{  int i;
   int time;
  
  cancl(evt);
  /* Check for overflow */
  if (etop == N_EVTS) syserr("Out of event space.");
  
  time = current.tick+aft;
  
  /* Bubble this event down */
  for (i = etop; i >= 1 && eventQueue[i-1].time <= time; i--) {
    eventQueue[i].event = eventQueue[i-1].event;
    eventQueue[i].time = eventQueue[i-1].time;
    eventQueue[i].where = eventQueue[i-1].where;
  }
  
  eventQueue[i].time = time;
  eventQueue[i].where = whr;
  eventQueue[i].event = evt;
  etop++;
}



/*----------------------------------------------------------------------

  findAttribute()

  Return a pointer to the AttributeEntry for the attribute with "code"

*/
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




/*----------------------------------------------------------------------

  getatr()

  Get an attribute value from an attribute list

 */
#ifdef _PROTOTYPES_
static Aword getatr(
     Aaddr atradr,              /* IN - ACODE address to attribute table */
     Aaddr code                  /* IN - The attribute to read */
)
#else
static Aword getatr(atradr, code)
     Aaddr atradr;              /* IN - ACODE address to attribute table */
     Aaddr code;                 /* IN - The attribute to read */
#endif
{
  AttributeEntry *attribute = findAttribute(atradr, code);

  return attribute->value;
}
  

/*----------------------------------------------------------------------

  setatr()

  Set a particular attribute to a value.

 */
#ifdef _PROTOTYPES_
static void setatr(
     Aaddr atradr,              /* IN - ACODE address to attribute table */
     Aword code,                 /* IN - attribute code */
     Aword val                  /* IN - new value */
)
#else
static void setatr(atradr, code, val)
     Aaddr atradr;              /* IN - ACODE address to attribute table */
     Aword code;                 /* IN - attribute code */
     Aword val;                 /* IN - new value */
#endif
{
  AttributeEntry *at = findAttribute(atradr, code);

  at->value = val;
}


/*----------------------------------------------------------------------

  make()

  */

#ifdef _PROTOTYPES_
void make(Aword id, Aword code, Aword val)
#else
void make(id, code, val)
	  Aword id, code, val;
#endif
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


/*----------------------------------------------------------------------------

  set()

 */

#ifdef _PROTOTYPES_
void set(Aword id, Aword atr, Aword val)
#else
void set(id, atr, val)
     Aword id, atr, val;
#endif
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


#ifdef _PROTOTYPES_
void setstr(Aword id, Aword atr, Aword str)
#else
void setstr(id, atr, str)
     Aword id, atr, str;
#endif
{
  free((char *)attribute(id, atr));
  set(id, atr, str);
}



/*-----------------------------------------------------------------------------

  incr/decr

  */

/*----------------------------------------------------------------------

  incratr()

  Increment a particular attribute by a value.

 */
#ifdef _PROTOTYPES_
static void incratr(
	Aaddr atradr,           /* IN - ACODE address to attribute table */
	Aword code,		/* IN - attribute code */
	Aword step              /* IN - step to increment by */
)
#else
static void incratr(atradr, code, step)
     Aaddr atradr:
     Aword atr, step;
#endif
{
  AttributeEntry *at = findAttribute(atradr, code);
  
  at->value += step;
}


#ifdef _PROTOTYPES_
void incr(Aword id, Aword atr, Aword step)
#else
void incr(id, atr, step)
     Aword id, atr, step;
#endif
{
  char str[80];

  if (id > 0 && id <= header->instanceMax)
    incratr(instance[id].attributes, atr, step);
  else {
    sprintf(str, "Can't INCR instance attribute (%ld, %ld).", id, atr);
    syserr(str);
  }
}

#ifdef _PROTOTYPES_
void decr(Aword id, Aword atr, Aword step)
#else
void decr(id, atr, step)
     Aword id, atr, step;
#endif
{
  char str[80];

  if (id > 0 && id <= header->instanceMax)
    incratr(instance[id].attributes, atr, -step);
  else {
    sprintf(str, "Can't DECR instance attribute (%ld, %ld).", id, atr);
    syserr(str);
  }
}



/*----------------------------------------------------------------------

  attribute()

  */

#ifdef _PROTOTYPES_
static Aword litatr(Aword lit, Aword atr)
#else
static Aword litatr(lit, atr)
     Aword lit, atr;
#endif
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


#ifdef _PROTOTYPES_
Aword attribute(Aword id, Aword atr)
#else
Aword attribute(id, atr)
     Aword id, atr;
#endif
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


#ifdef _PROTOTYPES_
Aword strattr(Aword id, Aword atr)
#else
Aword strattr(id, atr)
     Aword id, atr;
#endif
{
  return (Aword) strdup((char *)attribute(id, atr));
}


/*----------------------------------------------------------------------

  where()

  */

#ifdef FIXME
#ifdef _PROTOTYPES_
static Aword objloc(Aword obj)
#else
static Aword objloc(obj)
     Aword obj;
#endif
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


#ifdef _PROTOTYPES_
Aword where(Aword id)
#else
Aword where(id)
     Aword id;
#endif
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



/*----------------------------------------------------------------------

  aggregates

  */

#ifdef _PROTOTYPES_
Aint agrmax(Aword atr, Aword whr)
#else
Aint agrmax(atr, whr)
     Aword atr, whr;
#endif
{
  Aword i;
  Aint max = 0;

  for (i = 1; i <= header->instanceMax; i++) {
    if (isObj(i)) {
      if (isLoc(whr)) {
	if (where(i) == whr && attribute(i, atr) > max)
	  max = attribute(i, atr);
      } else if (instance[i].location == whr && attribute(i, atr) > max)
	max = attribute(i, atr);
    }
  }
  return(max);
}

#ifdef _PROTOTYPES_
Aint agrsum(Aword atr, Aword whr)
#else
Aint agrsum(atr, whr)
     Aword atr, whr;
#endif
{
  Aword i;
  Aint sum = 0;

  for (i = 1; i <= header->instanceMax; i++) {
    if (isObj(i)) {
      if (isLoc(whr)) {
	if (where(i) == whr)
	  sum += attribute(i, atr);
      } else if (instance[i].location == whr)
	sum += attribute(i, atr);
    }
  }
  return(sum);
}


#ifdef _PROTOTYPES_
Aint agrcount(Aword whr)
#else
Aint agrcount(whr)
     Aword whr;
#endif
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


/*----------------------------------------------------------------------

  locate()

  */

#ifdef _PROTOTYPES_
static void locateObject(Aword obj, Aword whr)
#else
static void locateObject(obj, whr)
     Aword obj, whr;
#endif
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


#ifdef _PROTOTYPES_
static void locateActor(Aword act, Aword whr)
#else
static void locateActor(act, whr)
     Aword act, whr;
#endif
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


#ifdef _PROTOTYPES_
void locate(Aword id, Aword whr)
#else
void locate(id, whr)
     Aword id, whr;
#endif
{
  char str[80];

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
  } else if (isAct(id)) {
    locateActor(id, whr);
  } else
    locateObject(id, whr);
}


/*----------------------------------------------------------------------

  isHere()

  */

#ifdef _PROTOTYPES_
static Abool instanceHere(Aword id)
#else
static Abool instanceHere(id)
     Aword obj;
#endif
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


#ifdef _PROTOTYPES_
Aword isHere(Aword id)
#else
Aword isHere(id)
     Aword id;
#endif
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


/*----------------------------------------------------------------------

  isNear()

  */

#ifdef _PROTOTYPES_
static Aword objnear(Aword obj)
#else
static Aword objnear(obj)
     Aword obj;
#endif
{
  if (isCnt(instance[obj].location)) {    /* In something? */
    if (isObj(instance[obj].location) || isAct(instance[obj].location))
      return(isNear(instance[obj].location));
    else  /* If the container wasn't anywhere, assume here, so not nearby! */
      return(FALSE);
  } else
    return(exitto(where(obj), current.location));
}


#ifdef _PROTOTYPES_
static Aword actnear(Aword act)
#else
static Aword actnear(act)
     Aword act;
#endif
{
  return(exitto(where(act), current.location));
}


#ifdef _PROTOTYPES_
Abool isNear(Aword id)
#else
Abool isNear(id)
     Aword id;
#endif
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



/*======================================================================

  isA()

  Is an instance a member of the class

*/
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
#ifdef _PROTOTYPES_
Abool in(Aword theInstance, Aword cnt)
#else
Abool in(theInstance, cnt)
     Aword theInstance;
     Aword cnt;
#endif
{
  if (!isCnt(cnt))
    syserr("IN in a non-container.");

  return (instance[theInstance].location == cnt);
}


/*----------------------------------------------------------------------

  say()

  */

#ifdef _PROTOTYPES_
void sayint(Aword val)
#else
void sayint(val)
     Aword val;
#endif
{
  char buf[25];

  if (isHere(HERO)) {
    sprintf(buf, "%ld", val);
    output(buf);
  }
}


#ifdef _PROTOTYPES_
void saystr(char *str)
#else
void saystr(str)
     char *str;
#endif
{
  if (isHere(HERO))
    output(str);
  free(str);
}


#ifdef _PROTOTYPES_
static void saylit(Aword lit)
#else
static void saylit(lit)
     Aword lit;
#endif
{
  char *str;

  if (isNum(lit))
    sayint(literal[lit-header->instanceMax].value);
  else {
    str = (char *)strdup((char *)literal[lit-header->instanceMax].value);
    saystr(str);
  }    
}

	
#ifdef _PROTOTYPES_
void sayarticle(Aword id)
#else
void sayarticle(id)
     Aword id;
#endif
{
  if (instance[id].article != 0)
    interpret(instance[id].article);
  else
    prmsg(M_ARTICLE);
}


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


#ifdef _PROTOTYPES_
void say(Aword id)
#else
void say(id)
     Aword id;
#endif
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
#ifdef _PROTOTYPES_
static void describeObject(Aword obj)
#else
static void describeObject(obj)
     Aword obj;
#endif
{
  if (instance[obj].description != 0)
    interpret(instance[obj].description);
  else if (haveDescription(obj))
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
#ifdef _PROTOTYPES_
static void describeActor(Aword act)
#else
static void describeActor(act)
     Aword act;
#endif
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
  else if (instance[act].description != 0)
    interpret(instance[act].description);
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

/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
void describe(Aword id)
#else
void describe(id)
     Aword id;
#endif
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


/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
void describeInstances(void)
#else
void describeInstances()
#endif
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
    if (instance[i].location == current.location && isA(i, ACTOR) &&
	!admin[i].alreadyDescribed && i != HERO)
      describe(i);

  /* Clear the describe flag for all objects */
  for (i = 1; i <= header->instanceMax; i++)
    admin[i].alreadyDescribed = FALSE;
}


/*======================================================================*/
#ifdef _PROTOTYPES_
void look(void)
#else
void look()
#endif
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


/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
void use(Aword act, Aword scr)
#else
void use(act, scr)
     Aword act, scr;
#endif
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
#ifdef _PROTOTYPES_
void list(Aword cnt)
#else
void list(cnt)
     Aword cnt;
#endif
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


/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
void empty(Aword cnt, Aword whr)
#else
void empty(cnt, whr)
     Aword cnt;
     Aword whr;
#endif
{
  int i;

  for (i = 1; i <= header->instanceMax; i++)
    if (in(i, cnt))
      locate(i, whr);
}





static char savfnm[256];


/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
void save(void)
#else
void save()
#endif
{
  int i;
  FILE *savfil;
  char str[256];
  AttributeEntry *atr;

  /* First save ? */
  if (savfnm[0] == '\0') {
    strcpy(savfnm, advnam);
    strcat(savfnm, ".sav");
  }
  prmsg(M_SAVEWHERE);
  sprintf(str, "(%s) : ", savfnm);
  output(str);
#ifdef USE_READLINE
  readline(str);
#else
  gets(str);
#endif
  if (str[0] == '\0')
    strcpy(str, savfnm);
  col = 1;
  if ((savfil = fopen(str, READ_MODE)) != NULL)
    /* It already existed */
    if (!confirm(M_SAVEOVERWRITE))
      error(MSGMAX);            /* Return to player without saying anything */
  if ((savfil = fopen(str, WRITE_MODE)) == NULL)
    error(M_SAVEFAILED);
  strcpy(savfnm, str);

  /* Save version of interpreter and name of game */
  fwrite((void *)&header->vers, sizeof(Aword), 1, savfil);
  fwrite((void *)advnam, strlen(advnam)+1, 1, savfil);
  /* Save current values */
  fwrite((void *)&current, sizeof(current), 1, savfil);

  /* Save admin about each instance and its attributes */
  for (i = 1; i <= header->instanceMax; i++) {
    fwrite((void *)&admin[i], sizeof(AdminEntry), 1, savfil);
    if (instance[i].attributes != 0)
      for (atr = (AttributeEntry *) pointerTo(instance[i].attributes); !endOfTable(atr); atr++)
	fwrite((void *)&atr->value, sizeof(Aword), 1, savfil);
  }

  /* Save the event queue */
  eventQueue[etop].time = 0;        /* Mark the top */
  fwrite((void *)&eventQueue[0], sizeof(eventQueue[0]), etop+1, savfil);

  /* Save scores */
  for (i = 0; scores[i] != EOF; i++)
    fwrite((void *)&scores[i], sizeof(Aword), 1, savfil);

  fclose(savfil);
}


/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
void restore(void)
#else
void restore()
#endif
{
  int i;
  FILE *savfil;
  char str[256];
  AttributeEntry *atr;
  char savedVersion[4];
  char savedName[256];

  /* First save ? */
  if (savfnm[0] == '\0') {
    strcpy(savfnm, advnam);
    strcat(savfnm, ".sav");
  }
  prmsg(M_RESTOREFROM);
  sprintf(str, "(%s) : ", savfnm);
  output(str);
#ifdef USE_READLINE
  readline(str);
#else
  gets(str);
#endif
  if (str[0] == '\0')
    strcpy(str, savfnm);
  col = 1;
  if (str[0] == '\0')
    strcpy(str, savfnm);        /* Use the name temporarily */
  if ((savfil = fopen(str, READ_MODE)) == NULL)
    error(M_SAVEMISSING);
  strcpy(savfnm, str);          /* Save it for future use */

  fread((void *)&savedVersion, sizeof(Aword), 1, savfil);
  /* 4f - save file version check doesn't seem to work on PC's! */
  if (strncmp(savedVersion, header->vers, 4)) {
    fclose(savfil);
    error(M_SAVEVERS);
    return;
  }
  i = 0;
  while ((savedName[i++] = fgetc(savfil)) != '\0');
  if (strcmp(savedName, advnam) != 0) {
    fclose(savfil);
    error(M_SAVENAME);
    return;
  }

  /* Restore current values */
  fread((void *)&current, sizeof(current), 1, savfil);

  /* Restore admin and attributes for instances */
  for (i = 1; i <= header->instanceMax; i++) {
    fread((void *)&admin[i], sizeof(AdminEntry), 1, savfil);
    if (instance[i].attributes != 0)
      for (atr = (AttributeEntry *) pointerTo(instance[i].attributes); !endOfTable(atr); atr++)
	fread((void *)&atr->value, sizeof(Aword), 1, savfil);
  }

  /* Restore the eventQueue */
  etop = 0;
  do {
    fread((void *)&eventQueue[etop], sizeof(eventQueue[0]), 1, savfil);
    etop++;
  } while (eventQueue[etop-1].time != 0);
  etop--;

  /* Restore scores */
  for (i = 0; scores[i] != EOF; i++)
    fread((void *)&scores[i], sizeof(Aword), 1, savfil);

  fclose(savfil);
}



/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
Aword rnd(Aword from, Aword to)
#else
Aword rnd(from, to)
     Aword from, to;
#endif
{
  if (to == from)
    return to;
  else if (to > from)
    return (rand()/10)%(to-from+1)+from;
  else
    return (rand()/10)%(from-to+1)+to;
}



/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
Abool btw(Aint val, Aint low, Aint high)
#else
Abool btw(val, low, high)
     Aint val, low, high;
#endif
{
  if (high > low)
    return low <= val && val <= high;
  else
    return high <= val && val <= low;
}



/*----------------------------------------------------------------------*/
#ifdef _PROTOTYPES_
Aword contains(Aword string, Aword substring)
#else
Aword contains(string, substring)
     Aword string, substring;;
#endif
{
  Abool found;

  strlow((char *)string);
  strlow((char *)substring);

  found = (strstr((char *)string, (char *)substring) != 0);

  free((char *)string);
  free((char *)substring);

  return(found);
}



/*----------------------------------------------------------------------

  streq()

  Compare two strings approximately, ignore case

  */
#ifdef _PROTOTYPES_
Abool streq(char a[], char b[])
#else
Abool streq(a, b)
     char a[], b[];             /* IN - Strings to compare */
#endif
{
  Boolean eq;

  strlow(a);
  strlow(b);

  eq = (strcmp(a, b) == 0);

  free(a);
  free(b);

  return(eq);
}
