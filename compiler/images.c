/*----------------------------------------------------------------------*\

			       images.c
			   Image Handling

\*----------------------------------------------------------------------*/

#include "images_x.h"
#include "adv_x.h"
#include "lst_x.h"


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

  sprintf(resourceFileName, "pic%03d%s", resourceNumber, extension);
  theCopy = fopen(resourceFileName, WRITE_MODE);

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
