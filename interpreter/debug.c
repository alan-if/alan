/*----------------------------------------------------------------------*\

  debug.c

  Debugger unit in Alan interpreter ARUN

\*----------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>

#include "types.h"
#ifdef HAVE_SHORT_FILENAMES
#include "av.h"
#else
#include "alan.version.h"
#endif


#ifdef USE_READLINE
#include "readline.h"
#endif

#include "types.h"
#include "inter.h"
#include "main.h"
#include "parse.h"
#include "stack.h"
#include "exe.h"

#include "debug.h"

#ifdef GLK
#include "glkio.h"
#endif


/* PUBLIC: */
int breakpointCount = 0;
Breakpoint breakpoint[BREAKPOINTMAX];





/*----------------------------------------------------------------------*/
static void showAttributes(Aword atradr)
{
  AttributeEntry *at;
  int i;
  char str[80];

  if (atradr == 0) return;

  i = 1;
  for (at = (AttributeEntry *) pointerTo(atradr); !endOfTable(at); at++) {
    sprintf(str, "$i$t%s(%ld) = %ld", (char *) pointerTo(at->stringAddress), at->code, at->value);
#if ISO == 0
    fromIso(str, str);
#endif
    output(str);
    i++;
  }
}


/*----------------------------------------------------------------------*/
static void showContents(int cnt)
{
  int i;
  char str[80];
  Abool found;

  output("$iContains:");
  for (i = 1; i <= header->instanceMax; i++) {
    if (in(i, cnt)) { /* Yes, it's in this container */
      if (!found)
	found = TRUE;
      output("$i$t");
      say(i);
      sprintf(str, "(%d) ", i);
      output(str);
    }
  }
  if (!found)
    output("nothing");
}



/*----------------------------------------------------------------------*/
static void showInstances(void)
{
  char str[80];
  int ins;

  output("Instances:");
  for (ins = 1; ins <= header->instanceMax; ins++) {
    sprintf(str, "$i%3d: ", ins);
    output(str);
    say(ins);
    if (instance[ins].container)
      output("(container)");
  }
}


/*----------------------------------------------------------------------*/
static void showInstance(int ins)
{
  char str[80];

  if (ins > header->instanceMax) {
    sprintf(str, "Instance code %d is out of range", ins);
    output(str);
    return;
  }

  output("The $$");
  say(ins);
  sprintf(str, "$$ (%d)", ins);
  output(str);
  if (instance[ins].parent) {
    sprintf(str, "Isa %s", (char *)pointerTo(class[instance[ins].parent].idAddress));
    output(str);
  }

  if (!isA(ins, header->locationClassId)) {
    sprintf(str, "$iLocation: ");
    output(str);
    if (admin[ins].location == 0)
      output("- (0)");
    else if (isLoc(admin[ins].location)) {
      say(admin[ins].location);
      sprintf(str, "(%ld)", where(ins));
      output(str);
    } else if (isCnt(admin[ins].location)) {

      if (isObj(admin[ins].location))
	output("in");
      else if (isAct(admin[ins].location))
	output("carried by");
      say(admin[ins].location);
      sprintf(str, "(%ld)", admin[ins].location);
      output(str);

    } else
      output("Illegal location!");
  }

  output("$iAttributes:");
  showAttributes(instance[ins].attributes);

  if (instance[ins].container)
    showContents(ins);
      
  if (isA(ins, header->actorClassId)) {
    if (admin[ins].script == 0)
      output("$iIs idle");
    else {
      sprintf(str, "$iExecuting script: %ld, Step: %ld", admin[ins].script, admin[ins].step);
      output(str);
    }
  }
}


/*----------------------------------------------------------------------*/
static void showObjects(void)
{
  char str[80];
  int obj;

  output("Objects:");
  for (obj = 1; obj <= header->instanceMax; obj++) {
    if (isObj(obj)) {
      sprintf(str, "$i%3d: ", obj);
      output(str);
      say(obj);
    }
  }
}


/*----------------------------------------------------------------------*/
static void showObject(int obj)
{
  char str[80];


  if (!isObj(obj)) {
    sprintf(str, "Instance %d is not an object", obj);
    output(str);
    return;
  }

  showInstance(obj);

}


/*----------------------------------------------------------------------*/
static void showcnts(void)
{
  char str[80];
  int cnt;

  output("Containers:");
  for (cnt = 1; cnt <= header->containerMax; cnt++) {
    sprintf(str, "$i%3d: ", cnt);
    output(str);
    if (container[cnt].owner != 0)
      say(container[cnt].owner);
  }

}


/*----------------------------------------------------------------------*/
static void showClass(int c)
{
  char str[80];

  output("$t");
  output((char *)pointerTo(class[c].idAddress));
  sprintf(str, "(%d)", c);
  output(str);
  if (class[c].parent != 0) {
    output("Isa");
    output((char *)pointerTo(class[class[c].parent].idAddress));
    sprintf(str, "(%ld)", class[c].parent);
    output(str);
  }
}


/*----------------------------------------------------------------------*/
static void showClasses(void)
{
  int c;

  output("Classes:");
  for (c = 1; c <= header->classMax; c++) {
    output("$n");
    showClass(c);
  }
}


