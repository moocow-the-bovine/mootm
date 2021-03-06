/*
   mootm  : moocow's part-of-speech tagger : morphology
   Copyright (C) 2002-2010 by Bryan Jurish <moocow@cpan.org>

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
 * File: mootm_main.cc
 * Author: Bryan Jurish <moocow@cpan.org>
 * Description:
 *   + Morphological analyzer moot : main()
 *--------------------------------------------------------------------------*/

#include <config.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <mootMorph.h>
#include <mootUtils.h>
#include <mootCIO.h>

#include <config.h>
#include "mootm_cmdparser.h"

using namespace std;
using namespace moot;
using namespace mootio;
using namespace mootm;

#ifdef USE_FSM_RWTH
# define MAIN_C main_c
# include <Core/Application.hh>
# include <Fsa/Application.hh>

using namespace Fsa;

APPLICATION

int MAIN_C(int argc, char **argv);

class mootmApplication : public virtual Core::Application, public virtual Fsa::Application {
public:
  mootmApplication() {
    setTitle("mootm");
    setDefaultLoadConfigurationFile(false);
  };
  virtual int main(const std::vector<std::string> &args)
  {
    int    argc = args.size()+1;
    char **argv = (char **)(malloc((args.size()+1)*sizeof(char *)));
    int    i, status;
    argv[0] = "mootm";
    for (i=0; i < (int)args.size(); i++) { argv[i+1] = (char *)args[i].c_str(); }
    status = MAIN_C(argc,argv);
    free(argv);
    return status;
  };
} app; //-- class mootmApplication

#else 
# define MAIN_C main
#endif // USE_FSM_RWTH

/*--------------------------------------------------------------------------
 * Globals
 *--------------------------------------------------------------------------*/
const char *PROGNAME = "mootm";

// options & file-churning
gengetopt_args_info args;
cmdutil_file_churner churner;

// -- files
mofstream out;

// -- global classes/structs
mootMorph morph;

// -- for verbose timing info
timeval t1, t2, t3;
double elapsed_i, elapsed_t;


/*-- now defined as moot::VerbosityLevel
typedef enum {
  vlSilent=0,
  vlErrors=1,
  vlProgress=2,
  vlTiming=3,
  vlWarnings=4,
  vlEverything=5
} VerbosityLevel;
*/

/*--------------------------------------------------------------------------
 * Option Processing
 *--------------------------------------------------------------------------*/
void GetMyOptions(int argc, char **argv)
{
  if (cmdline_parser(argc, argv, &args) != 0)
    exit(1);

  // -- load environmental defaults
  cmdline_parser_envdefaults(&args);

  //-- show banner
  if (args.verbose_arg >= vlProgress)
    fprintf(stderr,
	    "%s version %s by Bryan Jurish <moocow@cpan.org>\n\n",
	    PROGNAME, VERSION);

  //-- output file
  if (!out.open(args.output_arg,"w")) {
    fprintf(stderr,"%s: open failed for output-file '%s': %s\n",
	    PROGNAME, out.name.c_str(), out.errmsg().c_str());
    exit(1);
  }

  //-- set up file-churner
  churner.progname = PROGNAME;
  churner.inputs = args.inputs;
  churner.ninputs = args.inputs_num;
  churner.use_list = args.list_given;

  //-- get initialization start-time
  if (args.verbose_arg >= vlProgress) gettimeofday(&t1, NULL);

  //-- mophology object setup: flags
  morph.eow_str                = args.eow_arg;
  morph.want_att_escapes       = args.escapes_given;
  morph.want_avm               = args.att_given;
  morph.force_reanalysis       = args.reanalyze_given;
  morph.first_analysis_is_best = args.first_is_best_given;
  morph.ignore_first_analysis  = args.ignore_first_given;

  if      (args.verbose_arg <= vlSilent)   morph.verbose = mootMorph::vlSilent;
  else if (args.verbose_arg <= vlErrors)   morph.verbose = mootMorph::vlErrors;
  else if (args.verbose_arg <= vlWarnings) morph.verbose = mootMorph::vlWarnings;
  else                                     morph.verbose = mootMorph::vlEverything;

  morph.nprogress = args.dots_arg;

  //-- morphology object setup
  if (args.verbose_arg >= vlProgress) {
    fprintf(stderr, "%s: configuration\n", PROGNAME);
    fprintf(stderr, "%s:    + Symbols    : %s\n", PROGNAME, args.symbols_arg);
    fprintf(stderr, "%s:    + Transducer : %s\n", PROGNAME, args.morph_arg);
    fprintf(stderr, "%s:    + EOW String : \"%s\"\n", PROGNAME, args.eow_arg);
    fprintf(stderr, "%s: loading morphology...", PROGNAME);
  }
  if (!morph.load(args.morph_arg, args.symbols_arg)) {
    fprintf(stderr,"\n%s: load FAILED for morpholgy (syms='%s', fst='%s')!\n",
	    PROGNAME, args.symbols_arg, args.morph_arg);
    exit(1);
  } else if (args.verbose_arg >= vlProgress) {
    fprintf(stderr, " loaded.\n");
  }
  //-- morphology object setup : tag-extraction FST
  /*
  if (args.tagx_given) {
    if (args.verbose_arg >= vlProgress)
      fprintf(stderr, "%s: loading tag-extracion FST '%s'...", PROGNAME, args.tagx_arg);
    if (!morph.load_tagx_fst(args.tagx_arg)) {
      fprintf(stderr,"\n%s: load FAILED for morphological FST '%s'\n", PROGNAME, args.tagx_arg);
      exit(1);
    } else if (args.verbose_arg >= vlProgress) {
      fprintf(stderr," loaded.\n");
    }
  }
  */

  //-- report
  if (args.verbose_arg >= vlProgress) {
    fprintf(stderr, "%s: Initialization complete.\n", PROGNAME);
  }
}

