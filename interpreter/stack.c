/*----------------------------------------------------------------------*\

  STACK.C

  Stack Handling for Alan interpreter

\*----------------------------------------------------------------------*/

#include "stack.h"

#include "types.h"
#include "main.h"
#include "syserr.h"

/* ABSTRACT TYPE */
typedef struct StackStructure {
  Aint *stack;
  int stackSize;
  int stackp;
  int framePointer;
} StackStructure;


/* PRIVATE DATA */

#define STACKSIZE 100

/* The AMACHINE STACK */
static Aword stack[STACKSIZE];
static int stackp = 0;


/*======================================================================*/
Stack createStack(int size)
{
  StackStructure *theStack = NEW(StackStructure);

  theStack->stack = allocate(size*sizeof(Aword));
  theStack->stackSize = size;
  theStack->framePointer = -1;

  return theStack;
}


/*======================================================================*/
void deleteStack(Stack theStack)
{
  free(theStack->stack);
  free(theStack);
}


/*======================================================================*/
int stackDepth(Stack theStack) {
  return theStack->stackp;
}


/*======================================================================*/
void dumpStack(Stack theStack)
{
  int i;

  printf("[");
  if (theStack == NULL)
    for (i = 0; i < stackp; i++)
      printf("%ld ", stack[i]);
  else
    for (i = 0; i < theStack->stackp; i++)
      printf("%ld ", theStack->stack[i]);
  printf("]");
}


/*======================================================================*/
void push(Stack theStack, Aword i)
{
  if (theStack == NULL) {
    if (stackp == STACKSIZE)
      syserr("Out of stack space.");
    stack[stackp++] = i;
  } else {
    if (theStack->stackp == theStack->stackSize)
      syserr("Out of stack space.");
    theStack->stack[(theStack->stackp)++] = i;
  }    
}


/*======================================================================*/
Aword pop(Stack theStack)
{
  if (theStack == NULL) {
    if (stackp == 0)
      syserr("Stack underflow.");
    return(stack[--stackp]);
  } else {
    if (theStack->stackp == 0)
      syserr("Stack underflow.");
    return theStack->stack[--(theStack->stackp)];
  }
}


/*======================================================================*/
Aword top(Stack theStack)
{
  if (theStack == NULL)
    return(stack[stackp-1]);
  else
    return(theStack->stack[theStack->stackp-1]);
}


/* The AMACHINE Block Frames */
static int framePointer = -1;

/*======================================================================*/
void newFrame(Stack theStack, Aint noOfLocals)
{
  push(theStack, framePointer);
  if (theStack == NULL)
    framePointer = stackp;
  else
    theStack->framePointer = theStack->stackp;
  for (;noOfLocals > 0; noOfLocals--) push(theStack, 0);
}


/*======================================================================*/
/* Local variables are numbered 1 and up and stored on their index-1 */
Aword getLocal(Stack theStack, Aint framesBelow, Aint variableNumber)
{
  if (variableNumber < 1)
    syserr("Reading a non-existing block-local variable.");

  if (theStack == NULL) {
    int frame = framePointer;
    int frameCount;

    if (framesBelow != 0)
      for (frameCount = framesBelow; frameCount != 0; frameCount--)
	frame = stack[frame-1];

    return stack[frame + variableNumber-1];
  } else {
    int frame = theStack->framePointer;
    int frameCount;

    if (framesBelow != 0)
      for (frameCount = framesBelow; frameCount != 0; frameCount--)
	frame = theStack->stack[frame-1];

    return theStack->stack[frame + variableNumber-1];
  }
}


/*======================================================================*/
void setLocal(Stack theStack, Aint framesBelow, Aint variableNumber, Aword value)
{
  if (variableNumber < 1)
    syserr("Writing a non-existing block-local variable.");

  if (theStack == NULL) {
    int frame = framePointer;
    int frameCount;

    if (framesBelow != 0)
      for (frameCount = framesBelow; frameCount != 0; frameCount--)
	frame = stack[frame-1];

    stack[frame + variableNumber-1] = value;
  } else {
    int frame = theStack->framePointer;
    int frameCount;

    if (framesBelow != 0)
      for (frameCount = framesBelow; frameCount != 0; frameCount--)
	frame = theStack->stack[frame-1];

    theStack->stack[frame + variableNumber-1] = value;
  }
}

/*======================================================================*/
void endFrame(Stack theStack)
{
  if (theStack == NULL) {
    stackp = framePointer;
    framePointer = pop(NULL);
  } else {
    theStack->stackp = theStack->framePointer;
    theStack->framePointer = pop(theStack);
  }
}


