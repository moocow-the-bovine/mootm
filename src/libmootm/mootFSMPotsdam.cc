/*-*- Mode: C++ -*-*/

/*
   libmootm : moocow's morphology library:
   Copyright (C) 2003-2005 by Bryan Jurish <moocow@cpan.org>

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
 * Author: Bryan Jurish <moocow@cpan.org>
 * Description:
 *   + moot FSMs: uni-Potsdam
 *----------------------------------------------------------------------*/

#include <mootFSMPotsdam.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fstream>

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

#if defined(USE_FSM_POTSDAM)

namespace mootm {
  using namespace std;
  using namespace moot;

  /*------------------------------------------------------------------------*/
  bool mootFSMPotsdam::load(const string &fstfile, const string &symfile)
  {
    bool rc=true;

    //-- cleanup
    if (syms && !symfile.empty()) { delete syms; syms=NULL; }
    if (mfst && !fstfile.empty()) { delete mfst; mfst=NULL; }

    //-- load: symbols
    if (!symfile.empty()) {
      //-- argh: FSMSymSpec don't take a 'const char *'
      char *symfile_cp = (char *)malloc(symfile.size()+1);
      strcpy(symfile_cp, symfile.c_str());
      syms = new FSMSymSpec(symfile_cp, &syms_msgs, moot_SYM_ATT_COMPAT);

      if (!syms_msgs.empty()) {
	rc = false;

	carp("mootFSMPotsdam::load() Error: could not load symbols from file '%s'\n",
	     symfile.c_str());
	for (list<string>::iterator e = syms_msgs.begin(); e != syms_msgs.end(); e++) {
	  carp("%s\n",e->c_str());
	}
	syms_msgs.clear(); // -- clear messages
	syms = NULL;       // -- invalidate the object
      }

      //-- cleanup
      free(symfile_cp);
    }

    //-- load: FST
    if (!fstfile.empty()) {
      ifstream ifs(fstfile.c_str(), ios::in|ios::binary);
      mfst = new FSM(ifs,fstfile.c_str());
      ifs.close();

      if (!(mfst && *mfst)) {
	carp("mootFSMPotsdam::load(): Error: load failed for FSM file '%s'.\n", fstfile.c_str());
	if (mfst) delete mfst;
	mfst = NULL;
	rc = false;
      }
    }

    //-- load: use syms
    if (mfst && syms)  mfst->fsm_use_symbol_spec(*syms);

    return rc;
  };


  /*------------------------------------------------------------------------*/
  mootToken& mootFSMPotsdam::analyze_token(mootToken &tok, bool want_avm, bool want_warnings)
  {
    //-- analyze : fsm_lookup(string s, FSM *result, bool deterministic, bool connect, bool warn)
    //result = mfst->fsm_lookup(tok.text(), NULL, false, false, false); //-- no connect => 1.1K tok/s
    result = mfst->fsm_lookup(tok.text(), NULL, false, true, false); //-- +connect => 1.5K tok/s

    //-- serialize, NOT extracting tags
    analyses.clear();
    result.fsm_symbol_vectors(analyses, want_warnings);

    //-- output analyses to the mootToken
    for (MorphAnalysisSet::const_iterator anlsi = analyses.begin(); anlsi != analyses.end(); anlsi++) {
      mootToken::Analysis analysis;

      //-- "output" string is an extracted tag, if it is present
      if (!anlsi->ostr.empty()) {
	syms->symbol_vector_to_string(anlsi->ostr, analysis.tag, want_avm, want_warnings);
	syms->symbol_vector_to_string(anlsi->istr, analysis.details, want_avm, want_warnings);
      }
      else {
	syms->symbol_vector_to_string(anlsi->istr, analysis.tag, want_avm, want_warnings);
      }

      analysis.prob = anlsi->weight.weight();

      tok.insert(analysis);
    }

    //-- check for errors (hack)
    if (want_warnings) check_symspec_messages();

    return tok;
  };

  /*------------------------------------------------------------------------*/
  void mootFSMPotsdam::check_symspec_messages(void)
  {
    if (!syms_msgs.empty()) {
      for (list<string>::const_iterator e = syms_msgs.begin();
	   e != syms_msgs.end();
	   e++)
	{
	  carp("%s\n",e->c_str());
	}
      syms_msgs.clear();
    }
  };


}; //-- namespace mootm


#endif /* defined(USE_FSM_POTSDAM) */
