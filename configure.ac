dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.5)

dnl Some handy macros
define([THE_PACKAGE_NAME],    [mootm])
define([THE_PACKAGE_VERSION], [2.0.9-1])
define([THE_PACKAGE_AUTHOR],  [moocow@cpan.org])

AC_INIT(THE_PACKAGE_NAME, THE_PACKAGE_VERSION, THE_PACKAGE_AUTHOR)

dnl source && aux dir
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])

dnl canonical target (sets $target, $target_(cpu|vendor|os) : used for bindist)
AC_CANONICAL_TARGET

dnl use automake
AM_INIT_AUTOMAKE(THE_PACKAGE_NAME, THE_PACKAGE_VERSION)

dnl use autoheader
AM_CONFIG_HEADER([src/libmootm/mootmConfig.h])

dnl default prefix
AC_PREFIX_DEFAULT(/usr/local)

#-------------------------------------------------------------
# save user's *FLAGS
#USER_LIBS="$LIBS"
#USER_LDFLAGS="$LDFLAGS"
#USER_CPPFLAGS="$CPPFLAGS"
USER_CXXFLAGS="$CXXFLAGS"

dnl
dnl check for programs
dnl
AC_PROG_CC
AC_PROG_CXX
AC_LANG(C++)

dnl v--- needed if Makefile.am uses _LTLIBRARIES targets
AC_PROG_LIBTOOL

dnl v--- for static-only libraries (non-libtool)
dnl AC_PROG_RANLIB

dnl v--- needed if Makefile.am uses lex sources
dnl AM_PROG_LEX


