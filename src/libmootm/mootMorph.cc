/* -*- Mode: C++ -*- */

/*
   libmootm : moocow's morphology library
   Copyright (C) 2003-2004 by Bryan Jurish <moocow@ling.uni-potsdam.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/


/*--------------------------------------------------------------------------
 * File: mootMorph.cc
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + moocow's PoS tagger : morphology
 *--------------------------------------------------------------------------*/

#ifdef HAVE_CONFIG_H
# include <mootmConfig.h>
#endif

#include <stdio.h>
#include <stdarg.h>

#include <string>
#include <set>
#include <list>

#include "mootMorph.h"
#include "mootTokenIO.h"

namespace mootm {
  using namespace std;
  using namespace moot;

/*--------------------------------------------------------------------------
 * Constructor / Destructor
 *--------------------------------------------------------------------------*/

/*
 * mootMorph::~mootMorph()
 */
mootMorph::~mootMorph() {
  if (tmp) delete tmp;
  if (syms && i_made_syms) delete syms;
  if (mfst && i_made_mfst) delete mfst;

  syms = NULL;
  mfst = NULL;
  result = NULL;
  tmp = NULL;
}

/*--------------------------------------------------------------------------
 * Initialization
 *--------------------------------------------------------------------------*/

/*
 * FSMSymSpec *mootMorph::load_morph_symbols(const char *filename)
 */
FSMSymSpec *mootMorph::load_morph_symbols(const char *filename)
{
  // -- cleanup old symbols first
  if (filename) syms_filename = (const char *)filename;
  if (syms && i_made_syms) delete syms;

  //-- argh: FSMSymSpec don't take a 'const char *'
  char *syms_filename_cp = (char *)malloc(syms_filename.size()+1);
  strcpy(syms_filename_cp, syms_filename.c_str());
  syms = new FSMSymSpec(syms_filename_cp, &syms_msgs, moot_SYM_ATT_COMPAT);

  if (!syms_msgs.empty()) {
    carp("mootMorph::load_symbols() Error: could not load symbols from file '%s'\n",
	 syms_filename.c_str());
    for (list<string>::iterator e = syms_msgs.begin(); e != syms_msgs.end(); e++) {
      carp("%s\n",e->c_str());
    }
    syms_msgs.clear(); // -- clear messages
    syms = NULL;       // -- invalidate the tagger object
  }

  i_made_syms = true;

  //-- use symspec in FST(s) if not already in use
  if (mfst) mfst->fsm_use_symbol_spec(syms);
  if (xfst) xfst->fsm_use_symbol_spec(syms);

  //-- cleanup
  free(syms_filename_cp);

  return syms;
}

/*
 * FSM *mootMorph::load_fsm_file(const char *filename, FSM **fsm, bool *i_made_fsm=NULL);
 */
FSM *mootMorph::load_fsm_file(const char *filename, FSM **fsm, bool *i_made_fsm)
{
  // -- cleanup old FSM first
  if (*fsm && i_made_fsm && *i_made_fsm) { delete *fsm; }

  *fsm = new FSM(filename);
  if (!**fsm) {
    carp("mootMorph::load_fsm_file() Error: load failed for FSM file '%s'.\n", filename);
    *fsm = NULL; // -- invalidate the object
  }
  if (i_made_fsm) { *i_made_fsm = true; }
  return *fsm;
}


/*--------------------------------------------------------------------------
 * Tag-extraction (should be inlined eventually)
 *--------------------------------------------------------------------------*/

//mootMorph::MorphAnalysisSet& mootMorph::extract_tags(FSM &morph_w, MorphAnalysisSet &pos_w)

/*--------------------------------------------------------------------------
 * Top-level tagging methods
 *--------------------------------------------------------------------------*/

bool mootMorph::tag_stream(FILE *in, FILE *out, char *srcname)
{
  TokenReader treader;
  TokenWriter twriter;
  treader.select_stream(in, srcname);

  // -- sanity check
  if (!can_tag()) {
    carp("mootMorph::tag_stream(): cannot run uninitialized morphology!\n");
    return false;
  }

  // -- do analysis
  do {
    mootSentence &sent = treader.get_sentence();
    if (sent.empty()) break;

    for (mootSentence::iterator si = sent.begin(); si != sent.end(); si++) {
      if (force_reanalysis || si->analyses.empty()) analyze_token(*si);
      ntokens++;
      if (nprogress && ntokens % nprogress == 0) fputc('.',stderr);
    }
    twriter.sentence_put(out,sent);
  } while (1);
  if (nprogress) fputc('\n',stderr);

  return true;
}

bool mootMorph::tag_strings(int argc, char **argv, FILE *out, char *srcname)
{
  // -- sanity check
  if (!can_tag()) {
    fprintf(stderr, "mootMorph::tag_strings(): cannot run uninitialized tagger!\n");
    return false;
  }

  // -- ye olde guttes
  mootSentence sent;
  TokenWriter twriter;
  for ( ; --argc >= 0; argv++) {
    mootToken t(*argv);
    analyze_token(t);
    sent.push_back(t);
    ntokens++;
  }
  twriter.sentence_put(out, sent);

  return true;
}


/*--------------------------------------------------------------------------
 * mid-Level Tagging Methods
 *--------------------------------------------------------------------------*/

//(inlined)


/*--------------------------------------------------------------------------
 * public methods: tagging utilities: string-generation
 *--------------------------------------------------------------------------*/

//(inlined)

/*--------------------------------------------------------------------------
 * Mid-level tagging utilities: output
 *--------------------------------------------------------------------------*/

//(inlined)

/*--------------------------------------------------------------------------
 * Mid-level tagging utilities: conversions
 *--------------------------------------------------------------------------*/

void mootMorph::symbol_vector_to_string_dq(const vector<FSMSymbol> &vec, FSMSymbolString &str)
const
{
  bool last_was_char = true;
  str.clear();

  //-- Special case for first symbol --
  vector<FSMSymbol>::const_iterator vi;
  for (vi = vec.begin(); vi != vec.end(); vi++) {
    register const FSMSymbolString *isym = syms->symbol_to_symbolname(*vi);
    if (isym == NULL) {
      if (verbose >= vlWarnings) {
	carp("mootMorph::symbol_vector_to_string_dq(): Error: undefined symbol '%d' -- ignored\n",
	     *vi);
      }
      continue;
    }
    else { //-- isym==NULL
      if (isym->size() > 1) {
	if ((*isym)[0] == '_') {
	  //str.append(*isym, 1, isym->size());
	  str.append(isym->begin()+1, isym->end());
	} else {
	  str.append(*isym);
	}
	//str.push_back('.');
	last_was_char = false;
      } else {
	str.push_back((*isym)[0]);
	last_was_char = true;
      }
    }
    break;
  }


  //-- Normal case --
  for (vi++; vi != vec.end(); vi++) {
    if (*vi == EPSILON || *vi == FSMNOLABEL) continue;
    register const FSMSymbolString *isym = syms->symbol_to_symbolname(*vi);
    if (isym == NULL) {
      if (verbose >= vlWarnings) {
	carp("mootMorph::symbol_vector_to_string_dq(): Error: undefined symbol '%d' -- ignored\n",
	     *vi);
      }
      continue;
    }
    else { //-- isym==NULL
      // -- it's a kosher symbol
      if (isym->size() > 1) {
	str.push_back('.');
	if ((*isym)[0] == '_') {
	  //str.append(*isym, 1, isym->size());
	  str.append(isym->begin()+1, isym->end());
	} else {
	  str.append(*isym);
	}
	last_was_char = false;
      } else {
	if (!last_was_char) str.push_back('.');
	str.push_back((*isym)[0]);
	last_was_char = true;
      }
    }
  }
};



/*--------------------------------------------------------------------------
 * Debugging Methods
 *--------------------------------------------------------------------------*/

// (removed)

/*--------------------------------------------------------------------------
 * Error reporting
 *--------------------------------------------------------------------------*/

void mootMorph::carp(char *fmt, ...) const
{
  if (verbose >= vlErrors) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    /*
      fprintf(stderr, " %s%s at line %d, column %d, near '%s'\n",
      (srcname ? "in file " : ""),
      (srcname ? srcname : ""),
      lexer.theLine, lexer.theColumn, lexer.yytext);
    */
  }
}


