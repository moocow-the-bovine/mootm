/* -*- Mode: C++ -*- */

/*
   libmootm : moocow's morphology library
   Copyright (C) 2003-2005 by Bryan Jurish <moocow@ling.uni-potsdam.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/


/*--------------------------------------------------------------------------
 * File: mootMorph.h
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + moocow's PoS tagger : morphology
 *--------------------------------------------------------------------------*/

#ifndef _moot_MORPH_H_
#define _moot_MORPH_H_

/** \def moot_MORPH_DEBUG
 * Define this to enable verbose debugging
 */
#define moot_MORPH_DEBUG
//#undef moot_MORPH_DEBUG

/** \def MOOT_HAVE_HASH_STRING
 *  Hack to use libFSM hash<string> from libFSM */
#define MOOT_HAVE_HASH_STRING

#include <stdio.h>

#include <mootToken.h>
#include <mootTokenIO.h>

#include <mootFSMBase.h>
#include <mootFSMPotsdam.h>
#include <mootFSMgfsm.h>
#include <mootFSMRWTH.h>

namespace mootm {
  using namespace std;
  using namespace moot;

/*--------------------------------------------------------------------------
 * mootMorph : morphological tagger class
 *--------------------------------------------------------------------------*/

/**
 * \brief High-level class for morphological analyzers.
 */
class mootMorph {
public:

  /** Some handy values for the 'verbose' flag */
  typedef enum {
    vlSilent=0,       /**< Be as silent as possible */
    vlErrors=10,      /**< Output runtime errors with carp() */
    vlWarnings=20,    /**< Output runtime warnings */
    vlEverything=127  /**< Output everything */
  } VerbosityLevel;

public:
  /*------------------------------------------------------------
   * public data : guts
   */
  /// \name Ye Olde Guts
  //@{
  /** Underlying finite state transducer to use for analysis:
      \f[
        \begin{eqnarray*}
          Morph(w) &:=& \pi_{2}(ID_w \circ T_{morph})\cr
          Analyses(w) &:=& strings(Morph(w))
        \end{eqnarray*}
      \f]

      \warning The analyzer FST \f$T_{morph}\f$ may not contain any input epsilon cycles!
      \warning The analysis FSAs \f$Morph(w)\f$ may not contain any epsilon cycles!
  */
  mootFSM mfsm;
  //@}


  /*------------------------------------------------------------
   * public data : flags
   */

  /** \name Flags */
  //@{
  /** Generate output strings in AVM (madwds-"vector") mode? (default = false) */
  bool want_avm;

  /** Force re-analysis of pre-analyzed tokens? (default = false) */
  bool force_reanalysis;

  /**  Whether first analysis should be used to instantiate 'besttag' (default=false) */
  bool first_analysis_is_best;

  /**
   * Whether to (otherwise) ignore first analysis of each input token (re-analysis).
   * Default=false.
   */
  bool ignore_first_analysis;
  
  /** verbosity */
  int verbose;
  //@}
  
  /*------------------------------------------------------------
   * public data : statistics
   */

  /** \name Statistics / performance tracking */
  //@{
  /** number of tokens processed */
  unsigned int ntokens;

  /** number of tokens analyzed */
  unsigned int nanalyzed;

  /** number of unknown tokens encountered */
  unsigned int nunknown;

  /**
   * number of tokens after which to print a progress indicator,
   * 0 (default) for none
   */
  unsigned int nprogress;
  //@}


public:
  /*------------------------------------------------------------
   * public methods: constructor / destructor
   */

  /** \name Construction / Destruction */
  //@{
  /** constructor */
  mootMorph(void) :
    want_avm(false),
    force_reanalysis(false),
    first_analysis_is_best(false),
    ignore_first_analysis(false),
    verbose(vlErrors),
    ntokens(0),
    nanalyzed(0),
    nunknown(0),
    nprogress(0)
  {};

  /** destructor */
  ~mootMorph() {};
  //@}

  /*------------------------------------------------------------
   * public methods: initialization
   */

  /** \name Initialization */
  //@{

  /** Load morphology transducer: syms_filename is requird for Uni-Potsdam libFSM */
  bool load(const string &fst_filename, const string &syms_filename="")
  { return mfsm.load(fst_filename, syms_filename); };

  /** Returns true if this is a valid object */
  bool valid(void) const
  { return mfsm.valid(); };

  //@}

  /*------------------------------------------------------------
   * public methods: top-level: analysis utilities
   */

  /** \name Analysis: Top-level */
  //@{

  /** Top-level: tag tokens from a C-stream */
  bool analyze_io(TokenReader *reader, TokenWriter *writer);

  /** Top-level: tag tokens from a C-stream */
  bool analyze_stream(FILE *in=stdin, FILE *out=stdout, const char *srcname=NULL);

  /** Top-level: tag a C-array of token-strings */
  bool analyze_strings(int argc, char **argv, FILE *out=stdout, const char *srcname=NULL);

  /**
   * Mid-level analysis utility: analyze a mootToken in-place.
   * Note that any previous analyses in 'tok' are NOT cleared.
   *
   * Don't even THINK about calling this method unlesss
   * can_tag() returns true.
   *
   */
  inline mootToken& analyze_token(mootToken &tok)
  {
    //-- analyze
    mfsm.analyze_token(tok, want_avm, (verbose >= vlWarnings));

    //-- track statistics
    nanalyzed++;
    if (tok.analyses().empty()) nunknown++;

    return tok;
  };

  //@}




  /*------------------------------------------------------------
   * public methods: Miscellany
   */
  /** \name Miscellany */
  //@{

  /** Report an error */
  void carp(char *fmt, ...) const;

  //@}
};

}; /* namespace mootm */

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
#endif

#endif // _moot_MORPH_H_
