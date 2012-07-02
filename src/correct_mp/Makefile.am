# $Id: Makefile.am,v 1.1.1.1 2010/12/17 07:27:40 hccho Exp $

bin_PROGRAMS = nersuite_correct_mp

#man_MANS = nersuite.correct.mp.1

#EXTRA_DIST = ${man_MANS}

#EXTRA_DIST =

nersuite_correct_mp_SOURCES = \
	main.cpp \
	../nersuite_common/string_utils.h \
	../nersuite_common/string_utils.cpp \
	../nersuite_common/text_loader.h \
	../nersuite_common/text_loader.cpp \
	../nersuite_common/option_parser.h \
	seg_rep_changer.h \
	seg_rep_changer.cpp \
	mp_corrector.h \
	mp_corrector.cpp
	
nersuite_correct_mp_CPPFLAGS = @CFLAGS@
INCLUDES = -I../nersuite_common @INCLUDES@

#AM_CFLAGS = @CFLAGS@
#INCLUDES = @INCLUDES@
#AM_LDFLAGS = @LDFLAGS@
#nersuite_CFLAGS = -I$(top_builddir)/lib/crf/include
#nersuite_LDADD = $(top_builddir)/lib/crf/libcrf.la