/*----------------------------------------------------------------------*/
static void showContainer(int cnt)
{
  char str[80];

  if (cnt < 1 || cnt > header->containerMax) {
    sprintf(str, "Container number out of range. Between 1 and %ld, please.", header->containerMax);
    output(str);
    return;
  }

  sprintf(str, "Container %d :", cnt);
  output(str);
  if (container[cnt].owner != 0) {
    cnt = container[cnt].owner;
    say(cnt);
    sprintf(str, "$iLocation: %ld", where(cnt));
    output(str);
  }
  showContents(cnt);
}


/*----------------------------------------------------------------------*/
static void showLocations(void)
{
  char str[80];
  int loc;

  output("Locations:");
  for (loc = 1; loc <= header->instanceMax; loc++) {
    if (isLoc(loc)) {
      sprintf(str, "$i%3d: ", loc);
      output(str);
      say(loc);
    }
  }
}


/*----------------------------------------------------------------------*/
static void showLocation(int loc)
{
  char str[80];


  if (!isLoc(loc)) {
    sprintf(str, "Instance %d is not a location.", loc);
    output(str);
    return;
  }

  output("The ");
  say(loc);
  sprintf(str, "(%d) Isa location :", loc);
  output(str);

  output("$iAttributes =");
  showAttributes(instance[loc].attributes);
}


/*----------------------------------------------------------------------*/
static void showActors(void)
{
  char str[80];
  int act;

  output("Actors:");
  for (act = 1; act <= header->instanceMax; act++) {
    if (isAct(act)) {
      output("$i");
      say(act);
      sprintf(str, "(%d)", act);
      output(str);
      if (instance[act].container)
	output("(container)");
    }
  }
}


/*----------------------------------------------------------------------*/
static void showActor(int act)
{
  char str[80];
  
  if (!isAct(act)) {
    sprintf(str, "Instance %d is not an actor.", act);
    output(str);
    return;
  }
  
  showInstance(act);
}


/*----------------------------------------------------------------------*/
static void showEvents(void)
{
  int event, i;
  char str[80];
  Boolean scheduled;

  output("EVENTS:");
  for (event = 1; event <= header->eventMax; event++) {
    sprintf(str, "$i%d (%s):", event, (char *)pointerTo(events[event].stringAddress));
#if ISO == 0
    fromIso(str, str);
#endif
    output(str);
    scheduled = FALSE;
    for (i = 0; i < eventQueueTop; i++)
      if ((scheduled = (eventQueue[i].event == event)))
	break;
    if (scheduled) {
      sprintf(str, "Scheduled for +%d, at ", eventQueue[i].time-current.tick);
      output(str);
      say(eventQueue[i].where);
    } else
      output("Not scheduled.");
  }
}



/*----------------------------------------------------------------------*/
static char *sourceFileName(int fileNumber) {
  SourceFileEntry *entries = pointerTo(header->sourceFileTable);

  getStringFromFile(entries[fileNumber].fpos, entries[fileNumber].len);
  return (char *)pop();
}


/*----------------------------------------------------------------------*/
static void showSourceLine(int line, int fileNumber) {
  FILE *sourceFile;
  int count;
#define SOURCELINELENGTH 100
static char buffer[SOURCELINELENGTH];

  sourceFile  = fopen(sourceFileName(fileNumber), "r");
  if (sourceFile != NULL) {
    for (count = 0; count < line; count++) {
      if (fgets(buffer, SOURCELINELENGTH, sourceFile) == NULL)
	return;
      /* Make sure we have read to end of line */
      if (strchr(buffer, '\n') == NULL)
	while (fgetc(sourceFile) != '\n');
    }
    printf("abug> %s", buffer);
    if (buffer[strlen(buffer)-1] != '\n')
      printf("\n");
  }
}


/*----------------------------------------------------------------------*/
static void listBreakpoints() {
  int i;
  Boolean found = FALSE;

  for (i = 0; i < BREAKPOINTMAX; i++)
    if (breakpoint[i].line != 0) {
      if (!found)
	printf("Breakpoints set:\n");
      found = TRUE;
      printf("\t%d in '%s'\n", breakpoint[i].line, sourceFileName(breakpoint[i].file));
    }
  if (!found)
    printf("No breakpoints set.\n");
}


/*======================================================================*/
Boolean breakpointIndex(int line) {
  int i;

  for (i = 0; i < BREAKPOINTMAX; i++)
    if (breakpoint[i].line == line) {
      breakpoint[i].line = line;
      return i;
    }
  return(-1);
}


static int availableBreakpointSlot() {
  int i;
  for (i = 0; i < BREAKPOINTMAX; i++)
    if (breakpoint[i].line == 0) {
      return i;
    }
  return -1;
}



