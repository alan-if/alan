/*----------------------------------------------------------------------*\

			       images.c
			   Image Handling

\*----------------------------------------------------------------------*/

#include "images_x.h"
#include "adv_x.h"
#include "lst_x.h"
#include "util.h"
#include <errno.h>


/*----------------------------------------------------------------------*/
int imageNameComparer(List *element1, List *element2)
{
  return strcmp(element1->element.id->string, element2->element.id->string);
}


/*----------------------------------------------------------------------*/
static void numberImages(void)
{
  List *currentImage;
  char *currentImageName;
  int imageNumber = 1;

  currentImage = adv.images;
  while (currentImage != NULL) {
    currentImage->element.id->code = imageNumber;
    currentImageName = currentImage->element.id->string;
    while (currentImage->next != NULL
	   && strcmp(currentImage->next->element.id->string, currentImageName) == 0) {
      currentImage = currentImage->next;
      currentImage->element.id->code = imageNumber;
    }
    currentImage = currentImage->next;
    imageNumber++;
  }
}



/*----------------------------------------------------------------------*/
static void copyImageFile(char fileName[], int resourceNumber)
{
  FILE *original = fopen(fileName, READ_MODE);
  char *extension = strrchr(fileName, '.');
  FILE *theCopy;
  char resourceFileName[1000];
  int c;

  sprintf(resourceFileName, "pic%d%s", resourceNumber, extension);
  
  if (!unlink(resourceFileName) && errno != ENOFILE && errno != 0) {
    char errorString[1000];
    sprintf(errorString, "Could not remove old resource file for %s (%s).", resourceFileName, strerror(errno));
    syserr(errorString, NULL);
  }
  theCopy = fopen(resourceFileName, WRITE_MODE);
  if (!theCopy) syserr("Could not open resource file for %s.", resourceFileName);
  while ((c = fgetc(original)) != EOF)
    fputc(c, theCopy);

  fclose(original);
  fclose(theCopy);
}



/*----------------------------------------------------------------------*/
static void copyImages(void)
{
  List *currentImage = adv.images;
  char *currentImageName;

  while (currentImage != NULL) {
    IdNode *fileName = currentImage->element.id;
    copyImageFile(fileName->string, fileName->code);
    currentImageName = currentImage->element.id->string;
    while (currentImage->next != NULL
	   && strcmp(currentImage->next->element.id->string, currentImageName) == 0)
      currentImage = currentImage->next;
    currentImage = currentImage->next;
  }
}



/*======================================================================*/
void prepareImages(void)
{
  adv.images = sortList(adv.images, &imageNameComparer);
  numberImages();
  copyImages();
}
