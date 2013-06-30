#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "lists.h"

/* Required mocked modules */
#include "syserr.mock"

Describe(Lists);

BeforeEach(Lists) {
}

AfterEach(Lists) {
}

Ensure(Lists, can_count_length_of_empty_array) {
  Aword array = -1;
  assert_that(lengthOfArray(&array, sizeof(Aword)), is_equal_to(0));
}

Ensure(Lists, can_init_array_to_have_length_zero) {
  Aword *array = allocate(100);
  initArray(array);
  assert_that(lengthOfArray(array, 5), is_equal_to(0));
}

Ensure(Lists, can_add_element_to_empty_array) {
  Aword *array = allocate(100);
  Aword element = 74;

  initArray(array);
  addElement(array, &element, sizeof(element));
  assert_that(lengthOfArray(array, sizeof(element)), is_equal_to(1));
  assert_that(array[0], is_equal_to(74));
}
