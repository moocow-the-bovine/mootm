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
## Potential targets
##-----------------------------------------------------------------------

##-- pods
DOC_MAN1_PODS = $(DOC_MAN1_GOGS:.gog=.pod)
DOC_MAN5_PODS = $(DOC_MAN5_GOGS:.gog=.pod)
DOC_PODS      = $(DOC_GOGS:.gog=.pod)

##--- manpages (section 1: always)
DOC_MAN1_TARGETS = $(DOC_MAN1_PODS:.pod=.1)
DOC_MAN5_TARGETS = $(DOC_MAN5_PODS:.pod=.5)
DOC_MAN_TARGETS  = $(DOC_MAN1_TARGETS) $(DOC_MAN5_TARGETS)

##-- text
DOC_TXT_TARGETS = $(DOC_PODS:.pod=.txt)

##-- html
DOC_HTML_TARGETS = $(DOC_PODS:.pod=.html)
DOC_HTML_STATIC  = mootprogs.css

##-- latex
DOC_LATEX_TARGETS = $(DOC_PODS:.pod=.tex)
DOC_DVI_TARGETS   = $(DOC_PODS:.pod=.dvi)
DOC_PS_TARGETS    = $(DOC_PODS:.pod=.ps)
DOC_PDF_TARGETS   = $(DOC_PODS:.pod=.pdf)


##-----------------------------------------------------------------------
## Conditional targets: chosen by 'configure' & co.
##-----------------------------------------------------------------------

##-- automake 'EXTRA_*' vars
EXTRA_man1_MANS       = $(DOC_MAN1_TARGETS)
EXTRA_man5_MANS       = $(DOC_MAN5_TARGETS)

##-- automake doesn't appear to understand EXTRA_${whatever}_DATA
EXTRA_pkgdocprog_DATAFILES =
	$(DOC_TXT_TARGETS) \
	$(DOC_HTML_TARGETS) $(DOC_HTML_STATIC) \
	$(DOC_DVI_TARGETS) \
	$(DOC_PS_TARGETS) \
	$(DOC_PDF_TARGETS)

##-- accumulation variables
BUILD_PODS=
BUILD_MAN1_MANS=
BUILD_MAN5_MANS=
BUILD_DOCS=

STATIC_PODS=
STATIC_MAN1_MANS=
STATIC_MAN5_MANS=
STATIC_DOCS=

##--- selected: pods
if HAVE_OPTGEN
BUILD_PODS += $(DOC_PODS)
else
STATIC_PODS += $(DOC_PODS)
endif

##--- selected: manpages (section 1: always)
if DOC_WANT_MAN
if HAVE_POD2MAN
BUILD_MAN1_MANS += $(DOC_MAN1_TARGETS)
BUILD_MAN5_MANS += $(DOC_MAN5_TARGETS)
else
STATIC_MAN1_MANS += $(DOC_MAN1_TARGETS)
STATIC_MAN5_MANS += $(DOC_MAN5_TARGETS)
endif
endif

##-- selected: text
if DOC_WANT_TXT
if HAVE_POD2TEXT
BUILD_DOCS += $(DOC_TXT_TARGETS)
endif
endif

##-- selected: html
if DOC_WANT_HTML
if HAVE_POD2HTML
BUILD_DOCS  += $(DOC_HTML_TARGETS)
STATIC_DOCS += $(DOC_HTML_STATIC)
else
STATIC_DOCS += $(DOC_HTML_TARGETS) $(DOC_HTML_STATIC)
endif
endif

##-- selected: latex->(dvi|ps|pdf)
if HAVE_POD2LATEX

if DOC_WANT_DVI
BUILD_DOCS += $(DOC_DVI_TARGETS)
endif

if DOC_WANT_PS
BUILD_DOCS += $(DOC_PS_TARGETS)
endif

if DOC_WANT_PDF
BUILD_DOCS += $(DOC_PDF_TARGETS)
endif

endif


##-----------------------------------------------------------------------
## Primary targets
##-----------------------------------------------------------------------

##-- primaries
man1_MANS = $(BUILD_MAN1_MANS) $(STATIC_MAN1_MANS)
man5_MANS = $(BUILD_MAN5_MANS) $(STATIC_MAN5_MANS)
pkgdocprog_DATA = $(BUILD_DOCS) $(STATIC_DOCS)


##-----------------------------------------------------------------------
## sources
##-----------------------------------------------------------------------

SUFFIXES = .gog .pod .txt .1 .html .tex .dvi .ps .pdf

##-----------------------------------------------------------------------
## local hooks
##-----------------------------------------------------------------------
all-local: $(man_MANS)

txt: $(DOC_TXT_TARGETS)

man: $(DOC_MAN_TARGETS)

html: $(DOC_HTML_TARGETS)

latex: $(DOC_LATEX_TARGETS)

##-- distcheck always builds a 'dvi' target --
dvi-doc: $(DOC_DVI_TARGETS)

ps: $(DOC_PS_TARGETS)

pdf: $(DOC_PDF_TARGETS)


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
.pod.5:
	$(POD2MAN) $(POD2MANFLAGS) --section=5 $< $@
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
	latex -interaction=nonstopmode $*
	latex -interaction=nonstopmode $*

.dvi.ps:
	dvips $*

.ps.pdf:
	ps2pdf $* $@

##-----------------------------------------------------------------------
## Variables: cleanup
##-----------------------------------------------------------------------

## --- mostlyclean: built by 'make' & commonly rebuilt
#MOSTLYCLEANFILES =

## --- clean:  built by 'make'
CLEANFILES = \
	*.log *.aux *.toc *.idx *.tmp *~~ \
	$(BUILD_PODS) \
	$(BUILD_MAN1_MANS) \
	$(BUILD_MAN5_MANS) \
	$(BUILD_DOCS)


## --- distclean: built by 'configure'
DISTCLEANFILES = *.gog
if HAVE_OPTGEN
DISTCLEANFILES += $(DOC_PODS)
endif

## -- maintainerclean: built by maintainer / by hand
MAINTAINERCLEANFILES = \
	*~ .*~ Makefile Makefile.in \
	$(DOC_PODS) \
	*.man *.1 *.html *.txt *.tex *.dvi *.ps *.pdf


##-----------------------------------------------------------------------
## Variables: distribution
##-----------------------------------------------------------------------

## --- extra distribution files
EXTRA_DIST = \
	$(DOC_PODS) \
	$(DOC_MAN_TARGETS) \
	$(DOC_HTML_TARGETS) \
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
