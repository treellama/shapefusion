#
# ShapeFusion Makefile
#

CFLAGS = -g -Wall -O2

WXCFLAGS = `wx-config --cxxflags`
WXLIBS = `wx-config --libs`

OBJECTS =		GenericEndianBuffer.o BigEndianBuffer.o LittleEndianBuffer.o ShapeFusionApp.o \
				ShapeFusionMain.o ShapeFusionMenus.o
SHAPESOBJECTS = Shapes/BitmapBrowser.o Shapes/BitmapView.o Shapes/CTBrowser.o Shapes/CTView.o \
				Shapes/FrameBrowser.o Shapes/FrameView.o Shapes/SequenceView.o \
				Shapes/ShapesDocument.o Shapes/ShapesElements.o Shapes/ShapesView.o \
				Shapes/ShapesTreeItemData.o Shapes/utilities.o
SOUNDSOBJECTS = Sounds/SoundsDocument.o Sounds/SoundsView.o Sounds/SoundsElements.o

VPATH = Shapes:Sounds


shapefusion: $(OBJECTS) $(SHAPESOBJECTS) $(SOUNDSOBJECTS)
	g++ -o shapefusion $(OBJECTS) $(SHAPESOBJECTS) $(SOUNDSOBJECTS) $(WXLIBS) `pkg-config --libs sndfile`

GenericEndianBuffer.o: GenericEndianBuffer.cpp GenericEndianBuffer.h
	g++ -c $(CFLAGS) $<

BigEndianBuffer.o: BigEndianBuffer.cpp BigEndianBuffer.h GenericEndianBuffer.h
	g++ -c $(CFLAGS) $<

LittleEndianBuffer.o: LittleEndianBuffer.cpp LittleEndianBuffer.h GenericEndianBuffer.h
	g++ -c $(CFLAGS) $<

ShapeFusionApp.o: ShapeFusionApp.cpp ShapeFusionApp.h ShapeFusionMain.h ShapeFusionMenus.h \
					ShapesDocument.h ShapesView.h SoundsDocument.h SoundsView.h
	g++ -c $(CFLAGS) $(WXCFLAGS) -IShapes -ISounds $<

ShapeFusionMain.o: ShapeFusionMain.cpp ShapeFusionMain.h ShapeFusionMenus.h
	g++ -c $(CFLAGS) $(WXCFLAGS) -IShapes -ISounds $<

ShapeFusionMenus.o: ShapeFusionMenus.cpp ShapeFusionMenus.h
	g++ -c $(CFLAGS) $(WXCFLAGS) -IShapes -ISounds $<


Shapes/BitmapBrowser.o: BitmapBrowser.cpp BitmapBrowser.h ShapesElements.h utilities.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/BitmapView.o: BitmapView.cpp BitmapView.h ShapesElements.h utilities.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/CTBrowser.o: CTBrowser.cpp CTBrowser.h ShapesElements.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/CTView.o: CTView.cpp CTView.h ShapesElements.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/FrameBrowser.o: FrameBrowser.cpp FrameBrowser.h ShapesElements.h utilities.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/FrameView.o: FrameView.cpp FrameView.h ShapesElements.h utilities.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/ShapesTreeItemData.o: ShapesTreeItemData.cpp ShapesTreeItemData.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/SequenceView.o: SequenceView.cpp SequenceView.h ShapesElements.h utilities.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/ShapesDocument.o: ShapesDocument.cpp ShapesDocument.h ShapesElements.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/ShapesElements.o: ShapesElements.cpp ShapesElements.h BigEndianBuffer.h LittleEndianBuffer.h utilities.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/ShapesView.o: ShapesView.cpp ShapesView.h BitmapBrowser.h BitmapView.h CTBrowser.h \
						FrameBrowser.h FrameView.h SequenceView.h ShapesTreeItemData.h \
						utilities.h ShapesDocument.h ShapeFusionApp.h ShapeFusionMenus.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Shapes/utilities.o: utilities.cpp utilities.h ShapesElements.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@


Sounds/SoundsDocument.o: SoundsDocument.cpp SoundsDocument.h SoundsElements.h SoundsView.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@

Sounds/SoundsElements.o: SoundsElements.cpp SoundsElements.h BigEndianBuffer.h LittleEndianBuffer.h
	g++ -c $(CFLAGS) $(WXCFLAGS) `pkg-config --cflags sndfile` $< -o $@

Sounds/SoundsView.o: SoundsView.cpp SoundsView.h SoundsDocument.h ShapeFusionApp.h ShapeFusionMenus.h
	g++ -c $(CFLAGS) $(WXCFLAGS) $< -o $@


clean:
	rm -f Shapes/*.o
	rm -f Sounds/*.o
	rm -f *.o

