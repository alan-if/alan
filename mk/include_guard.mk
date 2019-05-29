# Include guard to prohibit running Makefile.* directly
ifndef INCLUDED
$(error You should not run this file directly, it will be included automatically)
else
  ifdef TRACE
    $(info INCLUDED: $(word $(words ${MAKEFILE_LIST}),$(words ${MAKEFILE_LIST}),1st ${MAKEFILE_LIST}))
  endif
endif
