RM:=rm -f

all:
	$(MAKE) -C src format all

test:
	$(MAKE) -C src/test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C src/test clean

distclean:
	$(MAKE) -C src clean
	$(MAKE) -C src/test clean
	$(RM) clock
