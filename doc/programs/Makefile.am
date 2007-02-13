## File: doc/programs/Makefile.am
## Package: mootm
## Description:
##   + program-doc-level automake file for moocow's PoS tagger programs
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

## --- manpages (section 1: always)
man1_MANS = @DOC_PROG_MAN_TARGETS@

pkgdocprog_DATA = \
	@DOC_PROG_TXT_TARGETS@ \
	@DOC_PROG_HTML_TARGETS@ \
	@DOC_PROG_DVI_TARGETS@ \
	@DOC_PROG_PS_TARGETS@ \
	@DOC_PROG_PDF_TARGETS@

##-----------------------------------------------------------------------
## sources
##-----------------------------------------------------------------------

SUFFIXES = .gog .pod .txt .1 .html .tex .dvi .ps .pdf

##-----------------------------------------------------------------------
## local hooks
##-----------------------------------------------------------------------
all-local: $(man_MANS)

if HAVE_POD2TEXT
txt: $(mootm_program_gogs:.gog=.txt)
endif

if HAVE_POD2MAN
man: $(mootm_program_gogs:.gog=.1)
endif

if HAVE_POD2HTML
html: $(mootm_program_gogs:.gog=.html)
endif

if HAVE_POD2LATEX
latex: $(mootm_program_gogs:.gog=.tex)

##-- distcheck always builds a 'dvi' target --
#dvi: $(mootm_program_gogs:.gog=.dvi)
dvi_doc: $(mootm_program_gogs:.gog=.dvi)

ps: $(mootm_program_gogs:.gog=.ps)

pdf: $(mootm_program_gogs:.gog=.pdf)
endif


##-----------------------------------------------------------------------
## pod compilation rules: Getopt::Gen
##-----------------------------------------------------------------------

.SECONDARY:

OPTGENFLAGS = -l

if HAVE_OPTGEN
.gog.pod:
	$(OPTGEN_PERL) $(OPTGENFLAGS) --nocfile --nohfile -F $* $<
endif

##-----------------------------------------------------------------------
## pod conversion rules
##-----------------------------------------------------------------------

POD2MANFLAGS = \
	--center="moocow's Morphology Tools" \
	--release="mootm v$(VERSION)"

POD2HTMLFLAGS = --podpath="$(srcdir):." --htmlroot="." --css="mootprogs.css"


if HAVE_POD2MAN
.pod.1:
	$(POD2MAN) $(POD2MANFLAGS) --section=1 $< $@
endif

if HAVE_POD2TEXT
.pod.txt:
	$(POD2TEXT) $< $@
endif

if HAVE_POD2HTML
.pod.html:
	$(POD2HTML) $(POD2HTMLFLAGS) --infile=$< --outfile=$@
endif

if HAVE_POD2LATEX
.pod.tex:
	$(POD2LATEX) --full --out $@ $<
endif

##-----------------------------------------------------------------------
## latex compilation
##-----------------------------------------------------------------------
.tex.dvi:
	latex $*
	latex $*

.dvi.ps:
	dvips $*

.tex.pdf:
	pdflatex $*
	pdflatex $*

##-----------------------------------------------------------------------
## Variables: cleanup
##-----------------------------------------------------------------------

## --- mostlyclean: built by 'make' & commonly rebuilt
#MOSTLYCLEANFILES =

## --- clean:  built by 'make'
CLEANFILES = \
	$(filter-out acknowledge.pod,$(wildcard *.pod)) \
	*.dvi *.ps *.pdf \
	*.log *.aux *.toc *.idx *.tmp *~~

if HAVE_POD2MAN
CLEANFILES += *.man *.1
endif
if HAVE_POD2HTML
CLEANFILES += *.html
endif
if HAVE_POD2TEXT
CLEANFILES += *.txt
endif
if HAVE_POD2LATEX
CLEANFILES += *.tex
endif

## --- distclean: built by 'configure'
DISTCLEANFILES = *.gog
if HAVE_OPTGEN
DISTCLEANFILES += *.pod
endif

## -- maintainerclean: built by maintainer / by hand
MAINTAINERCLEANFILES = \
	*~ .*~ Makefile Makefile.in \
	*.pod *.man *.1 *.html *.txt *.tex *.dvi *.ps *.pdf

##-----------------------------------------------------------------------
## Variables: distribution
##-----------------------------------------------------------------------

## --- extra distribution files
EXTRA_DIST = \
	@DOC_PROG_TXT_TARGETS@  \
	@DOC_PROG_MAN_TARGETS@  \
	mootprogs.css

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
