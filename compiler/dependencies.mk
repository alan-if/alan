adv.o: adv.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 lmList.h alanCommon.h token.h sym_x.h sym.h elm.h id.h acode.h atr.h \
 exp.h wht.h whr.h evt.h whr_x.h ins_x.h ins.h slt.h cnt.h cla_x.h \
 cla.h lst_x.h adv.h dump.h stm.h vrb.h stx.h ext.h rul.h wrd.h syn.h \
 atr_x.h msg.h sco.h opt.h options.h emit.h encode.h
advTest.o: advTest.c adv.c alan.h sysdep.h types.h lst.h util.h srcp.h \
 srcp_x.h lmList.h alanCommon.h token.h sym_x.h sym.h elm.h id.h \
 acode.h atr.h exp.h wht.h whr.h evt.h whr_x.h ins_x.h ins.h slt.h \
 cnt.h cla_x.h cla.h lst_x.h adv.h dump.h stm.h vrb.h stx.h ext.h \
 rul.h wrd.h syn.h atr_x.h msg.h sco.h opt.h options.h emit.h encode.h
alan.o: alan.c sysdep.h types.h util.h srcp.h alan.h lst.h \
 alan.version.h version.h srcp_x.h adv.h whr.h wht.h id.h sym.h dump.h \
 opt.h acode.h lmList.h alanCommon.h token.h pmParse.h smScan.h \
 lst_x.h options.h timing.h
alan.version.o: alan.version.c alan.version.h version.h
alt.o: alt.c util.h types.h srcp.h lmList.h alanCommon.h token.h \
 srcp_x.h id_x.h id.h sym.h lst.h lst_x.h alt.h ins.h slt.h whr.h \
 wht.h cnt.h acode.h chk.h exp.h alan.h sysdep.h evt.h stm.h elm.h \
 emit.h dump.h
atr.o: atr.c atr_x.h atr.h srcp.h sym.h types.h lst.h id.h exp.h \
 alan.h sysdep.h wht.h whr.h evt.h acode.h elm.h srcp_x.h id_x.h \
 lst_x.h util.h emit.h adv.h dump.h ins.h slt.h cnt.h opt.h lmList.h \
 alanCommon.h token.h encode.h
atrTest.o: atrTest.c atr.c atr_x.h atr.h srcp.h sym.h types.h lst.h \
 id.h exp.h alan.h sysdep.h wht.h whr.h evt.h acode.h elm.h srcp_x.h \
 id_x.h lst_x.h util.h emit.h adv.h dump.h ins.h slt.h cnt.h opt.h \
 lmList.h alanCommon.h token.h encode.h
chk.o: chk.c util.h types.h srcp.h options.h lst.h dump.h sysdep.h \
 lst_x.h exp.h alan.h wht.h id.h sym.h whr.h evt.h acode.h stm.h ins.h \
 slt.h cnt.h chk.h emit.h
cla.o: cla.c cla_x.h cla.h types.h id.h sym.h lst.h srcp.h slt.h whr.h \
 wht.h cnt.h acode.h srcp_x.h id_x.h sym_x.h elm.h atr.h exp.h alan.h \
 sysdep.h evt.h slt_x.h lst_x.h emit.h util.h dump.h lmList.h \
 alanCommon.h token.h
claTest.o: claTest.c cla.c cla_x.h cla.h types.h id.h sym.h lst.h \
 srcp.h slt.h whr.h wht.h cnt.h acode.h srcp_x.h id_x.h sym_x.h elm.h \
 atr.h exp.h alan.h sysdep.h evt.h slt_x.h lst_x.h emit.h util.h \
 dump.h lmList.h alanCommon.h token.h adv.h ins_x.h ins.h \
 ../interpreter/types.h ../interpreter/sysdep.h ../interpreter/acode.h
cnt.o: cnt.c cnt_x.h cnt.h acode.h srcp.h id.h sym.h types.h lst.h \
 wht.h alan.h sysdep.h srcp_x.h sym_x.h elm.h atr.h exp.h whr.h evt.h \
 lmList.h alanCommon.h token.h adv.h dump.h stm.h ins.h slt.h lim.h \
 emit.h
dump.o: dump.c sysdep.h lmList.h alanCommon.h types.h token.h srcp.h
elm.o: elm.c util.h types.h srcp.h srcp_x.h id_x.h id.h sym.h lst.h \
 lst_x.h lmList.h alanCommon.h token.h stx.h acode.h elm.h wrd.h \
 emit.h dump.h sysdep.h
