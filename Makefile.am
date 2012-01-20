## Process this file with automake to produce Makefile.in 

SUBDIRS = Physics Shapes Sounds

AUTOMAKE_OPTIONS = 1.5 foreign dist-bzip2 no-dist-gzip

EXTRA_DIST = ShapeFusion.xcodeproj/project.pbxproj \
  COPYING.txt DefaultNames.txt README.txt \
	ShapeFusion-Info.plist

if MAKE_WINDOWS
bin_PROGRAMS = ShapeFusion
else
bin_PROGRAMS = shapefusion
endif

shapefusion_SOURCES = DefaultNames.cpp GenericEndianBuffer.cpp BigEndianBuffer.cpp LittleEndianBuffer.cpp ShapeFusionApp.cpp ShapeFusionDocManager.cpp \
				ShapeFusionMain.cpp ShapeFusionMenus.cpp

shapefusion_LDADD = \
  Physics/libphysics.a Shapes/libshapes.a Sounds/libsounds.a

ShapeFusion_SOURCES = $(shapefusion_SOURCES)
ShapeFusion_LDADD = $(shapefusion_LDADD) shapefusion-resources.o

INCLUDES = -I$(top_srcdir)/Physics -I$(top_srcdir)/Shapes -I$(top_srcdir)/Sounds

shapefusion-resources.o: $(top_srcdir)/Resources/shapefusion.rc
	@WINDRES@ -I$(top_srcdir)/Resources -o shapefusion-resources.o $(top_srcdir)/Resources/shapefusion.rc
