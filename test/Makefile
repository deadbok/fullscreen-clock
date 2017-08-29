INDENT:=clang-format -i

SRC=config.cpp test-main.cpp

all: config Makefile

test-main.o: test-main.cpp
	${CXX} ${CXXFLAGS} -c test-main.cpp -o $@

config: config.cpp ../config.h ../config.cpp catch.hpp test-main.o
	${CXX} ${CXXFLAGS} test-main.o config.cpp ../config.cpp -o $@

catch.hpp:
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

format:
	$(INDENT) $(SRC)

clean:
	$(RM) $(ALL)
	$(RM) *.o
	$(RM) core