emit.o: emit.c sysdep.h types.h util.h srcp.h alan.h lst.h acode.h \
 alan.version.h version.h emit.h
emitTest.o: emitTest.c emit.c sysdep.h types.h util.h srcp.h alan.h \
 lst.h acode.h alan.version.h version.h emit.h
encode.o: encode.c alan.h sysdep.h types.h lst.h util.h srcp.h acode.h \
 opt.h emit.h encode.h
evt.o: evt.c util.h types.h srcp.h srcp_x.h sym_x.h sym.h lst.h elm.h \
 id.h acode.h atr.h exp.h alan.h sysdep.h wht.h whr.h evt.h id_x.h \
 lst_x.h lmList.h alanCommon.h token.h adv.h dump.h stm.h ins.h slt.h \
 cnt.h opt.h emit.h
exp.o: exp.c util.h types.h srcp.h srcp_x.h whr_x.h whr.h wht.h id.h \
 sym.h lst.h evt.h acode.h id_x.h atr_x.h atr.h exp.h alan.h sysdep.h \
 elm.h sym_x.h wht_x.h lmList.h alanCommon.h token.h adv.h dump.h \
 ins.h slt.h cnt.h emit.h encode.h
ext.o: ext.c ext_x.h ext.h acode.h srcp.h lst.h id.h sym.h types.h \
 alan.h sysdep.h util.h srcp_x.h sym_x.h elm.h atr.h exp.h wht.h whr.h \
 evt.h id_x.h lst_x.h stm.h ins.h slt.h cnt.h chk.h wrd.h emit.h \
 lmList.h alanCommon.h token.h dump.h
extTest.o: extTest.c ext.c ext_x.h ext.h acode.h srcp.h lst.h id.h \
 sym.h types.h alan.h sysdep.h util.h srcp_x.h sym_x.h elm.h atr.h \
 exp.h wht.h whr.h evt.h id_x.h lst_x.h stm.h ins.h slt.h cnt.h chk.h \
 wrd.h emit.h lmList.h alanCommon.h token.h dump.h
id.o: id.c id_x.h id.h sym.h types.h lst.h srcp.h srcp_x.h sym_x.h \
 elm.h acode.h atr.h exp.h alan.h sysdep.h wht.h whr.h evt.h emit.h \
 str.h util.h dump.h lmList.h alanCommon.h token.h
ins.o: ins.c ins_x.h ins.h srcp.h id.h sym.h types.h lst.h slt.h whr.h \
 wht.h cnt.h acode.h sym_x.h elm.h atr.h exp.h alan.h sysdep.h evt.h \
 slt_x.h id_x.h srcp_x.h lst_x.h util.h dump.h emit.h adv.h lmList.h \
 alanCommon.h token.h
insTest.o: insTest.c ins.c ins_x.h ins.h srcp.h id.h sym.h types.h \
 lst.h slt.h whr.h wht.h cnt.h acode.h sym_x.h elm.h atr.h exp.h \
 alan.h sysdep.h evt.h slt_x.h id_x.h srcp_x.h lst_x.h util.h dump.h \
 emit.h adv.h lmList.h alanCommon.h token.h
lim.o: lim.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 atr_x.h atr.h sym.h id.h exp.h wht.h whr.h evt.h acode.h elm.h \
 lst_x.h sym_x.h lmList.h alanCommon.h token.h adv.h dump.h stm.h \
 ins.h slt.h cnt.h chk.h vrb.h stx.h lim.h ext.h rul.h emit.h
lmList.o: lmList.c lmList.h alanCommon.h types.h token.h srcp.h
lst.o: lst.c lst_x.h lst.h util.h types.h srcp.h dump.h sysdep.h
macSpa.o: macSpa.c
main.o: main.c util.h types.h srcp.h alan.h sysdep.h lst.h spa.h \
 options.h dump.h alan.version.h version.h
msg.o: msg.c sysdep.h alan.h types.h lst.h util.h srcp.h srcp_x.h \
 lst_x.h lmList.h alanCommon.h token.h adv.h whr.h wht.h id.h sym.h \
 dump.h msg.h acode.h stm.h ins.h slt.h cnt.h exp.h evt.h opt.h emit.h \
 encode.h
nam.o: nam.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 lmList.h alanCommon.h token.h nam_x.h nam.h id.h sym.h str.h cnt.h \
 acode.h ext.h vrb.h ins.h slt.h whr.h wht.h stx.h msg.h dump.h emit.h
