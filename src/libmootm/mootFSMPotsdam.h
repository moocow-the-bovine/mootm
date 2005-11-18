/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
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

/*----------------------------------------------------------------------
 * Name: mootFSMPotsdam.h
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + mootm FSMs: uni-potsdam FSMlib
 *----------------------------------------------------------------------*/
/**
 * \file mootFSMPotsdam.h
 * \brief Finite state devices using Potsdam libFSM
 */
#ifndef _moot_FSM_POTSDAM_H
#define _moot_FSM_POTSDAM_H

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_POTSDAM)

#include <mootFSMBase.h>

#include <FSMTypes.h>
#include <FSM.h>
#include <FSMSymSpec.h>
#include <FSMWeight.h>

#include <vector>
#include <string>
#include <list>
#include <set>

/** Whether to load FSM symbol files in AT&T-compatibility mode */
#define moot_SYM_ATT_COMPAT true

namespace mootm {
  using namespace std;
  using namespace moot;

  /**
   * \brief finite-state transducer class using Uni-Potsdam libFSM
   */
  class mootFSMPotsdam : public mootFSMBase {
  protected:
    /*------------------------------------------------------------
     * protected types
     */
    /** Single POS tag string, with or without features */
    typedef FSMSymbolString TagString;

    /** Single morphological analysis, including features */
    typedef FSM::FSMWeightedSymbolVector MorphAnalysis;

    /** Set of morphological analyses, including features */
    typedef set<MorphAnalysis> MorphAnalysisSet;

    /** Set of POS tag strings, with or without features */
    typedef set<FSMSymbolString> TagStringSet;


  protected:
    /*------------------------------------------------------------
     * protected data
     */
    FSMSymSpec *syms;         /**< symbol spec */
    FSM        *mfst;         /**< morphology fst */

    list<string> syms_msgs;   /**< symspec messages */
    FSM          result;      /**< lookup temporary */

    /** pre-allocated temporary: morphological analysis output */
    MorphAnalysisSet analyses;

  public:
    /*------------------------------------------------------------
     * public methods
     */
    /** Default constructor */
    mootFSMPotsdam(void)
      : syms(NULL),
	mfst(NULL)
    {};

    /** Default destructor */
    ~mootFSMPotsdam(void) {
      if (mfst) delete mfst;
      if (syms) delete syms;
    };

    /** Load a morphology FST (& symbols) */
    virtual bool load(const string &fstfile, const string &symfile="");

    /** Return true if this is a valid object */
    virtual bool valid(void) const
    { return syms && mfst && *mfst; }

    /** Analyze a single mootToken in-place */
    virtual mootToken& analyze_token(mootToken &tok, bool want_avm=true, bool want_warnings=true);

    
  protected:
    /*------------------------------------------------------------
     * protected (non-API) methods
     */
    /** check for new symspec messages (hack!) */
    void check_symspec_messages(void);

  }; // class mootFSMPotsdam


  /** The real underlying library selection is this typedef */
  typedef class mootFSMPotsdam mootFSM;

}; // namespace mootm

#endif /* defined(USE_FSM_POTSDAM) */


#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
#endif


#endif /* _moot_FSM_POTSDAM_H */
