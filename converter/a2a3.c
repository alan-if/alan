/*----------------------------------------------------------------------*\

	Alan v2 to v3 converter

	Main program

\*----------------------------------------------------------------------*/

#include <stdio.h>
#include "a2a3.h"

#include "LmList.h"
#include "smScan.h"
#include "pmParse.h"


/* PUBLIC */
int fileNo = 0;

#define FILENAMESIZE 1000
static char inFileName[FILENAMESIZE];
static char fullInFileName[FILENAMESIZE];
static char outFileName[FILENAMESIZE];
static char fullOutFileName[FILENAMESIZE];

FILE *outFile;

#ifdef WINGUI
#include <windows.h>

static OPENFILENAME ofn;

static int getInFileName() {
  static char filter[] = "Alan Source Files (*.alan)\0*.alan\0";

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hInstance = NULL;
  ofn.lpstrFilter = filter;
  ofn.lpstrCustomFilter = NULL;
  ofn.lpstrFile = fullInFileName;
  ofn.nMaxFile = FILENAMESIZE;
  ofn.lpstrFileTitle = inFileName;
  ofn.nMaxFileTitle = FILENAMESIZE;
  ofn.Flags = OFN_HIDEREADONLY;
  return GetOpenFileName(&ofn);
}

static int getOutFileName() {
  ofn.lpstrFile = fullOutFileName;
  ofn.lpstrFileTitle = outFileName;
  ofn.Flags = OFN_OVERWRITEPROMPT;
  return GetSaveFileName(&ofn);
}

static char *argv[10];

static int splitCommandLine(char commandLine[])
{
  char *start = commandLine;
  char *end = strpbrk(commandLine, " ");
  int i = 0;
  int noOfSpaces;

  while (end) {
    argv[i] = start;
    *end = '\0';
    noOfSpaces = strspn(end+1, " ");
    start = end + 1 + noOfSpaces;
    end = strpbrk(start, " ");
    i++;
  }
  if (*start != '\0') {
    argv[i] = start;
    i++;
  }
  return i;
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
  int args = splitCommandLine(cmdLine);

  if (args>2) {
    printf("Can't have more than two arguments.");
    return -1;
  }

  if (args<1) {
    if (!getInFileName())
      return 0;
  } else
    strcpy(fullInFileName, argv[0]);

  if (args<2)
    if (getOutFileName())
      outFile = fopen(fullOutFileName, "w");
    else
      outFile = stdout;
  else
    outFile = fopen(argv[1], "w");
  if (outFile == NULL) {
    printf("Could not open output file");
    exit(-1);
  }      

  lmLiInit("", "", lm_ENGLISH_Messages);
  if (!smScanEnter(fullInFileName, FALSE)) {
    printf("Could not open source file");
    exit(-1);
  } else {
    pmParse();
  }
  return 0;
}

#else

int main(int argc, char* argv[]) {
  strcpy(inFileName, argv[1]);
  outFile = stdout;
  lmLiInit("", "", lm_ENGLISH_Messages);
  if (!smScanEnter(inFileName, FALSE)) {
    printf("Could not open source file");
    exit(-1);
  } else {
    pmParse();
  }
  return 0;
}
#endif
