CFLAGS = -Wall -O2 -I..

WXCFLAGS = `wx-config --cxxflags`

OBJECTS = SoundsDocument.o SoundsElements.o SoundsView.o

all: $(OBJECTS)

SoundsDocument.o: SoundsDocument.cpp SoundsDocument.h SoundsElements.h SoundsView.h
	g++ -c SoundsDocument.cpp $(CFLAGS) $(WXCFLAGS)

SoundsElements.o: SoundsElements.cpp SoundsElements.h ../BigEndianBuffer.h
	g++ -c SoundsElements.cpp $(CFLAGS) $(WXCFLAGS)

SoundsView.o: SoundsView.cpp SoundsView.h SoundsDocument.h
	g++ -c SoundsView.cpp $(CFLAGS) $(WXCFLAGS)

clean:
	rm -f *.o

