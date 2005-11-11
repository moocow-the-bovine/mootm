/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2004 by Bryan Jurish <moocow@ling.uni-potsdam.de>

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
 *   + moot-relevant FSM includes
 *----------------------------------------------------------------------*/
/**
 * \file mootFSM.h
 * \brief FSM interface for libmoot.
 * \detail May define placebo classes, etc. if HAVE_LIBFSM is not defined
 */

#ifndef _moot_FSM_H
#define _moot_FSM_H

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#ifdef HAVE_LIBFSM

# include <FSMTypes.h>
# include <FSM.h>
# include <FSMSymSpec.h>

# ifdef HAVE_CONFIG_H
#  include <mootmUnConfig.h>
#  include <mootmConfig.h>
# endif

#if FSM_API_REVISION == 0

# ifdef HAVE_CONFIG_H
#  include <mootmUnConfig.h>
#  include <mootmConfig.h>
# endif

# include <FSMCost.h>
# include <FSMRegexCompiler.h>
#else /* FSM_API_REVISION != 0 */
# include <FSMWeight.h>
#endif /* FSM_API_REVISION */
#else /* HAVE_LIBFSM */
# error "You need libFSM in order to compile this package!"
#endif

#include <vector>
#include <string>
#include <list>
#include <set>

/** Whether to load FSM symbol files in AT&T-compatibility mode */
#define moot_SYM_ATT_COMPAT true


#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
#endif

#endif /* _moot_FSM_H */
