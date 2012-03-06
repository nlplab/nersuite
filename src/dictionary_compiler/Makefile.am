# $Id: Makefile.am,v 1.1.1.1 2010/12/17 07:27:40 hccho Exp $

bin_PROGRAMS = nersuite_dic_compiler

#man_MANS = nersuite.dic.compiler.1

#EXTRA_DIST = ${man_MANS}

#EXTRA_DIST =

nersuite_dic_compiler_SOURCES = \
	main.cpp

nersuite_dic_compiler_CPPFLAGS = @CFLAGS@
INCLUDES = -Icdbpp-1.1/include @INCLUDES@

#AM_CFLAGS = @CFLAGS@
#INCLUDES = @INCLUDES@
#AM_LDFLAGS = @LDFLAGS@
nersuite_dic_compiler_LDADD = ../nersuite_common/libnersuite_common.a

