act.o: act.c alan.h sysdep.h types.h srcp.h lst.h dump.h adv.h whr.h \
 evt.h nam.h acode.h wht.h sym.h elm.h res.h stx.h exp.h atr.h emit.h \
 stm.h obj.h cnt.h act.h loc.h ext.h scr.h wrd.h vrb.h lmList.h \
 alanCommon.h token.h
adv.o: adv.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h sym.h \
 elm.h res.h stx.h stm.h exp.h obj.h cnt.h act.h loc.h ext.h vrb.h \
 rul.h wrd.h syn.h atr.h emit.h msg.h sco.h opt.h encode.h
alan.o: alan.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 alan.version.h version.h adv.h whr.h evt.h nam.h acode.h wht.h opt.h \
 lmList.h alanCommon.h token.h pmParse.h smScan.h options.h timing.h
alan.version.o: alan.version.c alan.version.h version.h
alt.o: alt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h alt.h \
 act.h cnt.h wht.h whr.h evt.h chk.h exp.h stm.h obj.h loc.h ext.h \
 emit.h
atr.o: atr.c alan.h sysdep.h types.h srcp.h lst.h dump.h adv.h whr.h \
 evt.h nam.h acode.h wht.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 obj.h cnt.h loc.h ext.h act.h opt.h lmList.h alanCommon.h token.h \
 encode.h
chk.o: chk.c alan.h sysdep.h types.h srcp.h lst.h dump.h exp.h wht.h \
 nam.h acode.h whr.h evt.h stm.h obj.h cnt.h act.h loc.h ext.h chk.h \
 emit.h
cnt.o: cnt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h sym.h \
 elm.h res.h stx.h stm.h exp.h obj.h cnt.h act.h loc.h ext.h lim.h \
 atr.h emit.h
dump.o: dump.c alan.h sysdep.h types.h srcp.h lst.h dump.h adv.h whr.h \
 evt.h nam.h acode.h wht.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 stm.h obj.h cnt.h act.h loc.h ext.h chk.h vrb.h lim.h scr.h stp.h \
 rul.h lmList.h alanCommon.h token.h
elm.o: elm.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h wrd.h \
 emit.h
emit.o: emit.c sysdep.h types.h alan.h srcp.h lst.h dump.h acode.h \
 alan.version.h version.h emit.h
encode.o: encode.c alan.h sysdep.h types.h srcp.h lst.h dump.h acode.h \
 opt.h emit.h encode.h
evt.o: evt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h stm.h exp.h obj.h cnt.h act.h loc.h ext.h opt.h \
 emit.h
exp.o: exp.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h obj.h cnt.h loc.h ext.h act.h \
 encode.h
ext.o: ext.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h stm.h \
 wht.h whr.h evt.h exp.h obj.h cnt.h act.h loc.h ext.h chk.h wrd.h \
 emit.h
lim.o: lim.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h exp.h \
 atr.h sym.h elm.h res.h stx.h emit.h stm.h obj.h cnt.h act.h loc.h \
 ext.h chk.h vrb.h lim.h rul.h
lmList.o: lmList.c lmList.h alanCommon.h srcp.h token.h
loc.o: loc.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h loc.h ext.h stm.h obj.h cnt.h \
 act.h vrb.h
lst.o: lst.c alan.h sysdep.h types.h srcp.h lst.h dump.h nam.h acode.h \
 act.h cnt.h wht.h whr.h evt.h alt.h atr.h sym.h elm.h res.h stx.h \
 exp.h emit.h chk.h ext.h lim.h loc.h obj.h rul.h scr.h stm.h stp.h \
 syn.h vrb.h
macSpa.o: macSpa.c
main.o: main.c alan.h sysdep.h types.h srcp.h lst.h dump.h spa.h \
 options.h alan.version.h version.h
msg.o: msg.c sysdep.h alan.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h msg.h \
 stm.h exp.h obj.h cnt.h act.h loc.h ext.h opt.h emit.h encode.h
nam.o: nam.c alan.h sysdep.h types.h srcp.h lst.h dump.h nam.h acode.h \
 str.h obj.h whr.h evt.h wht.h cnt.h act.h ext.h loc.h vrb.h stx.h \
 msg.h emit.h
obj.o: obj.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h stm.h obj.h cnt.h act.h loc.h \
 ext.h wrd.h vrb.h
opt.o: opt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h opt.h emit.h
options.o: options.c types.h dump.h sysdep.h options.h lst.h
pmErr.o: pmErr.c srcp.h smScan.h alanCommon.h token.h sysdep.h types.h \
 lst.h dump.h lmList.h act.h nam.h alan.h acode.h cnt.h wht.h whr.h \
 evt.h adv.h alt.h atr.h sym.h elm.h res.h stx.h exp.h emit.h chk.h \
 ext.h lim.h loc.h msg.h obj.h opt.h rul.h sco.h scr.h stm.h stp.h \
 str.h syn.h vrb.h
pmPaSema.o: pmPaSema.c srcp.h smScan.h alanCommon.h token.h sysdep.h \
 types.h lst.h dump.h lmList.h act.h nam.h alan.h acode.h cnt.h wht.h \
 whr.h evt.h adv.h alt.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 chk.h ext.h lim.h loc.h msg.h obj.h opt.h rul.h sco.h scr.h stm.h \
 stp.h str.h syn.h vrb.h
pmParse.o: pmParse.c srcp.h smScan.h alanCommon.h token.h sysdep.h \
 types.h lst.h dump.h lmList.h act.h nam.h alan.h acode.h cnt.h wht.h \
 whr.h evt.h adv.h alt.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 chk.h ext.h lim.h loc.h msg.h obj.h opt.h rul.h sco.h scr.h stm.h \
 stp.h str.h syn.h vrb.h pmParse.h
res.o: res.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h res.h stx.h nam.h acode.h sym.h elm.h stm.h \
 wht.h whr.h evt.h exp.h obj.h cnt.h act.h loc.h ext.h emit.h
