CFLAGS = -Wall -g -O2 -I..

WXCFLAGS = `wx-config --cxxflags`

OBJECTS = BitmapBrowser.o BitmapView.o CTBrowser.o FrameBrowser.o \
			FrameView.o MyTreeItemData.o SequenceView.o ShapesDocument.o \
			ShapesEditor.o ShapesElements.o ShapesView.o utilities.o

all: $(OBJECTS)

ShapesEditor.o: ShapesEditor.cpp ShapesEditor.h BitmapBrowser.h \
					BitmapView.h FrameBrowser.h FrameView.h \
					CTBrowser.h
	g++ -c ShapesEditor.cpp $(CFLAGS) $(WXCFLAGS)

BitmapBrowser.o: BitmapBrowser.cpp BitmapBrowser.h utilities.h
	g++ -c BitmapBrowser.cpp $(CFLAGS) $(WXCFLAGS)

BitmapView.o: BitmapView.cpp BitmapView.h utilities.h
	g++ -c BitmapView.cpp $(CFLAGS) $(WXCFLAGS)

CTBrowser.o: CTBrowser.cpp CTBrowser.h 
	g++ -c CTBrowser.cpp $(CFLAGS) $(WXCFLAGS)

FrameBrowser.o: FrameBrowser.cpp FrameBrowser.h utilities.h
	g++ -c FrameBrowser.cpp $(CFLAGS) $(WXCFLAGS)

FrameView.o: FrameView.cpp FrameView.h utilities.h
	g++ -c FrameView.cpp $(CFLAGS) $(WXCFLAGS)

MyTreeItemData.o: MyTreeItemData.cpp MyTreeItemData.h
	g++ -c MyTreeItemData.cpp $(CFLAGS) $(WXCFLAGS)

SequenceView.o: SequenceView.cpp SequenceView.h utilities.h
	g++ -c SequenceView.cpp $(CFLAGS) $(WXCFLAGS)

ShapesDocument.o: ShapesDocument.cpp ShapesDocument.h
	g++ -c ShapesDocument.cpp $(CFLAGS) $(WXCFLAGS)

ShapesElements.o: ShapesElements.cpp ShapesElements.h
	g++ -c ShapesElements.cpp $(CFLAGS) $(WXCFLAGS)

ShapesView.o: ShapesView.cpp ShapesView.h
	g++ -c ShapesView.cpp $(CFLAGS) $(WXCFLAGS)

utilities.o: utilities.cpp utilities.h
	g++ -c utilities.cpp $(CFLAGS) $(WXCFLAGS)

clean:
	rm -f *.o

