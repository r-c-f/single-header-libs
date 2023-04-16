.SUFFIXES:

M4 = m4

BASE_DEP = shl.m4 unused.h.in

all: cursutil.h fdio_full.h humansize.h sassert.h ssb.h sopt.h unused.h xmem.h

parinstall: all
	cp *.h ../

cursutil.h: $(BASE_DEP) cursutil.m4 cursutil.h.in
fdio_full.h: $(BASE_DEP) fdio_full.m4 fdio_full.h.in
humansize.h: $(BASE_DEP) humansize.m4 humansize.h.in
sassert.h: $(BASE_DEP) sassert.m4 sassert.h.in
sopt.h: $(BASE_DEP) sopt.m4 sopt.h.in
ssb.h: $(BASE_DEP) ssb.m4 ssb.h.in
unused.h: $(BASE_DEP) unused.m4 unused.h.in
xmem.h: $(BASE_DEP) xmem.m4 xmem.h.in

clean:
	rm -f *.h

.SUFFIXES: .h .m4
.m4.h:
	$(M4) shl.m4 $< > $@