### --- unmangle user's CXXFLAGS
if test "$CXXFLAGS" != "$USER_CXXFLAGS" ; then
  # autoconf likes to set this... who knows why...
  AC_MSG_NOTICE([Restoring user's original CXXFLAGS value])
  CXXFLAGS="$USER_CXXFLAGS"
fi

#---------------------------------------------------------------
# short package includes
spkgincludedir="\${includedir}/AC_PACKAGE_NAME"
AC_SUBST(spkgincludedir)
# short package includes
#---------------------------------------------------------------

#---------------------------------------------------------------
# get real prefix
AC_CACHE_CHECK([for installation prefix], [ac_cv_install_prefix],
	[if test "$prefix" = "NONE" ; then
	   ac_cv_install_prefix="/usr/local"
	 else
           ac_cv_install_prefix="$prefix"
	 fi
	])
dnl #  ... and add it to our flags
dnl CPPFLAGS="$CPPFLAGS -I${ac_cv_install_prefix}/include"
dnl LDFLAGS="$LDFLAGS -L${ac_cv_install_prefix}/lib"


dnl check for strdup
AC_CHECK_FUNC(strdup,[AC_DEFINE(HAVE_STRDUP,1,[Define this if you have the strdup() function])])


dnl ---------------------------------------------------------------
dnl pkg-config : program
dnl
AC_ARG_VAR(PKG_CONFIG, [How to run the pkg-config program])
AC_ARG_VAR(PKG_CONFIG_PATH, [Directories to search for pkg-config])
if test -z "$PKG_CONFIG" ; then
  AC_PATH_PROG(PKG_CONFIG,pkg-config,[])
fi
dnl pkg-config: destination directory
AC_ARG_WITH(pkgconfig-dir,
	AC_HELP_STRING([--with-pkgconfig-dir=DIR],
		[install pkg-config metafile(s) in DIR (default=LIBDIR/pkgconfig)]),
	[ac_cv_pkgconfigdir="$withval"])
if test -z "$ac_cv_pkgconfigdir" ; then
  ac_cv_pkgconfigdir="\$(libdir)/pkgconfig"
fi
pkgconfigdir="$ac_cv_pkgconfigdir"
AC_SUBST(pkgconfigdir)
dnl
dnl pkg-config
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



dnl vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
dnl check for time headers (argh)
AC_CHECK_HEADERS([time.h sys/time.h],
	[],
	[AC_MSG_WARN([Required header file not found!])
    	 AC_MSG_WARN([Are the 'struct timeval' and 'time()' headers in your 'CPPFLAGS' environment variable?])
    	 AC_MSG_ERROR([Required headers not found -- aborting.])
	])
dnl time headers
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



dnl ------------------------------------------------------------
dnl libs: all static?
dnl

 AC_ARG_ENABLE([all-static],
 	AC_HELP_STRING([--enable-all-static],
	               [Build standalone static binary]),
	[ac_cv_enable_all_static="$enableval"],[ac_cv_enable_all_static="no"])

 if test "$ac_cv_enable_all_static" = "yes" ; then
   LDFLAGS="-all-static $LDFLAGS"
 fi

dnl libs: static?
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


dnl ------------------------------------------------------------
dnl Which FSM lib? : ac_cv_fsm_which = | "gfsm" | "potsdam" | "rwth"(broken) | "gfsmxl"(broken)
dnl + TODO: OpenFst

ac_cv_fsm_try="gfsm,potsdam,rwth,gfsmxl"
ac_cv_fsm_which=""


AC_ARG_WITH([fsm],
	AC_HELP_STRING([--with-fsm=WHICH...],
		       [Specify comma-separated search order for underlying finite-state library.
		        Known values: "gfsm", "potsdam", "rwth".
			Default="gfsm,potsdam,rwth"
			]),
	[ac_cv_fsm_try="$withval"],
	[ac_cv_fsm_try="gfsm,potsdam,rwth"])

AC_ARG_WITH([rwth-fsa-dir],
	AC_HELP_STRING([--with-rwth-fsa-dir=DIR],
	               [Use RWTH Fsa library from DIR (default=search)]),
        [ac_cv_rwth_fsa_dir="$withval"]
	[ac_cv_rwth_fsa_dir=""])


fsm_which_libs="`echo $ac_cv_fsm_try | sed 's/\,/ /g'`"
for fsm_lib in $fsm_which_libs ; do
  case "$fsm_lib" in

    gfsmxl)
      PKG_CHECK_MODULES(GFSM, gfsmxl >= 0.0.6, [HAVE_GFSMXL="yes"], [HAVE_GFSMXL=""])
      if test "$HAVE_GFSMXL" = "yes" ; then
        ac_cv_fsm_which="gfsmxl"
	break
      fi
      ;;

    gfsm)
      PKG_CHECK_MODULES(GFSM, gfsm >= 0.0.8, [HAVE_GFSM="yes"], [HAVE_GFSM=""])
      if test "$HAVE_GFSM" = "yes" ; then
        ac_cv_fsm_which="gfsm"
	break
      fi
      ;;

    potsdam)
      PKG_CHECK_MODULES(FSMLIB, libFSM >= 0.9.0.1, [HAVE_FSMLIB="yes"], [HAVE_FSMLIB=""])
      if test "$HAVE_FSMLIB" = "yes" ; then
        ac_cv_fsm_which="potsdam"
	break
      fi
      ;;

    rwth)
      AC_MSG_CHECKING([For RWTH Fsa sources])
      if test -z "$ac_cv_rwth_fsa_dir"; then
        for bd in .. /usr/lib /usr/local/lib /usr/src /usr/local/src ; do
          for rwth_dir in $bd/fsa-* ; do
            if test -e "$rwth_dir/src/Fsa/Alphabet.cc" ; then
              ac_cv_rwth_fsa_dir="$rwth_dir"
              break
            fi
          done
          if test -n "$ac_cv_rwth_fsa_dir" ; then
	    break
          fi
        done
      fi ##-- test -z "$ac_cv_rwth_fsa_dir"

      if test -n "$ac_cv_rwth_fsa_dir" ; then
        AC_MSG_RESULT([found: \`$ac_cv_rwth_fsa_dir'])
	ac_cv_fsm_which="rwth"
	break;
      else
        AC_MSG_RESULT([not found])
      fi ##-- test -z "$ac_cv_rwth_fsa_dir"
      ;;

    *)
      AC_MSG_WARN([Unknown finite-state library "$fsm_lib" -- skipping.])
      continue
      ;;

  esac
done ##-- for fsm_lib in $fsm_which_libs


AC_MSG_CHECKING([which underlying finite-state library to use])

if test -z "$ac_cv_fsm_which" ; then
    AC_MSG_RESULT([none found])
    AC_MSG_WARN()
    AC_MSG_WARN([----------------------------------------------------------])
    AC_MSG_WARN([     Cannot find any underlying finite-state library!])
    AC_MSG_WARN()
    AC_MSG_WARN([        + Is your PKG_CONFIG_PATH set correctly ?])
    AC_MSG_WARN([        + Did you specify --with-rwth-fsa-dir=DIR ?])
    AC_MSG_WARN([----------------------------------------------------------])
    AC_MSG_WARN()
    AC_MSG_ERROR([Required package not found -- aborting.])
fi


case "$ac_cv_fsm_which" in

 potsdam)
   AC_MSG_RESULT([Potsdam libFSM])
   AC_DEFINE([USE_FSM_POTSDAM],1,
	     [Define this to use the Potsdam libFSM library])
   AC_DEFINE([MOOTM_FSM_LIBRARY],["Potsdam libFSM"],
	     [Human-readable name of the underlying finite-state library])
   ;;

 gfsmxl)
   AC_MSG_RESULT([libgfsmxl])
   AC_DEFINE([USE_FSM_GFSMXL],1,
	     [Define this to use the LGPL libgfsmxl library])
   AC_DEFINE([MOOTM_FSM_LIBRARY],["libgfsmxl"],
             [Human-readable name of the underlying finite-state library])
   ;;

 gfsm)
   AC_MSG_RESULT([libgfsm])
   AC_DEFINE([USE_FSM_GFSM],1,
	     [Define this to use the LGPL libgfsm library])
   AC_DEFINE([MOOTM_FSM_LIBRARY],["libgfsm"],
             [Human-readable name of the underlying finite-state library])
   ;;

 rwth)
   AC_MSG_RESULT([RWTH libFsa])

   AC_MSG_WARN()
   AC_MSG_WARN([----------------------------------------------------------])
   AC_MSG_WARN([        RWTH libFsa support is currently broken!])
   AC_MSG_WARN([----------------------------------------------------------])
   AC_MSG_WARN()

   AC_DEFINE([USE_FSM_RWTH],1,
	     [Define this to use the RWTH libFsa library])
   AC_DEFINE([MOOTM_FSM_LIBRARY],["RWTH libFsa"],
	     [Human-readable name of the underlying finite-state library])
   ;;

 *)
   AC_MSG_ERROR([Unknown finite-state library type "$ac_cv_fsm_which"])
   ;;

esac

dnl Which FSM lib?
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

dnl ------------------------------------------------------------
dnl FSM: gfsmxl

if test "$ac_cv_fsm_which" = "gfsmxl" ; then

  CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags-only-I gfsmxl`"
  LDFLAGS="$LDFLAGS `$PKG_CONFIG --libs-only-L gfsmxl`"
  FSM_LIBS="`$PKG_CONFIG --libs-only-l gfsmxl` $LIBS"

fi ##-- test "$ac_cv_fsm_which" = "gfsmxl"

dnl FSM: gfsmxl
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

dnl ------------------------------------------------------------
dnl FSM: gfsm

if test "$ac_cv_fsm_which" = "gfsm" ; then

  CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags-only-I gfsm`"
  LDFLAGS="$LDFLAGS `$PKG_CONFIG --libs-only-L gfsm`"
  FSM_LIBS="`$PKG_CONFIG --libs-only-l gfsm` $LIBS"

fi ##-- test "$ac_cv_fsm_which" = "gfsm"

dnl FSM: gfsm
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


dnl ------------------------------------------------------------
dnl FSM: Potsdam libFSM
dnl
if test "$ac_cv_fsm_which" = "potsdam" ; then

 CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags-only-I libFSM`"
 LDFLAGS="$LDFLAGS `$PKG_CONFIG --libs-only-L libFSM`"
 FSM_LIBS="`$PKG_CONFIG --libs-only-l libFSM` $LIBS"

 if test "$ac_cv_enable_all_static" = "yes" ; then
   FSM_LDADD_STATIC="-lFSMStaticHack"
 fi

fi ##-- test "$ac_cv_fsm_which" = "potsdam"

dnl FSM: Potsdam libFSM
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


dnl ------------------------------------------------------------
dnl FSM: RWTH libfsa
dnl

if test "$ac_cv_fsm_which" = "rwth" ; then

 RWTH_FSA_DIR="`(cd $ac_cv_rwth_fsa_dir ; pwd)`"
 RWTH_FSA_CONFIG="`make -C $RWTH_FSA_DIR config`"

 [RWTH_ARCH="`\
  echo \"$RWTH_FSA_CONFIG\" \
  | grep '^Processor:' \
  | perl -pe 's/^[^:]*:\s+//' \
  `"]

 [RWTH_OS="`\
  echo \"$RWTH_FSA_CONFIG\" \
  | grep '^Operating system:' \
  | perl -pe 's/^[^:]*:\s+//' \
  `"]

 [RWTH_VERSION_FULL="`\
  echo \"$RWTH_FSA_CONFIG\" \
  | grep '^Version:' \
  | perl -pe 's/^[^:]*:\s+//' \
  `"]
 [RWTH_VERSION=`echo "$RWTH_VERSION_FULL" | perl -pe 's/\s+-\s+.*$//'`]
 [RWTH_CVERSION=`echo "$RWTH_VERSION_FULL" | perl -pe 's/^.*\s+-\s+//'`]

 [RWTH_CPPFLAGS="`\
  echo \"$RWTH_FSA_CONFIG\" \
  | grep '^Preprocessor flags:' \
  | perl -p -e 's/^[^:]*:\s*//;' -e \"s|-I\.|-I$RWTH_FSA_DIR|g;\" \
  `"]

 [RWTH_CFLAGS="`\
  echo \"$RWTH_FSA_CONFIG\" \
  | grep '^C compiler flags:' \
  | perl -p -e 's/^[^:]*:\s+//;' -e \"s|-I\.|-I$RWTH_FSA_DIR|g;\" \
  `"]

 [RWTH_CXXFLAGS="`\
  echo \"$RWTH_FSA_CONFIG\" \
  | grep '^C\+\+ compiler flags:' \
  | perl -p -e 's/^[^:]*:\s+//;' -e \"s|-I\.|-I$RWTH_FSA_DIR|g;\" \
  `"]

 [RWTH_LDFLAGS_FULL="`\
  echo \"$RWTH_FSA_CONFIG\" \
  | grep '^Linker flags:' \
  | perl -p -e 's/^[^:]*:\s+//;' -e \"s|-L\.|-L$RWTH_FSA_DIR|g;\" \
  `"]

 RWTH_LDFLAGS=""
 RWTH_LIBS=""
 for f in $RWTH_LDFLAGS_FULL ; do
   case "$f" in
    -l*)
      RWTH_LIBS="$RWTH_LIBS $f"
      ;;
    -L*)
      RWTH_LDFLAGS="$RWTH_LDFLAGS $f"
      ;;
    *)
      ##-- ignore 'em (-rdynamic)
      ;;
   esac
 done

 [CPPFLAGS="$CPPFLAGS $RWTH_CPPFLAGS"]
 [LDFLAGS="$LDFLAGS $RWTH_LDFLAGS"]

 RWTH_OBJS="\
  $RWTH_FSA_DIR/src/Fsa/libSprintFsa.$RWTH_OS-$RWTH_ARCH-$RWTH_CVERSION.a \
  $RWTH_FSA_DIR/src/Core/libSprintCore.$RWTH_OS-$RWTH_ARCH-$RWTH_CVERSION.a \
  "

 FSM_LIBS="$FSM_LIBS $RWTH_LIBS"
 FSM_LDADD_STATIC="$RWTH_OBJS"

