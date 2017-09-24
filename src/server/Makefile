CXXFLAGS_DBG=
ifdef DEBUG
CXXFLAGS_DBG=-g
endif

CXXFLAGS=-Wall -I pistache/include -I. -I.. $(CXXFLAGS_DBG) -std=c++11
LDFLAGS=-pthread -L pistache/build/src -l pistache

RM:=rm -f

# Source formatting tool
INDENT:=clang-format -i

# Final executable name.
PROGRAM=clock-server

INC=
SRC=main.cpp
OBJ=main.o

all: $(PROGRAM) Makefile
	@echo "Compiling server for Linux done."

$(PROGRAM): $(OBJ) pistache/build/src/libpistache.a
	${CXX} $(OBJ) ${LDFLAGS} -o $(PROGRAM)

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

pistache/build/.dir:
	mkdir pistache/build
	touch $@

pistache/build/src/libpistache.a: Makefile pistache/CMakeLists.txt pistache/build/.dir
	cd pistache/build; cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
	make -C pistache/build

clean:
	$(RM) -r pistache/build
