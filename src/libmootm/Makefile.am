## File: src/Makefile.am
## Package: mootmorph
## Description:
##   + source-level automake file for DWDS tagger
##
## Process this file with Automake to create Makefile.in.
##-----------------------------------------------------------------------

##-----------------------------------------------------------------------
## subdirectories
##-----------------------------------------------------------------------
#SUBDIRS = 

##-----------------------------------------------------------------------
## primary targets
##-----------------------------------------------------------------------

## --- executable programs
#bin_PROGRAMS = ${prog_1} ... ${prog_N}
#bin_PROGRAMS = dwdspp moot moot-fstgen moot-pargen

## --- libtool libraries
#lib_LTLIBRARIES = ${lib_1}.la ... ${lib_N}.la
lib_LTLIBRARIES = libmootm.la

## --- shared data in @pkgdatadir@
#pkgdata_DATA = ${data_1} ... ${data_N}

## --- manpages -- section is auto-detected
#man_MANS = ${man_1} ... ${man_N}

##-----------------------------------------------------------------------
## sources
##-----------------------------------------------------------------------

## --- Required sources
#${prog_i}_SOURCES =
#${lib_i}_la_SOURCES =
libmootm_la_SOURCES = \
	mootFSMBase.cc \
	mootFSMPotsdam.cc \
	mootGfsmCommon.cc \
	mootFSMgfsm.cc \
	mootFSMgfsmxl.cc \
	mootFSMRWTH.cc \
	mootMorph.cc

## --- Extra (possible) sources
#EXTRA_${prog_i}_SOURCES = 
#EXTRA_LIBMOOTM_LA_SOURCES =

## --- Additional dependencies

##-----------------------------------------------------------------------
## headers
##-----------------------------------------------------------------------

## --- installable headers: @includedir@/@pkgname@
#pkginclude_HEADERS = ${hfile_1} ... ${hfile_N}
pkginclude_HEADERS = \
	mootmConfig.h \
	mootmUnConfig.h \
	mootFSMBase.h \
	mootFSMPotsdam.h \
	mootGfsmCommon.h \
	mootFSMgfsm.h \
	mootFSMgfsmxl.h \
	mootFSMRWTH.h \
	mootMorph.h


## --- no-install headers
#noinst_HEADERS = nopackage.h

##-----------------------------------------------------------------------
## pre-compile rules: gengetopt ; flex++ ; bison++
##-----------------------------------------------------------------------

SRCDIR = @srcdir@

## --- flex++/bison++ flags
FLEXXX ?= flex++
FLEXXX_SKEL = $(SRCDIR)/flexskel.cc
FLEXXX_HSKEL = $(SRCDIR)/flexskel.h
FLEXXX_DEPS = $(FLEXXX_SKEL) $(FLEXXX_HSKEL)
FLEXXX_FLAGS = -8 -S$(FLEXXX_SKEL) -H$(FLEXXX_HSKEL)
#FLEXXX_FLAGS = -8

BISONXX ?= bison++
BISONXX_SKEL = $(SRCDIR)/bison.cc
BISONXX_HSKEL = $(SRCDIR)/bison.h
BISONXX_DEPS = $(BISONXX_SKEL) $(BISONXX_HSKEL)
BISONXX_FLAGS = $(BISONXX_DFLAGS) -d -v -S$(BISONXX_SKEL) -H$(BISONXX_HSKEL)
#BISONXX_FLAGS = -d -v
#BISONXX_FLAGS = --debug -d -v

.ll.h: $(FLEXXX_DEPS)
	$(FLEXXX) $(FLEXXX_FLAGS) -h$*.h -o$*.cc $<

.ll.cc: $(FLEXXX_DEPS)
	$(FLEXXX) $(FLEXXX_FLAGS) -h$*.h -o$*.cc $<

.yy.cc: $(BISONXX_DEPS)
	$(BISONXX) $(BISONXX_FLAGS) -o $*.cc -h $*.h  $<

.yy.h: $(BISONXX_DEPS)
	$(BISONXX) $(BISONXX_FLAGS) -o $*.cc -h $*.h  $<

