/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2003-2008 by Bryan Jurish <moocow@ling.uni-potsdam.de>

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
 * Name: mootGfsmCommon.h
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + mootm FSMs: gfsm, gfsmxl common utilities
 *----------------------------------------------------------------------*/
/**
 * \file mootGfsmCommon.h
 * \brief Finite state devices using libgfsm: common utilities
 * \detail Only include this file if USE_FSM_GFSM or USE_FSM_GFSMXL is defined
 */
#ifndef _moot_GFSM_COMMON_H
#define _moot_GFSM_COMMON_H

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_GFSM) || defined(USE_FSM_GFSMXL)

extern "C" {
#include <gfsm.h>
}

#include <mootFSMBase.h>

namespace mootm {
  using namespace std;
  using namespace moot;


  /*------------------------------------------------------------
   * Utilities
   */

  /** Utility type for mootFSMgfsm::analyze_token() */
  struct _mootGfsmCommon_analyze_token_params {
    gfsmAlphabet *abet;              /**< alphabet for stringification */
    gboolean      warn_on_undefined;
    gboolean      att_style;
    mootToken    *tokp;              /**< token being analyzed */
    GString      *gstr;              /**< temporary for stringification */
  };
  typedef struct _mootGfsmCommon_analyze_token_params _mootFSMgfsm_analyze_token_params;   ///< Alias
  typedef struct _mootGfsmCommon_analyze_token_params _mootFSMgfsmxl_analyze_token_params; ///< Alias

  /** Utility function for mootFSMgfsm::analyze_token() */
  gboolean _mootGfsmCommon_analyze_token_foreach_func(gfsmPath *path,
						      gpointer value_dummy,
						      struct _mootGfsmCommon_analyze_token_params *params);

  #define _mootFSMgfsm_analyze_token_foreach_func(path,val,para) \
          _mootGfsmCommon_analyze_token_foreach_func((path),(val),(para))

  #define _mootFSMgfsmxl_analyze_token_foreach_func(path,val,para) \
          _mootGfsmCommon_analyze_token_foreach_func((path),(val),(para))

}; // namespace mootm

#endif /* defined(USE_FSM_GFSM) || defined(USE_FSM_GFSMXL)*/


#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
#endif


#endif /* _moot_GFSM_COMMON_H */