/*----------------------------------------------------------------------*/
static void setBreakpoint(int line, int file) {
  /* FIXME - we can only set breakpoints in main file */
  int i = breakpointIndex(line);

  if (i != -1)
    printf("Breakpoint already set at line %d.\n", line);
  else {
    i = availableBreakpointSlot();
    if (i == -1)
      printf("No room for more breakpoints. Delete one first.\n");
    else {
      breakpoint[i].line = line;
      breakpoint[i].file = file;
      printf("Breakpoint set at line %d in '%s'.\n", line, sourceFileName(file));
    }
  }
}


/*----------------------------------------------------------------------*/
static void deleteBreakpoint(int line, int file) {
  int i = breakpointIndex(line);

  if (i == -1)
    printf("Breakpoint at line %d in '%s' not found.\n", line, sourceFileName(file));
  else {
    breakpoint[i].line = 0;
    printf("Breakpoint at line %d in '%s' deleted.\n",
	   line, sourceFileName(file));
  }
}



static Boolean trc, stp, cap;
static int loc;

/*======================================================================*/
void saveInfo(void)
{
  /* Save some important things */
  cap = capitalize; capitalize = FALSE;
  trc = traceOption; traceOption = FALSE;
  stp = singleStepOption; singleStepOption = FALSE;
  loc = current.location; current.location = where(HERO);
}


/*======================================================================*/
void restoreInfo(void)
{
  /* Restore! */
  capitalize = cap;
  traceOption = trc;
  singleStepOption = stp;
  current.location = loc;
}


/*======================================================================*/
void debug(Boolean calledFromBreakpoint, int line, int fileNumber)
{
  char buf[256];
  char c;
  int i;

  saveInfo();

  if (calledFromBreakpoint) {
    char *cause;
    if (anyOutput) newline();
    if (breakpointIndex(line) != -1)
      cause = "Breakpoint hit at";
    else
      cause = "Stepping to";
    printf("abug> %s line %d in '%s':\n", cause, line,
	    sourceFileName(fileNumber));
    showSourceLine(line, fileNumber);
    anyOutput = FALSE;
  }

  while (TRUE) {
    if (anyOutput) newline();
    do {
      capitalize = FALSE;
      output("abug> ");
#ifdef USE_READLINE
      (void) readline(buf);
#else
      fgets(buf, 255, stdin);
#endif
      lin = 1;
      c = buf[0];
      i = 0;
      sscanf(&buf[1], "%d", &i);
    } while (buf && c == '\0');

    switch (toUpper(c)) {
    case 'H':
    case '?':
      output(alan.longHeader);
      output("$nABUG Commands:\
      $iB [n] -- set breakpoint at source line [n]\
      $iB     -- list breakpoints set\
      $iD [n] -- delete breakpoint at source line [n]\
      $iD     -- delete breakpoint at current source line [n]\
      $iN     -- execute to next source line\
      $iX     -- exit debug mode and return to game\
      $iQ     -- quit game\
      $iC     -- show class hierarchy\
      $iI [n] -- show instances or instance [n]\
      $iO [n] -- show instances that are object[s]\
      $iA [n] -- show instances that are actor[s]\
      $iL [n] -- show instances that are location[s]\
      $iE     -- show events\
      $iG     -- go another player turn\
      $iT     -- toggle trace mode\
      $iS     -- toggle single step trace mode\
");
      break;
    case 'Q':
      terminate(0);
    case 'X':
      debugOption = FALSE;		/* Fall through to 'G' */
    case 'G':
      restoreInfo();
      return;
    case 'I':
      if (i == 0)
	showInstances();
      else
	showInstance(i);
      break;
    case 'O':
      if (i == 0)
        showObjects();
      else
	showObject(i);
      break;
    case 'C':
      if (i == 0)
        showClasses();
      else
	showClass(i);
      break;
    case 'A':
      if (i == 0)
        showActors();
      else
	showActor(i);
      break;
    case 'L':
      if (i == 0)
        showLocations();
      else
	showLocation(i);
      break;
    case 'E':
      showEvents();
      break;
    case 'S':
      if ((stp = !stp))
        printf("Step on.");
      else
        printf("Step off.");
      break;      
    case 'T':
      if ((trc = !trc))
        printf("Trace on.");
      else
        printf("Trace off.");
      break;
    case 'B':
      if (i == 0)
	listBreakpoints();
      else
	setBreakpoint(i, fileNumber);
      break;
    case 'D':
      if (i == 0) {
	if (calledFromBreakpoint)
	  deleteBreakpoint(line, fileNumber);
	else
	  printf("No current breakpoint to delete.\n");
      } else
	deleteBreakpoint(i, fileNumber);
      break;
    case 'N':
      stopAtNextLine = TRUE;
      debugOption = FALSE;
      if (!calledFromBreakpoint)
	currentLine = 0;
      restoreInfo();
      return;
    default:
      output("Unknown ABUG command. ? for help.");
      break;
    }
  }
}


/*======================================================================*/
void traceSay(int item)
{
  /*
    Say somethin, but make sure we don't disturb anything and that it is
    shown to the player. Needed for tracing. During debugging things are
    set up to avoid this problem.
  */

  saveInfo();
  needSpace = FALSE;
  col = 1;
  if (item == 0)
    printf("$null$");
  else
    say(item);
  needSpace = FALSE;
  col = 1;
  restoreInfo();
}