rul.o: rul.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h exp.h \
 stm.h obj.h cnt.h act.h loc.h ext.h rul.h emit.h
sco.o: sco.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sco.h \
 emit.h
scr.o: scr.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h nam.h stm.h wht.h whr.h evt.h exp.h \
 obj.h cnt.h act.h loc.h ext.h stp.h scr.h emit.h
smScSema.o: smScSema.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 lmList.h alanCommon.h token.h encode.h acode.h smScan.h str.h
smScan.o: smScan.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 lmList.h alanCommon.h token.h encode.h acode.h smScan.h
smScanx.o: smScanx.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 lmList.h alanCommon.h token.h encode.h acode.h smScan.h
spa.o: spa.c spa.h
srcp.o: srcp.c alan.h sysdep.h types.h srcp.h lst.h dump.h
stm.o: stm.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h stm.h obj.h cnt.h act.h loc.h \
 ext.h scr.h sco.h opt.h encode.h
stp.o: stp.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h exp.h wht.h nam.h acode.h whr.h evt.h stm.h \
 obj.h cnt.h act.h loc.h ext.h stp.h emit.h
str.o: str.c sysdep.h types.h alan.h srcp.h lst.h dump.h str.h
stx.o: stx.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h wrd.h emit.h
sym.o: sym.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h vrb.h \
 obj.h whr.h evt.h wht.h cnt.h act.h ext.h loc.h
syn.o: syn.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h syn.h \
 wrd.h emit.h
sysdep.o: sysdep.c sysdep.h
timing.o: timing.c sysdep.h timing.h
vrb.o: vrb.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h alt.h \
 act.h cnt.h sym.h elm.h res.h stx.h obj.h vrb.h emit.h
whr.o: whr.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h nam.h sym.h elm.h res.h stx.h cnt.h \
 wht.h whr.h evt.h emit.h
wht.o: wht.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h wht.h nam.h acode.h emit.h
wrd.o: wrd.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h nam.h wrd.h opt.h emit.h
act.o: act.c alan.h sysdep.h types.h srcp.h lst.h dump.h adv.h whr.h \
 evt.h nam.h acode.h wht.h sym.h elm.h res.h stx.h exp.h atr.h emit.h \
 stm.h obj.h cnt.h act.h loc.h ext.h scr.h wrd.h vrb.h lmList.h \
 alanCommon.h token.h
adv.o: adv.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h sym.h \
 elm.h res.h stx.h stm.h exp.h obj.h cnt.h act.h loc.h ext.h vrb.h \
 rul.h wrd.h syn.h atr.h emit.h msg.h sco.h opt.h encode.h
alan.o: alan.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 alan.version.h version.h adv.h whr.h evt.h nam.h acode.h wht.h opt.h \
 lmList.h alanCommon.h token.h pmParse.h smScan.h options.h timing.h
alan.version.o: alan.version.c alan.version.h version.h
alt.o: alt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h alt.h \
 act.h cnt.h wht.h whr.h evt.h chk.h exp.h stm.h obj.h loc.h ext.h \
 emit.h
atr.o: atr.c alan.h sysdep.h types.h srcp.h lst.h dump.h adv.h whr.h \
 evt.h nam.h acode.h wht.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 obj.h cnt.h loc.h ext.h act.h opt.h lmList.h alanCommon.h token.h \
 encode.h
chk.o: chk.c alan.h sysdep.h types.h srcp.h lst.h dump.h exp.h wht.h \
 nam.h acode.h whr.h evt.h stm.h obj.h cnt.h act.h loc.h ext.h chk.h \
 emit.h
cla.o: cla.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h cla.h nam.h acode.h whr.h evt.h wht.h slt.h \
 cnt.h
cnt.o: cnt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h sym.h \
 elm.h res.h stx.h stm.h exp.h obj.h cnt.h act.h loc.h ext.h lim.h \
 atr.h emit.h
dump.o: dump.c alan.h sysdep.h types.h srcp.h lst.h dump.h adv.h whr.h \
 evt.h nam.h acode.h wht.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 stm.h obj.h cnt.h act.h loc.h ext.h chk.h vrb.h lim.h scr.h stp.h \
 rul.h lmList.h alanCommon.h token.h
elm.o: elm.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h wrd.h \
 emit.h
emit.o: emit.c sysdep.h types.h alan.h srcp.h lst.h dump.h acode.h \
 alan.version.h version.h emit.h
encode.o: encode.c alan.h sysdep.h types.h srcp.h lst.h dump.h acode.h \
 opt.h emit.h encode.h
evt.o: evt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h stm.h exp.h obj.h cnt.h act.h loc.h ext.h opt.h \
 emit.h
exp.o: exp.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h obj.h cnt.h loc.h ext.h act.h \
 encode.h
ext.o: ext.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h stm.h \
 wht.h whr.h evt.h exp.h obj.h cnt.h act.h loc.h ext.h chk.h wrd.h \
 emit.h
ins.o: ins.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h ins.h nam.h acode.h whr.h evt.h wht.h slt.h \
 cnt.h
lim.o: lim.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h exp.h \
 atr.h sym.h elm.h res.h stx.h emit.h stm.h obj.h cnt.h act.h loc.h \
 ext.h chk.h vrb.h lim.h rul.h
lmList.o: lmList.c lmList.h alanCommon.h srcp.h token.h
loc.o: loc.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h loc.h ext.h stm.h obj.h cnt.h \
 act.h vrb.h
lst.o: lst.c alan.h sysdep.h types.h srcp.h lst.h dump.h nam.h acode.h \
 act.h cnt.h wht.h whr.h evt.h alt.h atr.h sym.h elm.h res.h stx.h \
 exp.h emit.h chk.h ext.h lim.h cla.h slt.h ins.h loc.h obj.h rul.h \
 scr.h stm.h stp.h syn.h vrb.h
