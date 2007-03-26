CFLAGS = -Wall -O2 -IShapes -ISounds

WXCFLAGS = `wx-config --cxxflags`
WXLIBS = `wx-config --libs`

OBJECTS =	BigEndianBuffer.o ShapeFusionApp.o ShapeFusionMain.o ShapeFusionMenus.o
SHAPESOBJECTS = Shapes/BitmapBrowser.o Shapes/BitmapView.o Shapes/CTBrowser.o \
				Shapes/FrameBrowser.o Shapes/FrameView.o Shapes/SequenceView.o \
				Shapes/ShapesDocument.o Shapes/ShapesElements.o Shapes/ShapesView.o \
				Shapes/ShapesTreeItemData.o Shapes/utilities.o
SOUNDSOBJECTS = Sounds/SoundsDocument.o Sounds/SoundsView.o Sounds/SoundsElements.o

shapefusion: shapes sounds $(OBJECTS)
	g++ -o shapefusion $(OBJECTS) $(SHAPESOBJECTS) $(SOUNDSOBJECTS) $(WXLIBS)

BigEndianBuffer.o: BigEndianBuffer.cpp BigEndianBuffer.h
	g++ -c BigEndianBuffer.cpp $(CFLAGS)

ShapeFusionApp.o: ShapeFusionApp.cpp ShapeFusionApp.h
	g++ -c ShapeFusionApp.cpp $(CFLAGS) $(WXCFLAGS)

ShapeFusionMain.o: ShapeFusionMain.cpp ShapeFusionMain.h
	g++ -c ShapeFusionMain.cpp $(CFLAGS) $(WXCFLAGS)

ShapeFusionMenus.o: ShapeFusionMenus.cpp ShapeFusionMenus.h
	g++ -c ShapeFusionMenus.cpp $(CFLAGS) $(WXCFLAGS)

shapes:
	make -C Shapes

sounds:
	make -C Sounds

clean:
	make -C Shapes clean
	make -C Sounds clean
	rm -f *.o

