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
 * Name: mootGfsmCommon.cc
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + moot FSMs: libgfsm, libgfsmxl common
 *----------------------------------------------------------------------*/

#include <mootGfsmCommon.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_GFSM) || defined(USE_FSM_GFSMXL)

namespace mootm {
  using namespace std;
  using namespace moot;


  /*------------------------------------------------------------------------*/
  gboolean _mootGfsmCommon_analyze_token_foreach_func(gfsmPath *path,
						      gpointer value_dummy,
						      struct _mootGfsmCommon_analyze_token_params *params)
  {
    g_string_truncate(params->gstr,0);
    gfsm_alphabet_labels_to_gstring(params->abet,
				    path->hi,
				    params->gstr,
				    params->warn_on_undefined,
				    params->att_style);
#ifdef GFSM_WEIGHT_IS_UNION
    params->tokp->insert(mootToken::Analysis(params->gstr->str,"",path->w.f));
#else
    params->tokp->insert(mootToken::Analysis(params->gstr->str,"",path->w));
#endif

    return FALSE;
  };


}; //-- namespace mootm


#endif /* defined(USE_FSM_GFSM) || defined(USE_FSM_GFSMXL) */
