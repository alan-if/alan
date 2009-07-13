/*======================================================================*\

  resourceTest.c

  Unit tests for RESOURCE node in the Alan compiler

\*======================================================================*/

#include "resource.c"
#include <sys/stat.h>
#include <stdio.h>

#include "unit.h"
#include "unitList.h"


void testNumberImages()
{
  List e[5];
  int j;
  IdNode *i[5] = {NULL,
		  newId(nulsrcp, "a"),
		  newId(nulsrcp, "b"),
		  newId(nulsrcp, "b"),
		  newId(nulsrcp, "c")
  };
  List *resources;

  e[1].member.resource = newResource(nulsrcp, i[1]);
  e[2].member.resource = newResource(nulsrcp, i[2]);
  e[3].member.resource = newResource(nulsrcp, i[3]);
  e[4].member.resource = newResource(nulsrcp, i[4]);

  /* Zero elements */
  resources = NULL;
  resourceNumber = 1;
  numberResources(resources);
  ASSERT(resources == NULL);

  /* One element */
  e[1].next = NULL;
  e[1].member.resource->fileName->code = 0;
  resources = &e[1];
  resourceNumber = 1;
  numberResources(resources);
  ASSERT(resources == &e[1]);
  ASSERT(resources->next == NULL);
  ASSERT(resources->member.resource->fileName->code == 1);

  /* Four elements */
  e[1].next = &e[2];
  e[2].next = &e[3];
  e[3].next = &e[4];
  e[4].next = NULL;
  for (j = 1; j < 5; j++)
    i[j]->code = 0;

  resources = &e[1];
  resourceNumber = 1;
  numberResources(resources);

  ASSERT(e[1].member.resource->fileName->code == 1);
  ASSERT(e[2].member.resource->fileName->code == 2);
  ASSERT(e[3].member.resource->fileName->code == 2);
  ASSERT(e[4].member.resource->fileName->code == 3);
}


void testOpenNewBlcFile()
{
  FILE *file = openNewBlcFile("unittest");
  fclose(file);
  file = fopen("unittest.blc", "r");
  ASSERT(file != NULL);
  unlink("unittest.a3r");
}


void testGenerateBlcFile() {
  char *blcFileName = "unittest.blc";
  FILE *blcFile = openNewBlcFile("unittest.blc");
  struct stat fileStat;
  List *resources;
  static char *resourceName = "unittest.jpg";
  FILE *resourceFile = fopen(resourceName, "w");
  fclose(resourceFile);

  generateBlcFile(blcFile, NULL);
  fclose(blcFile);

  blcFile = fopen(blcFileName, "r");
  ASSERT(blcFile != NULL);
  stat("unittest.blc", &fileStat);
  ASSERT(fileStat.st_size == 0);

  resourceNumber = 1;
  resources = concat(NULL, newResource(nulsrcp, newId(nulsrcp, resourceName)), RESOURCE_LIST);
  analyzeResources(resources);
  analyzeResource(resources->member.resource);
  blcFile = openNewBlcFile("unittest.blc");
  generateBlcFile(blcFile, resources);
  fclose(blcFile);

  {
    char type[10];
    int number;
    char fileName[100];
    char chunk[10];
    blcFile = fopen(blcFileName, "r");
    fscanf(blcFile, "%s%d%s%s", type, &number, chunk, fileName);
    ASSERT(strcmp(type, "Pict") == 0);
    ASSERT(strcmp(chunk, "JPEG") == 0);
    ASSERT(number == 1);
    ASSERT(strcmp(fileName, resourceName) == 0);
  }
}

void testAnalyzeResource() {
  Resource *resource = newResource(nulsrcp, newId(nulsrcp, ""));
  char fileName[200] = "resourceUnitTest.jpg";
  char *legalExtensions[] = {".jpg", ".jpeg", ".png", ".PnG", ".mod", NULL};
  int i;

  resource->fileName->string = fileName;
  analyzeResource(resource);
  ASSERT(readEcode() == 153);

  for (i = 0; legalExtensions[i] != NULL; i++) {
    strcpy(&fileName[16], legalExtensions[i]);
    FILE *f = fopen(fileName, "w");
    fclose(f);
    resource->fileName->string = fileName;
    analyzeResource(resource);
    ASSERT(readEcode() == 0);
    unlink(fileName);
  }

  {
    char *fileName = "aslkd.phf";
    FILE *f = fopen(fileName, "w");
    fclose(f);
    resource->fileName->string = fileName;
    analyzeResource(resource);
    ASSERT(readEcode() == 801);
    unlink(fileName);
  }
}


void resourceUnitTests()
{
  registerUnitTest(testNumberImages);
  registerUnitTest(testAnalyzeResource);
  registerUnitTest(testOpenNewBlcFile);
  registerUnitTest(testGenerateBlcFile);
}

