/*----------------------------------------------------------------------*\

  state.c

  State and undo manager unit of Alan interpreter

\*----------------------------------------------------------------------*/

#include <string.h>

#include "sysdep.h"
#include "types.h"
#include "syserr.h"
#include "main.h"
#include "set.h"
#include "exe.h"
#include "parse.h"
#include "StateStack.h"


/* PUBLIC DATA */
Bool gameStateChanged = FALSE;


/* PRIVATE TYPES */
typedef struct GameState {
  /* Event queue */
  EventQueueEntry *eventQueue;
  int eventQueueTop;		/* Event queue top pointer */

  /* Scores */
  int score;
  Aword *scores;		/* Score table pointer */

  /* Instance data */
  AdminEntry *admin;		/* Administrative data about instances */
  AttributeEntry *attributes;	/* Attributes data area */
  /* Sets and strings are dynamically allocated areas for which the
     attribute is just a pointer to. So they are not catched by the
     saving of attributes, instead they require special storage */
  Aword *sets;			/* Array of set pointers */
  Aword *strings;		/* Array of string pointers */
} GameState;

/* PRIVATE DATA */
static GameState gameState;
static StateStack stateStack = NULL;

static char *playerCommand;


/*----------------------------------------------------------------------*/
static void freeGameState() {

  free(gameState.admin);
  free(gameState.attributes);

  if (gameState.eventQueueTop > 0) {
    free(gameState.eventQueue);
    gameState.eventQueue = NULL;
  }
  free(gameState.scores);

  memset(&gameState, 0, sizeof(GameState));
}


/*======================================================================*/
void forgetGameState(void) {
	char *playerCommand;
	popGameState(stateStack, &gameState, &playerCommand);
	freeGameState();
	free(playerCommand);
}


/*======================================================================*/
void initStateStack() {
	if (stateStack != NULL)
		deleteStateStack(stateStack);
	stateStack = createStateStack(sizeof(GameState));
}


/*======================================================================*/
Bool anySavedState(void) {
  return !stateStackIsEmpty(stateStack);
}


/*----------------------------------------------------------------------*/
static int setCount() {
  SetInitEntry *entry;
  int count = 0;

  if (header->setInitTable != 0)
    for (entry = pointerTo(header->setInitTable); *(Aword *)entry != EOF; entry++)
      count++;
  return(count);
}


/*----------------------------------------------------------------------*/
static Aword *collectSets() {
	// TODO Why is this Aword*? shouldn't it be void**? Track it to getSetAttribute()
	SetInitEntry *entry;
	int count = setCount();
	Aword *sets;
	int i;

	if (count == 0) return NULL;

	sets = allocate(count*sizeof(Set));

	entry = pointerTo(header->setInitTable);
	for (i = 0; i < count; i++)
		sets[i] = getSetAttribute(entry[i].instanceCode, entry[i].attributeCode);

	return sets;
}


/*----------------------------------------------------------------------*/
static int stringCount() {
  StringInitEntry *entry;
  int count = 0;

  if (header->stringInitTable != 0)
    for (entry = pointerTo(header->stringInitTable); *(Aword *)entry != EOF; entry++)
      count++;
  return(count);
}


/*----------------------------------------------------------------------*/
static Aword *collectStrings() {
	// TODO Why is this Aword*? shouldn't it be void**? Track it to getSetAttribute()
	StringInitEntry *entry;
	int count = stringCount();
	Aword *strings;
	int i;

	if (count == 0) return NULL;

	strings = allocate(count*sizeof(Set));

	entry = pointerTo(header->stringInitTable);
	for (i = 0; i < count; i++)
		strings[i] = getStringAttribute(entry[i].instanceCode, entry[i].attributeCode);

	return strings;
}


/*======================================================================*/
void rememberCommands(void) {
	char *command = playerWordsAsCommandString();
	attachPlayerCommandsToLastState(stateStack, command);
	free(command);
}


