# Remember to configure this to the actual path og the fltk config tool.
FLTKCONFIG=/usr/local/bin/fltk-config

CXX=`$(FLTKCONFIG) --cxx`
CXXFLAGS=`$(FLTKCONFIG) --cxxflags` -Wall -I.
LINKFLTK=`$(FLTKCONFIG) --ldstaticflags`
LINKFLTK_GL=`$(FLTKCONFIG) --use-gl --ldstaticflags`
LINKFLTK_IMG=`$(FLTKCONFIG) --use-images --ldstaticflags`
LINKFLTK_ALL=`$(FLTKCONFIG) --use-images --use-gl --ldstaticflags`

.cpp.o:
	${CXX} -I.. ${CXXFLAGS} -c $< -o $@

RM:=rm -f

# Final executable name.
PROGRAM=clock

OBJ=main.o

all: $(PROGRAM)
	
$(PROGRAM): $(OBJ)
	${CXX} $(OBJ) ${LINKFLTK_ALL} -o $(PROGRAM)

clean:
	$(RM) $(ALL)
	$(RM) *.o
	$(RM) core
