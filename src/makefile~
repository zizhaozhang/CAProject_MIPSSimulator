CC=g++ -std=c++0x
CFLAGS=-c -g -Wall
LDFLAGS=-g
SOURCES=MIPsSimulator.cpp PipeLine.cpp ReservationStations.cpp ReorderBuffer.cpp RegisterStatus.cpp OPMap.cpp MIPSdis.cpp Instruction.cpp InsParser.cpp BTB.cpp computeInstruction.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../bin/MIPsSimulator

#$(OBJECTS)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	        $(CC) $(LDFLAGS) $(OBJECTS) -o $@
		
#.cc.o:
#	        $(CC) $(CFLAGS) $< -o $@
		
clean:
	        rm -rf *.o MIPsSimulator
