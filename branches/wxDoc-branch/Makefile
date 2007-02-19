#CFLAGS = -Wall -g
CFLAGS = -Wall -O4

WXCFLAGS = `wx-config --cxxflags`
WXLIBS = `wx-config --libs`

OBJECTS = ShapeFusionApp.o ShapesEditor.o Shapes.o MyTreeItemData.o \
			BitmapBrowser.o BitmapView.o CTBrowser.o FrameBrowser.o \
			FrameView.o SequenceView.o utilities.o BigEndianBuffer.o

shapefusion: $(OBJECTS)
	g++ -o shapefusion $(OBJECTS) $(WXLIBS)

ShapeFusionApp.o: ShapeFusionApp.cpp ShapeFusionApp.h ShapesEditor.h
	g++ -c ShapeFusionApp.cpp $(CFLAGS) $(WXCFLAGS)

ShapesEditor.o: ShapesEditor.cpp ShapesEditor.h BitmapBrowser.h \
					BitmapView.h FrameBrowser.h FrameView.h \
					CTBrowser.h MyTreeItemData.h utilities.h
	g++ -c ShapesEditor.cpp $(CFLAGS) $(WXCFLAGS)

Shapes.o: Shapes.cpp Shapes.h BigEndianBuffer.h
	g++ -c Shapes.cpp $(CFLAGS) $(WXCFLAGS)

MyTreeItemData.o: MyTreeItemData.cpp MyTreeItemData.h
	g++ -c MyTreeItemData.cpp $(CFLAGS) $(WXCFLAGS)

BitmapBrowser.o: BitmapBrowser.cpp BitmapBrowser.h Shapes.h utilities.h
	g++ -c BitmapBrowser.cpp $(CFLAGS) $(WXCFLAGS)

BitmapView.o: BitmapView.cpp BitmapView.h Shapes.h utilities.h
	g++ -c BitmapView.cpp $(CFLAGS) $(WXCFLAGS)

CTBrowser.o: CTBrowser.cpp CTBrowser.h Shapes.h
	g++ -c CTBrowser.cpp $(CFLAGS) $(WXCFLAGS)

FrameBrowser.o: FrameBrowser.cpp FrameBrowser.h Shapes.h utilities.h
	g++ -c FrameBrowser.cpp $(CFLAGS) $(WXCFLAGS)

FrameView.o: FrameView.cpp FrameView.h Shapes.h utilities.h
	g++ -c FrameView.cpp $(CFLAGS) $(WXCFLAGS)

SequenceView.o: SequenceView.cpp SequenceView.h Shapes.h utilities.h
	g++ -c SequenceView.cpp $(CFLAGS) $(WXCFLAGS)

utilities.o: utilities.cpp utilities.h
	g++ -c utilities.cpp $(CFLAGS) $(WXCFLAGS)

BigEndianBuffer.o: BigEndianBuffer.cpp BigEndianBuffer.h
	g++ -c BigEndianBuffer.cpp $(CFLAGS)

clean:
	rm -f *.o

