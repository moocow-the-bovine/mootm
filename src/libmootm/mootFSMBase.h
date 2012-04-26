/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2004-2008 by Bryan Jurish <moocow@cpan.org>

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
 * Name: mootFSMBase.h
 * Author: Bryan Jurish <moocow@cpan.org>
 * Description:
 *   + moot FSMs: abstract base class
 *----------------------------------------------------------------------*/
/**
 * \file mootFSMBase.h
 * \brief Abstract base class for mootm morphology automata.
 * \detail May define placebo classes, etc. if HAVE_LIBFSM is not defined
 */

#ifndef _moot_FSM_BASE_H
#define _moot_FSM_BASE_H

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#ifdef USE_FSM_POTSDAM
  /* use hash<std::string> from libFSM */
# define MOOT_HAVE_HASH_STRING
#endif

#include <mootmUnConfig.h>


#include <string>
#include <mootToken.h>

namespace mootm {
  using namespace std;
  using namespace moot;

  /**
   * \brief Abstract base class for finite-state transducers.
   * \detail Really just an API specification
   */
  class mootFSMBase {
  public:
    /*------------------------------------------------------------
     * public methods
     */
    /** Default constructor */
    mootFSMBase(void) {};

    /** Default destructor */
    virtual ~mootFSMBase(void) {};

    /** Load a morphology FST (& symbols) */
    virtual bool load(const string &fstfile, const string &symfile="")
    { dummy("load"); return false; };

    /** Return true if this is a valid object */
    virtual bool valid(void) const
    { return false; };

    /** Analyze a single mootToken in-place */
    virtual mootToken& analyze_token(mootToken   &tok,
				     bool        use_att_escapes=true,
				     bool        want_avm=true,
				     bool        want_warnings=true)
    { dummy("analyze_token"); return tok; }

  public:
    /*------------------------------------------------------------
     * dummy methods, low-level
     */
    void dummy(const char *name="?")
    {
      this->carp("mootFSM::%s(): dummy method called -- exiting.\n", name);
      abort();
    };

    /** Error reporting */
    void carp(const char *fmt, ...) const;
  }; // class mootFSMBase


}; // namespace mootm

#endif /* _moot_FSM_BASE_H */
