/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2003-2010 by Bryan Jurish <jurish@uni-potsdam.de>

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
 * Name: mootFSMgfsm.h
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + mootm FSMs: GNU gfsm
 *----------------------------------------------------------------------*/
/**
 * \file mootFSMgfsm.h
 * \brief Finite state devices using libgfsm
 */
#ifndef _moot_FSM_GFSM_H
#define _moot_FSM_GFSM_H

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_GFSM)

#include <mootFSMBase.h>
#include <mootGfsmCommon.h>

extern "C" {
#include <gfsm.h>
}

namespace mootm {
  using namespace std;
  using namespace moot;

  /** The real underlying library selection is this typedef */
  typedef class mootFSMgfsm mootFSM;


  /*------------------------------------------------------------
   * Utilities: see mootGfsmCommon.h
   */

  /**
   * \brief finite-state transducer class using libgfsm
   */
  class mootFSMgfsm : public mootFSMBase {
  protected:
    /*------------------------------------------------------------
     * protected data
     */
    gfsmAlphabet  *abet_lo;    /**< input alphabet */
    gfsmAlphabet  *abet_hi;    /**< output alphabet */
    gfsmAutomaton *mfst;       /**< morphology fst */

    gfsmLabelVector *tlabs;    /**< input text labels */
    gfsmAutomaton   *result;   /**< lookup result */
    gfsmSet         *paths;    /**< full paths in result */
    struct _mootGfsmCommon_analyze_token_params analyze_params; /**< temporary params */

  public:
    /*------------------------------------------------------------
     * public methods
     */
    /** Default constructor */
    mootFSMgfsm(void)
      : abet_lo(NULL),
	abet_hi(NULL),
	mfst(NULL),
	tlabs(NULL),
	result(NULL),
	paths(NULL)
    {
      analyze_params.abet=NULL;
      analyze_params.warn_on_undefined=TRUE;
      analyze_params.att_style=TRUE;
      analyze_params.tokp=NULL;
      analyze_params.gstr=g_string_new("");
    };

    /** Default destructor */
    ~mootFSMgfsm(void) {
      if (abet_lo)                       gfsm_alphabet_free(abet_lo);
      if (abet_hi && abet_hi != abet_lo) gfsm_alphabet_free(abet_hi);
      if (mfst) gfsm_automaton_free(mfst);
      if (tlabs) g_ptr_array_free(tlabs,TRUE);
      if (result) gfsm_automaton_free(result);
      if (paths) gfsm_set_free(paths);
      if (analyze_params.gstr) { g_string_free(analyze_params.gstr,TRUE); }
    };

    /** Load a morphology FST (& input, output labels)
     *  @symfile may be a string of the form "INPUT_ALPHABET_FILENAME,OUTPUT_ALPHABET_FILENAME"
     */
    virtual bool load(const string &fstfile, const string &symfile="");

    /** Return true if this is a valid object */
    virtual bool valid(void) const
    { return abet_lo && abet_hi && mfst; }

    /** Analyze a single mootToken in-place */
    virtual mootToken& analyze_token(mootToken &tok,
				     bool want_att_escapes=false,
				     bool want_avm=true,
				     bool want_warnings=true);

  }; // class mootFSMgfsm



}; // namespace mootm

#endif /* defined(USE_FSM_GFSM) */


#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
#endif


#endif /* _moot_FSM_GFSM_H */
