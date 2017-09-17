# Remember to configure this to the actual path og the fltk config tool.
# FreeBSD
#FLTKCONFIG=/usr/local/bin/fltk-config
# Gentoo
FLTKCONFIG=/usr/bin/fltk-config

CXXFLAGS_FLTK=`$(FLTKCONFIG) --use-images --cxxflags`
LDFLAGS_FLTK=`$(FLTKCONFIG) --use-images --ldflags`

CXXFLAGS_CURL=`curl-config --cflags`
LDFLAGS_CURL=`curl-config --libs`

CXXFLAGS_JSON-C=`pkg-config json-c --cflags`
LDFLAGS_JSON-C=`pkg-config json-c --libs`

CXXFLAGS_DBG=
ifdef DEBUG
	CXXFLAGS_DBG=-g
endif
CXXFLAGS=$(CXXFLAGS_FLTK) $(CXXFLAGS_CURL) $(CXXFLAGS_JSON-C)-Wall -I. -I.. $(CXXFLAGS_DBG) -std=c++11
LDFLAGS=$(LDFLAGS_FLTK) $(LDFLAGS_CURL) $(LDFLAGS_JSON-C)

RM:=rm -f
INDENT:=clang-format -i

# Final executable name.
PROGRAM=clock

INC=config.h owm.h
SRC=main.cpp config.cpp owm.cpp
OBJ=main.o config.o owm.o

all: $(PROGRAM) Makefile

$(PROGRAM): $(OBJ)
	${CXX} $(OBJ) ${LDFLAGS} -o $(PROGRAM)

.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@

format:
	$(INDENT) $(SRC) $(INC)

clean:
	$(RM) $(ALL)
	$(RM) *.o
	$(RM) core
