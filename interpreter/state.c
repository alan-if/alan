/*----------------------------------------------------------------------*\

  state.c

  State and undo manager unit of Alan interpreter

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"
#include "syserr.h"
#include "main.h"
#include "set.h"
#include "exe.h"
#include "parse.h"


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

  /* List of word indices the player said, EOF terminated */
  Aint *playerCommandWords;
} GameState;

/* PRIVATE DATA */
static int gameStateTop = 0;
static int gameStateSize = 0;
static GameState *gameState = NULL;



/*----------------------------------------------------------------------*/
static void ensureSpaceForGameState() {
  static int increment = 10;

  if (gameStateTop == gameStateSize) {
    gameState = realloc(gameState, (gameStateSize+increment)*sizeof(GameState));
    if (gameState == NULL) syserr("Out of memory in ensureSpaceForGameState()");
    gameStateSize += increment;
  }
}


/*----------------------------------------------------------------------*/
static Aword *pushSets() {
  SetInitEntry *entry;
  int setCount = 0;
  Aword *sets;
  int i;

  if (header->setInitTable == 0) return NULL;

  for (entry = pointerTo(header->setInitTable); *(Aword *)entry != EOF; entry++)
    setCount++;

  if (setCount == 0) return NULL;

  sets = allocate(setCount*sizeof(Set));

  entry = pointerTo(header->setInitTable);
  for (i = 0; i < setCount; i++)
    sets[i] = getSetAttribute(entry[i].instanceCode, entry[i].attributeCode);

  return sets;
}


/*======================================================================*/
void forgetGameState(void) {
  if (gameStateTop == 0) syserr("forgetting nonexisting gameState");
  gameStateTop--;
}


/*======================================================================*/
void rememberCommands(void) {
  int n;
  GameState *state = &gameState[gameStateTop-1];

  state->playerCommandWords = allocate((lastWord-firstWord+2)*sizeof(Aword));
  for (n = firstWord; n <= lastWord; n++) {
    state->playerCommandWords[n-firstWord] = playerWords[n];
  }
  state->playerCommandWords[n-firstWord] = EOF;
}


/*----------------------------------------------------------------------*/
static void pushEvents() {
  gameState[gameStateTop].eventQueueTop = eventQueueTop;
  if (eventQueueTop > 0)
    gameState[gameStateTop].eventQueue = duplicate(eventQueue, eventQueueTop*sizeof(EventQueueEntry));
}


/*----------------------------------------------------------------------*/
static void pushInstanceData() {
  gameState[gameStateTop].admin = duplicate(admin, (header->instanceMax+1)*sizeof(AdminEntry));
  gameState[gameStateTop].attributes = duplicate(attributes, header->attributesAreaSize*sizeof(Aword));
  gameState[gameStateTop].sets = pushSets();
}


/*----------------------------------------------------------------------*/
static void pushScores() {
  gameState[gameStateTop].score = current.score;
  gameState[gameStateTop].scores = duplicate(scores, header->scoreCount*sizeof(Aword));
}


/*======================================================================*/
void pushGameState(void) {

  ensureSpaceForGameState();
  pushEvents();
  pushInstanceData();
  pushScores();
  gameStateTop++;
  gameStateChanged = FALSE;
}
  
  
/*----------------------------------------------------------------------*/
static void freeSets(void) {
  SetInitEntry *entry;

  if (header->setInitTable == 0) return;
  for (entry = pointerTo(header->setInitTable); *(Aword *)entry != EOF; entry++) {
    Aword attributeValue = getAttribute(admin[entry->instanceCode].attributes, entry->attributeCode);
    freeSet((Set*)attributeValue);
  }
}

/*----------------------------------------------------------------------*/
static void popSets(Aword *sets) {
  SetInitEntry *entry;
  int setCount = 0;
  int i;

  if (header->setInitTable == 0) return;

  for (entry = pointerTo(header->setInitTable); *(Aword *)entry != EOF; entry++)
    setCount++;

  if (setCount == 0) return;

  entry = pointerTo(header->setInitTable);
  for (i = 0; i < setCount; i++)
    setAttribute(admin[entry[i].instanceCode].attributes, entry[i].attributeCode, sets[i]);
}


/*----------------------------------------------------------------------*/
static void popEvents() {
  eventQueueTop = gameState[gameStateTop].eventQueueTop;
  if (eventQueueTop > 0) {
    memcpy(eventQueue, gameState[gameStateTop].eventQueue,
	   (eventQueueTop+1)*sizeof(EventQueueEntry));
    free(gameState[gameStateTop].eventQueue);
  }
}


/*----------------------------------------------------------------------*/
static void popInstances() {
  if (admin == NULL) syserr("admin[] == NULL in popGameState()");
  memcpy(admin, gameState[gameStateTop].admin,
 	 (header->instanceMax+1)*sizeof(AdminEntry));
  free(gameState[gameStateTop].admin);
  freeSets();			/* Need to free previous set values */
  if (attributes == NULL) syserr("attributes[] == NULL in popGameState");
  memcpy(attributes, gameState[gameStateTop].attributes,
	 header->attributesAreaSize*sizeof(Aword));
  free(gameState[gameStateTop].attributes);
  popSets(gameState[gameStateTop].sets);
}


/*----------------------------------------------------------------------*/
static void popScores() {
  current.score = gameState[gameStateTop].score;
  memcpy(scores, gameState[gameStateTop].scores,
	 header->scoreCount*sizeof(Aword));
  free(gameState[gameStateTop].scores);
}


/*======================================================================*/
Bool popGameState(void) {
  if (gameStateTop == 0) return FALSE;
  gameStateTop--;
  popEvents();
  popInstances();
  popScores();
  free(gameState[gameStateTop].playerCommandWords);
  gameState[gameStateTop].playerCommandWords = NULL;
  return TRUE;
}

/*----------------------------------------------------------------------*/
static char *recreatePlayerCommand() {
  int i;
  char *words = strdup("");
  GameState *state = &gameState[gameStateTop-1];

  for (i = 0; state->playerCommandWords[i] != EOF; i++) {
    int wordIndex = state->playerCommandWords[i];
    char *word = (char*)pointerTo(dictionary[wordIndex].string);
    words = realloc(words, strlen(words) + strlen(word) + 2);
    if (i > 0) words = strcat(words, " ");
    words = strcat(words, word);
  }
  return(words);
}


/*======================================================================*/
void undo(void) {
  gameStateTop--;
  if (gameStateTop > 0) {
    char *words;
    words = recreatePlayerCommand();
    popGameState();
    setupParameterForString(1, words);
    printMessage(M_UNDONE);
  } else
    printMessage(M_NO_UNDO);
  forceNewPlayerInput();
  longjmp(errorLabel, 2);	/* UNDO return value */
}