##-----------------------------------------------------------------------
## compile flags
##-----------------------------------------------------------------------
#SRCDIR = @srcdir@
#SRCINCLUDE = $(SRCDIR)/../include

## --- preprocessor flags
AM_CPPFLAGS = $(FSM_IFLAGS)

## --- compiler flags (c++)
AM_CXXFLAGS = $(mootm_WFLAGS) $(mootm_OFLAGS)

##-----------------------------------------------------------------------
## Additional variable: library versioning
##  + for -version-info $(CUR):$(REV):$(AGE) :
##  + base lib is created as libMYLIB.so.$(CUR).$(AGE).$(REV)
##-----------------------------------------------------------------------

## --- The most recent interface number that this library implements.
LIBCUR = 2

## --- The difference between the newest and oldest interfaces that this
##     library implements.  In other words, the library implements all the
##     interface numbers in the range from number `CURRENT - AGE' to
##     `CURRENT'.
LIBAGE = 0

## --- The implementation number of the CURRENT interface.
LIBREV = 0

##-----------------------------------------------------------------------
## linker flags
##   + library special handling of configure.in vars:
##-----------------------------------------------------------------------
#${prog_i}_LDFLAGS = -L. -static
#${prog_i}_LDADD   = ${non_src_file}.o -lsomelib

#${lib_i}_la_LDFLAGS = -L. -version-info ${lib_i_current}:${lib_i_rev}:${lib_i_age}
#${lib_i}_la_LIBADD  = -lotherlib

#libmootm_la_LDFLAGS = @FSM_LFLAGS@
#libmootm_la_LIBADD  = -lFSMExt @FSM_LIBS@

libmootm_la_LDFLAGS = -version-info $(LIBCUR):$(LIBREV):$(LIBAGE)

#libmootm_la_LDFLAGS =
#libmootm_la_LIBADD  = -lFSMExt -lFSM -lFSMSymSpec -lbumble++ @STL_LIBS@ -lz

#libmootm_la_LIBADD = @FSMEXT_LIBS@
libmootm_la_LIBADD = @FSM_LIBS@

##-----------------------------------------------------------------------
## Variables: cleanup
##-----------------------------------------------------------------------

## --- mostlyclean: built by 'make' & commonly rebuilt
#MOSTLYCLEANFILES =

## --- clean:  built by 'make'
#CLEANFILES =

## --- distclean: built by 'configure'
#DISTCLEANFILES =

## -- maintainerclean: built by maintainer / by hand
MAINTAINERCLEANFILES = \
	*~ .*~ Makefile Makefile.in \
	mootTaggerLexer.cc mootTaggerLexer.h \
	mootPPLexer.cc mootPPLexer.h \
	mootTaglistLexer.cc mootTaglistLexer.h \
	mootAlphabetLexer.cc mootAlphabetLexer.h \
	mootAlphabetParser.cc mootAlphabetParser.h \
	mootNgramsLexer.cc mootNgramsLexer.h \
	mootNgramsParser.cc mootNgramsParser.h \
	mootLexfreqsLexer.cc mootLexfreqsLexer.h \
	mootLexfreqsParser.cc mootLexfreqsParser.h \
	gmon.out

##-----------------------------------------------------------------------
## Variables: distribution
##-----------------------------------------------------------------------

#	moot.sym

## --- extra distribution files
#EXTRA_DIST =


## --- recursion subdirectories for 'make dist'
#DIST_SUBDIRS = $(SUBDIRS)

## --- dist-hook: when another 'Makefile.am' is overkill
#DISTHOOK_DIRS = foo
#DISTHOOK_FILES = foo/bar.txt foo/baz.txt
#dist-hook:
#	for d in $(DISTHOOK_DIRS); do\
#	  mkdir -p $(distdir)/$$d ;\
#	done
#	for f in $(DISTHOOK_FILES); do\
#	  cp -p $(srcdir)/$$f $(distdir)/$$f ;\
#	done

##-----------------------------------------------------------------------
## Rules: cleanup
##-----------------------------------------------------------------------
.PHONY: cvsclean

cvsclean: maintainer-clean ;
