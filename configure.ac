dnl Process this file with autoconf to produce a configure script.
dnl Written in 2000 by Christian Bauer <Christian.Bauer@uni-mainz.de>

AC_INIT([ShapeFusion], [0.6], [http://sourceforge.net/bugs/?group_id=1997], [shapefusion])
AC_PREREQ(2.52)

dnl Detect the canonical host and target build environment.
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl Init automake.
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_RANLIB

AX_CXX_COMPILE_STDCXX_11([noext])

dnl some platform specific stuff
case $target in
*-*-mingw32*)
	AC_CHECK_TOOL(WINDRES, windres, :)
	;;
*-*-netbsd*)
	CPPFLAGS="$CPPFLAGS -I/usr/pkg/include"	
	;;
*)
	;;
esac

dnl wxWidgets
AM_OPTIONS_WXCONFIG
reqwx=2.8.0
case $target in
*-*-mingw32*)
  AM_PATH_WXCONFIG($reqwx, wxWidgets=1, , , [--unicode=no])
	;;
*)
	AM_PATH_WXCONFIG($reqwx, wxWidgets=1)
  ;;
esac
if test "$wxWidgets" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

PKG_CHECK_MODULES([SNDFILE], [sndfile], [
		  CPPFLAGS="$SNDFILE_CFLAGS $CPPFLAGS"
		  LIBS="$SNDFILE_LIBS $LIBS"], AC_ERROR([ShapeFusion requires libsndfile]))

dnl Check for libsndfile
dnl AC_CHECK_HEADER(sndfile.h, , AC_ERROR([ShapeFusion requires libsndfile]))
dnl AC_CHECK_LIB(sndfile, sf_open, [LIBS="-lvorbis -lvorbisenc -lvorbisfile -logg -lsndfile $LIBS"], AC_ERROR([ShapeFusion requires libsndfile]), [-lvorbis -lvorbisenc -lvorbisfile -logg -lFLAC -lsndfile])

dnl add some windows goodies
case $target in
*-*-mingw32*)
	make_windows=true
	;;
*)
	make_windows=false ;;
esac
AM_CONDITIONAL(MAKE_WINDOWS, test x$make_windows = xtrue)

dnl Generate Makefiles.
AC_CONFIG_FILES([
Makefile
Physics/Makefile
Shapes/Makefile
Sounds/Makefile
])
AC_OUTPUT

dnl Print summary.
echo "Configuration done. Now type \"make\"."
