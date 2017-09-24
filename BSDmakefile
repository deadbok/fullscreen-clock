RM:=rm -f

all: client server

test: client-test

clean: client-clean server-clean

client:
	$(MAKE) -C src/client -f BSDmakefile all

client-test:
	$(MAKE) -C src/client/test -f BSDmakefile all

client-format:
	$(MAKE) -C src/client -f BSDmakefile format
	$(MAKE) -C src/client/test -f BSDmakefile format

client-clean:
	$(MAKE) -C src/client -f BSDmakefile clean
	$(MAKE) -C src/client/test -f BSDmakefile clean

server:
	$(MAKE) -C src/server -f BSDmakefile all

server-format:
	$(MAKE) -C src/server -f BSDmakefile format

server-clean:
	$(MAKE) -C src/server -f BSDmakefile clean

distclean: client-clean server-clean
	$(RM) clock