opt.o: opt.c alan.h sysdep.h types.h lst.h srcp_x.h srcp.h lmList.h \
 alanCommon.h token.h acode.h opt.h emit.h
options.o: options.c types.h dump.h sysdep.h options.h lst.h
pmErr.o: pmErr.c util.h types.h srcp.h smScan.h alanCommon.h token.h \
 sysdep.h lst_x.h lst.h id_x.h id.h sym.h ins_x.h ins.h slt.h whr.h \
 wht.h cnt.h acode.h cla_x.h cla.h sym_x.h elm.h atr.h exp.h alan.h \
 evt.h res_x.h res.h stx.h whr_x.h slt_x.h ext_x.h ext.h atr_x.h \
 lmList.h adv.h dump.h alt.h chk.h lim.h msg.h opt.h rul.h sco.h scr.h \
 stm.h stp.h str.h syn.h vrb.h
pmPaSema.o: pmPaSema.c util.h types.h srcp.h smScan.h alanCommon.h \
 token.h sysdep.h lst_x.h lst.h id_x.h id.h sym.h ins_x.h ins.h slt.h \
 whr.h wht.h cnt.h acode.h cla_x.h cla.h sym_x.h elm.h atr.h exp.h \
 alan.h evt.h res_x.h res.h stx.h whr_x.h slt_x.h ext_x.h ext.h \
 atr_x.h lmList.h adv.h dump.h alt.h chk.h lim.h msg.h opt.h rul.h \
 sco.h scr.h stm.h stp.h str.h syn.h vrb.h
pmParse.o: pmParse.c util.h types.h srcp.h smScan.h alanCommon.h \
 token.h sysdep.h lst_x.h lst.h id_x.h id.h sym.h ins_x.h ins.h slt.h \
 whr.h wht.h cnt.h acode.h cla_x.h cla.h sym_x.h elm.h atr.h exp.h \
 alan.h evt.h res_x.h res.h stx.h whr_x.h slt_x.h ext_x.h ext.h \
 atr_x.h lmList.h adv.h dump.h alt.h chk.h lim.h msg.h opt.h rul.h \
 sco.h scr.h stm.h stp.h str.h syn.h vrb.h pmParse.h
res.o: res.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 lmList.h alanCommon.h token.h res_x.h res.h id.h sym.h acode.h stx.h \
 sym_x.h elm.h atr.h exp.h wht.h whr.h evt.h id_x.h lst_x.h stm.h \
 ins.h slt.h cnt.h emit.h dump.h
rul.o: rul.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 lst_x.h lmList.h alanCommon.h token.h acode.h adv.h whr.h wht.h id.h \
 sym.h dump.h exp.h evt.h stm.h ins.h slt.h cnt.h rul.h emit.h
sco.o: sco.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 lmList.h alanCommon.h token.h adv.h whr.h wht.h id.h sym.h dump.h \
 sco.h acode.h emit.h
scr.o: scr.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 id_x.h id.h sym.h lst_x.h lmList.h alanCommon.h token.h acode.h stm.h \
 wht.h whr.h ins.h slt.h cnt.h exp.h evt.h stp.h scr.h emit.h dump.h
slt.o: slt.c slt_x.h slt.h types.h lst.h whr.h wht.h id.h sym.h srcp.h \
 cnt.h acode.h alan.h sysdep.h util.h dump.h emit.h lmList.h \
 alanCommon.h token.h stm.h ins.h exp.h evt.h nam_x.h nam.h whr_x.h \
 cla_x.h cla.h sym_x.h elm.h atr.h cnt_x.h ext_x.h ext.h id_x.h \
 atr_x.h lst_x.h
smScSema.o: smScSema.c sysdep.h types.h alan.h lst.h lmList.h \
 alanCommon.h token.h srcp.h encode.h acode.h smScan.h lst_x.h str.h
smScan.o: smScan.c sysdep.h types.h alan.h lst.h lmList.h alanCommon.h \
 token.h srcp.h encode.h acode.h smScan.h lst_x.h
smScanx.o: smScanx.c sysdep.h types.h alan.h lst.h lmList.h \
 alanCommon.h token.h srcp.h encode.h acode.h smScan.h lst_x.h
spa.o: spa.c spa.h
srcp.o: srcp.c alan.h sysdep.h types.h lst.h dump.h srcp_x.h srcp.h
stm.o: stm.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 sym_x.h sym.h elm.h id.h acode.h atr.h exp.h wht.h whr.h evt.h id_x.h \
 whr_x.h atr_x.h lst_x.h lmList.h alanCommon.h token.h adv.h dump.h \
 stm.h ins.h slt.h cnt.h scr.h sco.h opt.h emit.h encode.h