macSpa.o: macSpa.c
main.o: main.c alan.h sysdep.h types.h srcp.h lst.h dump.h spa.h \
 options.h alan.version.h version.h
msg.o: msg.c sysdep.h alan.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h msg.h \
 stm.h exp.h obj.h cnt.h act.h loc.h ext.h opt.h emit.h encode.h
nam.o: nam.c alan.h sysdep.h types.h srcp.h lst.h dump.h nam.h acode.h \
 str.h obj.h whr.h evt.h wht.h cnt.h act.h ext.h loc.h vrb.h stx.h \
 msg.h emit.h
obj.o: obj.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h stm.h obj.h cnt.h act.h loc.h \
 ext.h wrd.h vrb.h
opt.o: opt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h opt.h emit.h
options.o: options.c types.h dump.h sysdep.h options.h lst.h
pmErr.o: pmErr.c srcp.h smScan.h alanCommon.h token.h sysdep.h types.h \
 lst.h dump.h lmList.h act.h nam.h alan.h acode.h cnt.h wht.h whr.h \
 evt.h adv.h alt.h atr.h sym.h elm.h res.h stx.h exp.h emit.h chk.h \
 cla.h slt.h ext.h ins.h lim.h loc.h msg.h obj.h opt.h rul.h sco.h \
 scr.h stm.h stp.h str.h syn.h vrb.h
pmPaSema.o: pmPaSema.c srcp.h smScan.h alanCommon.h token.h sysdep.h \
 types.h lst.h dump.h lmList.h act.h nam.h alan.h acode.h cnt.h wht.h \
 whr.h evt.h adv.h alt.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 chk.h cla.h slt.h ext.h ins.h lim.h loc.h msg.h obj.h opt.h rul.h \
 sco.h scr.h stm.h stp.h str.h syn.h vrb.h
pmParse.o: pmParse.c srcp.h smScan.h alanCommon.h token.h sysdep.h \
 types.h lst.h dump.h lmList.h act.h nam.h alan.h acode.h cnt.h wht.h \
 whr.h evt.h adv.h alt.h atr.h sym.h elm.h res.h stx.h exp.h emit.h \
 chk.h cla.h slt.h ext.h ins.h lim.h loc.h msg.h obj.h opt.h rul.h \
 sco.h scr.h stm.h stp.h str.h syn.h vrb.h pmParse.h
res.o: res.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h res.h stx.h nam.h acode.h sym.h elm.h stm.h \
 wht.h whr.h evt.h exp.h obj.h cnt.h act.h loc.h ext.h emit.h
rul.o: rul.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h exp.h \
 stm.h obj.h cnt.h act.h loc.h ext.h rul.h emit.h
sco.o: sco.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sco.h \
 emit.h
scr.o: scr.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h nam.h stm.h wht.h whr.h evt.h exp.h \
 obj.h cnt.h act.h loc.h ext.h stp.h scr.h emit.h
slt.o: slt.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h slt.h nam.h acode.h whr.h evt.h wht.h cnt.h
smScSema.o: smScSema.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 lmList.h alanCommon.h token.h encode.h acode.h smScan.h str.h
smScan.o: smScan.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 lmList.h alanCommon.h token.h encode.h acode.h smScan.h
smScanx.o: smScanx.c sysdep.h types.h alan.h srcp.h lst.h dump.h \
 lmList.h alanCommon.h token.h encode.h acode.h smScan.h
spa.o: spa.c spa.h
srcp.o: srcp.c alan.h sysdep.h types.h srcp.h lst.h dump.h
stm.o: stm.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h exp.h atr.h emit.h stm.h obj.h cnt.h act.h loc.h \
 ext.h scr.h sco.h opt.h encode.h
stp.o: stp.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h exp.h wht.h nam.h acode.h whr.h evt.h stm.h \
 obj.h cnt.h act.h loc.h ext.h stp.h emit.h
str.o: str.c sysdep.h types.h alan.h srcp.h lst.h dump.h str.h
stx.o: stx.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h nam.h acode.h wht.h sym.h \
 elm.h res.h stx.h wrd.h emit.h
sym.o: sym.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h sym.h nam.h acode.h elm.h res.h stx.h vrb.h \
 obj.h whr.h evt.h wht.h cnt.h act.h ext.h loc.h
syn.o: syn.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h syn.h \
 wrd.h emit.h
sysdep.o: sysdep.c sysdep.h
timing.o: timing.c sysdep.h timing.h
vrb.o: vrb.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h nam.h wht.h alt.h \
 act.h cnt.h sym.h elm.h res.h stx.h obj.h vrb.h emit.h
whr.o: whr.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h nam.h sym.h elm.h res.h stx.h cnt.h \
 wht.h whr.h evt.h emit.h
wht.o: wht.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h wht.h nam.h acode.h emit.h
wrd.o: wrd.c alan.h sysdep.h types.h srcp.h lst.h dump.h lmList.h \
 alanCommon.h token.h acode.h nam.h wrd.h opt.h emit.h
act.o: act.c util.h srcp.h types.h adv.h lst.h whr.h evt.h id.h sym.h \
 acode.h wht.h dump.h sysdep.h exp.h alan.h nam.h elm.h atr.h emit.h \
 stm.h ins.h slt.h cnt.h scr.h act.h loc.h ext.h wrd.h vrb.h stx.h \
 lmList.h alanCommon.h token.h
adv.o: adv.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h wht.h \
 dump.h stm.h ins.h slt.h cnt.h exp.h nam.h elm.h vrb.h stx.h ext.h \
 rul.h wrd.h syn.h atr.h emit.h msg.h sco.h opt.h options.h encode.h
alan.o: alan.c sysdep.h types.h srcp.h util.h alan.h lst.h \
 alan.version.h version.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h opt.h lmList.h alanCommon.h token.h pmParse.h smScan.h \
 options.h timing.h