/*--------------------------------------------------------------------------
 * main
 *--------------------------------------------------------------------------*/
int MAIN_C (int argc, char **argv)
{
  int nfiles = 0;
  GetMyOptions(argc,argv);

  // -- get init-stop time = analysis-start time
  if (args.verbose_arg >= vlProgress) gettimeofday(&t2, NULL);


  // -- the guts
  if (args.words_given) {
    morph.analyze_strings(args.inputs_num, args.inputs, out.file, out.name.c_str());
  }
  else {
    // -- big loop
    for (churner.first_input_file(); churner.in.file; churner.next_input_file()) {
      nfiles++;
      if (args.verbose_arg >= vlProgress) {
	fprintf(stderr, "%s: analyzing file '%s'...",
		PROGNAME, churner.in.name.c_str());
	fflush(stderr);
      }
      fprintf(out.file, "\n%%%% %s: File: %s\n\n",
	      PROGNAME,
	      churner.in.name.c_str());

      morph.analyze_stream(churner.in.file, out.file, churner.in.name.c_str());

      if (args.verbose_arg >= vlProgress) {
	fprintf(stderr," done.\n");
	fflush(stderr);
      }
    }
    out.close();
  }

  // -- summary
  if (args.verbose_arg >= vlProgress) {
      // -- timing
      gettimeofday(&t3, NULL);

      elapsed_i = (t2.tv_sec-t1.tv_sec) + (double)(t2.tv_usec-t1.tv_usec)/1000000.0;
      elapsed_t = (t3.tv_sec-t2.tv_sec) + (double)(t3.tv_usec-t2.tv_usec)/1000000.0;

      // -- print summary
      fprintf(stderr, "\n-----------------------------------------------------\n");
      fprintf(stderr, "%s Summary:\n", PROGNAME);
      fprintf(stderr, "  + Morphology\n");
      fprintf(stderr, "    - Tokens processed    : %d\n", morph.ntokens);
      fprintf(stderr, "    - Tokens analyzed     : %d\n", morph.nanalyzed);
      fprintf(stderr, "    - Unknown tokens      : %d\n", morph.nunknown);
      if (morph.ntokens > 0) {
	// -- avoid div-by-zero errors
	fprintf(stderr, "    - Analysis Rate       : %.2f %%\n",
		100.0*(double)(morph.nanalyzed)/(double)morph.ntokens);
	fprintf(stderr, "    - Recognition Rate    : %.2f %%\n",
		100.0*(double)(morph.ntokens-morph.nunknown)/(double)morph.ntokens);
      } else {
	fprintf(stderr, "    - Analysis Rate       : -NaN-\n");
	fprintf(stderr, "    - Recognition Rate    : -NaN-\n");
      }
      fprintf(stderr, "  + General\n");
      fprintf(stderr, "    - Files processed     : %d\n", nfiles);
      fprintf(stderr, "    - Initialize Time     : %.2f sec\n", elapsed_i);
      fprintf(stderr, "    - Analysis Time       : %.2f sec\n", elapsed_t);
      fprintf(stderr, "    - Throughput          : %.2f tok/sec\n", (float)morph.ntokens/elapsed_t);
      fprintf(stderr, "-----------------------------------------------------\n");
  }

  return 0;
}
