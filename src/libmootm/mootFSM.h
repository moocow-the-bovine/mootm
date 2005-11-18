/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2004-2005 by Bryan Jurish <moocow@ling.uni-potsdam.de>

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
 * Name: mootFSM.h
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + moot FSMs: abstract base class
 *----------------------------------------------------------------------*/
/**
 * \file mootFSM.h
 * \brief Abstract base class for mootm morphology automata.
 * \detail May define placebo classes, etc. if HAVE_LIBFSM is not defined
 */

#ifndef _moot_FSM_H
#define _moot_FSM_H

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#include <stdio.h>
#include <mootToken.h>

namespace mootm {
  using namespace std;
  using namespace moot;

  /**
   * \brief Abstract base class for finite-state transducers.
   */
  class mootFSM {
  public:
    /*------------------------------------------------------------
     * public methods
     */
    /** Default constructor */
    virtual mootFSM(void) {};

    /** Default destructor */
    virtual ~mootFSM(void) {};

    /** Load a morphology FST (& symbols) */
    virtual bool load(const char *fstfile, const char *symfile=NULL)
    { dummy("load"); };

    /** Analyze a single mootToken */
    virtual mootFSM analyze_token(const mootToken &tok)
    { dummy("analyze_token"); };

    /** Return true if this is a valid object */
    virtual bool valid(void) const
    { return false; };

    /** Serialize to a mootToken */
    virtual mootToken &to_token(mootToken &tok, bool want_avm=true, int verbose=0)
    { dummy("to_token"); };

  public:
    /*------------------------------------------------------------
     * dummy methods, low-level
     */
    void dummy(const char *name="?")
    {
      self->carp("mootFSM::%s(): dummy method called -- exiting.\n", name);
      abort();
    };

    /** Error reporting */
    void carp(char *fmt, ...) const;

  }; // class mootFSM

}; // namespace mootm

#endif /* _moot_FSM_H */
