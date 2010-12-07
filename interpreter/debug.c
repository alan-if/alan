/*----------------------------------------------------------------------*\

  debug.c

  Debugger unit in Alan interpreter ARUN

  \*----------------------------------------------------------------------*/

#include "debug.h"


/* Imports: */

#include "sysdep.h"
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#include "alan.version.h"

#ifdef USE_READLINE
#include "readline.h"
#endif

#include "lists.h"
#include "inter.h"
#include "current.h"
#include "options.h"
#include "utils.h"
#include "instance.h"
#include "memory.h"
#include "output.h"
#include "class.h"
#include "event.h"
#include "exe.h"

#ifdef HAVE_GLK
#define MAP_STDIO_TO_GLK
#include "glkio.h"
#endif

#define BREAKPOINTMAX 50


/* PUBLIC: */
int breakpointCount = 0;
Breakpoint breakpoint[BREAKPOINTMAX];

#define debugPrefix "adbg: "

/*----------------------------------------------------------------------*/
static void showAttributes(AttributeEntry *attributes)
{
    AttributeEntry *at;
    int i;
    char str[80];

    if (attributes == 0)
        return;

    i = 1;
    for (at = attributes; !isEndOfArray(at); at++) {
        sprintf(str, "$i$t%s[%d] = %d", (char *) pointerTo(at->id), at->code, (int)at->value);
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
    Abool found = FALSE;

    output("$iContains:");
    for (i = 1; i <= header->instanceMax; i++) {
        if (in(i, cnt, TRUE)) { /* Yes, it's directly in this container */
            if (!found)
                found = TRUE;
            output("$i$t");
            say(i);
            sprintf(str, "[%d] ", i);
            output(str);
        }
    }
    if (!found)
        output("nothing");
}


/*----------------------------------------------------------------------*/
static void showInstanceLocation(int ins, char *prefix) {
    char buffer[1000];

    if (admin[ins].location == 0)
        return;
    else {
        output(prefix);
        if (isALocation(admin[ins].location)) {
            output("at");
            say(admin[ins].location);
            sprintf(buffer, "[%d]", admin[ins].location);
            output(buffer);
        } else if (isContainer(admin[ins].location)) {
		  
            if (isObject(admin[ins].location))
                output("in");
            else if (isActor(admin[ins].location))
                output("carried by");
            say(admin[ins].location);
            sprintf(buffer, "[%d]", admin[ins].location);
            output(buffer);

        } else
            output("Illegal location!");
    }
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
        if (instances[ins].container)
            output("(container)");
        showInstanceLocation(ins, ", ");
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

    output("The");
    say(ins);
    sprintf(str, "[%d]", ins);
    output(str);
    if (instances[ins].parent) {
        sprintf(str, "Isa %s[%d]", idOfClass(instances[ins].parent), instances[ins].parent);
        output(str);
    }

    if (!isA(ins, header->locationClassId) || (isA(ins, header->locationClassId) && admin[ins].location != 0)) {
        sprintf(str, "$iLocation: ");
        output(str);
        showInstanceLocation(ins, " ");
    }

    output("$iAttributes:");
    showAttributes(admin[ins].attributes);

    if (instances[ins].container)
        showContents(ins);

    if (isA(ins, header->actorClassId)) {
        if (admin[ins].script == 0)
            output("$iIs idle");
        else {
            sprintf(str, "$iExecuting script: %d, Step: %d", admin[ins].script, admin[ins].step);
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
        if (isObject(obj)) {
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


    if (!isObject(obj)) {
        sprintf(str, "Instance %d is not an object", obj);
        output(str);
        return;
    }

    showInstance(obj);

}

#ifdef UNDEF_WHEN_NEEDED
/*----------------------------------------------------------------------*/
static void showcnts(void)
{
    char str[80];
    int cnt;

    output("Containers:");
    for (cnt = 1; cnt <= header->containerMax; cnt++) {
        sprintf(str, "$i%3d: ", cnt);
        output(str);
        if (containers[cnt].owner != 0)
            say(containers[cnt].owner);
    }

}


/*----------------------------------------------------------------------*/
static void showContainer(int cnt)
{
    char str[80];

    if (cnt < 1 || cnt > header->containerMax) {
        sprintf(str, "Container number out of range. Between 1 and %d, please.", header->containerMax);
        output(str);
        return;
    }

    sprintf(str, "Container %d :", cnt);
    output(str);
    if (containers[cnt].owner != 0) {
        cnt = containers[cnt].owner;
        say(cnt);
        sprintf(str, "$iLocation: %d", where(cnt, TRUE));
        output(str);
    }
    showContents(cnt);
}


/*----------------------------------------------------------------------*/
static int sourceFileNumber(char *fileName) {
    SourceFileEntry *entries = pointerTo(header->sourceFileTable);
    int n;

    for (n = 0; *(Aword*)&entries[n] != EOF; n++) {
        char *entryName;
        entryName = getStringFromFile(entries[n].fpos, entries[n].len);
        if (strcmp(entryName, fileName) == 0) return n;
        entryName = baseNameStart(entryName);
        if (strcmp(entryName, fileName) == 0) return n;
    }
    return -1;
}
#endif



/*----------------------------------------------------------------------*/
static void printClassName(int c) {
    output(idOfClass(c));
}


/*----------------------------------------------------------------------*/
static void showClassInheritance(int c) {
    char str[80];

    if (classes[c].parent != 0) {
        output(", Isa");
        printClassName(classes[c].parent);
        sprintf(str, "[%d]", classes[c].parent);
        output(str);
    }
}


/*----------------------------------------------------------------------*/
static void showClass(int c)
{
    char str[80];

    output("$t");
    printClassName(c);
    sprintf(str, "[%d]", c);
    output(str);
    showClassInheritance(c);
}


/*----------------------------------------------------------------------*/
static void listClass(int c)
{
    char str[80];

    sprintf(str, "%3d: ", c);
    output(str);
    printClassName(c);
    showClassInheritance(c);
}


/*----------------------------------------------------------------------*/
static void showClassHierarchy(int this, int depth)
{
    int i;
    int child;

    output("$i");
    for (i=0; i < depth; i++)
        output("$t");
    listClass(this);
    for (child = 1; child <= header->classMax; child++) {
        if (classes[child].parent == this) {
            showClassHierarchy(child, depth+1);
        }
    }
}


/*----------------------------------------------------------------------*/
static void showLocations(void)
{
    char str[80];
    int loc;

    output("Locations:");
    for (loc = 1; loc <= header->instanceMax; loc++) {
        if (isALocation(loc)) {
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


    if (!isALocation(loc)) {
        sprintf(str, "Instance %d is not a location.", loc);
        output(str);
        return;
    }

    output("The ");
    say(loc);
    sprintf(str, "(%d) Isa location :", loc);
    output(str);

    output("$iAttributes =");
    showAttributes(admin[loc].attributes);
}


/*----------------------------------------------------------------------*/
static void showActors(void)
{
    char str[80];
    int act;

    output("Actors:");
    for (act = 1; act <= header->instanceMax; act++) {
        if (isActor(act)) {
            sprintf(str, "$i%3d: ", act);
            output(str);
            say(act);
            if (instances[act].container)
                output("(container)");
        }
    }
}


/*----------------------------------------------------------------------*/
static void showActor(int act)
{
    char str[80];

    if (!isActor(act)) {
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
    Bool scheduled;

    output("Events:");
    for (event = 1; event <= header->eventMax; event++) {
        sprintf(str, "$i%d [%s]:", event, (char *)pointerTo(events[event].id));
#if ISO == 0
        fromIso(str, str);
#endif
        output(str);
        scheduled = FALSE;
        for (i = 0; i < eventQueueTop; i++)
            if ((scheduled = (eventQueue[i].event == event)))
                break;
        if (scheduled) {
            sprintf(str, "Scheduled for +%d, at ", eventQueue[i].after);
            output(str);
            say(eventQueue[i].where);
        } else
            output("Not scheduled.");
    }
}


/*======================================================================*/
char *sourceFileName(int fileNumber) {
    SourceFileEntry *entries = pointerTo(header->sourceFileTable);

    return getStringFromFile(entries[fileNumber].fpos, entries[fileNumber].len);
}


/*======================================================================*/
char *readSourceLine(int file, int line) {
    int count;
#define SOURCELINELENGTH 1000
    static char buffer[SOURCELINELENGTH];

#ifdef HAVE_GLK
    frefid_t sourceFileRef;
    strid_t sourceFile;

    sourceFileRef = glk_fileref_create_by_name(fileusage_TextMode, sourceFileName(file), 0);
    sourceFile = glk_stream_open_file(sourceFileRef, filemode_Read, 0);
#else
    FILE *sourceFile;

    sourceFile  = fopen(sourceFileName(file), "r");
#endif

    if (sourceFile != NULL) {
        for (count = 0; count < line; count++) {
            if (fgets(buffer, SOURCELINELENGTH, sourceFile) == 0)
                return NULL;
            /* If not read the whole line, or no newline, try to read again */
            while (strchr(buffer, '\n') == NULL)
                if (fgets(buffer, SOURCELINELENGTH, sourceFile) == 0)
                    return buffer;
        }
        fclose(sourceFile);
        return buffer;
    }
    return NULL;
}

/*======================================================================*/
void showSourceLine(int fileNumber, int line) {
    char *buffer = readSourceLine(fileNumber, line);
    if (buffer != NULL) {
        if (buffer[strlen(buffer)-1] == '\n')
            buffer[strlen(buffer)-1] = '\0';
        printf("<%05d>: %s", line, buffer);
    }
}


/*----------------------------------------------------------------------*/
static void listFiles() {
    SourceFileEntry *entry;
    int i = 0;
    for (entry = pointerTo(header->sourceFileTable); *((Aword*)entry) != EOF; entry++) {
        printf("  %2d : %s\n", i, sourceFileName(i));
        i++;
    }
}


/*----------------------------------------------------------------------*/
static void listLines() {
    SourceLineEntry *entry;
    for (entry = pointerTo(header->sourceLineTable); *((Aword*)entry) != EOF; entry++)
        printf("  %s:%d\n", sourceFileName(entry->file), entry->line);
}


/*----------------------------------------------------------------------*/
static int findSourceLineIndex(int file, int line) {
    /* Will return index to the closest line available */
    SourceLineEntry *entry = pointerTo(header->sourceLineTable);
    int i = 0;

    while (!isEndOfArray(&entry[i]) && entry[i].file <= file && entry[i].line < line)
        i++;
    if (isEndOfArray(entry))
        return i-1;
    else
        return i;
}


/*----------------------------------------------------------------------*/
static void listBreakpoints() {
    int i;
    Bool found = FALSE;

    for (i = 0; i < BREAKPOINTMAX; i++)
        if (breakpoint[i].line != 0) {
            if (!found)
                printf("Breakpoints set:\n");
            found = TRUE;
            printf("    %s:%d\n", sourceFileName(breakpoint[i].file), breakpoint[i].line);
        }
    if (!found)
        printf("No breakpoints set\n");
}


/*======================================================================*/
Bool breakpointIndex(int file, int line) {
    int i;

    for (i = 0; i < BREAKPOINTMAX; i++)
        if (breakpoint[i].line == line && breakpoint[i].file == file)
            return i;
    return(-1);
}


/*----------------------------------------------------------------------*/
static int availableBreakpointSlot() {
    int i;

    for (i = 0; i < BREAKPOINTMAX; i++)
        if (breakpoint[i].line == 0)
            return i;
    return -1;
}


/*----------------------------------------------------------------------*/
static void setBreakpoint(int file, int line) {
    int i = breakpointIndex(file, line);

    if (i != -1)
        printf("Breakpoint already set at %s:%d\n", sourceFileName(file), line);
    else {
        i = availableBreakpointSlot();
        if (i == -1)
            printf("No room for more breakpoints. Delete one first\n");
        else {
            int lineIndex = findSourceLineIndex(file, line);
            SourceLineEntry *entry = pointerTo(header->sourceLineTable);
            char leadingText[100] = "Breakpoint";
            if (entry[lineIndex].file == EOF) {
                printf("Line %d not available\n", line);
            } else {
                if (entry[lineIndex].line != line)
                    sprintf(leadingText, "Line %d not available, breakpoint instead", line);
                breakpoint[i].file = entry[lineIndex].file;
                breakpoint[i].line = entry[lineIndex].line;
                printf("%s set at %s:%d\n", leadingText, sourceFileName(entry[lineIndex].file), entry[lineIndex].line);
                showSourceLine(entry[lineIndex].file, entry[lineIndex].line);
                printf("\n");
            }
        }
    }
}


/*----------------------------------------------------------------------*/
static void deleteBreakpoint(int line, int file) {
    int i = breakpointIndex(file, line);

    if (i == -1)
        printf("No breakpoint set at %s:%d\n", sourceFileName(file), line);
    else {
        breakpoint[i].line = 0;
        printf("Breakpoint at %s:%d deleted\n", sourceFileName(file), line);
    }
}



static Bool trc, stp, cap, psh, stk;
static int loc;

/*======================================================================*/
void saveInfo(void)
{
    /* Save some important things */
    cap = capitalize; capitalize = FALSE;
    trc = sectionTraceOption; sectionTraceOption = FALSE;
    stp = singleStepOption; singleStepOption = FALSE;
    psh = tracePushOption; tracePushOption = FALSE;
    stk = traceStackOption; traceStackOption = FALSE;
    loc = current.location; current.location = where(HERO, TRUE);
}


/*======================================================================*/
void restoreInfo(void)
{
    /* Restore! */
    capitalize = cap;
    sectionTraceOption = trc;
    singleStepOption = stp;
    tracePushOption = psh;
    traceStackOption = stk;
    current.location = loc;
}

#define HELP_COMMAND 'H'
#define QUIT_COMMAND 'Q'
#define EXIT_COMMAND 'X'
#define GO_COMMAND 'G'
#define FILES_COMMAND 'F'
#define INSTANCES_COMMAND 'I'
#define CLASSES_COMMAND 'C'
#define OBJECTS_COMMAND 'O'
#define ACTORS_COMMAND 'A'
#define LOCATIONS_COMMAND 'L'
#define EVENTS_COMMAND 'E'
#define BREAK_COMMAND 'B'
#define DELETE_COMMAND 'D'
#define INSTRUCTION_TRACE_COMMAND 'S'
#define SECTION_TRACE_COMMAND 'T'
#define NEXT_COMMAND 'N'

typedef struct DebugParseEntry {
	char *command;
	char code;
} DebugParseEntry;

static DebugParseEntry commands[] = {
		{"help", HELP_COMMAND},
		{"?", HELP_COMMAND}
	};


/*----------------------------------------------------------------------*/
static char parseDebugCommand(char *commandLine) {
	DebugParseEntry *entry = commands;
	return entry->code;
}


/*----------------------------------------------------------------------*/
static char readDebugCommand(int *parameter) {
	char c;
	char buf[5000];

	capitalize = FALSE;
	if (anyOutput) newline();
	do {
		output("adbg> ");

#ifdef USE_READLINE
		(void) readline(buf);
#else
		fgets(buf, 255, stdin);
#endif
		lin = 1;
		c = buf[0];
		*parameter = 0;
		sscanf(&buf[1], "%d", parameter);
	} while (c == '\0');
	return toUpper(c);
}


static void displayLocation(int line, int fileNumber) {
	char *cause;
	if (anyOutput) newline();
	if (breakpointIndex(fileNumber, line) != -1)
		cause = "Breakpoint hit at";
	else
		cause = "Stepping to";
	printf("%s %s %s:%d\n", debugPrefix, cause, sourceFileName(fileNumber), line);
	showSourceLine(fileNumber, line);
	printf("\n");
	anyOutput = FALSE;
}

static void toggleSectionTrace() {
	if ((trc = !trc))
		printf("Section trace on.");
	else
		printf("Section trace off.");
}

static void handleBreakCommand(int fileNumber, int parameter) {
	if (parameter == 0)
		listBreakpoints();
	else
		setBreakpoint(fileNumber, parameter);
}

static void handleDeleteCommand(Bool calledFromBreakpoint, int line, int fileNumber, int parameter) {
	if (parameter == 0) {
		if (calledFromBreakpoint)
			deleteBreakpoint(line, fileNumber);
		else
			printf("No current breakpoint to delete\n");
	} else
		deleteBreakpoint(parameter, fileNumber);
}

static void handleNextCommand(Bool calledFromBreakpoint) {
	stopAtNextLine = TRUE;
	debugOption = FALSE;
	if (!calledFromBreakpoint)
		current.sourceLine = 0;
	restoreInfo();
}

static void toggleInstructionTrace() {
	if ((stp = !stp))
		printf("Single instruction trace on.");
	else
		printf("Single instruction trace off.");
}

static void handleLocationsCommand(int parameter) {
	if (parameter == 0)
		showLocations();
	else
		showLocation(parameter);
}

static void handleActorsCommand(int parameter) {
	if (parameter == 0)
		showActors();
	else
		showActor(parameter);
}

static void handleClassesCommand(int parameter) {
	if (parameter == 0) {
		output("Classes:");
		showClassHierarchy(1, 0);
	} else
		showClass(parameter);
}

static void handleObjectsCommand(int parameter) {
	if (parameter == 0)
		showObjects();
	else
		showObject(parameter);
}

static void handleInstancesCommand(int parameter) {
	if (parameter == 0)
		showInstances();
	else
		showInstance(parameter);
}

static void handleHelpCommand() {
	output(alan.longHeader);
	output("$nADBG Commands:\
      $iB [n] -- set breakpoint at source line [n]\
      $iB     -- list breakpoints set\
      $iD [n] -- delete breakpoint at source line [n]\
      $iD     -- delete breakpoint at current source line\
      $iF     -- list files\
      $iN     -- execute to next source line\
      $iX     -- exit debug mode and return to game, get back with 'debug'\
      $iQ     -- quit game\
      $iC     -- show class hierarchy\
      $iI [n] -- show instances or instance [n]\
      $iO [n] -- show instances that are objects\
      $iA [n] -- show instances that are actors\
      $iL [n] -- show instances that are locations\
      $iE     -- show events\
      $iG     -- go another player turn\
      $iT     -- toggle trace mode\
      $iS     -- toggle single step trace mode\
");
}




/*======================================================================*/
void debug(Bool calledFromBreakpoint, int line, int fileNumber)
{
    char command;
    int parameter;

    saveInfo();

    if (calledFromBreakpoint)
		displayLocation(line, fileNumber);

    while (TRUE) {
		command = readDebugCommand(&parameter);

        switch (command) {
        case HELP_COMMAND: handleHelpCommand(); break;
        case QUIT_COMMAND: terminate(0); break;
        case EXIT_COMMAND: debugOption = FALSE;		/* Fall through to 'G' */
        case GO_COMMAND: restoreInfo(); return;
        case FILES_COMMAND: listFiles(); break;
        case INSTANCES_COMMAND: handleInstancesCommand(parameter); break;
        case OBJECTS_COMMAND: handleObjectsCommand(parameter); break;
        case CLASSES_COMMAND: handleClassesCommand(parameter); break;
        case ACTORS_COMMAND: handleActorsCommand(parameter); break;
        case LOCATIONS_COMMAND: handleLocationsCommand(parameter); break;
        case EVENTS_COMMAND: showEvents(); break;
        case INSTRUCTION_TRACE_COMMAND: toggleInstructionTrace(); break;
        case SECTION_TRACE_COMMAND: toggleSectionTrace(); break;
        case BREAK_COMMAND: handleBreakCommand(fileNumber, parameter); break;
        case DELETE_COMMAND: handleDeleteCommand(calledFromBreakpoint, line, fileNumber, parameter); break;
        case NEXT_COMMAND: handleNextCommand(calledFromBreakpoint); return;
        default: output("Unknown ADBG command. ? for help."); break;
        }
    }
}


/*======================================================================*/
void traceSay(int item)
{
    /*
      Say something, but make sure we don't disturb anything and that it is
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
