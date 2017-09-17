INDENT:=clang-format -i


SRC=config.cpp test-main.cpp

all: test-config Makefile

test-main.o: test-main.cpp
	${CXX} ${CXXFLAGS} -g -c test-main.cpp -o $@

test-config: config.cpp ../config.h ../config.cpp catch.hpp test-main.o
	${CXX} ${CXXFLAGS} -g test-main.o config.cpp ../config.cpp -o $@
	./test-config

.PHONY: config
config-clean:
	$(RM) config

catch.hpp:
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp

format:
	$(INDENT) $(SRC)

clean: config-clean
	$(RM) *.o
	$(RM) core
