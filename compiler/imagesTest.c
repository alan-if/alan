/*======================================================================*\

  imagesTest.c

  Unit tests for IMAGES node in the Alan compiler

\*======================================================================*/

#include "images.c"

void testNumberImages()
{
  List e[5];
  IdNode i[5];
  int j;

  e[1].element.id = &i[1];
  e[1].element.id->string = "a";
  e[1].element.id = &i[1];
  e[1].element.id->string = "a";
  e[2].element.id = &i[2];
  e[2].element.id->string = "b";
  e[3].element.id = &i[3];
  e[3].element.id->string = "b";
  e[4].element.id = &i[4];
  e[4].element.id->string = "c";

  /* Zero elements */
  adv.images = NULL;
  numberImages();
  ASSERT(adv.images == NULL);

  /* One element */
  e[1].next = NULL;
  i[1].code = 0;
  adv.images = &e[1];
  numberImages();
  ASSERT(adv.images == &e[1]);
  ASSERT(adv.images->next == NULL);
  ASSERT(adv.images->element.id->code == 1);

  /* Four elements */
  e[1].next = &e[2];
  e[2].next = &e[3];
  e[3].next = &e[4];
  e[4].next = NULL;
  for (j = 1; j < 5; j++)
    i[j].code = 0;

  adv.images = &e[1];
  numberImages();

  ASSERT(e[1].element.id->code == 1);
  ASSERT(e[2].element.id->code == 2);
  ASSERT(e[3].element.id->code == 2);
  ASSERT(e[4].element.id->code == 3);
}


void registerImagesUnitTests()
{
  registerUnitTest(testNumberImages);
}

