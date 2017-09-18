RM:=rm -f

all:
	$(MAKE) -C src -f BSDmakefile all

test:
	$(MAKE) -C src/test -f BSDmakefile all

format:
	$(MAKE) -C src -f BSDmakefile format
	$(MAKE) -C src/test -f BSDmakefile format

clean:
	$(MAKE) -C src -f BSDmakefile clean
	$(MAKE) -C src/test -f BSDmakefile clean

distclean:
	$(MAKE) -C src -f BSDmakefile clean
	$(MAKE) -C src/test -f BSDmakefile clean
	$(RM) clock