fi ##-- ac_cv_fsm_which = "rwth"

dnl FSM: RWTH libFsa
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



dnl ------------------------------------------------------------
dnl FSM: vars (generic)

AC_SUBST(FSM_LIBS)
AC_SUBST(FSM_LDADD_STATIC)

dnl FSM: vars (generic)
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


dnl vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
dnl libmoot
PKG_CHECK_MODULES(MOOT, moot >= 2.0.8,
	[PC_HAVE_MOOT="yes"],
	[PC_HAVE_MOOT=""])
  
if test -n "$PC_HAVE_MOOT" ; then
    CPPFLAGS="$CPPFLAGS `$PKG_CONFIG --cflags-only-I moot`"
    CXXFLAGS="$CXXFLAGS `$PKG_CONFIG --cflags-only-other moot`"
    LDFLAGS="$LDFLAGS `$PKG_CONFIG --libs-only-L moot`"
    LIBS="$LIBS `$PKG_CONFIG --libs-only-l moot`"
else
    AC_MSG_WARN()
    AC_MSG_WARN([----------------------------------------------------------])
    AC_MSG_WARN([             Cannot find moot >= 2.0.0!])
    AC_MSG_WARN()
    AC_MSG_WARN([        Is your PKG_CONFIG_PATH set correctly?])
    AC_MSG_WARN([----------------------------------------------------------])
    AC_MSG_WARN()
    AC_MSG_ERROR([Required package not found -- aborting.])

