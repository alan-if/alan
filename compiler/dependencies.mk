act.o : act.c alan.h sysdep.h types.h srcp.h adv.h lst.h dump.h whr.h evt.h \
  nam.h acode.h wht.h sym.h elm.h cla.h stx.h exp.h atr.h emit.h stm.h obj.h cnt.h \
  act.h loc.h ext.h scr.h wrd.h vrb.h lmList.h alanCommon.h token.h 
adv.o : adv.c alan.h sysdep.h types.h srcp.h lmList.h alanCommon.h token.h acode.h \
  adv.h lst.h dump.h whr.h evt.h nam.h wht.h sym.h elm.h cla.h stx.h stm.h exp.h \
  obj.h cnt.h act.h loc.h ext.h vrb.h rul.h wrd.h syn.h atr.h emit.h msg.h sco.h \
  opt.h encode.h 
alan.o : alan.c sysdep.h types.h alan.h srcp.h version.h adv.h lst.h dump.h \
  whr.h evt.h nam.h acode.h wht.h opt.h lmList.h alanCommon.h token.h pmParse.h \
  smScan.h time.h spa.h 
