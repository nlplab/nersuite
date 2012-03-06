# $Id: Makefile.am,v 1.1.1.1 2010/12/17 07:27:40 hccho Exp $

bin_PROGRAMS = nersuite_correct_mp

#man_MANS = nersuite.correct.mp.1

#EXTRA_DIST = ${man_MANS}

#EXTRA_DIST =

nersuite_correct_mp_SOURCES = \
	main.cpp \
	string_utils/string_utils.h \
	string_utils/string_utils.cpp \
	text_loader/text_loader.h \
	text_loader/text_loader.cpp \
	opt_parser/opt_parser.h \
	opt_parser/opt_parser.cpp \
	seg_rep_changer/seg_rep_changer.h \
	seg_rep_changer/seg_rep_changer.cpp \
	mp_corrector/mp_corrector.h \
	mp_corrector/mp_corrector.cpp
	
nersuite_correct_mp_CPPFLAGS = @CFLAGS@
INCLUDES = -Istring_utils -Itext_loader -Iopt_parser -Iseg_rep_changer -Imp_corrector @INCLUDES@

#AM_CFLAGS = @CFLAGS@
#INCLUDES = @INCLUDES@
#AM_LDFLAGS = @LDFLAGS@
#nersuite_CFLAGS = -I$(top_builddir)/lib/crf/include
#nersuite_LDADD = $(top_builddir)/lib/crf/libcrf.la

