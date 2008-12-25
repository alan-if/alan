#include "StateStack.h"

/* IMPORTS: */
#include "main.h"
#include "syserr.h"

/* CONSTANTS: */
#define EXTENT 10


/* PRIVATE TYPES: */
typedef struct StateStackStructure {
	void **stack;
	char **playerCommands;
	int stackSize;
	int stackPointer;	/* Points above used stack, 0 initially */
	int elementSize;	/* Size of elements in the stack */
} StateStackStructure;


/*======================================================================*/
StateStack createStateStack(int elementSize) {
	StateStack stack = NEW(StateStackStructure);
	stack->stackSize = 0;
	stack->stackPointer = 0;
	stack->elementSize = elementSize;
	return stack;
}


/*======================================================================*/
void deleteStateStack(StateStack stateStack) {
	while (stateStack->stackPointer >0)
		free(stateStack->stack[--stateStack->stackPointer]);
	free(stateStack->stack);
	free(stateStack);
}


/*======================================================================*/
Bool stateStackIsEmpty(StateStack stateStack) {
	return stateStack->stackPointer == 0;
}


/*----------------------------------------------------------------------*/
static void *reallocateStack(void *from, int newSize)
{
	void * newArea = realloc(from, newSize);
    if (newArea == NULL)
    	syserr("Out of memory in 'ensureSpaceForGameState()'");
    return newArea;
}

/*----------------------------------------------------------------------*/
static void ensureSpaceForGameState(StateStack stack)
{
    if (stack->stackPointer == stack->stackSize) {
    	stack->stack = reallocateStack(stack->stack, (stack->stackSize+EXTENT)*sizeof(void*));
    	stack->playerCommands = reallocateStack(stack->playerCommands, (stack->stackSize+EXTENT)*sizeof(char*));
    	stack->stackSize += EXTENT;
    }
}


/*======================================================================*/
void pushGameState(StateStack stateStack, void *gameState) {
	void *element = allocate(stateStack->elementSize);
	memcpy(element, gameState, stateStack->elementSize);
    ensureSpaceForGameState(stateStack);
    stateStack->playerCommands[stateStack->stackPointer] = NULL;
	//printf("Push:StateStackPointer = %d, StateStack[StateStackPointer] = %p\n", stateStack->stackPointer, stateStack->stack[stateStack->stackPointer]);
    stateStack->stack[stateStack->stackPointer++] = element;
	//printf("Push:StateStackPointer = %d, StateStack[%d] = %p\n", stateStack->stackPointer, stateStack->stackPointer, stateStack->stack[stateStack->stackPointer-1]);
}


/*======================================================================*/
void attachPlayerCommandsToLastState(StateStack stateStack, char *playerCommands) {
	stateStack->playerCommands[stateStack->stackPointer-1] = strdup(playerCommands);
}


/*======================================================================*/
void popGameState(StateStack stateStack, void *gameState, char** playerCommand) {
	if (stateStack->stackPointer == 0)
		syserr("Popping GameState from empty stack");
	else {
		stateStack->stackPointer--;
		//printf("Pop:StateStackPointer = %d, StateStack[StateStackPointer] = %p\n", stateStack->stackPointer, stateStack->stack[stateStack->stackPointer]);
		memcpy(gameState, stateStack->stack[stateStack->stackPointer], stateStack->elementSize);
		free(stateStack->stack[stateStack->stackPointer]);
		*playerCommand = stateStack->playerCommands[stateStack->stackPointer];
	}
}
