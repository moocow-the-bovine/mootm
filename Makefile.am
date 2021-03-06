#-----------------------------------------------------------------------
# File: ./Makefile.am
# Package: mootmorph
# Description:
#   + top-level automake file for DWDS tagger
#
# Process this file with Automake to create Makefile.in.
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Options & Subdirectories
#-----------------------------------------------------------------------

## --- automake options
#AUTOMAKE_OPTIONS = foreign dist-bzip2
AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

## --- recursion subdirectories
SUBDIRS = config src doc

## --- pseudo-deps for '.SUFFIXES'
SUFFIXES = .pod .txt .deb .rpm .spec .ifiles

#-----------------------------------------------------------------------
# Additional Variables & Rules: PODS
#-----------------------------------------------------------------------
PODS = README.pod

if HAVE_POD2TEXT
.pod.txt:
	pod2text $< $@
endif

if HAVE_POD2HTML
.pod.html:
	pod2html --outfile $@ $<
endif

#-----------------------------------------------------------------------
# Variables: docs (requires that 'pkgdocdir' gets set in configure.in
#-----------------------------------------------------------------------
pkgdoc_DATA = \
	$(PODS:.pod=.txt) \
	$(PODS:.pod=.html) \
	AUTHORS \
	COPYING \
	ChangeLog


#-----------------------------------------------------------------------
# Variables: installation (pkg-config)
#-----------------------------------------------------------------------
pkgconfig_DATA = @PACKAGE@.pc

#-----------------------------------------------------------------------
# file-list (for .rpm building) [OBSOLETE (argh!)]
#-----------------------------------------------------------------------
ifiles: $(PACKAGE).ifiles
$(PACKAGE).ifiles: $(DISTFILES)
	$(MAKE) all
	chmod -R u+w =ifiles; rm -rf =ifiles
	mkdir =ifiles
	$(MAKE) install prefix=`(cd =ifiles && pwd)`${prefix}
	( cd =ifiles \
	  && find . \! -type d -print \
	     | sed 's/^\.//1' \
	) > $@
	chmod -R u+w =ifiles; rm -rf =ifiles

#-----------------------------------------------------------------------
# Variables: debian
#-----------------------------------------------------------------------
DEBIAN_FILES = \
	debian/README.Debian \
	debian/$(BINDIST_PKGNAME).dirs \
	debian/$(BINDIST_PKGNAME).files \
	debian/$(BINDIST_PKGNAME)-doc.dirs \
	debian/$(BINDIST_PKGNAME)-doc.files \
	debian/changelog \
	debian/control \
	debian/copyright \
	debian/docs \
	debian/shlibs \
	debian/rules

