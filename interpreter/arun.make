! Makefile for arun on VAX		      Date: 91-09-06/THONI@_GOLLUM
!

CQ =/STANDARD=PORTABLE

OBJECTS = arun.obj debug.obj exe.obj inter.obj parse.obj rules.obj stack.obj params.obj term.obj decode.obj sysdep.obj

arun.obj : arun.c

debug.obj : debug.c

exe.obj : exe.c

inter.obj : inter.c

parse.obj : parse.c

rules.obj : rules.c

stack.obj : stack.c

term.obj : term.c

params.obj : params.c

decode.obj : decode.c

sysdep.obj : sysdep.c

version.obj : version.c

arun.exe : #(OBJECTS) version.obj
	$ link/exe=arun #(LQ) arun.obj,debug.obj,exe.obj,inter.obj,parse.obj,rules.obj,stack.obj,params.obj,term.obj,decode.obj,sysdep.obj,version.obj
	$ copy arun.exe <->

