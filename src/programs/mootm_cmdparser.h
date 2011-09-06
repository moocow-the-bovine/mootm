/* -*- Mode: C -*-
 *
 * File: mootm_cmdparser.h
 * Description: Headers for command-line parser struct gengetopt_args_info.
 *
 * File autogenerated by optgen.perl version 0.06.
 *
 */

#ifndef mootm_cmdparser_h
#define mootm_cmdparser_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * moocow: Never set PACKAGE and VERSION here.
 */

struct gengetopt_args_info {
  char * rcfile_arg;	 /* Read an alternate configuration file. (default=NULL). */
  int verbose_arg;	 /* Verbosity level. (default=3). */
  int dots_arg;	 /* Print a dot for every NTOKS tokens processed. (default=0). */
  int words_flag;	 /* INPUTs are input tokens, not filenames. (default=0). */
  int list_flag;	 /* INPUTs are file-lists, not filenames. (default=0). */
  char * output_arg;	 /* Specify output file (default=stdout). (default=-). */
  int escapes_flag;	 /* Honor AT&T regex escapes in input. (default=0). */
  int att_flag;	 /* Produce AT&T-style output format. (default=1). */
  int reanalyze_flag;	 /* Force re-analysis of pre-analyzed tokens. (default=0). */
  int first_is_best_flag;	 /* Assume 1st analysis is 'best' for each input token. (default=0). */
  int ignore_first_flag;	 /* Ignore the first analysis for each input token. (default=0). */
  char * symbols_arg;	 /* Specify morphological symbols file. (default=moot.sym). */
  char * morph_arg;	 /* Specify morphological transducer. (default=moot.fst). */
  char * eow_arg;	 /* Specify implicit EOW string. (default=). */

  int help_given;	 /* Whether help was given */
  int version_given;	 /* Whether version was given */
  int rcfile_given;	 /* Whether rcfile was given */
  int verbose_given;	 /* Whether verbose was given */
  int dots_given;	 /* Whether dots was given */
  int words_given;	 /* Whether words was given */
  int list_given;	 /* Whether list was given */
  int output_given;	 /* Whether output was given */
  int escapes_given;	 /* Whether escapes was given */
  int att_given;	 /* Whether att was given */
  int reanalyze_given;	 /* Whether reanalyze was given */
  int first_is_best_given;	 /* Whether first-is-best was given */
  int ignore_first_given;	 /* Whether ignore-first was given */
  int symbols_given;	 /* Whether symbols was given */
  int morph_given;	 /* Whether morph was given */
  int eow_given;	 /* Whether eow was given */
  
  char **inputs;         /* unnamed arguments */
  unsigned inputs_num;   /* number of unnamed arguments */
};

/* read rc files (if any) and parse all command-line options in one swell foop */
int  cmdline_parser (int argc, char *const *argv, struct gengetopt_args_info *args_info);

/* instantiate defaults from environment variables: you must call this yourself! */
void cmdline_parser_envdefaults (struct gengetopt_args_info *args_info);

/* read a single rc-file */
void cmdline_parser_read_rcfile (const char *filename,
				    struct gengetopt_args_info *args_info,
				    int user_specified);

/* read a single rc-file (stream) */
void cmdline_parser_read_rc_stream (FILE *rcfile,
				       const char *filename,
				       struct gengetopt_args_info *args_info);

/* parse a single option */
int cmdline_parser_parse_option (char oshort, const char *olong, const char *val,
				    struct gengetopt_args_info *args_info);

/* print help message */
void cmdline_parser_print_help(void);

/* print version */
void cmdline_parser_print_version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* mootm_cmdparser_h */