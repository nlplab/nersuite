# $Id: Makefile.am,v 1.1.1.1 2010/12/17 07:27:39 hccho Exp $
# Edited 2010-09-07 hancheol $

SUBDIRS = \
	src/nersuite_common \
	src/tokenizer \
	src/gtagger \
	src/nersuite \
	src/dictionary_compiler \
	src/dictionary_tagger \
	src/correct_mp

docdir = $(prefix)/share/doc/@PACKAGE@
doc_DATA = README INSTALL COPYING AUTHORS ChangeLog

#models_gtaggerdir = $(prefix)/models_gtagger
#models_gtagger_DATA = \
#pkgdata_DATA = \
#	models_gtagger/WORDNETLICENSE \
#	....

#models_nersuitedir = $(prefix)/models_nersuite
#models_nersuite_DATA = 

EXTRA_DIST = \
	sample/test.txt
#	${models_gtagger_DATA} \
#	${models_nersuite_DATA} \
#	sample/test.sh

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
