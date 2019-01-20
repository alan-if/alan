#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "dictionary.h"

#include "memory.h"

#include "lists.h"

/* Mocked modules */
#include "options.mock"
#include "inter.mock"
#include "output.mock"
#include "compatibility.mock"
#include "debug.mock"
#include "utils.mock"
#include "current.mock"
#include "syserr.mock"
#include "args.mock"
#include "instance.mock"
#include "decode.mock"
#include "params.mock"
#include "msg.mock"
#include "score.mock"
#include "state.mock"
#include "term.mock"
#include "readline.mock"
#include "save.mock"
#include "word.mock"
#include "event.mock"
#include "actor.mock"

#include "types.h"

#define MAX_NO_OF_PRONOUN_REFERENCES 3
#define MAX_NO_OF_PRONOUNS 5

static Aword *allocate_memory() {
    int sizeOfPronounList = MAX_NO_OF_PRONOUN_REFERENCES*MAX_NO_OF_PRONOUNS;
    return (Aword *)allocate((sizeOfPronounList+1)*sizeof(Pronoun));
}


Describe(Dictionary);

BeforeEach(Dictionary) {
    memory = allocate_memory();
}

AfterEach(Dictionary) {
    free(memory);
}


Ensure(Dictionary, generates_empty_list_of_pronouns_for_empty_dictionary) {
    Pronoun *pronounList;
    DictionaryEntry *mocked_dictionary = (DictionaryEntry*)allocate(sizeof(DictionaryEntry));

    dictionary = mocked_dictionary;
    setEndOfArray(dictionary);

    pronounList = generatePronounList();

    assert_that(pronounList, is_not_equal_to(NULL));
    assert_that(isEndOfArray(pronounList));
}

static void given_a_dictionary_with_only_a_pronoun_with_one_reference(int pronoun, int instance) {
    DictionaryEntry *mocked_dictionary = (DictionaryEntry*)allocate(sizeof(DictionaryEntry)*2);

    /* Build pronounRefs, which must be in memory */
    int address_of_pronounRefs = 1;
    memory[address_of_pronounRefs] = instance; /* First instance */
    memory[address_of_pronounRefs+1] = EOF;

    /* Build dictionary of one word/pronoun */
    dictionary = mocked_dictionary;
    dictionary[0].classBits = PRONOUN_BIT;
    dictionary[0].code = pronoun;
    dictionary[0].pronounRefs = address_of_pronounRefs;
    setEndOfArray(&dictionary[1]);
}


Ensure(Dictionary, generates_list_for_single_pronoun_with_single_reference) {
    Pronoun *pronounList;

    given_a_dictionary_with_only_a_pronoun_with_one_reference(34, 42);

    pronounList = generatePronounList();

    assert_that(pronounList, is_not_equal_to(NULL));
    assert_that(pronounList[0].pronoun, is_equal_to(34));
    assert_that(pronounList[0].instance, is_equal_to(42));
}

static void given_a_dictionary_with_a_noun_and_a_pronoun_with_one_reference(int pronoun, int instance) {
    DictionaryEntry *mocked_dictionary = (DictionaryEntry*)allocate(sizeof(DictionaryEntry)*2);

    /* Build pronounRefs, which must be in memory */
    int address_of_pronounRefs = 1;
    memory[address_of_pronounRefs] = instance; /* First instance */
    memory[address_of_pronounRefs+1] = EOF;

    /* Build dictionary of two words, one being a pronoun */
    dictionary = mocked_dictionary;
    dictionary[0].classBits = NOUN_BIT;
    dictionary[1].classBits = PRONOUN_BIT|NOUN_BIT;
    dictionary[1].code = pronoun;
    dictionary[1].pronounRefs = address_of_pronounRefs;
    setEndOfArray(&dictionary[2]);
}

Ensure(Dictionary, generates_list_for_dictionary_having_noun_and_pronoun) {
    Pronoun *pronounList;

    given_a_dictionary_with_a_noun_and_a_pronoun_with_one_reference(34, 42);

    pronounList = generatePronounList();

    assert_that(pronounList, is_not_equal_to(NULL));
    assert_that(pronounList[0].pronoun, is_equal_to(34));
    assert_that(pronounList[0].instance, is_equal_to(42));
}
