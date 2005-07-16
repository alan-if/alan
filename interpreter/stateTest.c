/*======================================================================*\

  stateTest.c

  Unit tests for state module in the Alan interpreter

\*======================================================================*/

#include "set.h"
#include "state.c"

static void testPushGameState() {
  int instanceCount = 3;
  int adminSize = (instanceCount+1)*sizeof(AdminEntry)/sizeof(Aword);
  int attributeCount = 5;
  int attributeAreaSize = attributeCount*instanceCount*sizeof(AttributeEntry)/sizeof(Aword);
  int i;

  header = allocate(sizeof(ACodeHeader));
  header->instanceMax = instanceCount;
  admin = allocate(adminSize*sizeof(Aword));
  header->attributesAreaSize = attributeAreaSize;
  attributes = allocate(attributeAreaSize*sizeof(Aword));
  eventQueueTop = 0;

  for (i = 0; i < attributeAreaSize; i++)
    ((Aword *)attributes)[i] = i;
  for (i = 0; i < adminSize; i++)
    ((Aword *)admin)[i] = i;

  gameState = NULL;
  gameStateTop = 0;
  gameStateSize = 0;
  gameStateChanged = TRUE;

  pushGameState();

  ASSERT(gameState != NULL);
  ASSERT(gameStateTop == 1);
  ASSERT(memcmp(gameState->attributes, attributes, attributeAreaSize*sizeof(Aword)) == 0);
  ASSERT(memcmp(gameState->admin, admin, adminSize*sizeof(Aword)) == 0);
}

static void testPushPopGameStateWithSet() {
  int instanceCount = 1;
  int adminSize = (instanceCount+1)*sizeof(AdminEntry)/sizeof(Aword);
  int attributeCount = 1;
  int attributeAreaSize = attributeCount*instanceCount*sizeof(AttributeEntry)/sizeof(Aword);
  Set *originalSet = newSet(3);
  SetInitEntry *initEntry;

  header = allocate(sizeof(ACodeHeader));
  header->instanceMax = instanceCount;
  admin = allocate(adminSize*sizeof(Aword));
  header->attributesAreaSize = attributeAreaSize;
  attributes = allocate(attributeAreaSize*sizeof(Aword));
  admin[1].attributes = attributes;
  attributes[0].code = 1;
  attributes[0].value = (Aword)originalSet;
  addToSet(originalSet, 7);
  eventQueueTop = 0;

  /* Set up a set initialization */
  memory = allocate(sizeof(SetInitEntry)+2*sizeof(Aword));
  header->setInitTable = 1;
  initEntry = (SetInitEntry*)&memory[1];
  initEntry->instanceCode = 1;
  initEntry->attributeCode = 1;
  memory[1+sizeof(SetInitEntry)/sizeof(Aword)] = EOF;

  gameState = NULL;
  gameStateTop = 0;
  gameStateSize = 0;

  pushGameState();

  ASSERT(gameState->sets[0] != (Aword)originalSet);
  ASSERT(equalSets((Set*)gameState->sets[0], originalSet));

  Set *modifiedSet = newSet(4);
  attributes[0].value = (Aword)modifiedSet;
  addToSet(modifiedSet, 11);
  addToSet(modifiedSet, 12);
  ASSERT(!equalSets((Set*)gameState->sets[0], modifiedSet));
  ASSERT(equalSets((Set*)attributes[0].value, modifiedSet));

  popGameState();

  ASSERT(attributes[0].value != (Aword)modifiedSet);
  ASSERT(attributes[0].value != (Aword)originalSet);
  ASSERT(equalSets((Set*)attributes[0].value, originalSet));
}