alan.version.o: alan.version.c alan.version.h version.h
alt.o: alt.c util.h srcp.h types.h lmList.h alanCommon.h token.h sym.h \
 lst.h nam.h alan.h sysdep.h elm.h id.h acode.h alt.h chk.h exp.h \
 wht.h whr.h evt.h ins.h slt.h cnt.h stm.h emit.h dump.h
atr.o: atr.c util.h srcp.h types.h adv.h lst.h whr.h evt.h id.h sym.h \
 acode.h wht.h dump.h sysdep.h atr.h alan.h nam.h elm.h exp.h emit.h \
 ins.h slt.h cnt.h opt.h lmList.h alanCommon.h token.h encode.h
chk.o: chk.c util.h srcp.h types.h options.h lst.h dump.h sysdep.h \
 exp.h alan.h wht.h id.h sym.h whr.h evt.h acode.h nam.h elm.h stm.h \
 ins.h slt.h cnt.h chk.h emit.h
cla.o: cla.c cla.h srcp.h id.h types.h sym.h slt.h lst.h whr.h evt.h \
 acode.h wht.h cnt.h util.h dump.h sysdep.h lmList.h alanCommon.h \
 token.h
cnt.o: cnt.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h wht.h \
 dump.h nam.h elm.h stm.h ins.h slt.h cnt.h exp.h lim.h atr.h emit.h \
 obj.h
dump.o: dump.c alan.h sysdep.h types.h srcp.h lst.h adv.h whr.h evt.h \
 id.h sym.h acode.h wht.h dump.h nam.h elm.h atr.h exp.h emit.h stm.h \
 ins.h slt.h cnt.h chk.h vrb.h stx.h lim.h ext.h scr.h stp.h rul.h \
 lmList.h alanCommon.h token.h
elm.o: elm.c util.h srcp.h types.h lmList.h alanCommon.h token.h sym.h \
 lst.h stx.h id.h acode.h nam.h alan.h sysdep.h elm.h wrd.h emit.h \
 dump.h
emit.o: emit.c sysdep.h types.h srcp.h util.h alan.h lst.h acode.h \
 alan.version.h version.h emit.h
encode.o: encode.c alan.h sysdep.h types.h srcp.h lst.h util.h acode.h \
 opt.h emit.h encode.h
evt.o: evt.c util.h srcp.h types.h lmList.h alanCommon.h token.h adv.h \
 lst.h whr.h evt.h id.h sym.h acode.h wht.h dump.h sysdep.h nam.h \
 alan.h elm.h stm.h ins.h slt.h cnt.h exp.h opt.h emit.h
exp.o: exp.c util.h srcp.h types.h lmList.h alanCommon.h token.h adv.h \
 lst.h whr.h evt.h id.h sym.h acode.h wht.h dump.h sysdep.h exp.h \
 alan.h nam.h elm.h atr.h emit.h ins.h slt.h cnt.h encode.h
ext.o: ext.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h sym.h nam.h elm.h id.h acode.h stm.h wht.h whr.h \
 evt.h ins.h slt.h cnt.h exp.h chk.h ext.h wrd.h emit.h dump.h
id.o: id.c id.h types.h srcp.h sym.h str.h util.h dump.h sysdep.h
ins.o: ins.c ins.h srcp.h id.h types.h sym.h lst.h whr.h evt.h acode.h \
 wht.h slt.h cnt.h sysdep.h util.h dump.h lmList.h alanCommon.h \
 token.h
lim.o: lim.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h nam.h elm.h exp.h atr.h emit.h stm.h ins.h slt.h cnt.h chk.h \
 vrb.h stx.h lim.h ext.h rul.h
lmList.o: lmList.c lmList.h alanCommon.h srcp.h token.h
loc.o: loc.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h nam.h elm.h exp.h atr.h emit.h loc.h ext.h stm.h ins.h slt.h \
 cnt.h vrb.h stx.h
lst.o: lst.c lst.h util.h srcp.h types.h dump.h sysdep.h
macSpa.o: macSpa.c
main.o: main.c util.h srcp.h types.h alan.h sysdep.h lst.h spa.h \
 options.h dump.h alan.version.h version.h
msg.o: msg.c sysdep.h alan.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h msg.h stm.h ins.h slt.h cnt.h exp.h nam.h elm.h opt.h emit.h \
 encode.h
nam.o: nam.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h nam.h elm.h id.h sym.h acode.h str.h obj.h whr.h \
 evt.h wht.h cnt.h act.h ext.h loc.h vrb.h ins.h slt.h stx.h msg.h \
 dump.h emit.h
obj.o: obj.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h nam.h elm.h exp.h atr.h emit.h stm.h ins.h slt.h cnt.h obj.h \
 loc.h ext.h wrd.h vrb.h stx.h
opt.o: opt.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h acode.h opt.h emit.h
options.o: options.c types.h srcp.h dump.h sysdep.h options.h lst.h
pmErr.o: pmErr.c util.h srcp.h types.h smScan.h alanCommon.h token.h \
 sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h alt.h atr.h alan.h nam.h elm.h exp.h emit.h chk.h ins.h slt.h \
 cnt.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h sco.h scr.h \
 stm.h stp.h str.h syn.h vrb.h
pmPaSema.o: pmPaSema.c util.h srcp.h types.h smScan.h alanCommon.h \
 token.h sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h acode.h \
 wht.h dump.h alt.h atr.h alan.h nam.h elm.h exp.h emit.h chk.h ins.h \
 slt.h cnt.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h sco.h \
 scr.h stm.h stp.h str.h syn.h vrb.h
pmParse.o: pmParse.c util.h srcp.h types.h smScan.h alanCommon.h \
 token.h sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h acode.h \
 wht.h dump.h alt.h atr.h alan.h nam.h elm.h exp.h emit.h chk.h ins.h \
 slt.h cnt.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h sco.h \
 scr.h stm.h stp.h str.h syn.h vrb.h pmParse.h
