/*----------------------------------------------------------------------*\

  args.h

  Argument handling 

\*----------------------------------------------------------------------*/

#ifdef __amiga__
#include <libraries/dosextens.h>

extern struct FileHandle *con;

#endif

#ifdef _PROTOTYPES_
extern void args(int argc, char *argv[]);
#else
extern void args();
#endif

