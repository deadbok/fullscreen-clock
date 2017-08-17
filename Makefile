# Remember to configure this to the actual path og the fltk config tool.
# FreeBSD
#FLTKCONFIG=/usr/local/bin/fltk-config
# Gentoo
FLTKCONFIG=/usr/bin/fltk-config

CXX=`$(FLTKCONFIG) --cxx`
CXXFLAGS=`$(FLTKCONFIG) --cxxflags` -Wall -I.
LINKFLTK=`$(FLTKCONFIG) --ldflags`

.cpp.o:
	${CXX} -I.. ${CXXFLAGS} -c $< -o $@

RM:=rm -f

# Final executable name.
PROGRAM=clock

OBJ=main.o

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	${CXX} $(OBJ) ${LINKFLTK} -o $(PROGRAM)

clean:
	$(RM) $(ALL)
	$(RM) *.o
	$(RM) core
