## File: src/programs/Makefile.am
## Package: mootm
## Description:
##   + src-level automake file for moot PoS tagger programs: morph
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
bin_PROGRAMS = mootm

## --- libtool libraries
#lib_LTLIBRARIES = ${lib_1}.la ... ${lib_N}.la

## --- shared data in @pkgdatadir@
#pkgdata_DATA = ${data_1} ... ${data_N}

## --- manpages -- section is auto-detected
#man_MANS = ${man_1} ... ${man_N}

##-----------------------------------------------------------------------
## sources
##-----------------------------------------------------------------------

SUFFIXES = \
	.gog _cmdparser.c _cmdparser.cc _cmdparser.h \
	.pod .txt .1 .html .tex

## --- Required sources
#${prog_i}_SOURCES =
#${lib_i}_la_SOURCES =
mootm_SOURCES = \
	mootm_main.cc \
	mootm_cmdparser.cc mootm_cmdparser.h

## --- Extra (possible)sources
#EXTRA_${prog_i}_SOURCES = 
#EXTRA_${lib_i}_la_SOURCES = 

## --- Additional dependencies
mootm_main.o: mootm_cmdparser.h

##-----------------------------------------------------------------------
## headers
##-----------------------------------------------------------------------

## --- installable headers: @includedir@/@pkgname@
#pkginclude_HEADERS = ${hfile_1} ... ${hfile_N}

## --- no-install headers
noinst_HEADERS = config.h

##-----------------------------------------------------------------------
## pre-compile rules: gengetopt
##-----------------------------------------------------------------------

#.ggo.h:
#	gengetopt -u -F$* < $^
#	mv $*.c $*.cc

#.ggo.cc:
#	gengetopt -u -F$* < $^
#	mv $*.c $*.cc

##-----------------------------------------------------------------------
## pre-compile rules: Getopt::Gen
##-----------------------------------------------------------------------

.SECONDARY:

OPTGENFLAGS = -u -l

.gog_cmdparser.h:
	optgen.perl $(OPTGENFLAGS) --nopod -F $*_cmdparser $<
	mv $*_cmdparser.c $*_cmdparser.cc

.gog_cmdparser.cc:
	optgen.perl $(OPTGENFLAGS) --nopod -F $*_cmdparser $<
	mv $*_cmdparser.c $*_cmdparser.cc

.gog.pod:
	optgen.perl $(OPTGENFLAGS) --nocfile --nohfile -F $* $<

##-----------------------------------------------------------------------
## pod conversion
##-----------------------------------------------------------------------

POD2MANFLAGS = --center="moocow's morphology tools"

.pod.1:
	pod2man $(POD2MANFLAGS) --section=1 $< $@

.pod.txt:
	pod2text $< $@

.pod.html:
	pod2html --infile=$< --outfile=$@

.pod.tex:
	pod2latex --full --out $@ $<

##-----------------------------------------------------------------------
## compile flags
##-----------------------------------------------------------------------

## --- preprocessor flags
AM_CPPFLAGS = $(FSM_IFLAGS) -I@srcdir@/../libmootm -I../libmootm

## --- compiler flags (c++)
AM_CXXFLAGS = $(mootm_WFLAGS) $(mootm_OFLAGS)

##-----------------------------------------------------------------------
## linker flags
##   + library special handling of configure.in vars:
##-----------------------------------------------------------------------
#${prog_i}_LDFLAGS = -L. -static
#${prog_i}_LDADD   = ${non_src_file}.o -lsomelib
#mootm_LDFLAGS = -L.

#LDFLAGS_COMMON = @FSM_LFLAGS@ -L../libmootm
LDFLAGS_COMMON = -L../libmootm

#LDADD_COMMON   = -lmootm -lFSMExt @FSM_LIBS@
#LDADD_COMMON   = -lmootm -lFSMExt -lFSM -lFSMSymSpec -lbumble++ @STL_LIBS@ -lz

LDADD_COMMON   = ../libmootm/libmootm.la

mootm_LDFLAGS = $(LDFLAGS_COMMON)
mootm_LDADD = $(LDADD_COMMON)


#${lib_i}_la_LDFLAGS = -L. -version-info ${lib_i_current}:${lib_i_rev}:${lib_i_age}
#${lib_i}_LIBADD     = -lotherlib

##-----------------------------------------------------------------------
## Variables: cleanup
##-----------------------------------------------------------------------

## --- mostlyclean: built by 'make' & commonly rebuilt
#MOSTLYCLEANFILES =

## --- clean:  built by 'make'
CLEANFILES = \
	*.o \
	*.man *.1 *.3 *.5 *.html *.pdf *.tex *.txt *.tmp

## --- distclean: built by 'configure'
#DISTCLEANFILES =

## -- maintainerclean: built by maintainer / by hand
MAINTAINERCLEANFILES = \
	*~ .*~ Makefile Makefile.in \
	*_cmdparser.cc *_cmdparser.h

##-----------------------------------------------------------------------
## Variables: distribution
##-----------------------------------------------------------------------

## --- extra distribution files
EXTRA_DIST = \
	mootm.gog \
	acknowledge.pod \
	mootmrc

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
