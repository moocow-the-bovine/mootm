/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2005 by Bryan Jurish <moocow@ling.uni-potsdam.de>

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
 * Name: mootFSMRWTH.h
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + mootm FSMs: RWTH libFsa
 *----------------------------------------------------------------------*/
/**
 * \file mootFSMRWTH.h
 * \brief Finite state devices using RWTH libFsa
 */
#ifndef _moot_FSM_RWTH_H
#define _moot_FSM_RWTH_H

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_RWTH)

#include <mootFSMBase.h>

#include <Fsa/Automaton.hh>
#include <Fsa/Basic.hh>
#include <Fsa/Static.hh>
#include <Fsa/Types.hh>
#include <Fsa/Input.hh>

#include <vector>
#include <string>
#include <set>

#include <sstream>


namespace mootm {
  using namespace std;
  using namespace moot;
  using namespace Fsa;


  /*------------------------------------------------------------------------
   * Types
   */

  /** The real underlying library selection is this typedef */
  typedef class mootFSMRWTH mootFSM;


  /** type for enumeration of an FSM path */
  typedef std::vector<LabelId> LabelVector;

  /** Class for enumerating FSM paths */
  class WeightedLabelVectorPair {
  public:
    Weight      weight;
    LabelVector in;
    LabelVector out;

    WeightedLabelVectorPair(void) {};

    WeightedLabelVectorPair(const Weight w_)
      : weight(w_)
    {};

    WeightedLabelVectorPair(const Weight w_, const LabelVector &in_)
      : weight(w_), in(in_)
    {};

    WeightedLabelVectorPair(const Weight w_, const LabelVector &in_, const LabelVector &out_)
      : weight(w_), in(in_), out(out_) {};

    WeightedLabelVectorPair(const WeightedLabelVectorPair &p)
      : weight(p.weight), in(p.in), out(p.out)
    {};

    WeightedLabelVectorPair(const WeightedLabelVectorPair &p,
			    const Weight arc_weight, const LabelId arc_in, const LabelId arc_out,
			    ConstSemiringRef sr = TropicalSemiring)
      : weight(p.weight), in(p.in), out(p.out)
    {
      weight = sr->extend(weight, arc_weight);
      if (arc_in  != Epsilon) in.push_back(arc_in);
      if (arc_out != Epsilon) out.push_back(arc_out);
    };

    ~WeightedLabelVectorPair(void) {};


    bool operator<(const WeightedLabelVectorPair &w2) const
    { return weight < w2.weight || in < w2.in || out < w2.out; };

    bool operator==(const WeightedLabelVectorPair &w2) const
    { return weight == w2.weight && in == w2.in && out == w2.out; };
  }; //-- class WeightedLabelVectorPair


  /*----------------------------------------------------------------------
   * Local Functions
   */
  /** serialize automaton to a set of path-vectors */
  bool automatonLabelVectors(ConstAutomatonRef a,
			     set<WeightedLabelVectorPair> &paths,
			     bool cycle_test=true);

  /** serialize automaton state to a set of path-vectors (recursive guts) */
  bool _automatonLabelVectors_guts(ConstAutomatonRef             a,
				   set<WeightedLabelVectorPair> &result,
				   StateId                       stateid,
				   WeightedLabelVectorPair      &path
				   );

  /** convert a LabelVector to a string */
  bool labelVector2String(const LabelVector& v, std::string &s, ConstAlphabetRef abet, bool warnings=true);



  /*----------------------------------------------------------------------
   * FSM Wrapper: RWTH Fsa
   */

  /**
   * \brief finite-state transducer class using Uni-Potsdam libFSM
   */
  class mootFSMRWTH : public mootFSMBase {
  protected:
    /*------------------------------------------------------------
     * protected data
     */
    //StorageAutomaton  *mfst;   /**< Underlying morphological transducer */
    ConstAutomatonRef mfstr;     /**< Morph transducer as a ConstAutomatonRef */


    istringstream     iss;              /**< dummy input stream for readLinear() */
    StorageAutomaton  *linfsa;          /**< input automaton */

    ConstAutomatonRef result;           /**< Result of morphological transduction */
    set<WeightedLabelVectorPair> paths; /**< pre-allocated temporary */


  public:
    /*------------------------------------------------------------
     * public methods
     */
    /** Default constructor */
    mootFSMRWTH(void)
    {
      linfsa = new StaticAutomaton();
    };

    /** Default destructor */
    virtual ~mootFSMRWTH(void)
    {
      if (linfsa) delete linfsa;
    };

    /** Load a morphology FST
     *  (no support for extra symbols here
     */
    virtual bool load(const string &fstfile, const string &symfile="");

    /** Return true if this is a valid object */
    virtual bool valid(void) const
    { return mfstr; }

    /** Analyze a single mootToken in-place.
     * want_avm, want_warnings are ignored here.
     */
    virtual mootToken& analyze_token(mootToken &tok, bool want_avm=true, bool want_warnings=true);

  }; //-- class mootFSMRWTH



}; //-- namespace mootm


#endif /* defined(USE_FSM_RWTH) */


#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
#endif


#endif /* _moot_FSM_RWTH_H */