/*----------------------------------------------------------------------*/
static void collectEvents() {
  gameState.eventQueueTop = eventQueueTop;
  if (eventQueueTop > 0)
    gameState.eventQueue = duplicate(eventQueue, eventQueueTop*sizeof(EventQueueEntry));
}


/*----------------------------------------------------------------------*/
static void collectInstanceData() {
  gameState.admin = duplicate(admin, (header->instanceMax+1)*sizeof(AdminEntry));
  gameState.attributes = duplicate(attributes, header->attributesAreaSize*sizeof(Aword));
  gameState.sets = collectSets();
  gameState.strings = collectStrings();
}


/*----------------------------------------------------------------------*/
static void collectScores() {
  gameState.score = current.score;
  gameState.scores = duplicate(scores, header->scoreCount*sizeof(Aword));
}


/*======================================================================*/
void rememberGameState(void) {
	collectEvents();
	collectInstanceData();
	collectScores();

	if (stateStack == NULL)
		initStateStack();

	pushGameState(stateStack, &gameState);
	gameStateChanged = FALSE;
}


/*----------------------------------------------------------------------*/
static void freeSetAttributes(void) {
  SetInitEntry *entry;

  if (header->setInitTable == 0) return;
  for (entry = pointerTo(header->setInitTable); *(Aword *)entry != EOF; entry++) {
    Aword attributeValue = getAttribute(admin[entry->instanceCode].attributes, entry->attributeCode);
    freeSet((Set*)attributeValue);
  }
}


/*----------------------------------------------------------------------*/
static void recallSets(Aword *sets) {
  SetInitEntry *entry;
  int count = setCount();
  int i;

  if (header->setInitTable == 0) return;

  entry = pointerTo(header->setInitTable);
  for (i = 0; i < count; i++)
    setAttribute(admin[entry[i].instanceCode].attributes, entry[i].attributeCode, sets[i]);
}


/*----------------------------------------------------------------------*/
static void freeStringAttributes(void) {
  StringInitEntry *entry;

  if (header->stringInitTable == 0) return;
  for (entry = pointerTo(header->stringInitTable); *(Aword *)entry != EOF; entry++) {
    Aword attributeValue = getAttribute(admin[entry->instanceCode].attributes, entry->attributeCode);
    free((char*)attributeValue);
  }
}


/*----------------------------------------------------------------------*/
static void recallStrings(Aword *strings) {
  StringInitEntry *entry;
  int count = stringCount();
  int i;

  if (header->stringInitTable == 0) return;

  entry = pointerTo(header->stringInitTable);
  for (i = 0; i < count; i++)
    setAttribute(admin[entry[i].instanceCode].attributes, entry[i].attributeCode, strings[i]);
}


/*----------------------------------------------------------------------*/
static void recallEvents() {
  eventQueueTop = gameState.eventQueueTop;
  if (eventQueueTop > 0) {
    memcpy(eventQueue, gameState.eventQueue,
	   (eventQueueTop+1)*sizeof(EventQueueEntry));
  }
}


/*----------------------------------------------------------------------*/
static void recallInstances() {

  if (admin == NULL) syserr("admin[] == NULL in recallInstances()");

  memcpy(admin, gameState.admin,
 	 (header->instanceMax+1)*sizeof(AdminEntry));

  freeSetAttributes();		/* Need to free previous set values */
  freeStringAttributes();	/* Need to free previous string values */

  memcpy(attributes, gameState.attributes,
	 header->attributesAreaSize*sizeof(Aword));

  recallSets(gameState.sets);
  recallStrings(gameState.strings);
}


/*----------------------------------------------------------------------*/
static void recallScores() {
  current.score = gameState.score;
  memcpy(scores, gameState.scores,
	 header->scoreCount*sizeof(Aword));
}


/*======================================================================*/
void recallGameState(void) {
	popGameState(stateStack, &gameState, &playerCommand);
	recallEvents();
	recallInstances();
	recallScores();
	freeGameState();
}


/*======================================================================*/
char *recreatePlayerCommand() {
  return playerCommand;
}
