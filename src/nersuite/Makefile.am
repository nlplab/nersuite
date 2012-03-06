# $Id: Makefile.am,v 1.2 2011/02/25 20:41:12 hccho Exp $

bin_PROGRAMS = nersuite

#man_MANS = nersuite.main.1

#EXTRA_DIST = ${man_MANS}

#EXTRA_DIST =

nersuite_SOURCES = \
	main.cpp \
	nersuite.cpp \
	nersuite.h \
	typedefs.h \
	FExtor.h \
	FExtor.cpp \
	crfsuite2.h \
	crfsuite2.cpp

nersuite_CPPFLAGS = @CFLAGS@
INCLUDES = @INCLUDES@

#AM_CFLAGS = @CFLAGS@
#INCLUDES = @INCLUDES@
#AM_LDFLAGS = @LDFLAGS@
#nersuite_CFLAGS = -I$(top_builddir)/lib/crf/include
#nersuite_LDADD = $(top_builddir)/lib/crf/libcrf.la
nersuite_LDADD = ../nersuite_common/libnersuite_common.a


