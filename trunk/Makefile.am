## Process this file with automake to produce Makefile.in 

SUBDIRS = Physics Shapes Sounds

AUTOMAKE_OPTIONS = 1.5 foreign

EXTRA_DIST = ShapeFusion.xcodeproj/project.pbxproj COPYING.txt	\
	DefaultNames.txt README.txt ShapeFusion-Info.plist	\
	Resources/shapefusion.icns Resources/shapefusion.ico	\
	Resources/shapefusion.rc Resources/shapefusion.svg

if MAKE_WINDOWS
bin_PROGRAMS = ShapeFusion
else
bin_PROGRAMS = shapefusion
endif

shapefusion_SOURCES = BigEndianBuffer.h DefaultNames.h			\
	GenericEndianBuffer.h LittleEndianBuffer.h ShapeFusionApp.h	\
	ShapeFusionDocManager.h ShapeFusionMain.h ShapeFusionMenus.h	\
	BigEndianBuffer.cpp DefaultNames.cpp GenericEndianBuffer.cpp	\
	LittleEndianBuffer.cpp ShapeFusionApp.cpp			\
	ShapeFusionDocManager.cpp ShapeFusionMain.cpp			\
	ShapeFusionMenus.cpp

shapefusion_LDADD = \
  Physics/libphysics.a Shapes/libshapes.a Sounds/libsounds.a

ShapeFusion_SOURCES = $(shapefusion_SOURCES)
ShapeFusion_LDADD = $(shapefusion_LDADD) shapefusion-resources.o

INCLUDES = -I$(top_srcdir)/Physics -I$(top_srcdir)/Shapes -I$(top_srcdir)/Sounds

shapefusion-resources.o: $(top_srcdir)/Resources/shapefusion.rc
	@WINDRES@ -I$(top_srcdir)/Resources -o shapefusion-resources.o $(top_srcdir)/Resources/shapefusion.rc
