! Makefile for alan on VAX/VMS
!

CQ =/STANDARD=PORTABLE

OBJECTS = pmPaSema.obj pmParse.obj pmErr.obj smScan.obj -
smScSema.obj lmList.obj alan.obj act.obj adv.obj -
alt.obj atr.obj chk.obj res.obj cnt.obj dump.obj elm.obj -
emit.obj encode.obj evt.obj exp.obj ext.obj lim.obj -
loc.obj lst.obj msg.obj nam.obj obj.obj opt.obj res.obj rul.obj -
sco.obj scr.obj srcp.obj stm.obj stp.obj str.obj stx.obj -
sym.obj syn.obj vrb.obj whr.obj wht.obj wrd.obj -
time.obj sysdep.obj spa.obj version.obj

pmPaSema.obj : pmPaSema.c
pmParse.obj : pmParse.c
pmErr.obj : pmErr.c
smScan.obj : smScan.c
smScSema.obj : smScSema.c
lmList.obj : lmList.c
alan.obj : alan.c
act.obj : act.c
adv.obj : adv.c
alt.obj : alt.c
atr.obj : atr.c
chk.obj : chk.c
res.obj : res.c
cnt.obj : cnt.c
dump.obj : dump.c
elm.obj : elm.c
emit.obj : emit.c
encode.obj : encode.c
evt.obj : evt.c
exp.obj : exp.c
ext.obj : ext.c
lim.obj : lim.c
loc.obj : loc.c
lst.obj : lst.c
msg.obj : msg.c
nam.obj : nam.c
obj.obj : obj.c
opt.obj : opt.c
res.obj : res.c
rul.obj : rul.c
sco.obj : sco.c
scr.obj : scr.c
srcp.obj : srcp.c
stm.obj : stm.c
stp.obj : stp.c
str.obj : str.c
stx.obj : stx.c
sym.obj : sym.c
syn.obj : syn.c
vrb.obj : vrb.c
whr.obj : whr.c
wht.obj : wht.c
wrd.obj : wrd.c
time.obj : time.c
spa.obj : spa.c
sysdep.obj : sysdep.c
version.obj : version.c

alan.exe : #(OBJECTS) sysdep.obj version.obj
	$ link/exe=alan #(LQ) ACT,ADV,ALAN,PMERR,LMLIST,PMPARSE,PMPASEMA,SMSCAN,SMSCSEMA,ALT,ATR,CHK,CNT,DUMP,EMIT,ENCODE,ELM,EVT,EXP,EXT,LIM,LOC,LST,MSG,NAM,OBJ,OPT,RES,RUL,SCO,SCR,SPA,SRCP,STM,STP,STR,STX,SYM,SYN,SYSDEP,TIME,VERSION,VRB,WHR,WHT,WRD
	$ copy alan.exe <->
