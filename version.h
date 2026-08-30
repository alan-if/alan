#ifndef _version_h_
#define _version_h_ 3

typedef struct {
  char*  string;                /* e.g. "3.0beta8" */
  int    version;
  int    revision;
  int    correction;
  char*  state;                 /* e.g. "beta", first character goes
                                   into the .a3c header */
} Version;

typedef struct {
  char*   shortHeader;
  char*   longHeader;
  char*   date;                 /* of the commit, not of the build */
  char*   time;
  Version version;
} Product;

#endif