fi ## test -n "$PC_HAVE_MOOT"

dnl moot
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

dnl ------------------------------------------------------------
dnl DISTCHECK: simulate missing development-only packages
AC_ARG_VAR(DISTCHECK,
	[(Maintainer): set to nonempty value to simulate missing development packages])
if test -n "$DISTCHECK" ; then
   test -z "$OPTGEN_PERL" && OPTGEN_PERL=no
   test -z "$DOXYGEN" && DOXYGEN=no
   test -z "$POD2X" && POD2TEXT=no
   test -z "$POD2X" && POD2MAN=no
   test -z "$POD2X" && POD2HTML=no
   test -z "$POD2X" && POD2LATEX=no
fi
dnl /DISTCHECK
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


dnl ------------------------------------------------------------
dnl check for perl
AC_ARG_VAR(PERL, [Path to your perl interpreter.])
if test -z "$PERL" ; then
  AC_PATH_PROG(PERL,[perl],[])
fi
AC_SUBST(PERL)
dnl perl
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


dnl ------------------------------------------------------------
dnl check for optgen.perl
dnl
AC_ARG_VAR(OPTGEN_PERL, [Path to the 'optgen.perl' script (disable with OPTGEN_PERL=no)])

##-- test for working optgen.perl
AC_MSG_CHECKING([whether Getopt::Gen works])
  if $PERL -MGetopt::Gen -e'exit 0;' >>config.log 2>&1; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
  AC_MSG_WARN([])
  AC_MSG_WARN([Getopt::Gen (or something it depends on) is broken!])
  AC_MSG_WARN([- you probably need to fix your Parse::Lex module])
  AC_MSG_WARN([- hope you're not building from CVS])
  AC_MSG_WARN([])
  OPTGEN_PERL="no"
fi

if test -z "$OPTGEN_PERL" ; then
  AC_PATH_PROG(OPTGEN_PERL,[optgen.perl],[no])
  if test "$OPTGEN_PERL" = "no" ; then
    AC_MSG_WARN([])
    AC_MSG_WARN([missing or broken optgen.perl program])
    AC_MSG_WARN([- builds from CVS will probably fail])
    AC_MSG_WARN([])
  fi
fi

AC_MSG_NOTICE([setting OPTGEN_PERL=$OPTGEN_PERL])
AM_CONDITIONAL(HAVE_OPTGEN, [test "$OPTGEN_PERL" != "no"])
dnl
dnl /optgen.perl
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


dnl ------------------------------------------------------------
dnl documentation
dnl
AC_ARG_WITH(docdir,
	AC_HELP_STRING([--with-docdir=DIR],
		[install documentation in DIR/AC_PACKAGE_NAME (default=DATADIR/doc)]),
	[docdir="$withval"],
	[docdir="\${datadir}/doc"])
AC_ARG_ENABLE(doc,
	AC_HELP_STRING([--enable-doc],[Synonym for --with-doc-formats=none]),
	[enable_doc="$enableval"],[enable_doc="yes"])

pkgdocdir="\${docdir}/\${PACKAGE}"
pkgdocprogdir="\${docdir}/\${PACKAGE}/programs"
pkgdoclibdir="\${docdir}/\${PACKAGE}/lib\${PACKAGE}"

AC_SUBST(docdir)
AC_SUBST(pkgdocdir)
AC_SUBST(pkgdocprogdir)
AC_SUBST(pkgdoclibdir)

AC_ARG_WITH(doc-formats,
	AC_HELP_STRING([--with-doc-formats=LIST],
		       [Build documentation formats in LIST. \
                        Available formats: txt, man, html, dvi, ps, pdf, none. \
	                Default='man html']),
	[ac_cv_doc_formats="$withval"])
AC_MSG_CHECKING([which documentation formats to build])

##-- set default doc formats if unspecified
if test -z "$ac_cv_doc_formats" ; then
  ac_cv_doc_formats="man html"
fi

##-- un-comma-tize the doc-formats
mootm_doc_formats=`echo "$ac_cv_doc_formats" | sed 's/\,/ /g'`


if test "$enable_doc" != "yes" ; then
 ##-- disable all docs
 mootm_doc_formats="none"
fi
AC_MSG_RESULT([$mootm_doc_formats])
#echo ">> post RESULT"

AC_ARG_VAR(DOXYGEN,[Path to doxygen documentation generator (disable by setting DOXYGEN=no)])

if test "$mootm_doc_formats" != "none" ; then
  ##-- docs: prog doxygen
  if test -z "$DOXYGEN" ; then
    AC_PATH_PROG(DOXYGEN,doxygen,[no])
  fi

  ##-- report doxygen-check results
  if test -z "$DOXYGEN" -o "$DOXYGEN" = "no"; then
    DOXYGEN="no"
    AC_MSG_WARN([doxygen not found: library documentation will not be built!]) 
  fi
  AC_MSG_NOTICE([setting DOXYGEN=$DOXYGEN])
  AM_CONDITIONAL(HAVE_DOXYGEN, [test "$DOXYGEN" != "no"])

  ##-- doxygen: test for working 'doxy-filter.perl'
  AC_ARG_VAR(DOXY_FILTER,[doxygen input filter; "no" to disable])
  AC_MSG_CHECKING([for doxygen input filter])
  if test -z "$DOXY_FILTER"; then
    DOXY_SRCDIR="`find ${srcdir}/doc/ -name 'lib*' -print | head -n1`"
    if test -d "$DOXY_SRCDIR"; then
      DOXY_SRCDIR=`cd "$DOXY_SRCDIR"; pwd`
      DOXY_FILTER="$PERL $DOXY_SRCDIR/doxy-filter.perl"
    else
      DOXY_FILTER="no"
    fi
  fi
  AC_MSG_RESULT([$DOXY_FILTER])

  if test "$DOXY_FILTER" != "no"; then
    AC_MSG_CHECKING([whether doxygen input filter works])
    if test -n "$DOXY_FILTER" && $DOXY_FILTER </dev/null 2>&1 >>config.log ; then
      AC_MSG_RESULT([yes])
    else
      AC_MSG_RESULT([no])
      DOXY_FILTER="no"
    fi
  fi
  AC_MSG_NOTICE([setting DOXY_FILTER=$DOXY_FILTER])
  if test "$DOXY_FILTER" != "no"; then
    DOXY_INPUT_FILTER="$DOXY_FILTER"
  else
    DOXY_INPUT_FILTER=""
  fi
  AC_SUBST(DOXY_FILTER)
  AC_SUBST(DOXY_INPUT_FILTER)
  AM_CONDITIONAL(HAVE_DOXY_FILTER, [test "$DOXY_FILTER" != "no"])


  ##-- doxygen: sources
  AC_SUBST(DOXYGEN_SOURCES)

  ### docs: pod converters
  AC_ARG_VAR(POD2TEXT, [Path to perl 'pod2text' script; "no" for none])
  AC_ARG_VAR(POD2MAN,  [Path to perl 'pod2man' script; "no" for none])
  AC_ARG_VAR(POD2HTML, [Path to perl 'pod2html' script; "no" for none])
  AC_ARG_VAR(POD2LATEX,[Path to perl 'pod2latex' script; "no" for none])
  if test -z "$POD2TEXT" ; then
     AC_PATH_PROG(POD2TEXT,pod2text,[no])
  fi
  if test -z "$POD2MAN"  ; then
     AC_PATH_PROG(POD2MAN,pod2man,[no])
  fi
  if test -z "$POD2HTML" ; then
     AC_PATH_PROG(POD2HTML,pod2html,[no])
  fi
  if test -z "$POD2LATEX"; then
     AC_PATH_PROG(POD2LATEX,pod2latex,[no])
  fi
  AM_CONDITIONAL(HAVE_POD2TEXT,  [test "$POD2TEXT"  != "no"])
  AM_CONDITIONAL(HAVE_POD2MAN,   [test "$POD2MAN"   != "no"]) 
  AM_CONDITIONAL(HAVE_POD2HTML,  [test "$POD2HTML"  != "no"])
  AM_CONDITIONAL(HAVE_POD2LATEX, [test "$POD2LATEX" != "no"])

  ### -- docs: .gog file linkup
  #mootm_program_gogs="moot.gog mootpp.gog moot-fstgen.gog moot-pargen.gog"
  #mootm_program_gogs="mootpp.gog mootrain.gog moothmm.gog mootm.gog moot.gog"
  DOC_MAN1_GOGS="mootm.gog"
  DOC_MAN5_GOGS=""
  for g in $DOC_MAN1_GOGS $DOC_MAN5_GOGS ; do
    AC_CONFIG_LINKS(doc/programs/${g}:src/programs/${g})  
  done
  DOC_GOGS="$DOC_MAN1_GOGS $DOC_MAN5_GOGS"
  AC_SUBST(DOC_MAN1_GOGS)
  AC_SUBST(DOC_MAN5_GOGS)
  AC_SUBST(DOC_GOGS)
  AC_CONFIG_LINKS(doc/programs/acknowledge.pod:src/programs/acknowledge.pod)

  ##-- set "CONFIG_DOC_WANT_*" variables
  ##   + test with automake conditionals "DOC_WANT_*"
  CONFIG_DOC_WANT_TXT="no"
  CONFIG_DOC_WANT_MAN="no"
  CONFIG_DOC_WANT_HTML="no"
  CONFIG_DOC_WANT_LATEX="no"
  CONFIG_DOC_WANT_DVI="no"
  CONFIG_DOC_WANT_PS="no"
  CONFIG_DOC_WANT_PDF="no"

  ##-- check requested formats
  for fmt in $mootm_doc_formats ; do
    case "$fmt" in

      txt)
        CONFIG_DOC_WANT_TXT="yes"
	;;
      man)
        CONFIG_DOC_WANT_MAN="yes"
	;;
      html)
        CONFIG_DOC_WANT_HTML="yes"
	;;
      dvi)
        CONFIG_DOC_WANT_LATEX="yes"
        CONFIG_DOC_WANT_DVI="yes"
	;;
      ps)
	CONFIG_DOC_WANT_LATEX="yes"
        CONFIG_DOC_WANT_PS="yes"
	;;
      pdf)
        CONFIG_DOC_WANT_LATEX="yes"
	CONFIG_DOC_WANT_PDF="yes"
	;;
      *)
	AC_MSG_WARN(ignoring unknown documentation format: $fmt)
	;;
    esac; # case "$fmt" in ...
  done; # for fmt in $mootm_doc_formats ...

  ##-- set automake documentation conditionals
  AM_CONDITIONAL(DOC_WANT_TXT, [test "$CONFIG_DOC_WANT_TXT"  != "no"])
  AM_CONDITIONAL(DOC_WANT_MAN, [test "$CONFIG_DOC_WANT_MAN"  != "no"])
  AM_CONDITIONAL(DOC_WANT_HTML,[test "$CONFIG_DOC_WANT_HTML" != "no"])
  AM_CONDITIONAL(DOC_WANT_LATEX, [test "$CONFIG_DOC_WANT_LATEX"  != "no"])
  AM_CONDITIONAL(DOC_WANT_DVI, [test "$CONFIG_DOC_WANT_DVI"  != "no"])
  AM_CONDITIONAL(DOC_WANT_PS,  [test "$CONFIG_DOC_WANT_PS"   != "no"])
  AM_CONDITIONAL(DOC_WANT_PDF, [test "$CONFIG_DOC_WANT_PDF"  != "no"])

  ##-- automake conditionals: indicator values
  AC_SUBST(CONFIG_DOC_WANT_TXT)
  AC_SUBST(CONFIG_DOC_WANT_MAN)
  AC_SUBST(CONFIG_DOC_WANT_HTML)
  AC_SUBST(CONFIG_DOC_WANT_LATEX)
  AC_SUBST(CONFIG_DOC_WANT_DVI)
  AC_SUBST(CONFIG_DOC_WANT_PS)
  AC_SUBST(CONFIG_DOC_WANT_PDF)

  ##-- doxygen vars (compatibility hack)
  DOXY_WANT_MAN="$CONFIG_DOC_WANT_MAN"
  DOXY_WANT_HTML="$CONFIG_DOC_WANT_HTML"
  DOXY_WANT_LATEX="$CONFIG_DOC_WANT_LATEX"
  DOXY_WANT_HTMLHELP="NO"
  AC_SUBST(DOXY_WANT_MAN)
  AC_SUBST(DOXY_WANT_HTML)
  AC_SUBST(DOXY_WANT_LATEX)
  AC_SUBST(DOXY_WANT_HTMLHELP)

  ##-- doxygen: check for tag-files (this needs an overhaul!)
  for ext in moot gfsm gfsmxl libFSM ; do
    extdocdir="`$PKG_CONFIG --variable=pkgdocdir ${ext}`"
    if test -n "$extdocdir" -a "$extdocdir" != "no" ; then
      exttagfiles="`find $extdocdir -name '*.tag'`"
      for exttag in $exttagfiles ; do
        exttagdir="`dirname $exttag`/html"
        if test -d "$exttagdir" ; then
          DOXY_TAGFILES="$DOXY_TAGFILES $exttag=$exttagdir"
        fi
      done
    fi
  done 
  AC_SUBST(DOXY_TAGFILES)


