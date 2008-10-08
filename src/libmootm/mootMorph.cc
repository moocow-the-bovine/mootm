/* -*- Mode: C++ -*- */

/*
   libmootm : moocow's morphology library
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


/*--------------------------------------------------------------------------
 * File: mootMorph.cc
 * Author: Bryan Jurish <moocow@ling.uni-potsdam.de>
 * Description:
 *   + moocow's PoS tagger : morphology
 *--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>

#include <string>
#include <set>
#include <list>

#include <mootMorph.h>
#include <mootTokenIO.h>
#include <mootFSMBase.h>
#include <mootFSMPotsdam.h>
#include <mootFSMRWTH.h>

#ifdef HAVE_CONFIG_H
# include <mootmUnConfig.h>
# include <mootmConfig.h>
#endif

namespace mootm {
  using namespace std;
  using namespace moot;

/*--------------------------------------------------------------------------
 * Initialization
 *--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------
 * Top-level tagging methods
 *--------------------------------------------------------------------------*/

bool mootMorph::analyze_io(TokenReader *reader, TokenWriter *writer)
{
  // -- sanity check
  if (!valid()) {
    carp("mootMorph::analyze_io(): cannot run uninitialized morphology FST!\n");
    return false;
  }

  // -- do analysis
  int rtok;
  while (reader && (rtok = reader->get_sentence()) != TokTypeEOF) {
    mootSentence *sent = reader->sentence();
    if (!sent) break;

    for (mootSentence::iterator si = sent->begin(); si != sent->end(); si++) {
      if (si->toktype() != TokTypeVanilla) continue; //-- only analyze vanilla toks
      if (force_reanalysis || si->analyses().empty()) analyze_token(*si);
      ntokens++;
      if (nprogress && ntokens % nprogress == 0) fputc('.',stderr);
    }
    if (writer) writer->put_sentence(*sent);
  }
  if (nprogress) fputc('\n',stderr);

  return true;
}

/*------------------------------------------------------------------------*/
bool mootMorph::analyze_stream(FILE *in, FILE *out, const char *srcname)
{
  // -- sanity check
  if (!valid()) {
    carp("mootMorph::analyze_stream(): cannot run uninitialized morphology FST!\n");
    return false;
  }

  int ifmt = ignore_first_analysis  ? tiofWellDone : tiofMediumRare;
  int ofmt = first_analysis_is_best ? tiofWellDone : tiofMediumRare;

  //tr.lexer.ignore_first_analysis = ignore_first_analysis;
  TokenReaderNative tr(ifmt);
  TokenWriterNative tw(ofmt);

  tr.from_file(in);
  tw.to_file(out);

  if (srcname) tr.reader_name(srcname);
  return analyze_io(&tr,&tw);
};


/*------------------------------------------------------------------------*/
bool mootMorph::analyze_strings(int argc, char **argv, FILE *out, const char *srcname)
{
  // -- sanity check
  if (!valid()) {
    carp("mootMorph::analyze_strings(): cannot run uninitialized morphology FST!\n");
    return false;
  }

  // -- ye olde guttes
  mootSentence sent;
  TokenWriterNative twriter(tiofMediumRare);
  twriter.to_file(out);
  mootToken tok;
  for ( ; --argc >= 0; argv++) {
    tok.clear();
    tok.text((const char *)*argv);
    analyze_token(tok);
    sent.push_back(tok);
    ntokens++;
  }
  twriter.put_sentence(sent);

  sent.clear();
  return true;
}



/*--------------------------------------------------------------------------
 * Error reporting
 *--------------------------------------------------------------------------*/

void mootMorph::carp(const char *fmt, ...) const
{
  if (verbose >= vlErrors) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
  }
}


/*--------------------------------------------------------------------------
 * Debugging Methods
 *--------------------------------------------------------------------------*/

#if 0

/* Convert a symbol-vector to a numeric string */
string
mootMorph::symbol_vector_to_string_n(const FSMSymbolVector &v)
{
  string vs;
  char buf[256];
  for (FSMSymbolVector::const_iterator vi = v.begin(); vi != v.end(); vi++)
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
      if ((double)asi->weight != (double)0.0) {
	char buf[64];
#if FSM_API_REVISION == 0
	sprintf(buf, " <%g>", asi->weight);
#else /* FSM_API_REVISION != 0 */
	sprintf(buf, " <%g>", asi->weight.weight());
#endif /* FSM_API_REVISION */
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
      if ((double)asi->weight != (double)0.0) {
	char buf[64];
#if FSM_API_REVISION == 0
	sprintf(buf, " <%g>", asi->weight);
#else /* FSM_API_REVISION != 0 */
	sprintf(buf, " <%g>", asi->weight.weight());
#endif /* FSM_API_REVISION */
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

#endif /* 0 (debugging methods) */

}; //namespace mootm;