res.o: res.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h res.h stx.h id.h sym.h acode.h nam.h elm.h stm.h \
 wht.h whr.h evt.h ins.h slt.h cnt.h exp.h emit.h dump.h
rul.o: rul.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h wht.h \
 dump.h exp.h nam.h elm.h stm.h ins.h slt.h cnt.h rul.h emit.h
sco.o: sco.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h sco.h emit.h
scr.o: scr.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h nam.h elm.h id.h sym.h stm.h wht.h whr.h \
 evt.h ins.h slt.h cnt.h exp.h stp.h scr.h act.h emit.h dump.h
slt.o: slt.c slt.h srcp.h lst.h whr.h evt.h id.h types.h sym.h acode.h \
 wht.h cnt.h util.h dump.h sysdep.h
smScSema.o: smScSema.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h str.h
smScan.o: smScan.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h
smScanx.o: smScanx.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h
spa.o: spa.c spa.h
srcp.o: srcp.c alan.h sysdep.h types.h srcp.h lst.h dump.h
stm.o: stm.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h exp.h nam.h elm.h atr.h emit.h stm.h ins.h slt.h cnt.h scr.h \
 sco.h opt.h encode.h
stp.o: stp.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h exp.h wht.h id.h sym.h whr.h evt.h acode.h nam.h \
 elm.h stm.h ins.h slt.h cnt.h stp.h emit.h dump.h
str.o: str.c str.h sysdep.h types.h srcp.h util.h
stx.o: stx.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h acode.h wht.h \
 dump.h res.h stx.h nam.h elm.h wrd.h emit.h
sym.o: sym.c sym.h srcp.h types.h sysdep.h util.h lmList.h \
 alanCommon.h token.h cla.h id.h slt.h lst.h whr.h evt.h acode.h wht.h \
 cnt.h
syn.o: syn.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h wht.h \
 dump.h nam.h elm.h syn.h wrd.h emit.h
sysdep.o: sysdep.c sysdep.h
timing.o: timing.c sysdep.h timing.h
unit.o: unit.c lmList.h alanCommon.h srcp.h token.h util.h types.h \
 sym.h cla.h id.h slt.h lst.h whr.h evt.h acode.h wht.h cnt.h
util.o: util.c util.h srcp.h types.h sysdep.h lmList.h alanCommon.h \
 token.h
vrb.o: vrb.c vrb.h srcp.h lst.h ins.h id.h types.h sym.h whr.h evt.h \
 acode.h wht.h slt.h cnt.h stx.h util.h lmList.h alanCommon.h token.h \
 adv.h dump.h sysdep.h alt.h emit.h
vrbout.o: vrbout.c
whr.o: whr.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h nam.h elm.h id.h sym.h cnt.h wht.h whr.h \
 evt.h dump.h emit.h
wht.o: wht.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h wht.h id.h sym.h emit.h acode.h dump.h
wrd.o: wrd.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h nam.h elm.h id.h sym.h wrd.h opt.h \
 emit.h dump.h
act.o: act.c util.h srcp.h types.h adv.h lst.h whr.h evt.h id.h sym.h \
 elm.h acode.h wht.h dump.h sysdep.h exp.h alan.h nam.h atr.h emit.h \
 stm.h ins.h slt.h cnt.h scr.h act.h loc.h ext.h wrd.h vrb.h stx.h \
 lmList.h alanCommon.h token.h
adv.o: adv.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h stm.h ins.h slt.h cnt.h exp.h nam.h vrb.h stx.h ext.h rul.h \
 wrd.h syn.h atr.h emit.h msg.h sco.h opt.h options.h encode.h
alan.o: alan.c sysdep.h types.h srcp.h util.h alan.h lst.h \
 alan.version.h version.h adv.h whr.h evt.h id.h sym.h elm.h acode.h \
 wht.h dump.h opt.h lmList.h alanCommon.h token.h pmParse.h smScan.h \
 options.h timing.h
alan.version.o: alan.version.c alan.version.h version.h
alt.o: alt.c util.h srcp.h types.h lmList.h alanCommon.h token.h sym.h \
 elm.h id.h lst.h acode.h alt.h chk.h exp.h alan.h sysdep.h wht.h \
 whr.h evt.h nam.h ins.h slt.h cnt.h stm.h emit.h dump.h
atr.o: atr.c util.h srcp.h types.h adv.h lst.h whr.h evt.h id.h sym.h \
 elm.h acode.h wht.h dump.h sysdep.h atr.h alan.h nam.h exp.h emit.h \
 ins.h slt.h cnt.h opt.h lmList.h alanCommon.h token.h encode.h
chk.o: chk.c util.h srcp.h types.h options.h lst.h dump.h sysdep.h \
 exp.h alan.h wht.h id.h sym.h elm.h acode.h whr.h evt.h nam.h stm.h \
 ins.h slt.h cnt.h chk.h emit.h
cla.o: cla.c cla.h srcp.h id.h types.h sym.h elm.h lst.h acode.h slt.h \
 whr.h evt.h wht.h cnt.h util.h dump.h sysdep.h lmList.h alanCommon.h \
 token.h
cnt.o: cnt.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h stm.h ins.h slt.h cnt.h exp.h nam.h lim.h atr.h emit.h obj.h
dump.o: dump.c alan.h sysdep.h types.h srcp.h lst.h adv.h whr.h evt.h \
 id.h sym.h elm.h acode.h wht.h dump.h nam.h atr.h exp.h emit.h stm.h \
 ins.h slt.h cnt.h chk.h vrb.h stx.h lim.h ext.h scr.h stp.h rul.h \
 lmList.h alanCommon.h token.h
elm.o: elm.c util.h srcp.h types.h lmList.h alanCommon.h token.h sym.h \
 elm.h id.h lst.h acode.h stx.h wrd.h emit.h dump.h sysdep.h
