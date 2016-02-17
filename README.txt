    README for package 'mootm'

    Last updated for mootm version 2.0.6

DESCRIPTION
    mootm - moocow's part-of-speech tagger: morphology.

REQUIREMENTS
    moot >= v2.0.0
        Available from http://www.ling.uni-potsdam.de/~moocow/projects/moot

        mootm uses libmoot for high-level I/O operations. These should
        probably wander into their own library at some point, but they
        currently live in moot. moot version >= 2.0.6-pre1 is reccommended.

    Underlying finite-state device library
        mootm can use any one of several underlying libraries for
        finite-state device operations, including the following:

        GNU libgfsm >= v0.0.7
            Available from
            http://www.ling.uni-potsdam.de/~moocow/projects/gfsm

            libgfsm version >= 0.0.8-pre9 is reccommended.

        University of Potsdam FSMlib > v0.9.0.1
            For details and availability, contact its author, Thomas
            Hanneforth.

        RWTH libFsa
            Available from
            http://www.informatik.rwth-aachen.de/I6/~kanthak/fsa.html

            RWTH libFsa support is implemented but currently broken. Feel
            free to fix it if you like.

    pkg-config
        Available from: http://www.freedesktop.org/software/pkgconfig/

    Perl (Optional)
        Get it from http://www.cpan.org or http://www.perl.com Required for
        building command-line parsers, utility documentation, library
        documentation, etc.

    Getopt::Gen
        A Perl module used to generate command-line option parsers.
        Available from:
        http://www.ling.uni-potsdam.de/~moocow/projects/perl/index.html#gog

    pod2man, pod2text, pod2html (Optional)
        The Perl documentation converstion utilities, required for building
        the correspdonding program documentation formats, should have come
        with your Perl.

INSTALLATION
    Issue the following commands to the shell:

     sh ./configure
     make
     make install

    See the file INSTALL in the top-level distribution directory for
    details.

BUILD FROM CVS/SVN
    To build from CVS or Subversion sources, you need the GNU utilities
    aclocal, automake, autoconf, and libtool. If you have these, you can
    just run the top-level script:

     sh ./autogen.sh

    This will create the 'configure' script and other necessary build files.

KNOWN BUGS
    Probably many.

ACKNOWLEDGEMENTS
    Development of this package was supported by the project 'Kollokationen
    im W�rterbuch' ("collocations in the dictionary",
    http://www.bbaw.de/forschung/kollokationen ) in association with the
    project 'Digitales W�rterbuch der deutschen Sprache des 20. Jahrhunderts
    (DWDS)' ("digital dictionary of the German language of the 20th
    century", http://www.dwds.de ) at the Berlin-Brandenburgische Akademie
    der Wissenschaften ( http://www.bbaw.de ) with funding from the
    Alexander von Humboldt Stiftung ( http://www.avh.de ) and from the
    Zukunftsinvestitionsprogramm of the German federal government.

    I am grateful to Christiane Fellbaum, Alexander Geyken, Thomas
    Hanneforth, Gerald Neumann, Edmund Pohl, Alexey Sokirko, and others for
    offering useful insights in the course of development of this package.

    Thomas Hanneforth wrote and maintains the libFSM C++ library for
    finite-state device operations used in the development of this package.

    Alexander Geyken and Thomas Hanneforth developed the rule-based
    morphological analysis system for German which was used in the
    development and testing of this package.

AUTHOR
    Bryan Jurish <moocow@cpan.org>