fi; # if "$mootm_doc_formats" != "none" ...


#---------------------------------------------------------------
# Binary distribution
BINDIST_RELEASE=1

## -- hack: canonicalize package-name to lower-case (for debian)
BINDIST_PKGNAME=`echo "$PACKAGE" | tr '[[:upper:]]' '[[:lower:]]'`

## -- hack: downgrade ix86 -> i386
case "$target_cpu" in
  i[[3-9]]86)
	BINDIST_CPU=i386
	;;
  *)
	BINDIST_CPU="$target_cpu"
	;;
esac
BINDIST_OS="$target_os"

AC_SUBST(BINDIST_PKGNAME)
AC_SUBST(BINDIST_RELEASE)
AC_SUBST(BINDIST_CPU)
AC_SUBST(BINDIST_OS)
# Binary distribution
#---------------------------------------------------------------

dnl ------------------------------------------------------------
dnl debug ?
dnl
AC_MSG_CHECKING([whether to build debug version])
AC_ARG_ENABLE(debug,
	AC_HELP_STRING([--enable-debug],[build debug version (default=no)]))

if test "$enable_debug" == "yes" ; then
   AC_MSG_RESULT(yes)
   mootm_OFLAGS="-g"
else
  AC_MSG_RESULT(no)
  #mootm_OFLAGS="-O2"
  #mootm_OFLAGS="-O3 -fomit-frame-pointer -funroll-loops -finline-limit-100000"
  mootm_OFLAGS="-O3 -fomit-frame-pointer -funroll-loops -Wno-deprecated"
