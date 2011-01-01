#include "cgreen/cgreen.h"

#include "main.c"


Ensure canCopyAttributes() {
    AttributeEntry *attributeArea;
    AttributeEntry *a = (AttributeEntry *)&memory[1];

    memory = allocate(6*sizeof(AttributeEntry));
    a = (AttributeEntry *)&memory[1];

    header = allocate(sizeof(ACodeHeader));
    header->instanceMax = 2;
    admin = allocate(3*sizeof(AdminEntry));
    instances = allocate(3*sizeof(InstanceEntry));

    /* Create two attribute lists which consists of two attributes each,
       each is terminated by EOF */
    a[0].code = 13;
    a[0].value = 15;
    a[0].id = 17;
    a[1].code = 19;
    a[1].value = 21;
    a[1].id = 23;
    a[2].code = EOF;

    a[3].code = 130;
    a[3].value = 150;
    a[3].id = 170;
    a[4].code = 190;
    a[4].value = 210;
    a[4].id = 230;
    a[5].code = EOF;

    instances[1].initialAttributes = 1;
    instances[2].initialAttributes = 10;

    attributeArea = initializeAttributes(5*sizeof(AttributeEntry)/sizeof(Aword));

    assert_true(admin[1].attributes[0].code == 13);
    assert_true(admin[1].attributes[0].value == 15);
    assert_true(admin[1].attributes[0].id == 17);
    assert_true(admin[1].attributes[1].code == 19);
    assert_true(admin[1].attributes[1].value == 21);
    assert_true(admin[1].attributes[1].id == 23);
    assert_true(*(Aword*)&admin[1].attributes[2] == EOF);

    assert_true(admin[2].attributes[0].code == 130);
    assert_true(admin[2].attributes[0].value == 150);
    assert_true(admin[2].attributes[0].id == 170);
    assert_true(admin[2].attributes[1].code == 190);
    assert_true(admin[2].attributes[1].value == 210);
    assert_true(admin[2].attributes[1].id == 230);
    assert_true(*(Aword*)&admin[2].attributes[2] == EOF);
}

Ensure canHandleMemoryStartForPre3_0alpha5IsShorter() {
    char version[4];
    version[3] = 3;
    version[2] = 0;
    version[1] = 4;
    version[0] = 'a';

    assert_true(sizeof(ACodeHeader)/sizeof(Aword)-2==memoryStart(version));
}

Ensure canHandleMemoryStartForPre3_0beta1IsShorter() {
    char version[4];
    version[3] = 3;
    version[2] = 0;
    version[1] = 9;
    version[0] = 'a';

    assert_true(sizeof(ACodeHeader)/sizeof(Aword)-1==memoryStart(version));
}

Ensure canSetEof() {
    Parameter parameters[10];

    setEndOfArray(&parameters[7]);
    assert_false(isEndOfArray(&parameters[0]));
    assert_true(isEndOfArray(&parameters[7]));
}

TestSuite *mainTests()
{
    TestSuite *suite = create_test_suite();

    add_test(suite, canCopyAttributes);
    add_test(suite, canHandleMemoryStartForPre3_0alpha5IsShorter);
    add_test(suite, canSetEof);

    return suite;
}
