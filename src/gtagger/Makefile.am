# $Id: Makefile.am,v 1.2 2011/02/25 15:21:30 hccho Exp $

bin_PROGRAMS = nersuite_gtagger

#man_MANS = nersuite.gtagger.1

#EXTRA_DIST = ${man_MANS}

#EXTRA_DIST =

nersuite_gtagger_SOURCES = \
	run.gtagger.cpp \
	option_parser/option_parser.h \
	../nersuite_common/text_loader.h \
	../nersuite_common/text_loader.cpp \
	geniatagger-3.0.1/bidir.cpp \
	geniatagger-3.0.1/chunking.cpp \
	geniatagger-3.0.1/maxent.cpp \
	geniatagger-3.0.1/maxent.h \
	geniatagger-3.0.1/morph.cpp \
	geniatagger-3.0.1/tokenize.cpp \
	geniatagger-3.0.1/postag.cpp \
	geniatagger-3.0.1/common.h

nersuite_gtagger_CPPFLAGS = -Wno-deprecated @CFLAGS@
INCLUDES = -I./geniatagger-3.0.1 -I./option_parser -I../nersuite_common

#AM_CFLAGS = @CFLAGS@
#INCLUDES = @INCLUDES@
#AM_LDFLAGS = @LDFLAGS@



