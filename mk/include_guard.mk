# Include guard to prohibit running Makefile.* directly
ifndef INCLUDED
$(error You should not run this file directly, it will be included automatically)
else
$(info INCLUDED: $(lastword $(MAKEFILE_LIST)))
endif
