RM:=rm -f
CP:=cp

all: client

test: client-test

clean: client-clean server-clean

client:
	$(MAKE) -C src/client -f BSDmakefile all
	$(CP) src/client/clock ./clock

client-test:
	$(MAKE) -C src/client/test -f BSDmakefile all

client-format:
	$(MAKE) -C src/client -f BSDmakefile format
	$(MAKE) -C src/client/test -f BSDmakefile format

client-clean:
	$(MAKE) -C src/client -f BSDmakefile clean
	$(MAKE) -C src/client/test -f BSDmakefile clean

client-distclean:
	$(MAKE) -C src/client -f Makefile.Linux distclean
	$(MAKE) -C src/client/test -f Makefile.Linux clean

server-clean:
	$(MAKE) -C src/server -f BSDmakefile clean

distclean: client-distclean server-clean
	$(RM) ./clock