emit.o: emit.c sysdep.h types.h srcp.h util.h alan.h lst.h acode.h \
 alan.version.h version.h emit.h
encode.o: encode.c alan.h sysdep.h types.h srcp.h lst.h util.h acode.h \
 opt.h emit.h encode.h
evt.o: evt.c util.h srcp.h types.h lmList.h alanCommon.h token.h adv.h \
 lst.h whr.h evt.h id.h sym.h elm.h acode.h wht.h dump.h sysdep.h \
 nam.h alan.h stm.h ins.h slt.h cnt.h exp.h opt.h emit.h
exp.o: exp.c util.h srcp.h types.h lmList.h alanCommon.h token.h adv.h \
 lst.h whr.h evt.h id.h sym.h elm.h acode.h wht.h dump.h sysdep.h \
 exp.h alan.h nam.h atr.h emit.h ins.h slt.h cnt.h encode.h
ext.o: ext.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h sym.h elm.h id.h acode.h nam.h stm.h wht.h whr.h \
 evt.h ins.h slt.h cnt.h exp.h chk.h ext.h wrd.h emit.h dump.h
id.o: id.c id.h types.h srcp.h sym.h elm.h lst.h acode.h str.h util.h \
 dump.h sysdep.h
ins.o: ins.c ins.h srcp.h id.h types.h sym.h elm.h lst.h acode.h whr.h \
 evt.h wht.h slt.h cnt.h sysdep.h util.h dump.h lmList.h alanCommon.h \
 token.h
lim.o: lim.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h nam.h exp.h atr.h emit.h stm.h ins.h slt.h cnt.h chk.h vrb.h \
 stx.h lim.h ext.h rul.h
lmList.o: lmList.c lmList.h alanCommon.h srcp.h token.h
loc.o: loc.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h nam.h exp.h atr.h emit.h loc.h ext.h stm.h ins.h slt.h cnt.h \
 vrb.h stx.h
lst.o: lst.c lst.h util.h srcp.h types.h dump.h sysdep.h
macSpa.o: macSpa.c
main.o: main.c util.h srcp.h types.h alan.h sysdep.h lst.h spa.h \
 options.h dump.h alan.version.h version.h
msg.o: msg.c sysdep.h alan.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h msg.h stm.h ins.h slt.h cnt.h exp.h nam.h opt.h emit.h \
 encode.h
nam.o: nam.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h nam.h elm.h id.h sym.h acode.h str.h obj.h whr.h \
 evt.h wht.h cnt.h act.h ext.h loc.h vrb.h ins.h slt.h stx.h msg.h \
 dump.h emit.h
obj.o: obj.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h nam.h exp.h atr.h emit.h stm.h ins.h slt.h cnt.h obj.h loc.h \
 ext.h wrd.h vrb.h stx.h
opt.o: opt.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h acode.h opt.h emit.h
options.o: options.c types.h srcp.h dump.h sysdep.h options.h lst.h
pmErr.o: pmErr.c util.h srcp.h types.h smScan.h alanCommon.h token.h \
 sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h elm.h acode.h \
 wht.h dump.h alt.h atr.h alan.h nam.h exp.h emit.h chk.h ins.h slt.h \
 cnt.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h sco.h scr.h \
 stm.h stp.h str.h syn.h vrb.h
pmPaSema.o: pmPaSema.c util.h srcp.h types.h smScan.h alanCommon.h \
 token.h sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h elm.h \
 acode.h wht.h dump.h alt.h atr.h alan.h nam.h exp.h emit.h chk.h \
 ins.h slt.h cnt.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h \
 sco.h scr.h stm.h stp.h str.h syn.h vrb.h
pmParse.o: pmParse.c util.h srcp.h types.h smScan.h alanCommon.h \
 token.h sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h elm.h \
 acode.h wht.h dump.h alt.h atr.h alan.h nam.h exp.h emit.h chk.h \
 ins.h slt.h cnt.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h \
 sco.h scr.h stm.h stp.h str.h syn.h vrb.h pmParse.h
res.o: res.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h res.h stx.h id.h sym.h elm.h acode.h nam.h stm.h \
 wht.h whr.h evt.h ins.h slt.h cnt.h exp.h emit.h dump.h
rul.o: rul.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h exp.h nam.h stm.h ins.h slt.h cnt.h rul.h emit.h
sco.o: sco.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h sco.h emit.h
scr.o: scr.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h stm.h wht.h id.h sym.h elm.h whr.h evt.h \
 ins.h slt.h cnt.h exp.h nam.h stp.h scr.h act.h emit.h dump.h
slt.o: slt.c slt.h srcp.h lst.h whr.h evt.h id.h types.h sym.h elm.h \
 acode.h wht.h cnt.h util.h dump.h sysdep.h
smScSema.o: smScSema.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h str.h
smScan.o: smScan.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h
smScanx.o: smScanx.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h
spa.o: spa.c spa.h
srcp.o: srcp.c alan.h sysdep.h types.h srcp.h lst.h dump.h
stm.o: stm.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h exp.h nam.h atr.h emit.h stm.h ins.h slt.h cnt.h scr.h sco.h \
 opt.h encode.h
stp.o: stp.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h exp.h wht.h id.h sym.h elm.h acode.h whr.h evt.h \
 nam.h stm.h ins.h slt.h cnt.h stp.h emit.h dump.h
str.o: str.c str.h sysdep.h types.h srcp.h util.h
stx.o: stx.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h res.h stx.h nam.h wrd.h emit.h
sym.o: sym.c sym.h srcp.h types.h elm.h id.h lst.h acode.h sysdep.h \
 util.h lmList.h alanCommon.h token.h cla.h slt.h whr.h evt.h wht.h \
 cnt.h
