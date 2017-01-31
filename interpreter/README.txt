This is the directory of the Alan interpreter

Build using "make" which compiles and runs a number of unittests
which requires a 32bit version of Cgreen unittest and mock framework.

They can currently only be built in a 32bit environment (Cygwin32) because
the interpreter stores some pointers in 32bit words to fit into the 32bit
architecture of the Amachine (see Acode.h).

Makefiles assumes that the libraries are in $(CGREENLIB) and the binaries
in $(CGREENBIN). E.g. you can put 32bit libs in $(CGREENROT)/lib32 and the
runner in $(CGREENROT)/bin32 to ensure full 32-bit handling on a 64-bit
system. You can persuade Cgreen to place them there by 'ccmake' its build
parameters.
