/*----------------------------------------------------------------------*\

  winargs - Argument handling for arun on DOS, WINDOWS and CYGWIN

  This file is included in args.c when compiling for DOS, WINDOWS or CYGWIN

  Handles the various startup methods on all machines.

  Main function args() will set up global variables adventureName,
  adventureFileName and the flags, the terminal will also be set up
  and connected if necessary.

\*----------------------------------------------------------------------*/

#ifdef __windows__
#include <windows.h>
#endif


/*======================================================================*/
void args(int argc, char * argv[])
{
  char *programName;

  if ((programName = strrchr(argv[0], '\\')) == NULL
      && (programName = strrchr(argv[0], '/')) == NULL
      && (programName = strrchr(argv[0], ':')) == NULL)
    programName = strdup(argv[0]);
  else
    programName = strdup(&programName[1]);

  if (strlen(programName) > 4 && stricmp(&programName[strlen(programName)-4], ".EXE") == 0)
    programName[strlen(programName)-4] = '\0';

  /* Now look at the switches and arguments */
  switches(argc, argv);

  if (adventureFileName == NULL) {
    /* No game given, try program name */
    if (!matchInterpreterName(programName)) {
      adventureFileName = duplicate(argv[0],
				    strlen(argv[0])
				    +strlen(ACODEEXTENSION)+1);
      strcat(adventureFileName, ACODEEXTENSION);
    }
  }
}