syn.o: syn.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h nam.h syn.h wrd.h emit.h
sysdep.o: sysdep.c sysdep.h
timing.o: timing.c sysdep.h timing.h
unit.o: unit.c lmList.h alanCommon.h srcp.h token.h util.h types.h \
 sym.h elm.h id.h lst.h acode.h cla.h slt.h whr.h evt.h wht.h cnt.h
util.o: util.c util.h srcp.h types.h sysdep.h lmList.h alanCommon.h \
 token.h
vrb.o: vrb.c vrb.h srcp.h lst.h ins.h id.h types.h sym.h elm.h acode.h \
 whr.h evt.h wht.h slt.h cnt.h stx.h util.h lmList.h alanCommon.h \
 token.h adv.h dump.h sysdep.h alt.h emit.h
vrbout.o: vrbout.c
whr.o: whr.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h sym.h elm.h id.h cnt.h wht.h whr.h evt.h \
 dump.h emit.h
wht.o: wht.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h wht.h id.h sym.h elm.h acode.h emit.h dump.h
wrd.o: wrd.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h wrd.h id.h sym.h elm.h opt.h emit.h \
 dump.h
act.o: act.c util.h types.h srcp.h adv.h lst.h whr.h evt.h id.h sym.h \
 elm.h acode.h wht.h dump.h sysdep.h exp.h alan.h atr.h emit.h stm.h \
 ins.h slt.h cnt.h scr.h act.h nam.h loc.h ext.h wrd.h vrb.h stx.h \
 lmList.h alanCommon.h token.h
adv.o: adv.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h cla.h slt.h cnt.h ins.h stm.h exp.h vrb.h stx.h ext.h rul.h \
 wrd.h syn.h atr.h emit.h msg.h sco.h opt.h options.h encode.h
alan.o: alan.c sysdep.h types.h srcp.h util.h alan.h lst.h \
 alan.version.h version.h adv.h whr.h evt.h id.h sym.h elm.h acode.h \
 wht.h dump.h opt.h lmList.h alanCommon.h token.h pmParse.h smScan.h \
 options.h timing.h
alan.version.o: alan.version.c alan.version.h version.h
alt.o: alt.c util.h types.h srcp.h lmList.h alanCommon.h token.h sym.h \
 elm.h id.h lst.h acode.h alt.h ins.h whr.h evt.h wht.h slt.h cnt.h \
 chk.h exp.h alan.h sysdep.h stm.h emit.h dump.h
atr.o: atr.c util.h types.h srcp.h adv.h lst.h whr.h evt.h id.h sym.h \
 elm.h acode.h wht.h dump.h sysdep.h atr.h alan.h exp.h emit.h ins.h \
 slt.h cnt.h opt.h lmList.h alanCommon.h token.h encode.h
chk.o: chk.c util.h types.h srcp.h options.h lst.h dump.h sysdep.h \
 exp.h alan.h wht.h id.h sym.h elm.h acode.h whr.h evt.h stm.h ins.h \
 slt.h cnt.h chk.h emit.h
cla.o: cla.c cla.h srcp.h id.h types.h sym.h elm.h lst.h acode.h slt.h \
 whr.h evt.h wht.h cnt.h util.h dump.h sysdep.h lmList.h alanCommon.h \
 token.h
claTest.o: claTest.c cla.c cla.h srcp.h id.h types.h sym.h elm.h lst.h \
 acode.h slt.h whr.h evt.h wht.h cnt.h util.h dump.h sysdep.h lmList.h \
 alanCommon.h token.h unitTest.h
cnt.o: cnt.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h stm.h ins.h slt.h cnt.h exp.h lim.h atr.h emit.h obj.h nam.h
dump.o: dump.c alan.h sysdep.h types.h srcp.h lst.h adv.h whr.h evt.h \
 id.h sym.h elm.h acode.h wht.h dump.h nam.h atr.h exp.h emit.h stm.h \
 ins.h slt.h cnt.h chk.h vrb.h stx.h lim.h ext.h scr.h stp.h rul.h \
 lmList.h alanCommon.h token.h
elm.o: elm.c util.h types.h srcp.h lmList.h alanCommon.h token.h sym.h \
 elm.h id.h lst.h acode.h stx.h wrd.h emit.h dump.h sysdep.h
emit.o: emit.c sysdep.h types.h srcp.h util.h alan.h lst.h acode.h \
 alan.version.h version.h emit.h
encode.o: encode.c alan.h sysdep.h types.h srcp.h lst.h util.h acode.h \
 opt.h emit.h encode.h
evt.o: evt.c util.h types.h srcp.h lmList.h alanCommon.h token.h adv.h \
 lst.h whr.h evt.h id.h sym.h elm.h acode.h wht.h dump.h sysdep.h \
 stm.h alan.h ins.h slt.h cnt.h exp.h opt.h emit.h
exp.o: exp.c util.h types.h srcp.h lmList.h alanCommon.h token.h adv.h \
 lst.h whr.h evt.h id.h sym.h elm.h acode.h wht.h dump.h sysdep.h \
 exp.h alan.h atr.h emit.h ins.h slt.h cnt.h encode.h
ext.o: ext.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h sym.h elm.h id.h acode.h nam.h stm.h wht.h whr.h \
 evt.h ins.h slt.h cnt.h exp.h chk.h ext.h wrd.h emit.h dump.h
id.o: id.c id.h types.h srcp.h sym.h elm.h lst.h acode.h str.h util.h \
 dump.h sysdep.h
ins.o: ins.c ins.h srcp.h id.h types.h sym.h elm.h lst.h acode.h whr.h \
 evt.h wht.h slt.h cnt.h sysdep.h util.h dump.h lmList.h alanCommon.h \
 token.h
lim.o: lim.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h exp.h atr.h emit.h stm.h ins.h slt.h cnt.h chk.h vrb.h stx.h \
 lim.h ext.h rul.h