static void testPopGameState() {
  int instanceMax = 2;
  attributes = allocate((instanceMax+1)*sizeof(AttributeEntry));
  admin = allocate((instanceMax+1)*sizeof(AdminEntry));

  header->attributesAreaSize = (instanceMax+1)*sizeof(AttributeEntry)/sizeof(Aword);
  header->instanceMax = instanceMax;

  gameState = NULL;
  gameStateTop = 0;
  gameStateSize = 0;
  attributes[0].value = 12;
  attributes[2].value = 3;
  gameStateChanged = TRUE;

  pushGameState();

  ASSERT(gameState != NULL);
  ASSERT(gameStateTop == 1);

  eventQueue = allocate(5*sizeof(EventQueueEntry));
  eventQueueTop = 2;
  eventQueue[1].time = 47;
  attributes[0].value = 11;
  attributes[2].value = 4;
  admin[2].location = 12;
  admin[2].alreadyDescribed = 2;
  admin[2].visitsCount = 13;
  admin[2].script = 33;
  admin[2].step = 3886;
  admin[2].waitCount = 38869878;

  gameStateChanged = TRUE;
  pushGameState();

  admin[2].location = 55;
  admin[2].alreadyDescribed = 55;
  admin[2].visitsCount = 55;
  admin[2].script = 55;
  admin[2].step = 55;
  admin[2].waitCount = 55;

  eventQueueTop = 0;
  eventQueue[1].time = 1;
  popGameState();
  ASSERT(admin[2].location == 12);
  ASSERT(admin[2].alreadyDescribed == 2);
  ASSERT(admin[2].visitsCount == 13);
  ASSERT(admin[2].script == 33);
  ASSERT(admin[2].step == 3886);
  ASSERT(admin[2].waitCount == 38869878);

  ASSERT(eventQueueTop == 2);
  ASSERT(eventQueue[1].time == 47);

  popGameState();

  ASSERT(attributes[0].value == 12);
  ASSERT(attributes[2].value == 3);
}

static void testPopEvents() {
  eventQueue = NULL;
  eventQueueTop = 0;
  gameState = allocate(sizeof(GameState));
  gameStateSize = 1;
  gameStateTop = 0;
  popEvents();
}

static void testRememberCommand() {
  int i;
  playerWords[0] = EOF;
  for (i = 0; i < 4; i++)
    playerWords[i] = i;
  playerWords[4] = EOF;

  gameStateTop = 1;
  gameState = allocate(sizeof(GameState));

  firstWord = 0;
  lastWord = 3;
  rememberCommands();

  for (i = 0; i < 4; i++)
    ASSERT(gameState[0].playerCommandWords[i] == playerWords[i]);
  ASSERT(gameState[0].playerCommandWords[4] == EOF);	   
}

static void testRecreatePlayerCommand() {
  static char *word[5] = {"w1","w2","w3","w4", NULL};	/* NOTE no strings longer than 3 chars!! */
  static Aint words[7] = {0,2,1,3,2,1,-1};
  int length = 0;
  int i;
  char *command = strdup("");
  char *recreatedCommand;

  dictionary = allocate(4*sizeof(DictionaryEntry));

  /* Calculate size of memory needed */
  for (i = 0; words[i] != -1; i++)
    length += strlen(word[words[i]]);
  memory = allocate((length+1)*sizeof(Aword)); /* Much too big... */

  /* Initialize the dictonary */
  for (i = 0; word[i] != NULL; i++) {
    strcpy((char *)&memory[1+i], word[i]);
    dictionary[i].string = 1+i;
  }

  /* Create the resulting command */
  for (i = 0; words[i] != -1; i++) {
    command = realloc(command, strlen(command)+strlen(word[words[i]])+2);
    if (i > 0)
      command = strcat(command, " ");
    command = strcat(command, word[words[i]]);
  }

  gameStateTop = 1;
  gameState = allocate(sizeof(GameState));
  gameState[0].playerCommandWords = allocate(7*sizeof(Aint));
  for (i = 0; words[i] != -1; i++)
    gameState[0].playerCommandWords[i] = words[i];
  gameState[0].playerCommandWords[i] = EOF;

  recreatedCommand = recreatePlayerCommand();

  ASSERT(strcmp(recreatedCommand, command) == 0);
}

void registerStateUnitTests() {
  registerUnitTest(testRememberCommand);
  registerUnitTest(testPushGameState);
  registerUnitTest(testPopEvents);
  registerUnitTest(testPopGameState);
  registerUnitTest(testRecreatePlayerCommand);
  registerUnitTest(testPushPopGameStateWithSet);
}