DEBIAN_CLEANFILES = \
	debian/files \
	debian/dirs \
	debian/*.debhelper \
	debian/*~

#-----------------------------------------------------------------------
# Rules: binary package: debian .deb
#-----------------------------------------------------------------------

BINDIST_DEB = $(BINDIST_PKGNAME)_$(VERSION)-$(BINDIST_RELEASE)_$(BINDIST_CPU).deb
DPKG_BUILDPACKAGE_FLAGS = -us -uc -rfakeroot

deb: $(BINDIST_DEB)
$(BINDIST_DEB): $(DISTFILES)
	rm -rf $(BINDIST_PKGNAME)
	$(MAKE) distdir
	mv $(distdir) $(BINDIST_PKGNAME)
	cd $(BINDIST_PKGNAME) && dpkg-buildpackage $(DPKG_BUILDPACKAGE_FLAGS)
#	#rm -rf $(BINDIST_PKGNAME)

# cleanup debs
nodeb:
	rm -rf $(BINDIST_PKGNAME)
	rm -f $(BINDIST_PKGNAME)_*-*_$(BINDIST_CPU).deb
	rm -f $(BINDIST_PKGNAME)_*-*_$(BINDIST_CPU).changes
	rm -f $(BINDIST_PKGNAME)_*-*.tar.gz
	rm -f $(BINDIST_PKGNAME)_*-*.dsc


#-----------------------------------------------------------------------
# Rules: binary package: slackware .tgz
#-----------------------------------------------------------------------
BINDIST_TGZ = $(BINDIST_PKGNAME)-$(VERSION).tgz

tgz: $(BINDIST_TGZ)
$(BINDIST_TGZ): $(BINDIST_DEB)
	fakeroot alien --keep-version --to-tgz $(BINDIST_DEB)

notgz:
	rm -f $(BINDIST_TGZ)

#-----------------------------------------------------------------------
# Rules: binary package: hacked native
#-----------------------------------------------------------------------
BINDIST_NATIVE = $(PACKAGE)-$(VERSION)-$(BINDIST_RELEASE)-$(BINDIST_CPU)-$(BINDIST_OS)

bindist: $(BINDIST_NATIVE).tar.gz
$(BINDIST_NATIVE).tar.gz: all
	rm -rf $(BINDIST_NATIVE)
	mkdir $(BINDIST_NATIVE)
	($(MAKE) prefix="$(PWD)/$(BINDIST_NATIVE)" install \
	  && cd $(BINDIST_NATIVE) \
	  && fakeroot $(AMTAR) cof - . | GZIP=$(GZIP_ENV) gzip -c >../$@ \
	)
	rm -rf $(BINDIST_NATIVE)

nobindist:
	rm -f $(PACKAGE)-*-*-$(BINDIST_CPU)-$(BINDIST_OS).tar.gz

#-----------------------------------------------------------------------
# Rules: binary package: static .tar.gz
#-----------------------------------------------------------------------
BINDIST_STATIC = $(BINDIST_NATIVE)-static

static: $(BINDIST_STATIC).tar.gz

$(BINDIST_STATIC).tar.gz: distdir
	mv $(distdir) $(BINDIST_STATIC)
	( cd $(BINDIST_STATIC) \
	  && ./configure --disable-shared --enable-all-static \
	     CXXFLAGS="-mcpu=$(BINDIST_CPU)" \
	  && $(MAKE) all \
	  && mkdir $(BINDIST_STATIC) \
	  && $(MAKE) prefix="`pwd`/$(BINDIST_STATIC)" install \
	  && fakeroot $(AMTAR) cof - $(BINDIST_STATIC) | GZIP=$(GZIP_ENV) gzip -c >../$@ \
	) && \
	rm -rf $(BINDIST_STATIC)

$(BINDIST_STATIC)-___OLD___.tar.gz: distdir
	mv $(distdir) $(BINDIST_STATIC)
	( cd $(BINDIST_STATIC) \
	  && ./configure --disable-shared \
	     CXXFLAGS="-mcpu=$(BINDIST_CPU)" \
	  && $(MAKE) all LDFLAGS="-all-static" \
	  && mkdir $(BINDIST_STATIC) \
	  && $(MAKE) prefix="`pwd`/$(BINDIST_STATIC)" install \
	  && fakeroot $(AMTAR) cof - $(BINDIST_STATIC) | GZIP=$(GZIP_ENV) gzip -c >../$@ \
	) && \
	rm -rf $(BINDIST_STATIC)

nostatic:
	rm -rf $(PACKAGE)-*-*-$(BINDIST_CPU)-$(BINDIST_OS)-static*

#-----------------------------------------------------------------------
# Rules: binary package: rpm
#-----------------------------------------------------------------------

BINDIST_RPM = $(PACKAGE)-$(VERSION)-$(BINDIST_RELEASE).$(BINDIST_CPU).rpm
RPMSPEC ?= $(PACKAGE).spec
RPMROOT ?= $(PWD)/rpm
RPMFLAGS ?= -bb -vv --define '_topdir $(RPMROOT)'
RPM ?= rpm

# .rpm derived from .deb
drpm: $(BINDIST_DEB)
	for deb in \
	   $(BINDIST_PKGNAME)*_$(VERSION)-$(BINDIST_RELEASE)_$(BINDIST_CPU).deb \
	   $(BINDIST_PKGNAME)*_$(VERSION)-$(BINDIST_RELEASE)_all.deb ; do \
	 fakeroot alien --keep-version --to-rpm "$$deb" ;\
	done

# .rpm built from its own .spec
rpm: $(BINDIST_RPM)
$(BINDIST_RPM): $(DISTFILES) $(RPMSPEC)
	$(MAKE) dist
	mkdir -p \
	  $(RPMROOT) \
	  $(RPMROOT)/BUILD \
	  $(RPMROOT)/SOURCES \
	  $(RPMROOT)/SPECS \
	  $(RPMROOT)/SRPMS \
	  $(RPMROOT)/RPMS
	rm -f $(RPMROOT)/SOURCES/$(PACKAGE)-$(VERSION).tar.gz
	ln -s $(PWD)/$(PACKAGE)-$(VERSION).tar.gz $(RPMROOT)/SOURCES
	$(RPM) $(RPMFLAGS) $(RPMSPEC)
	mv $(RPMROOT)/RPMS/$(BINDIST_CPU)/$(PACKAGE)*-$(VERSION)-$(BINDIST_RELEASE).*.rpm .
#	## -- cleanup
	rm -f $(RPMROOT)/SOURCES/$(PACKAGE)-$(VERSION).tar.gz
	rm -rf $(RPMROOT)/BUILD/$(PACKAGE)-$(VERSION)

norpm:
	rm -f *.rpm


#-----------------------------------------------------------------------
# Rules: packaging: cleanup
#-----------------------------------------------------------------------
noarc: nodist nodeb norpm notgz nobindist

nodist:
	rm -f $(PACKAGE)-$(VERSION).tar.* $(PACKAGE)-$(VERSION).zip


#-----------------------------------------------------------------------
# Variables: cleanup
#-----------------------------------------------------------------------

## --- mostlyclean: built by 'make' & commonly rebuilt
#MOSTLYCLEANFILES =

## --- clean: built by 'make'
CLEANFILES = \
	*.tmp

## --- distclean: built by 'configure'
DISTCLEANFILES = \
	config.cache \
	config.log \
	config.status

## -- maintainerclean: built by maintainer / by hand
MAINTAINERCLEANFILES = *~ \
	$(PODS:.pod=.txt) \
	$(PODS:.pod=.html) \
	Makefile Makefile.in \
	aclocal.m4 \
	configure

#CVSCLEAN_SUBDIRS = $(SUBDIRS)

#CVSCLEANFILES = Makefile.in Makefile


#-----------------------------------------------------------------------
# Variables: distribution
#-----------------------------------------------------------------------

## --- extra distribution files
EXTRA_DIST = \
	INSTALL \
	AUTHORS \
	$(PODS) \
	$(PODS:.pod=.txt) \
	$(PODS:.pod=.html) \
	$(DEBIAN_FILES) \
	autogen.sh \
	configure

## --- recursion subdirectories for 'make dist'
DIST_SUBDIRS = $(SUBDIRS)

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


#-----------------------------------------------------------------------
# Rules: cleanup
#-----------------------------------------------------------------------
.PHONY: cvsclean cvsclean-hook

cvsclean: maintainer-clean ;

#cvsclean-hook: ;
#	test -z "$(CVSCLEAN_SUBDIRS)" ||\
#	  for d in $(CVSCLEAN_SUBDIRS) ; do\
#	    $(MAKE) -C $$d cvsclean ;\
#	  done
#	rm -f $(CVSCLEANFILES)
