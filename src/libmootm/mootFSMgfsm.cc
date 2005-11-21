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
 * Name: mootFSM.cc
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + moot FSMs: GNU libgfsm
 *----------------------------------------------------------------------*/

#include <mootFSMgfsm.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_GFSM)

namespace mootm {
  using namespace std;
  using namespace moot;

  /*------------------------------------------------------------------------*/
  bool mootFSMgfsm::load(const string &fstfile, const string &symfile)
  {
    bool rc=true;

    //-- Alphabets
    if (!symfile.empty()) {
      //-- cleanup
      if (abet_lo) { gfsm_alphabet_free(abet_lo); }
      if (abet_hi && abet_hi != abet_lo) { gfsm_alphabet_free(abet_hi); }
      abet_lo = NULL;
      abet_hi = NULL;

      //-- parse symfile name
      string labfile_lo(""), labfile_hi("");
      size_t commapos = symfile.find(',',0);
      if (commapos != symfile.npos) {
	labfile_lo.assign(symfile,0,commapos-1);
	labfile_hi.assign(symfile,commapos+1,symfile.size()-commapos-1);
      } else {
	labfile_lo = symfile;
      }

      //-- load: lower
      if (!labfile_lo.empty()) {
	gfsmError *err = NULL;
	abet_lo = gfsm_string_alphabet_new();
	if (!gfsm_alphabet_load_filename(abet_lo, labfile_lo.c_str(), &err)) {
	  carp("mootFSMgfsm::load(): load failed for input labels file '%s': %s\n",
	       labfile_lo.c_str(), (err ? err->message : "?"));
	  rc = false;
	  if (err) g_error_free(err);
	}
      }

      //-- load: upper
      if (!labfile_hi.empty()) {
	gfsmError *err = NULL;
	abet_hi = gfsm_string_alphabet_new();
	if (!gfsm_alphabet_load_filename(abet_hi, labfile_hi.c_str(), &err)) {
	  carp("mootFSMgfsm::load(): load failed for output labels file '%s': %s\n",
	       labfile_hi.c_str(), (err ? err->message : "?"));
	  rc = false;
	  if (err) g_error_free(err);
	}
      } else {
	abet_hi = abet_lo;
      }
    }

    //-- FST
    if (!fstfile.empty()) {
      //-- cleanup
      if (mfst) { gfsm_automaton_free(mfst); mfst=NULL; }
      if (result) { gfsm_automaton_free(result); result=NULL; }

      gfsmError *err=NULL;
      mfst = gfsm_automaton_new();
      if (!gfsm_automaton_load_bin_filename(mfst,fstfile.c_str(),&err)) {
	carp("mootFSMgfsm::load(): load failed for FST file '%s': %s\n",
	     fstfile.c_str(), (err ? err->message : "?"));
	rc = false;
	if (err) g_error_free(err);
      }
    }

    return rc;
  };


  /*------------------------------------------------------------------------*/
  mootToken& mootFSMgfsm::analyze_token(mootToken &tok, bool want_avm, bool want_warnings)
  {
    //-- get input labels
    if (tlabs) { g_ptr_array_set_size(tlabs,0); }
    tlabs = gfsm_alphabet_string_to_labels(abet_lo, tok.text().c_str(), tlabs, want_warnings);

    //-- lookup
    result = gfsm_lookup(tlabs, mfst, result);

    //-- serialize
    if (paths) gfsm_set_clear(paths);
    paths = gfsm_automaton_paths(result, paths);

    //-- stringify directly to the mootToken
    analyze_params.abet = abet_hi;
    analyze_params.warn_on_undefined = want_warnings;
    analyze_params.att_style = want_avm;
    analyze_params.tokp = &tok;

    gfsm_set_foreach(paths, (GTraverseFunc)_mootFSMgfsm_analyze_token_foreach_func, &analyze_params);

    return tok;
  };


  /*------------------------------------------------------------------------*/
  gboolean _mootFSMgfsm_analyze_token_foreach_func(gfsmPath *path,
						   gpointer value_dummy,
						   struct _mootFSMgfsm_analyze_token_params *params)
  {
    g_string_truncate(params->gstr,0);
    gfsm_alphabet_labels_to_gstring(params->abet,
				    path->lo,
				    params->gstr,
				    params->warn_on_undefined,
				    params->att_style);
    params->tokp->insert(mootToken::Analysis(params->gstr->str,"",path->w));

    return FALSE;
  };


}; //-- namespace mootm


#endif /* defined(USE_FSM_GFSM) */
