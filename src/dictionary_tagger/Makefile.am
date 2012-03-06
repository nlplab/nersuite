# $Id: Makefile.am,v 1.1.1.1 2010/12/17 07:27:40 hccho Exp $

bin_PROGRAMS = nersuite_dic_tagger

#man_MANS = nersuite.dic.tagger.1

#EXTRA_DIST = ${man_MANS}

#EXTRA_DIST =

nersuite_dic_tagger_SOURCES = \
	sentence_tagger.cpp \
	sentence_tagger.h \
	main.cpp

nersuite_dic_tagger_CPPFLAGS = @CFLAGS@
INCLUDES = -Icdbpp-1.1/include @INCLUDES@

#AM_CFLAGS = @CFLAGS@
#INCLUDES = @INCLUDES@
#AM_LDFLAGS = @LDFLAGS@
nersuite_dic_tagger_LDADD = ../nersuite_common/libnersuite_common.a