lmList.o: lmList.c lmList.h alanCommon.h srcp.h token.h
loc.o: loc.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h nam.h exp.h atr.h emit.h loc.h ext.h stm.h ins.h slt.h cnt.h \
 vrb.h stx.h
lst.o: lst.c lst.h util.h types.h srcp.h dump.h sysdep.h
macSpa.o: macSpa.c
main.o: main.c util.h types.h srcp.h alan.h sysdep.h lst.h spa.h \
 options.h dump.h alan.version.h version.h
msg.o: msg.c sysdep.h alan.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h msg.h stm.h ins.h slt.h cnt.h exp.h opt.h emit.h encode.h
nam.o: nam.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h nam.h elm.h id.h sym.h acode.h str.h obj.h whr.h \
 evt.h wht.h cnt.h act.h ext.h loc.h vrb.h ins.h slt.h stx.h msg.h \
 dump.h emit.h
obj.o: obj.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h nam.h exp.h atr.h emit.h stm.h ins.h slt.h cnt.h obj.h loc.h \
 ext.h wrd.h vrb.h stx.h
opt.o: opt.c alan.h sysdep.h types.h srcp.h lst.h lmList.h \
 alanCommon.h token.h acode.h opt.h emit.h
options.o: options.c types.h srcp.h dump.h sysdep.h options.h lst.h
pmErr.o: pmErr.c util.h types.h srcp.h smScan.h alanCommon.h token.h \
 sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h elm.h acode.h \
 wht.h dump.h alt.h ins.h slt.h cnt.h atr.h alan.h exp.h emit.h chk.h \
 cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h sco.h scr.h stm.h \
 stp.h str.h syn.h vrb.h
pmPaSema.o: pmPaSema.c util.h types.h srcp.h smScan.h alanCommon.h \
 token.h sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h elm.h \
 acode.h wht.h dump.h alt.h ins.h slt.h cnt.h atr.h alan.h exp.h \
 emit.h chk.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h sco.h \
 scr.h stm.h stp.h str.h syn.h vrb.h
pmParse.o: pmParse.c util.h types.h srcp.h smScan.h alanCommon.h \
 token.h sysdep.h lst.h lmList.h adv.h whr.h evt.h id.h sym.h elm.h \
 acode.h wht.h dump.h alt.h ins.h slt.h cnt.h atr.h alan.h exp.h \
 emit.h chk.h cla.h ext.h lim.h msg.h opt.h res.h stx.h rul.h sco.h \
 scr.h stm.h stp.h str.h syn.h vrb.h pmParse.h
res.o: res.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h res.h stx.h id.h sym.h elm.h acode.h stm.h wht.h \
 whr.h evt.h ins.h slt.h cnt.h exp.h emit.h dump.h
rul.o: rul.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h exp.h stm.h ins.h slt.h cnt.h rul.h emit.h
sco.o: sco.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h sco.h emit.h
scr.o: scr.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h stm.h wht.h id.h sym.h elm.h whr.h evt.h \
 ins.h slt.h cnt.h exp.h stp.h scr.h act.h nam.h emit.h dump.h
slt.o: slt.c slt.h srcp.h lst.h whr.h evt.h id.h types.h sym.h elm.h \
 acode.h wht.h cnt.h util.h dump.h sysdep.h
smScSema.o: smScSema.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h str.h
smScan.o: smScan.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h
smScanx.o: smScanx.c sysdep.h types.h srcp.h alan.h lst.h lmList.h \
 alanCommon.h token.h encode.h acode.h smScan.h
spa.o: spa.c spa.h
srcp.o: srcp.c alan.h sysdep.h types.h srcp.h lst.h dump.h
stm.o: stm.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h exp.h atr.h emit.h stm.h ins.h slt.h cnt.h scr.h sco.h opt.h \
 encode.h
stp.o: stp.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h exp.h wht.h id.h sym.h elm.h acode.h whr.h evt.h \
 stm.h ins.h slt.h cnt.h stp.h emit.h dump.h
str.o: str.c str.h sysdep.h types.h srcp.h util.h
stx.o: stx.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h adv.h whr.h evt.h id.h sym.h elm.h acode.h wht.h \
 dump.h res.h stx.h wrd.h emit.h
sym.o: sym.c sym.h types.h srcp.h elm.h id.h lst.h acode.h sysdep.h \
 util.h lmList.h alanCommon.h token.h cla.h slt.h whr.h evt.h wht.h \
 cnt.h
symTest.o: symTest.c sym.c sym.h types.h srcp.h elm.h id.h lst.h \
 acode.h sysdep.h util.h lmList.h alanCommon.h token.h cla.h slt.h \
 whr.h evt.h wht.h cnt.h unitTest.h
syn.o: syn.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h adv.h whr.h evt.h id.h sym.h elm.h wht.h \
 dump.h syn.h wrd.h emit.h
sysdep.o: sysdep.c sysdep.h
timing.o: timing.c sysdep.h timing.h
unit.o: unit.c lmList.h alanCommon.h srcp.h token.h util.h types.h \
 sym.h elm.h id.h lst.h acode.h cla.h slt.h whr.h evt.h wht.h cnt.h
util.o: util.c util.h types.h srcp.h sysdep.h lmList.h alanCommon.h \
 token.h
vrb.o: vrb.c vrb.h srcp.h lst.h ins.h id.h types.h sym.h elm.h acode.h \
 whr.h evt.h wht.h slt.h cnt.h stx.h util.h lmList.h alanCommon.h \
 token.h adv.h dump.h sysdep.h alt.h emit.h
whr.o: whr.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h sym.h elm.h id.h cnt.h wht.h whr.h evt.h \
 dump.h emit.h
wht.o: wht.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h wht.h id.h sym.h elm.h acode.h emit.h dump.h
wrd.o: wrd.c alan.h sysdep.h types.h srcp.h lst.h util.h lmList.h \
 alanCommon.h token.h acode.h wrd.h id.h sym.h elm.h opt.h emit.h \
 dump.h