/*--------------------------------------------------------------------------
 * Debugging Methods
 *--------------------------------------------------------------------------*/

/* Convert a symbol-vector to a numeric string */
string
mootMorph::symbol_vector_to_string_n(const FSM::FSMSymbolVector &v)
{
  string vs;
  char buf[256];
  for (FSM::FSMSymbolVector::const_iterator vi = v.begin(); vi != v.end(); vi++)
    {
      sprintf(buf,"%d",*vi);
      vs.append(buf);
    }
  return vs;
}

/* Stringify a token-analysis-set (weighted-vector version) */
string mootMorph::analyses_to_string(const set<FSM::FSMWeightedSymbolVector> &analyses)
{
  string s = "{";
  for (set<FSM::FSMWeightedSymbolVector>::const_iterator asi = analyses.begin();
       asi != analyses.end();
       asi++)
    {
      if (asi != analyses.begin()) s += ", ";
      s += symbol_vector_to_string_n(asi->istr);
      if (!asi->ostr.empty()) {
	s += " : ";
	s += symbol_vector_to_string_n(asi->istr);
      }
      if (asi->weight != 0.0) {
	char buf[256];
	sprintf(buf, " <%g>", asi->weight);
	s += buf;
      }
    }
  s += "}";
  return s;
}


/* Stringify a token-analysis-set (weighted-string-version) */
string
mootMorph::analyses_to_string(const set<FSM::FSMStringWeight> &analyses)
{
  string s = "{";
  for (set<FSM::FSMStringWeight>::const_iterator asi = analyses.begin();
       asi != analyses.end();
       asi++)
    {
      if (asi != analyses.begin()) s += ", ";
      s += asi->istr;
      if (!asi->ostr.empty() && asi->ostr != "") {
	s += " : ";
	s += asi->ostr;
      }
      if (asi->weight != 0.0) {
	char buf[256];
	sprintf(buf, " <%g>", asi->weight);
	s += buf;
      }
    }
  s += "}";
  return s;
}

/* Stringify a token-analysis-set (numeric-tags version) */
string
mootMorph::analyses_to_string(const set<FSMSymbol> &analyses)
{
  string s = "{";
  char buf[256];
  for (set<FSMSymbol>::const_iterator asi = analyses.begin();
       asi != analyses.end();
       asi++)
    {
      if (asi != analyses.begin()) s += ", ";
      sprintf(buf,"%d", *asi);
      s += buf;
    }
  s += "}";
  return s;
}

/* Stringify a token-analysis-set (string-tags version) */
string
mootMorph::analyses_to_string(const set<FSMSymbolString> &analyses)
{
  string s = "{";
  for (set<FSMSymbolString>::const_iterator asi = analyses.begin();
       asi != analyses.end();
       asi++)
    {
      if (asi != analyses.begin()) s += ", ";
      s += *asi;
    }
  s += "}";
  return s;
}

}; //namespace mootm;
