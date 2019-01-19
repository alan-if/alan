#include <cgreen/cgreen.h>

/* Mocks: */
#include "id.mock"
#include "sym.mock"
#include "exp.mock"
#include "srcp.mock"
#include "cla.mock"
#include "ins.mock"
#include "encode.mock"
#include "adv.mock"
#include "context.mock"
#include "wht.mock"
#include "lmList.mock"
#include "smScSema.mock"
#include "emit.mock"
#include "atr.mock"
#include "cnt.mock"
#include "ext.mock"
#include "whr.mock"
#include "stm.mock"
#include "article.mock"
#include "vrb.mock"
#include "scr.mock"
#include "nam.mock"
#include "description.mock"
#include "initialize.mock"

/* Utility modules: */
#include "lst_x.h"

/* Ensure also private functions are available in the SUT */
#define UNITTESTING
#include "prop_x.h"


Describe(Properties);
BeforeEach(Properties) {}
AfterEach(Properties) {}


Ensure(Properties, generates_no_list_for_empty_pronouns) {
    assert_that(generatePronouns(NULL), is_equal_to((Aword)EOF));
}

Ensure(Properties, generates_pronoun_list_of_two) {
    Id *id1 = newId(nulsrcp, "pronoun");
    Id *id2 = newId(nulsrcp, "pronoun");
    List *pronoun_list_of_one = concat(concat(NULL, id1, ID_LIST), id2, ID_LIST);
    Aaddr start = nextEmitAddress();
    id1->code = 55;
    id2->code = 56;
    expect(emit, when(word, is_equal_to(id1->code)));
    expect(emit, when(word, is_equal_to(id2->code)));
    expect(emit, when(word, is_equal_to((Aword)EOF)));
    assert_that(generatePronouns(pronoun_list_of_one), is_equal_to(start));
}
