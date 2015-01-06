#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "instance.h"

#include "memory.h"


/* Mocked modules */
#include "literal.mock"
#include "class.mock"
#include "attribute.mock"
#include "syserr.mock"
#include "set.mock"
#include "compatibility.mock"
#include "current.mock"
#include "location.mock"
#include "inter.mock"
#include "output.mock"
#include "dictionary.mock"
#include "msg.mock"
#include "container.mock"
#include "checkentry.mock"
#include "actor.mock"
#include "debug.mock"
#include "params.mock"



static Aword *allocate_memory(size_t instance_count) {
    size_t total_size = sizeof(ACodeHeader) +
        (instance_count+1)*sizeof(InstanceEntry) +
        (instance_count+1)*sizeof(AdminEntry);
    return (Aword *)allocate(total_size);
}

static void setup_memory(size_t instance_count) {
    memory = allocate_memory(instance_count);
    header = (ACodeHeader*)memory;
    header->instanceMax = instance_count;
    admin = (AdminEntry*)memory + sizeof(ACodeHeader);
    instances = (InstanceEntry*)admin + sizeof(AdminEntry)*instance_count;
}


#define MAX_NO_OF_INSTANCES 5

Describe(Instance);

BeforeEach(Instance) {
	setup_memory(MAX_NO_OF_INSTANCES);
}

AfterEach(Instance) {
    free(memory);
}


Ensure(Instance, can_handle_direct_transitivity) {
    Aint container = 1;
    Aint containee = 2;
    Aint non_containee = 3;

    admin[containee].location = container;
    admin[non_containee].location = 5;

    instances[container].container = 1;
    
    assert_that(isIn(containee, container, DIRECTLY));
    assert_that(!isIn(non_containee, container, DIRECTLY));
}


Ensure(Instance, can_handle_default_transitivity) {
    Aint outer_container = 1;
    Aint containee = 2;
    Aint non_containee = 3;
    Aint intermediate_container = 4;
        
    admin[containee].location = intermediate_container;
    admin[intermediate_container].location = outer_container;
    admin[non_containee].location = 5;

    instances[intermediate_container].container = 1;
    instances[outer_container].container = 2;
    
    assert_that(isIn(containee, outer_container, DEFAULT));
    assert_that(isIn(intermediate_container, outer_container, DEFAULT));
    assert_that(!isIn(non_containee, outer_container, DIRECTLY));
}

Ensure(Instance, can_handle_indirect_transitivity) {
    Aint outer_container = 1;
    Aint containee = 2;
    Aint non_containee = 3;
    Aint intermediate_container = 4;
        
    admin[containee].location = intermediate_container;
    admin[intermediate_container].location = outer_container;
    admin[non_containee].location = 5;

    instances[intermediate_container].container = 1;
    instances[outer_container].container = 2;
    
    assert_that(isIn(containee, outer_container, INDIRECTLY));
    assert_that(!isIn(intermediate_container, outer_container, INDIRECTLY));
    assert_that(!isIn(non_containee, outer_container, INDIRECTLY));
}