stp.o: stp.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 lst_x.h lmList.h alanCommon.h token.h exp.h wht.h id.h sym.h whr.h \
 evt.h acode.h stm.h ins.h slt.h cnt.h stp.h emit.h dump.h
str.o: str.c str.h sysdep.h types.h util.h srcp.h dump.h
stx.o: stx.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 id_x.h id.h sym.h sym_x.h elm.h acode.h atr.h exp.h wht.h whr.h evt.h \
 res_x.h res.h stx.h lst_x.h lmList.h alanCommon.h token.h adv.h \
 dump.h wrd.h emit.h
sym.o: sym.c sym_x.h sym.h types.h lst.h elm.h srcp.h id.h acode.h \
 atr.h exp.h alan.h sysdep.h wht.h whr.h evt.h util.h lmList.h \
 alanCommon.h token.h srcp_x.h cla_x.h cla.h slt.h cnt.h id_x.h \
 atr_x.h
symTest.o: symTest.c sym.c sym_x.h sym.h types.h lst.h elm.h srcp.h \
 id.h acode.h atr.h exp.h alan.h sysdep.h wht.h whr.h evt.h util.h \
 lmList.h alanCommon.h token.h srcp_x.h cla_x.h cla.h slt.h cnt.h \
 id_x.h atr_x.h
syn.o: syn.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 lst_x.h lmList.h alanCommon.h token.h acode.h adv.h whr.h wht.h id.h \
 sym.h dump.h id_x.h syn.h wrd.h emit.h
sysdep.o: sysdep.c sysdep.h
timing.o: timing.c sysdep.h timing.h
unit.o: unit.c sysdep.h acode.h unitTest.h lmList.h alanCommon.h \
 types.h token.h srcp.h claTest.c cla.c cla_x.h cla.h id.h sym.h lst.h \
 slt.h whr.h wht.h cnt.h srcp_x.h id_x.h sym_x.h elm.h atr.h exp.h \
 alan.h evt.h slt_x.h lst_x.h emit.h util.h dump.h adv.h ins_x.h ins.h \
 ../interpreter/types.h ../interpreter/sysdep.h ../interpreter/acode.h \
 insTest.c ins.c advTest.c adv.c whr_x.h stm.h vrb.h stx.h ext.h rul.h \
 wrd.h syn.h atr_x.h msg.h sco.h opt.h options.h encode.h symTest.c \
 sym.c whrTest.c whr.c wht_x.h extTest.c ext.c ext_x.h chk.h \
 emitTest.c emit.c alan.version.h version.h atrTest.c atr.c
unitList.o: unitList.c lmList.h alanCommon.h types.h token.h srcp.h
util.o: util.c util.h types.h srcp.h sysdep.h lmList.h alanCommon.h \
 token.h
vrb.o: vrb.c vrb.h srcp.h lst.h ins.h id.h sym.h types.h slt.h whr.h \
 wht.h cnt.h acode.h stx.h util.h srcp_x.h sym_x.h elm.h atr.h exp.h \
 alan.h sysdep.h evt.h lst_x.h id_x.h lmList.h alanCommon.h token.h \
 adv.h dump.h alt.h emit.h
whr.o: whr.c whr_x.h whr.h wht.h id.h sym.h types.h lst.h srcp.h evt.h \
 acode.h alan.h sysdep.h util.h srcp_x.h wht_x.h sym_x.h elm.h atr.h \
 exp.h id_x.h lmList.h alanCommon.h token.h cnt.h dump.h emit.h
whrTest.o: whrTest.c whr.c whr_x.h whr.h wht.h id.h sym.h types.h \
 lst.h srcp.h evt.h acode.h alan.h sysdep.h util.h srcp_x.h wht_x.h \
 sym_x.h elm.h atr.h exp.h id_x.h lmList.h alanCommon.h token.h cnt.h \
 dump.h emit.h
wht.o: wht.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 wht_x.h wht.h id.h sym.h id_x.h lmList.h alanCommon.h token.h emit.h \
 acode.h dump.h
wrd.o: wrd.c alan.h sysdep.h types.h lst.h util.h srcp.h srcp_x.h \
 id_x.h id.h sym.h lst_x.h lmList.h alanCommon.h token.h acode.h wrd.h \
 opt.h emit.h dump.h
