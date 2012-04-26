/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2005 by Bryan Jurish <moocow@cpan.org>

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
 * Name: mootFSMRWTH.cc
 * Author: Bryan Jurish <moocow@cpan.org>
 * Description:
 *   + moot FSMs: RWTH libFsa
 *----------------------------------------------------------------------*/

#include <mootFSMRWTH.h>

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_RWTH)

#include <Fsa/Compose.hh>
#include <Fsa/Project.hh>
#include <Fsa/RemoveEpsilons.hh>


namespace mootm {
  using namespace std;
  using namespace moot;
  using namespace Fsa;


  /*------------------------------------------------------------------------*/
  bool mootFSMRWTH::load(const string &fstfile, const string &symfile)
  {
    mfstr = read(fstfile);
    return mfstr;
  };


  /*------------------------------------------------------------------------*/
  mootToken& mootFSMRWTH::analyze_token(mootToken &tok, bool want_avm, bool want_warnings)
  {
    std::string istr;
    for (string::const_iterator si=tok.text().begin(); si != tok.text().end(); si++) {
      istr.push_back(*si);
      istr.push_back(' ');
    }
    iss.str(istr);

    StorageAutomaton *linfsa = new StaticAutomaton();
    linfsa->setSemiring(mfstr->semiring());
    linfsa->setInputAlphabet(mfstr->getInputAlphabet());

    readLinear(linfsa,iss);

    //-- compose, trim
    result = trim(projectOutput(composeMatching(ConstAutomatonRef(linfsa), mfstr)));

    //-- get vectors
    automatonLabelVectors(result, paths);

    //-- add vectors as analyses to token
    for (set<WeightedLabelVectorPair>::const_iterator pi=paths.begin(); pi!=paths.end(); pi++) {
      mootToken::Analysis analysis;

      labelVector2String(pi->in, analysis.tag, result->getInputAlphabet(), want_warnings);
      analysis.prob = float(pi->weight); //-- hack: floating-point weights only!

      tok.insert(analysis);
    };

    return tok;
  };


  /*===========================================================================
   * automatonLabelVectors()
   *===========================================================================*/

  /*---------------------------------------------------------------------------
   * automatonLabelVectors(): top-level
   */
  bool automatonLabelVectors(ConstAutomatonRef a,
			     set<WeightedLabelVectorPair> &result,
			     bool cycle_test)
  {
    if (cycle_test && !a->hasProperty(PropertyAcyclic)) {
      fprintf(stderr, "automatonLabelVectors(): error: input FSM is cyclic!\n");
      return false;
    }

    WeightedLabelVectorPair path(a->semiring()->one());
    return _automatonLabelVectors_guts(a, result, a->initialStateId(), path);
  };


  /*---------------------------------------------------------------------------
   * automatonLabelVectors(): guts (recursive)
   */
  bool _automatonLabelVectors_guts(ConstAutomatonRef             a,
				   set<WeightedLabelVectorPair> &result,
				   StateId                       stateid,
				   WeightedLabelVectorPair      &path
				   //set<StateId>                 &visited
				   )
  {
    ConstStateRef qr = a->getState(stateid);

    //-- if final state, add to set of full paths
    if (qr->isFinal()) {
      result.insert(WeightedLabelVectorPair(path, qr->weight_, Epsilon, Epsilon, a->semiring()));
    }

    //-- investigate all outgoing arcs
    for (State::const_iterator ai = qr->begin(); ai != qr->end(); ai++) {
      WeightedLabelVectorPair apair(path, ai->weight_, ai->input_, ai->output_, a->semiring());
      if (!_automatonLabelVectors_guts(a, result, ai->target_, apair)) return false;
    }

    return true;
  };


  /*===========================================================================
   * labelVector2String()
   *===========================================================================*/
  bool labelVector2String(const LabelVector& v, std::string &s, ConstAlphabetRef abet, bool warnings)
  {
    bool status = true;
    for (LabelVector::const_iterator vi = v.begin(); vi != v.end(); vi++) {
      if (*vi == Epsilon) continue;
      const std::string sym = abet->symbol(*vi);
      if (sym.empty()) {
	status = false;
	if (warnings) fprintf(stderr,"labelVector2String(): Warning: unknown LabelId `%d'\n",*vi);
	continue;
      }
      if (sym.size()==1) {
	s.append(sym);
      } else {
	s.push_back('[');
	s.append(sym);
	s.push_back(']');
      }
    }
    
    return status;
  };


}; //-- namespace mootm


#endif /* defined(USE_FSM_RWTH) */
