#include <cgreen/cgreen.h>

#include "memory.h"

#include "syserr.mock"

Describe(Memory);
BeforeEach(Memory) {}
AfterEach(Memory) {}

Ensure(Memory, can_convert_a_single_pointer_to_and_from_aptr) {
    assert_that(fromAptr(toAptr("")), is_equal_to_string(""));
}

Ensure(Memory, can_convert_two_pointers_to_and_from_aptr) {
    Aptr a = toAptr("a");
    Aptr b = toAptr("b");

    assert_that(fromAptr(a), is_equal_to_string("a"));
    assert_that(fromAptr(b), is_equal_to_string("b"));
}

Ensure(Memory, can_convert_three_pointers_to_and_from_aptr) {
    Aptr a = toAptr("a");
    Aptr b = toAptr("b");
    Aptr c = toAptr("c");

    assert_that(fromAptr(b), is_equal_to_string("b"));
    assert_that(fromAptr(a), is_equal_to_string("a"));
    assert_that(fromAptr(c), is_equal_to_string("c"));
}
