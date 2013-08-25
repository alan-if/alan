#include "cgreen/cgreen.h"

#include "StateStack.c"

#include "instance.h"
#include "event.h"

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

    /* List of word indices the player said, EOF terminated */
    char *playerCommand;
} GameState;



static StateStack stateStack;

Describe(StateStack);
BeforeEach(StateStack) {
    stateStack = createStateStack(sizeof(GameState));
}

AfterEach(StateStack) {
}

Ensure(StateStack, canSeeAnEmptyStateStack) {
    assert_true(stateStackIsEmpty(stateStack));
}

Ensure(StateStack, extendsANonAllocatedStack) {
    GameState gameState;

    assert_equal(stateStack->stackSize, 0);
    pushGameState(stateStack, &gameState);
    assert_equal(stateStack->stackSize, EXTENT);
}

Ensure(StateStack, canPushAndPopAGameState) {
    GameState originalGameState;
    GameState poppedGameState;
    char *playerCommand;
    
    originalGameState.admin = (AdminEntry*) NEW(int);
    originalGameState.attributes = (AttributeEntry*) NEW(int);
    originalGameState.eventQueue = (EventQueueEntry*) NEW(int);

    assert_true(stateStackIsEmpty(stateStack));
    pushGameState(stateStack, &originalGameState);
    assert_false(stateStackIsEmpty(stateStack));
    assert_equal(stateStack->stackPointer, 1);
    
    popGameState(stateStack, &poppedGameState, &playerCommand);
    assert_equal(poppedGameState.admin, originalGameState.admin);
    assert_equal(poppedGameState.attributes, originalGameState.attributes);
    assert_equal(poppedGameState.eventQueue, originalGameState.eventQueue);
}

Ensure(StateStack, canPush100Times) {
    GameState gameState;
    int i;

    for (i = 0; i<100; i++)
        pushGameState(stateStack, &gameState);
}

Ensure(StateStack, canRememberPlayerCommands) {
    GameState gameState;
    char *expectedPlayerCommands = "some player commands";
    char *playerCommands;

    pushGameState(stateStack, &gameState);
    attachPlayerCommandsToLastState(stateStack, expectedPlayerCommands);

    popGameState(stateStack, &gameState, &playerCommands);
    assert_string_equal(playerCommands, expectedPlayerCommands);
}

Ensure(StateStack, pushClearsPlayerCommand) {
    GameState gameState;
    pushGameState(stateStack, &gameState);
    assert_equal(NULL, stateStack->playerCommands[stateStack->stackPointer-1]);
}

static bool syserrCalled;

static void syserrHandler(char *message) {
    syserrCalled = TRUE;
}

Ensure(StateStack, willGenerateSyserrorWhenPoppingFromEmptyStack) {
    GameState gameState;
    char *playerCommand;

    syserrCalled = FALSE;
    setSyserrHandler(syserrHandler);

    popGameState(stateStack, &gameState, &playerCommand);

    assert_true(syserrCalled);
}

TestSuite *stateStackTests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, StateStack, canSeeAnEmptyStateStack);
    add_test_with_context(suite, StateStack, extendsANonAllocatedStack);
    add_test_with_context(suite, StateStack, canPushAndPopAGameState);
    add_test_with_context(suite, StateStack, canPush100Times);
    add_test_with_context(suite, StateStack, pushClearsPlayerCommand);
    add_test_with_context(suite, StateStack, willGenerateSyserrorWhenPoppingFromEmptyStack);
    add_test_with_context(suite, StateStack, canRememberPlayerCommands);

    return suite;
}