fi
AC_SUBST(mootm_OFLAGS)


dnl ---------------------------------------------------------------
dnl cxxflags: template depth (should be handled by moot)
dnl
dnl CXXFLAGS="$CXXFLAGS -ftemplate-depth-24"
dnl
dnl cxxflags: template depth
dnl ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

dnl ------------------------------------------------------------
dnl unconfig
AC_CONFIG_LINKS(src/libmootm/mootmUnConfig.h:src/libmootm/mootmConfig.h.in)


dnl ------------------------------------------------------------
dnl warnings ?
dnl
AC_MSG_CHECKING([whether to display compiler warnings])
AC_ARG_ENABLE(warnings,
	AC_HELP_STRING([--disable-warnings],[disable compiler warnings (default=no)]))

if test "$enable_warnings" != "no" ; then
   AC_MSG_RESULT(yes)
   mootm_WFLAGS="-Wall"
else
  AC_MSG_RESULT(no)
  mootm_WFLAGS=""
fi
AC_SUBST(mootm_WFLAGS)


dnl
dnl output
dnl
AC_CONFIG_FILES(src/libmootm/Makefile src/programs/Makefile src/Makefile)
AC_CONFIG_FILES(doc/libmootm/mainpage.dox)
AC_CONFIG_FILES(doc/libmootm/libmootm.doxy doc/libmootm/Makefile)
AC_CONFIG_FILES(doc/programs/Makefile)
AC_CONFIG_FILES(doc/Makefile)
AC_CONFIG_FILES(config/Makefile)
AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(mootm.pc)
AC_CONFIG_FILES(mootm.spec)
AC_OUTPUT
