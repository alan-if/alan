#include "cgreen.h"


#include "state.c"


static void setupInstances(int instanceMax, int attributeCount) {
  int adminSize = (instanceMax+1)*sizeof(AdminEntry)/sizeof(Aword);
  int attributeAreaSize = (instanceMax+1)*attributeCount*sizeof(AttributeEntry)/sizeof(Aword);
  int i;

  header = allocate(sizeof(ACodeHeader));
  header->attributesAreaSize = attributeAreaSize;
  header->instanceMax = instanceMax;

  admin = allocate((instanceMax+1)*sizeof(AdminEntry));
  for (i = 0; i < adminSize; i++) ((Aword *)admin)[i] = i;

  attributes = allocate((instanceMax+1)*attributeCount*sizeof(AttributeEntry));
  for (i = 0; i < attributeAreaSize; i++) ((Aword *)attributes)[i] = i;

}

static void buildGameStateStack(int depth, int attributeCount) {
  int i;

  if (depth == 0)
    gameState = NULL;
  else {
    gameState = allocate(depth*sizeof(GameState));
    for (i = 0; i<depth; i++) {
      gameState[i].admin = allocate((header->instanceMax+1)*sizeof(AdminEntry));
      gameState[i].attributes = allocate((header->instanceMax+1)*attributeCount*sizeof(AttributeEntry));
    }
  }

  gameStateTop = depth;
  gameStateSize = depth;
}


static void testPushGameState() {
  int instanceCount = 3;
  int adminSize = (instanceCount+1)*sizeof(AdminEntry)/sizeof(Aword);
  int attributeCount = 5;
  int attributeAreaSize = attributeCount*instanceCount*sizeof(AttributeEntry)/sizeof(Aword);

  buildGameStateStack(0, instanceCount);
  setupInstances(instanceCount, attributeCount);

  eventQueueTop = 0;

  pushGameState();

  assert_not_equal(NULL, gameState);
  assert_equal(1, gameStateTop);
  assert_true(memcmp(gameState->attributes, attributes, attributeAreaSize*sizeof(Aword)) == 0);
  assert_true(memcmp(gameState->admin, admin, adminSize*sizeof(Aword)) == 0);
}


static void testPushAndPopAttributesWithSet() {
  int instanceCount = 1;
  int attributeCount = 1;
  Set *originalSet = newSet(3);
  SetInitEntry *initEntry;

  setupInstances(instanceCount, attributeCount);

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

  buildGameStateStack(0, instanceCount);

  pushGameState();

  assert_not_equal(gameState->sets[0], (Aword)originalSet);
  assert_true(equalSets((Set*)gameState->sets[0], originalSet));

  Set *modifiedSet = newSet(4);
  attributes[0].value = (Aword)modifiedSet;
  addToSet(modifiedSet, 11);
  addToSet(modifiedSet, 12);
  assert_false(equalSets((Set*)gameState->sets[0], modifiedSet));
  assert_true(equalSets((Set*)attributes[0].value, modifiedSet));

  popGameState();

  assert_not_equal(attributes[0].value, (Aword)modifiedSet);
  assert_not_equal(attributes[0].value, (Aword)originalSet);
  assert_true(equalSets((Set*)attributes[0].value, originalSet));
}

static void testPushAndPopAttributeState() {
  int instanceCount = 2;
  setupInstances(instanceCount, 3);
  buildGameStateStack(0, instanceCount);

  attributes[0].value = 12;
  attributes[2].value = 3;

  pushGameState();

  assert_not_equal(NULL, gameState);
  assert_equal(1, gameStateTop);

  attributes[0].value = 11;
  attributes[2].value = 4;

  pushGameState();

  attributes[0].value = 55;
  attributes[2].value = 55;

  popGameState();

  assert_equal(11, attributes[0].value);
  assert_equal(4, attributes[2].value);

  popGameState();

  assert_equal(12, attributes[0].value);
  assert_equal(3, attributes[2].value);
}



static void testPushAndPopAdminState() {
  int INSTANCE_COUNT = 2;
  int INSTANCE1_LOCATION = 12;
  int INSTANCE2_LOCATION = 22;
  int INSTANCE2_FIRST_SCRIPT = 3;

  setupInstances(INSTANCE_COUNT, 3);
  buildGameStateStack(0, INSTANCE_COUNT);

  admin[1].location = INSTANCE1_LOCATION;
  admin[2].script = INSTANCE2_FIRST_SCRIPT;

  pushGameState();

  assert_not_equal(NULL, gameState);
  assert_equal(1, gameStateTop);

  admin[2].location = INSTANCE2_LOCATION;
;
  admin[2].alreadyDescribed = 2;
  admin[2].visitsCount = 13;
  admin[2].script = 33;
  admin[2].step = 3886;
  admin[2].waitCount = 38869878;

  pushGameState();

  admin[2].location = 55;
  admin[2].alreadyDescribed = 55;
  admin[2].visitsCount = 55;
  admin[2].script = 55;
  admin[2].step = 55;
  admin[2].waitCount = 55;

  popGameState();

  assert_equal(INSTANCE2_LOCATION, admin[2].location);
  assert_equal(2, admin[2].alreadyDescribed);
  assert_equal(13, admin[2].visitsCount);
  assert_equal(33, admin[2].script);
  assert_equal(3886, admin[2].step);
  assert_equal(38869878, admin[2].waitCount);

  popGameState();

  assert_equal(INSTANCE1_LOCATION, admin[1].location);
  assert_equal(INSTANCE2_FIRST_SCRIPT, admin[2].script);
}

static void testPushAndPopEvents() {
  int instanceCount = 1;

  buildGameStateStack(1, instanceCount);
  setupInstances(1, 1);

  eventQueue = NULL;
  eventQueueTop = 0;

  pushGameState();

  eventQueue = allocate(5*sizeof(EventQueueEntry));
  eventQueueTop = 2;
  eventQueue[1].after = 47;

  pushGameState();

  eventQueueTop = 0;
  eventQueue[1].after = 1;

  popGameState();

  assert_equal(2, eventQueueTop);
  assert_equal(47, eventQueue[1].after);

  popGameState();

  assert_equal(0, eventQueueTop);
}

static void testRememberCommand() {
  int instanceCount = 1;
  int i;
  char *command = "n, w, e and south";

  playerWords[0].code = EOF;
  for (i = 0; i < 4; i++)
    playerWords[i].code = i;
  playerWords[4].code = EOF;

  buildGameStateStack(1, instanceCount);

  firstWord = 0;
  lastWord = 3;
  playerWords[firstWord].start = command;
  playerWords[lastWord].end = &command[4];
  rememberCommands();

  assert_true(strcmp(gameState[0].playerCommand, command) != 0);
  assert_true(strncmp(gameState[0].playerCommand, command, 3) == 0);
}

static void testUndoStackFreesMemory() {
  int instanceCount = 1;

  buildGameStateStack(1, instanceCount);
  gameState[0].admin = allocate(100);

  initStateStack();

  assert_equal(0, gameStateTop);
  assert_equal(NULL, gameState[0].admin);
  assert_equal(NULL, gameState[0].attributes);
}

TestSuite *stateTests() {
  TestSuite *suite = create_test_suite();
  add_test(suite, testUndoStackFreesMemory);
  add_test(suite, testRememberCommand);
  add_test(suite, testPushGameState);
  add_test(suite, testPushAndPopAttributeState);
  add_test(suite, testPushAndPopAdminState);
  add_test(suite, testPushAndPopEvents);
  add_test(suite, testPushAndPopAttributesWithSet);
  return suite;
}
