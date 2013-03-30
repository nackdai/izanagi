/*
 *                              M A I N . C
 *                  C P P   M a i n   P r o g r a m
 *
 * Edit history of DECUS CPP (MM: Martin Minow)
 * 21-May-84    MM      "Field test" release
 * 11-Jul-84    MM      "Official" first release (that's what I thought!)
 * 31-Aug-84    MM      USENET net.sources release.
 *  7-Dec-84    MM      Stuff in Nov 12 Draft Standard
 * 07-Jun-85    KR      Latest revision
 */

/*
 * 1985/12      Implemented for MS-DOS compilers.
 *                                                  by Ted Lemon & Jym Dyer
 * 1988/01, 1988/12     Re-implemented.
 *                                  by Blake McBride & Roberto Artigas Jr.
 */

/*
 * 1989/04      Implemented for OS-9/6809 Microware C.
 *                                                  by Gigo & others
 */

/*
 * CPP Version 2.0
 * 1998/08      Re-written according to ISO 9899:1990 and it's Amendment 1,
 *      Corrigendum 1, 2.               by Psycho (kmatsui@t3.rim.or.jp)
 *
 *      Implemented translation phases precisely.       (support.c)
 *      Revised tokenization according to the Standard and Amendment1.
 *                                                      (support.c & others)
 *      Implemented the pre-defined macros __STDC__, __STDC_VERSION__,
 *          __TIME__, and revised __DATE__.  Made these standard macros can't
 *          be undefined nor redefined.                 (main.c)
 *      Implemented _Pragma() operator.                 (main.c & others)
 *      Revised some non-standard pre-defined macros.   (main.c)
 *      Implemented #error directive.  The error message is output to the
 *          stderr.                                     (control.c)
 *      Implemented #pragma __once directive, -i option and -M* option,
 *          imported from GNU C / cpp.                  (control.c & others)
 *      Implemented #pragma __put_defines, #pragma __debug directives and the
 *          old style directives corresponding to them. (system.c & others)
 *      Made #pragma lines to be output with warning to the stderr for the
 *          compiler which can't recognize the directive.       (system.c)
 *      Made #line argument to be subject to macro expansion.
 *                                                      (control.c, support.c)
 *      Reinforced the test of #define syntax.          (control.c)
 *      Created Standard conforming mode of macro-expansion (including the
 *          processing of #, ## operators).  The Standard mode of expansion is
 *          very different from the pre-Standard modes. (expand.c)
 *      Created "post-Standard" mode of preprocessing, which is a simplified
 *          version of Standard mode.                   (all files)
 *      Simplified CON_FALSE mode corresponding to K&R 1st. specifications.
 *          CON_NOEXPAND, CON_EXPAND modes of the original version are
 *          retained (after revision).  (main.c, control.c eval.c, expand.c)
 *      Revised # operator so as to inserts \ before \ or " in stringized
 *          arguments (except in MBCHAR) in Standard mode.      (expand.c)
 *      Changed the type of #if expression from int to long / unsigned long.
 *          Reinforced expression evaluation.           (eval.c)
 *      Implemented wide character constant, multi-character character
 *          constant, and revised multi-byte character constant in #if
 *          expression.                                 (eval.c)
 *      Revised the handling of MBCHAR in string literal and character
 *          constant.                   (support.c, expand.c, eval.c, main.c)
 *      Supplemented the optional phase for the pre-Standard compiler-proper
 *          to concatenate adjacent string literals, convert '\a' and '\v' to
 *          octals, convert digraphs.                   (main.c)
 *      Implemented the features of C99-1997/11 draft except Unicode-related
 *          features (_Pragma operator, variable arguments of macro, //
 *          comments, long long of #if expression, p+ of pp-number)
 *                                                      (all files)
 *      Supplemented the C++ preprocessor option.       (support.c, system.c)
 *      Refined error checks and diagnostic messages.   (all files)
 *      Implemented -M* option.                         (main.c, system.c)
 *      Updated MS-DOS memory model option.             (system.c)
 *      Revised command line options.                   (system.c)
 *      Made the source files compilable by C++ as well as C.   (all files)
 *      Re-organized and re-written the source files to be portable to many
 *           systems.                                   (all files)
 *
 *      See "cpp_20.man", "cpp_20.doc" and "cpp_test.doc" for details.
 *
 *      Dependencies among the source files are as follows:
 *          main.c, control.c, eval.c, expand.c, support.c and system.c
 *              depend on system.H and internal.H
 *              system.H should be included prior to internal.H.
 *          lib.c depends on system.H.
 *          lib.c is provided for the library which has not the particular
 *              functions.
 *      You should add to stack size
 *              NMACWORK + (NEXP * 30) + (sizeof (int) * 100)
 *          and for MODE >= STANDARD
 *              (sizeof (char *) * 12 * RESCAN_LIMIT)
 *          other than the size needed by the system.
 */

/*
 * CPP Version 2.1
 * 1998/09      Psycho
 *      Updated C9X features according to 1998/08 draft (including UCN,
 *          optional multi-byte-character in identifier, type of #if
 *          expression in integer of maximum size and concatenation of
 *          wide-character-string-literal and character-string-literal).
 *                                              (main.c, eval.c, support.c)
 */

/*
 * CPP Version 2.2
 * 1998/11      Psycho
 *      Updated according C++ Standard (ISO/IEC 14882:1998).
 *                                              (eval.c, support.c)
 *      Fixed the bug of interaction of predefined non-standard macro with
 *          -D option.              (main.c, control.c, expand.c, system.c)
 */

/*
 * CPP Version 2.0 / main.c
 * 1998/08      Psycho
 *      Renamed cpp1.c main.c.
 *      Created do_pragma_op(), de_stringize(), devide_line(), putout(),
 *          putline(), post_preproc(), conv_esc(), conv2oct(), is_last_esc(),
 *          esc_mbchar(), conv_a_digraph().
 *      Removed output().
 *      Moved sharp() from cpp1.c to system.c,
 *          addfile(), openfile(), initdefines(), unpredefine()
 *              from cpp3.c to main.c,
 *      Revised most of the functions and variables.
 */

/*
 * CPP Version 2.1 / main.c
 * 1998/09      Psycho
 *      Revised several functions.
 */

/*
 * CPP Version 2.2 / main.c
 * 1998/11      Psycho
 *      Created undef_a_predef().
 *      Revised several functions.
 */

#if PREPROCESSED                /* Use "pre-preprocessed" header    */
#include    "cpp.H"
#else
#include    "system.H"
#include    "internal.H"
#endif

/*
 * If eflag is set, CPP returns "success" even if non-fatal errors were
 * detected.
 */
    int     eflag = FALSE;          /* -E option (never fail)       */
D09 int     cflag = FALSE;          /* -C option (keep comments)    */
D09 int     iflag = FALSE;      /* -i option (no output of included file)   */
D09 int     lflag = FALSE;          /* -L option (output source as  */
                                    /* comments for compiler-proper)*/
D09 int     pflag = FALSE;          /* -P option (no #line output)  */
    int     qflag = FALSE;      /* -Q option (diagnostics to "cpp.err")     */
#if MODE != CON_FALSE && OK_TRIGRAPHS
D09 int     tflag = TFLAG_INIT;     /* -3 option (trigraphs)        */
#endif
#if MODE >= STANDARD
#if OK_DIGRAPHS
D09 int     digraphs = DIGRAPHS_INIT;       /* -2 option (digraphs) */
#endif
    long    cplus = 1L;             /* Value of __cplusplus for C++ */
    long    stdc_ver = 0L;          /* Value of __STDC_VERSION__    */
D09 int     stdc_val = STDC;        /* Value of __STDC__            */
    int     stdc2;                  /* cplus || stdc_ver >= 199901L */
#endif
D09 int     std_line_prefix = STD_LINE_PREFIX;
            /* Output line and file information in C source style   */

#if MODE >= STANDARD
/*
 * Translation limits specified C90, C99 or C++.
 */
    int     slenmin;                /* Least maxmum of string len.  */
    int     idlenmin;               /* Least maximum of ident len.  */
    int     nmacparsmin;            /* Least maximum of num of pars.*/
    int     exp_nestmin;            /* Least maximum of expr nest   */
    int     blk_nestmin;            /* Least maximum of block nest  */
    int     inc_nestmin;            /* Least maximum of include nest*/
    int     nmacromin;          /* Least maximum of num of macros   */

    long    line_limit;             /* Maximum source line number   */
#endif

/*
 * Commonly used global variables:
 * line         is the current input line number.
 * wrongline    is set in many places when the actual output line is out of
 *              sync with the numbering, e.g, when expanding a macro with an
 *              embedded newline.
 * identifier   holds the last identifier scanned (which might be a candidate
 *              for macro expansion).
 * errors       is the running cpp error counter.
 * infile       is the head of a linked list of input files (extended by
 *              #include and macros being expanded).  'infile' always points
 *              to the current file/macro.  'infile->parent' to the includer,
 *              etc.  'infile->fp' is NULL if this input stream is a macro.
 */
D09 long        line;               /* Current line number          */
D09 int         wrongline;          /* Force #line to compiler      */
    int         errors = 0;         /* Cpp error counter            */
D09 int         warn_level = -1;    /* Level of warning (have to initialize)*/
D09 FILEINFO *  infile = NULL;      /* Current input file           */
    int         include_nest;       /* Nesting level of #include    */
D09 char        identifier[ IDMAX + IDMAX/8];       /* Current identifier   */
#if DEBUG || DEBUG_EVAL
D09 int         debug = 0;          /* != 0 if debugging now        */
#endif

/*
 *   in_directive is set TRUE while a control line is scanned by control().
 * It modifies the behavior of squeeze_ws() in expand.c so that newline is
 * not skipped even if getting macro arguments.
 */
D09 int     in_directive = FALSE;   /* TRUE scanning control line   */
D09 int     in_define = FALSE;      /* TRUE scanning #define line   */
#if MODE < STANDARD
D09 long    in_asm = 0L;    /* Starting line of #asm - #endasm block*/
#endif

/*
 *   macro_line is set to the line number of start of a macro call while
 * expanding the macro, else set to 0.  Line number is remembered for
 * diagnostics of unterminated macro call.  On unterminated macro call
 * macro_line is set to MACRO_ERROR.
 */
D09 long    macro_line = 0L;

/*
 * openum is the return value of scanop() in support.c.
 */
D09 int     openum;

/*
 *   mkdep means to output source file dependency line, specified by -M*
 * option.  The OR of the following values is used.
 *      MD_MKDEP    (1) :   Output dependency line.
 *      MD_SYSHEADER(2) :   Print also system headers or headers with
 *          absolute path not only user headers.
 *      MD_FILE     (4) :   Output to the file named *.d instead of stdout.
 *          Normal output is done to stdout as usual.
 */
#if OK_MAKE
    int     mkdep = 0;
#endif

/*
 *   If iflag is TRUE, no_output is incremented when a file is #included,
 * and decremented when the file is finished.
 * If no_output is larger than 0, processed files are not output, meanwhile
 * the macros in the files are defined.
 *   If mkdep != 0 && (mkdep & MD_FILE) == 0, no_output is set to 1 initially.
 */
D09 int     no_output = 0;

/*
 * keepcomments is set TRUE by the -C option.  If TRUE, comments are written
 * directly to the output stream.  This is needed if the output from cpp is
 * to be passed to lint (which uses commands embedded in comments).  cflag
 * contains the permanent state of the -C flag.  keepcomments is always
 * falsified when compilation is supressed by a false #if or when no_output
 * is TRUE.
 */
D09 int     keepcomments;               /* Write out comments flag  */

/*
 * ifstack[] holds information about nested #if's.  It is always accessed via
 * ifptr->stat.  The information is as follows:
 *      WAS_COMPILING   state of compiling flag at outer level.
 *      ELSE_SEEN       set TRUE when #else seen to prevent 2nd #else.
 *      TRUE_SEEN       set TRUE when #if or #elif succeeds
 * ifstack[0].stat holds the compiling flag.  It is WAS_COMPILING if compila-
 * tion is currently enabled.  Note that this must be initialized to
 * WAS_COMPILING.
 */
    IFINFO      ifstack[ BLK_NEST + 1] = { {WAS_COMPILING, 0L, 0L}, };
                /* Note: '+1' is necessary for the initial state.   */
D09 IFINFO *    ifptr = ifstack;        /* -> current ifstack[]     */

#if MODE == POST_STANDARD
/* insert_sep is set to INSERT_SEP when :
 *  1. the next get() shall insert a token separator.
 *  2. unget() has been called when insert_sep == INSERTED_SEP.
 * set to INSERTED_SEP when :
 *  get() has been called when insert_sep == INSERT_SEP.
 * set to NO_SEP when :
 *  get() has been called when insert_sep == INSERTED_SEP.
 */
D09 int     insert_sep = NO_SEP;
#endif

#if MODE >= STANDARD
/* has_pragma is set to TRUE so as to execute _Pragma() operator when the
 * psuedo macro _Pragma() is found.
 */
D09 int     has_pragma = FALSE;
#endif

/*
 * work[] and workp are used to store one piece of text in a temporary buffer.
 * To initialize storage, set workp = work.  Note that the work buffer is used
 * by several subroutines -- be sure that your data won't be overwritten.
 * work[] is used for:
 *      1. temporary buffer in macro expansion (exp_special(), expand(),
 *         catenate())
 *      2. temporary buffer in processing control line (other than #define).
 */
    char        work[ NWORK + IDMAX];   /* Work buffer              */
D09 char *      workp;                  /* Pointer into work[]      */
#ifdef __cplusplus
extern char * const     work_end = & work[ NWORK];
#else
D09 char * const    work_end = & work[ NWORK];
                                        /* End of buffer work[]     */
#endif


#if PROTO

static void     addfile( FILE * fp, const char * filename);
static void     initdefines( void);
static void     cppmain( void);
static void     putout( char * out);
static void     putline( const char * out);
#if MODE >= STANDARD && OK_PRAGMA_OP
static void     do_pragma_op( void);
static void     put_seq( char * begin, char * seq);
static char *   de_stringize( char * in, char * out);
#endif
#if NWORK < NMACWORK
static void     devide_line( void);
#endif
#if CONCAT_STRINGS || (MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS)    \
        || (BSL_IN_MBCHAR && ESC_MBCHAR) || !HAVE_ALERT
static int      post_preproc( char * out);
#endif
#if ! HAVE_ALERT
static char *   conv_esc( char * cp);
#endif
#if CONCAT_STRINGS
static int      is_last_esc( char * cp);
static char *   conv2oct( char * in, char * out);
#endif
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
static char *   conv_a_digraph( char * cp);
#endif
#if BSL_IN_MBCHAR && ESC_MBCHAR
static char *   esc_mbchar( char * str, char * str_end);
#endif

#else   /* ! PROTO  */

static void     addfile();      /* Chain the included file          */
static void     initdefines();  /* Predefine macros                 */
static void     cppmain();      /* Main loop to process input lines */
static void     putout();       /* May concatenate adjacent string  */
static void     putline();      /* Put out the processed line       */
#if MODE >= STANDARD && OK_PRAGMA_OP
static void     do_pragma_op(); /* Exucute the _Pragma() operator   */
static void     put_seq();      /* Put out the failed sequence      */
static char *   de_stringize(); /* "De-stringize" for _Pragma() op. */
#endif
#if NWORK < NMACWORK
static void     devide_line();  /* Devide long line for compiler    */
#endif
#if CONCAT_STRINGS || (MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS)
static int      post_preproc(); /* Post-preprocess for older comps  */
#else
#if (BSL_IN_MBCHAR && ESC_MBCHAR) || !HAVE_ALERT
static int      post_preproc(); /* Post-preprocess for older comps  */
#endif
#endif
#if ! HAVE_ALERT
static char *   conv_esc();     /* Convert '\a' and '\v' to octals  */
#endif
#if CONCAT_STRINGS
static int      is_last_esc();  /* String end with a hex or octal ? */
static char *   conv2oct();     /* Convert hexdigit char to octal   */
#endif
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
static char *   conv_a_digraph();   /* Convert a digraph in place   */
#endif
#if BSL_IN_MBCHAR && ESC_MBCHAR
static char *   esc_mbchar();   /* Insert \ before 2nd byte of SJIS */
#endif

#endif

int
#if PROTO
preproc_main( int argc, char ** argv)
#else
preproc_main( argc, argv)
    int     argc;
    char ** argv;
#endif
{
    char *  in_file = NULL;
    char *  out_file = NULL;

    argc = getredirection( argc, argv);     /* vms >file and <file  */
    initdefines();                          /* Predefine macros     */
    dooptions( argc, argv, &in_file, &out_file);    /* Command line options */

    /* Open input file, "-" means stdin.    */
    if (in_file != NULL && ! str_eq( in_file, "-")) {
        if (freopen( in_file, "r", stdin) == NULL) {
            fprintf( stderr, "Can't open input file \"%s\".\n", in_file);
            exit( IO_ERROR);
        }
        strcpy( work, in_file);         /* Remember input filename  */
    } else {
        setstdin( work);                    /* Just get stdin       */
    }
    /* Open output file, "-" means stdout.  */
    if (out_file != NULL && ! str_eq( out_file, "-")) {
        if (! reopen_stdout( out_file)) {
            fprintf( stderr, "Can't open output file \"%s\".\n", out_file);
            exit( IO_ERROR);
        }
    }
    if (qflag) {                            /* Redirect diagnostics */
        if (freopen( "cpp.err", "a", stderr) == NULL) {
            fprintf( stdout, "Can't open \"cpp.err\"\n");
            exit( IO_ERROR);
        }
    }
    addfile( stdin, work);          /* "open" main input file       */
#if OK_MAKE
    if (mkdep)
        put_depend( work);          /* Putout target file name      */
#endif

    cppmain();                      /* Process main file            */

#if OK_MAKE
    if (mkdep)
        put_depend( NULLST);    /* Append '\n' to dependency line   */
#endif
#if DEBUG
    if (debug & MEMORY)
        print_heap();
#endif
    if (errors > 0) {
        fprintf( stderr, "%d error%s in preprocessor.\n",
                errors, (errors == 1) ? "" : "s");
        if (! eflag)
            exit( IO_ERROR);
    }
    exit( IO_SUCCESS);              /* No errors or -E option set   */
    return  IO_SUCCESS;             /* Never reach here             */
}

void
#if PROTO
curfile( void)
#else
curfile()
#endif
/*
 * Output a current source file name.
 */
{
    static char *   sharpfilename = NULL;
    register char *     name;

    if (infile->fp != NULL) {
        name = infile->filename;
        if (sharpfilename == NULL || ! str_eq( name, sharpfilename)) {
            if (sharpfilename != NULL)
                free( sharpfilename);
            sharpfilename = savestring( name);
            putfname( name);
        }
    }
}

static void
#if PROTO
addfile( FILE * fp, const char * filename)
#else
addfile( fp, filename)
    FILE *      fp;                         /* Open file pointer    */
    char *      filename;                   /* Name of the file     */
#endif
/*
 * Initialize tables for this open file.  This is called from openfile()
 * below (for #include files), and from the entry to cpp to open the main
 * input file.  It calls a common routine getfile() to build the FILEINFO
 * structure which is used to read characters.
 */
{
    register FILEINFO *     file;

#if MODE >= STANDARD
    if ((warn_level & 2) && include_nest == inc_nestmin + 1)
            cwarn( "More than %.0s%ld nesting of #include"  /* _W2_ */
                    , NULLST , (long) inc_nestmin , NULLST);
#endif

    file = getfile( filename, (size_t) NBUFF);  /* file == infile   */
    file->fp = fp;                      /* Better remember FILE *   */
    include_nest++;
}

int
#if PROTO
openfile( const char * filename, int local)
#else
openfile( filename, local)
    char *      filename;
    int         local;
#endif
/*
 * Open a file, add it to the linked list of open files, close the includer
 * if nessesary and truncate the includer's buffer.
 * This is called only from openinclude() in system.c.
 */
{
#if HOST_COMPILER == TURBOC
    /* Borland's fopen() fails to set errno to EMFILE.  */
    static int  max_open = 15;
#else
    static int  max_open;
#endif
    int         len;
    FILEINFO *  file = infile;
    register FILE *     fp;

    if ((max_open != 0 && max_open <= include_nest)
                            /* Exceed the known limit of open files */
            || ((fp = fopen( filename, "r")) == NULL && errno == EMFILE)) {
                            /* Reached the limit for the first time */
        /*
         * Table of open files is full.
         * Remember the file position and close the includer.
         * The state will be restored by getline() on end of the included.
         */
        file->pos = ftell( file->fp);
        fclose( file->fp);
        /* In case of failure, re-open the includer */
        if ((fp = fopen( filename, "r")) == NULL) {
            file->fp = fopen( file->filename, "r");
            fseek( file->fp, file->pos, SEEK_SET);
            return  FALSE;
        }
        if (max_open == 0)      /* Remember the limit of the system */
            max_open = include_nest;
    } else if (fp == NULL) {    /* No file, illegal path name or so */
        return  FALSE;
    }
    /* Truncate buffer of the includer to save memory   */
    len = (int) (file->bptr - file->buffer);
    file->buffer = xrealloc( file->buffer, len + 1);
    file->bptr = file->buffer + len;

#if OK_MAKE
    if (mkdep && ((mkdep & MD_SYSHEADER) || local))
        put_depend( filename);          /* Output dependency line   */
#endif
    addfile( fp, filename); /* Add the file-info to the linked list */

    if (iflag) {
        no_output++;        /* Don't output the included file       */
    } else {
        line = 1;                       /* Working on line 1 now    */
        sharp();            /* Print out the included file name     */
    }
    line = 0;                           /* To read the first line   */
    return  TRUE;
}

/*
 * This is the table used to predefine target machine and operating
 * system designators.  It may need hacking for specific circumstances.
 * The -N option supresses preset definitions.
 */
static const char *     preset[] = {

#ifdef  CPU_OLD
        CPU_OLD,
#endif
#ifdef  SYSTEM_OLD
        SYSTEM_OLD,
#endif
#ifdef  CMP_OLD
        CMP_OLD,
#endif

        NULL,           /* End of macros beginning with alphabet    */

#ifdef  CPU_STD
        CPU_STD,
#endif
#ifdef  SYSTEM_STD
        SYSTEM_STD,
#endif

        NULL,                   /* End of macros with value of 1    */

#ifdef  SYSTEM_EXTRA
        SYSTEM_EXTRA,
#endif
#ifdef  CMP_STD
        CMP_STD,
#endif
#ifdef  CMP_EXTRA
        CMP_EXTRA,
#endif
#if     MODE >= STANDARD
#ifdef  CMP_CPLUS
        CMP_CPLUS,
#endif
#endif
        NULL,           /* End of macros with value of any integer  */
};

static void
#if PROTO
initdefines( void)
#else
initdefines()
#endif
/*
 * Initialize the built-in #define's.  There are two flavors:
 *      #define __decus_cpp     2           (static definitions)
 *      #define __FILE__        ??          (dynamic, evaluated by magic)
 * Called only on cpp startup.
 *
 * Note: the built-in static definitions are supressed by the -N option,
 * definitions beginning with alphabet are supressed by the -S1 option,
 * __LINE__, __FILE__, __DATE__, __TIME__, __STDC__ or __cplusplus,
 * __STDC_VERSION__ and __decus_cpp are always present (if defined).
 */
{
#if MODE != CON_FALSE
#if MODE >= STANDARD
    char    tmp[ 16];
#endif
    char    timestr[ 14];
    time_t  tvec;
    char *  tstring;
#endif
    int     n = sizeof preset / sizeof (char *);
    int     nulls = 0;
    const char * const  null = "";
    register const char * const *   pp;

    /* Predefine the built-in symbols.  */
    for (pp = preset; pp < preset + n; pp++) {
        if (*pp) {
            if (**pp)
                install( *pp, DEF_NOARGS - 1, null, "1");
        } else {
            if (++nulls == 2)
                break;
        }
    }

#ifdef  SYSTEM_EXTRA
    if (**++pp)
        install( *pp, DEF_NOARGS - 1, null, SYS_EXT_VAL);
#endif
#ifdef  CMP_STD
    if (**++pp)
        install( *pp, DEF_NOARGS - 1, null, CMP_STD_VAL);
#endif
#ifdef  CMP_EXTRA
    if (**++pp)
        install( *pp, DEF_NOARGS - 1, null, CMP_EXT_VAL);
#endif
#if     MODE >= STANDARD
#ifdef  CMP_CPLUS
    if (**++pp)
        install( *pp, DEF_NOARGS - 1, null, CMP_CPLUS_VAL);
#endif
#endif

#if MODE != CON_FALSE
/*
 * The magic pre-defines (Standard predefined macros) are initialized with
 * negative argument counts.  expand() notices this and calls the appropriate
 * routine.  DEF_NOARGS is one greater than the first "magic" definition.
 * 'DEF_NOARGS - n' is reserved for the pre-defined macros.
 */
    install( "__LINE__", DEF_NOARGS - 3, null, "00000");
                                            /* Room for 5 digits    */
    install( "__FILE__", DEF_NOARGS - 4, null, null);
                                            /* Should be stuffed    */

    /*
     * Define __DATE__, __TIME__ as present date and time.
     */
    time( &tvec);
    tstring = ctime( &tvec);
    sprintf( timestr, "\"%.3s %c%c %.4s\"",
        tstring + 4,
        *(tstring + 8) == '0' ? ' ' : *(tstring + 8),
        *(tstring + 9),
        tstring + 20);
    install( "__DATE__", DEF_NOARGS - 2, null, timestr);
    sprintf( timestr, "\"%.8s\"", tstring + 11);
    install( "__TIME__", DEF_NOARGS - 2, null, timestr);

#if MODE >= STANDARD
/* Define __STDC__ to 1 or such for Standard conforming compiler.   */
    sprintf( tmp, "%d", STDC);
    install( "__STDC__", DEF_NOARGS - 2, null, tmp);
    sprintf( tmp, "%ldL", STDC == 1 ? STDC1_VERSION
            : STDC == 2 ? STDC2_VERSION : 0L);
    install( "__STDC_VERSION__", DEF_NOARGS - 2, null, tmp);
#endif  /* MODE >= STANDARD */
/* Not define __STDC__ for pre-Standard compiler.   */
#endif  /* MODE != CON_FALSE    */

    install( "__decus_cpp", DEF_NOARGS - 1, null, "2"); /* DECUS CPP V.2.x  */
    /* This macro is predefined yet can be undefined by -U or #undef.   */
}

void
#if PROTO
unpredefine( int clearall)
#else
unpredefine( clearall)
    int     clearall;                       /* TRUE for -N option   */
#endif
/*
 * Remove predefined symbols from the symbol table.
 */
{
    register const char * const *   pp;
    int     n = sizeof preset / sizeof (char *);

    for (pp = preset; pp < preset + n; pp++) {
        if (*pp) {
            if (**pp && lookid( *pp) != NULL)
                undefine( *pp);
        } else if (clearall == FALSE) {     /* -S<n> option         */
            break;
        }
    }
}

void
#if PROTO
undef_a_predef( const char * name)
#else
undef_a_predef( name)
    char *  name;
#endif
/*
 * Remove a predefined name from the preset[] table so that the name can be
 * redefined by -D option.
 * The strange ordering (insert, command-line-scan, remove)
 * is needed to avoid interaction with -D arguments.
 */
{
    register const char **      pp;
    int     n = sizeof preset / sizeof (char *);

    for (pp = preset; pp < preset + n; pp++) {
        if (*pp && **pp && str_eq( *pp, name)) {
            *pp = "";
            break;
        }
        pp++;
    }
}

/*
 * These definitions are used by the routines of string concatenation.
 */
#if CONCAT_STRINGS
#define DONE            0
#define SUSPEND         1
#define FLUSH           (-1)
#endif

/*
 * output[] and out_ptr are used for:
 *      buffer to store preprocessed line (this line is put out or handed to
 *      post_preproc() via putout() for string concatenation)
 */
static char     output[ NMACWORK];  /* Buffer for preprocessed line */
static D09 char * const out_end = & output[ NWORK - 2];
                                    /* Limit of output line         */
static D09 char * const out_wend = & output[ NMACWORK - 2];
                                    /* Buffer end of output line    */
static D09 char *       out_ptr;    /* Current pointer into output[]*/
#if CONCAT_STRINGS
static char     catbuf[ NWORK + IDMAX + 4]; /* Buffer for string catenation */
static D09 char *       catbufp = catbuf;   /* Pointer into catbuf[]*/
static D09 char * const catbuf_end = & catbuf[ NWORK + IDMAX];
static D09 int          catflag;    /* Flag returned from post_preproc()    */
#endif

static void
#if PROTO
cppmain( void)
#else
cppmain()
#endif
/*
 * Main process for cpp -- copies tokens from the current input stream
 * (main file or included file) to the output file.
 */
{
    register int    c;              /* Current character            */
    int     newlines;               /* Blank lines and control lines*/
    char *  wp;                     /* Temporary pointer            */
    DEFBUF *    defp;               /* Macro definition             */

    if (! no_output)
        put_start_file( work);
    keepcomments = cflag && !no_output;

    /*
     * This loop is started "from the top" at the beginning of each line.
     * 'wrongline' is set TRUE in many places if it is necessary to write
     * a #line record.  (But we don't write them when expanding macros.)
     *
     * 'newlines' variable counts the number of blank lines that have been
     * skipped over.  These are then either output via #line records or
     * by outputting explicit blank lines.
     */
    while (1) {                             /* For the whole file   */
        newlines = 0;                       /* Count empty lines    */

        while (1) {                         /* For each line, ...   */
            c = get();                      /* First of the line    */
            out_ptr = output;               /* Top of the line buf  */
#if TOP_SPACE
            if (c == ' ') {
                *out_ptr++ = ' ';           /* Retain a space       */
                c = get();          /* First of token (else '\n')   */
            }
#if COMMENT_INVISIBLE
            if (c == COM_SEP)               /* Skip 0-length comment*/
                 c = get();
#endif
            /* Else line top spaces are already skipped */
#endif
            if (c == '#') {                 /* Is 1st non-space '#' */
                newlines = control( newlines);      /* Do a #command*/
#if MODE == STANDARD && OK_DIGRAPHS
            } else if (digraphs && c == '%') {
                if (get() == ':') {  /* '%:' i.e. '#'*/
                    newlines = control( newlines);  /* Do a #command*/
                } else {
                    unget();
                    if (! compiling) {
                        skipnl();
                        newlines++;
                    } else {
                        break;
                    }
                }
#endif
            } else if (c == CHAR_EOF) {     /* End of input         */
                break;
            } else if (! compiling) {       /* #ifdef false?        */
                skipnl();                   /* Skip to newline      */
                newlines++;                 /* Count it, too.       */
#if MODE < STANDARD
            } else if (in_asm && ! no_output) { /* In #asm block    */
                put_asm();                  /* Put out as it is     */
#endif
            } else if (c == '\n') {         /* Blank line           */
                if (keepcomments)
                    fputc( '\n', stdout);   /* May flush comments   */
                else
                    newlines++;             /* Wait for a token     */
            } else {
                break;                      /* Actual token         */
            }
        }

        if (c == CHAR_EOF)                  /* Exit process at      */
            break;                          /*   end of input       */

        /*
         * If the loop didn't terminate because of end of file, we
         * know there is a token to compile.  First, clean up after
         * absorbing newlines.  newlines has the number we skipped.
         */
#if CONCAT_STRINGS
        if (catflag != SUSPEND)
#endif
        {
            if (no_output) {
                wrongline = FALSE;
            } else {
                if (wrongline || newlines > 10) {
                    sharp();                /* Output # line number */
                } else {                    /* If just a few, stuff */
                    while (newlines-- > 0)  /* them out ourselves   */
                        fputc('\n', stdout);
                }
            }
        }

        /*
         * Process each token on this line.
         */
        while (c != '\n' && c != CHAR_EOF) {    /* For the whole line   */
            if (scantoken( c, (wp = out_ptr, &wp), out_wend) == NAM
                    && (defp = is_macro( &wp)) != NULL) {   /* A macro  */
                wp = expand( defp, out_ptr, out_wend);
                                            /* Expand it completely */
#if MODE >= STANDARD && OK_PRAGMA_OP
                if (has_pragma) {           /* Found _Pramga()      */
                    do_pragma_op();         /* Do _Pragma() operator*/
                    has_pragma = FALSE;     /* Reset signal         */
                    out_ptr = output;       /* Do the rest of line  */
                    wrongline = TRUE;       /* Line-num out of sync */
                } else
#endif
                {
                    out_ptr = wp;
                }
            } else {                        /* Not a macro call     */
                out_ptr = wp;               /* Advance the place    */
            }
#if NWORK < NMACWORK
            if (out_end <= out_ptr)         /* Too long line        */
                devide_line();              /* Devide long line     */
#endif
            if ((c = get()) == ' ') {       /* Token separator      */
                *out_ptr++ = ' ';
                c = get();                  /* First of token       */
            }
#if COMMENT_INVISIBLE
            if (c == COM_SEP)
                c = get();                  /* Skip 0-length comment*/
#endif
        }                                   /* Line for loop        */

        putout( output);                    /* Output the line      */
    }                                       /* Continue until EOF   */

#if CONCAT_STRINGS
    if (catflag == SUSPEND)                 /* Input end with string*/
        putline( catbuf);
#endif
}

#if MODE >= STANDARD && OK_PRAGMA_OP

static void
#if PROTO
do_pragma_op( void)
#else
do_pragma_op()
#endif
/*
 * Execute the _Pragma() operator contained in an expanded macro.
 * Note: _Pragma() operator is also implemented as a special macro.  Therefore
 *      it is always searched as a macro.
 * There might be more than one _Pragma() in a expanded macro and those may be
 *      surrounded by other token sequences.
 * Since all the macros have been expanded completely, any name identical to
 *      macro should not be re-expanded.
 */
{
    FILEINFO *  file;
    DEFBUF *    defp;
    int     prev = output < out_ptr;        /* There is a previous sequence */
    int     token_type;
    char *  cp1, * cp2;
    register int    c;

    file = ungetstring( out_ptr, NULLST);
    while (c = get(), file == infile) {
        if (c == ' ') {
            *out_ptr++ = ' ';
            continue;
        }
        if ((token_type = scantoken( c, (cp1 = out_ptr, &cp1), out_wend))
                    == NAM && (defp = is_macro( &cp1)) != NULL
                && defp->nargs == DEF_PRAGMA) {     /* _Pragma() operator   */
            if (prev) {
                putout( output);    /* Putout the previous sequence */
                sharp();
                cp1 = stpcpy( output, "pragma ");   /* From top of buffer   */
            }
            *cp1++ = get();                                 /* '('  */
            while ((c = get()) == ' ')
                *cp1++ = ' ';
            if (((token_type = scantoken( c, (cp2 = cp1, &cp1), out_wend))
                    != STR && token_type != WSTR)) {
                /* Not a string literal */
                put_seq( output, cp1);
                return;
            }
            workp = de_stringize( cp2, work);
            while ((c = get()) == ' ')
                *cp1++ = ' ';
            if (c != ')') {         /* More than a string literal   */
                unget();
                put_seq( output, cp1);
                return;
            }
            strcpy( workp, "\n");       /* Terminate with <newline> */
            ungetstring( work, NULLST);
            dopragma();                 /* Do the #pragma "line"    */
            infile->bptr += strlen( infile->bptr);      /* Clear sequence   */
            sharp();
            cp1 = out_ptr = output;     /* From the top of buffer   */
            prev = FALSE;
        } else {                        /* Not pragma sequence      */
            out_ptr = cp1;
            prev = TRUE;
        }
    }
    unget();
    if (prev)
        putout( output);
    sharp();
}

static void
#if PROTO
put_seq( char * begin, char * seq)
#else
put_seq( begin, seq)
    char *  begin;                  /* Sequence already in buffer   */
    char *  seq;                    /* Sequence to be read          */
#endif
/* Put out the failed sequence as it is.    */
{
    FILEINFO *  file = infile;
    register int    c;

    cerror( "Operand of _Pragma() is not a string literal"  /* _E_  */
            , NULLST, 0L, NULLST);
    while (c = get(), file == infile)
        *seq++ = c;
    unget();
    out_ptr = seq;
    putout( begin);
}

static char *
#if PROTO
de_stringize( char * in, char * out)
#else
de_stringize( in, out)
    char *  in;                 /* Null terminated string literal   */
    char *  out;                            /* Output buffer        */
#endif
/*
 * Make token sequence from a string literal for _Pragma() operator.
 */
{
    char *  in_p;
    int     c1;
    register int   c;

    in_p = in;
    if (*in_p == 'L')
        in_p++;                             /* Skip 'L' prefix      */
    while ((c = *++in_p) != EOS) {
        if (c == '\\' && ((c1 = *(in_p + 1), c1 == '\\') || c1 == '"'))
            c = *++in_p;            /* "De-escape" escape sequence  */
        *out++ = c;
    }
    *--out = EOS;                   /* Remove the closing '"'       */
    return  out;
}

#endif  /* MODE >= STANDARD && OK_PRAGMA_OP */

#if NWORK < NMACWORK
static void
#if PROTO
devide_line( void)
#else
devide_line()
#endif
/*
 * Devide a too long line into output lines shorter than NWORK.
 * This routine is called prior to post_preproc() to save size of catbuf[]
 * for 8-bits systems.
 */
{
    FILEINFO *  file;
    char *  save;
    char *  wp;
    int     token_type;
    register int    c;

    file = ungetstring( output, NULLST);    /* To re-read the line  */
    wp = out_ptr = output;
#if ! TOP_SPACE
    c = skipws();
    unget();
#endif

    while ((c = get()), file == infile) {
        if (c == ' ') {
            *out_ptr++ = ' ';
            wp++;
            continue;
        }
        token_type = scantoken( c, &wp, out_wend);  /* Read a token */
        if (NWORK-1 <= wp - out_ptr) {          /* Too long a token */
            cfatal( "Too long token %s", out_ptr, 0L, NULLST);      /* _F_  */
        } else if (out_end <= wp || token_type == STR
#if MODE >= STANDARD
                    || token_type == WSTR
#endif
                ) {
            /* Too long line or string literal (maybe concatenated later)   */
            save = savestring( out_ptr);        /* Save the token   */
            while (*(out_ptr - 1) == ' ')
                out_ptr--;              /* Remove trailing spaces   */
            putout( output);            /* Putout the former tokens */
#if CONCAT_STRINGS
            if (catflag != SUSPEND)
                sharp();
            else
                wrongline = TRUE;
#else
            sharp();                        /* Correct line number  */
#endif
            wp = out_ptr = stpcpy( output, save);   /* Restore the token    */
            free( save);
        } else {                            /* Still in size        */
            out_ptr = wp;                   /* Advance the pointer  */
        }
    }

    unget();
}
#endif

static void
#if PROTO
putout( char * out)
#else
putout( out)
    char *  out;    /* Output line (line-end is always 'out_ptr')   */
#endif
/*
 * Put out a line with or without "post-preprocessing".
 */
{
    *out_ptr++ = '\n';                      /* Put out a newline    */
    *out_ptr = EOS;

#if CONCAT_STRINGS
    do {                                    /* String concatenation */
        catflag = post_preproc( out);
        switch (catflag) {
        case DONE   :               /* The line has been done       */
        case FLUSH  :               /* The line has been suspended  */
            putline( catbuf);               /* Put out the line     */
            catbufp = catbuf;               /* This line has done   */
            break;
        case SUSPEND:
            catbufp = NULL;                 /* Suspend the line     */
            break;
        }
    } while (catflag == FLUSH);     /* Process the current line     */
#else
#if (BSL_IN_MBCHAR && ESC_MBCHAR) || !HAVE_ALERT
    post_preproc( out);
#else
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
    if (digraphs)
        post_preproc( out);
#endif
#endif
    putline( out);                          /* No post-preprocess   */
#endif
}

static void
#if PROTO
putline( const char * out)
#else
putline( out)
    char *  out;
#endif
/*
 * Finally put out the preprocessed line.
 */
{
    if (no_output)
        return;
    if (NWORK <= strlen( out))
        cfatal( "Too long output line %s", out, 0L, NULLST);        /* _F_  */
    if (fputs( out, stdout) == EOF)
        cfatal( "File write error", NULLST, 0L, NULLST);    /* _F_  */
}


/*
 *      Routines to  P O S T - P R E P R O C E S S
 *
 * 1998/08      created     Psycho
 *    Supplementary phase for the older compiler-propers.
 *      1. Convert digraphs to usual tokens.
 *      2. Convert '\a' and '\v' to octal escape sequences.
 *      3. Concatenate string literals.
 *      4. Double '\\' of the second byte of SJIS multi-byte characters.
 *    These conversions are done selectively according to the macros defined
 *  in system.H.
 *      1. Digraphs are converted if MODE == STANDARD && CONV_DIGRAPHS == TRUE
 *  and digraph recoginition is enabled by DIGRAPHS_INIT and/or -2 option on
 *  execution.
 *      2. Convert '\a', '\v' in string literal or character constant to the
 *  values defined by ALERT_STR, VT_STR (provided HAVE_ALERT == FALSE).
 *      3. String concatenation is done if CONCAT_STRINGS == TRUE, through the
 *  following phases in this order.
 *      3.1. When a character string literal is succeded by another character
 *  string literal or a wide string literal is succeded by another wide string
 *  literal, and the former literal ends with a octal or hexadecimal escape
 *  sequence and the latter literal starts with a hexa-decimal-digit
 *  character, convert the latter character to 3-digits octal escape sequence.
 *      3.2. Concatenate adjacent character string literals, concatenate
 *  adjacent wide string literals.  (Adjacency of character string literal
 *  and wide string literal is an error.)
 *      4. '\\' of the second byte of SJIS is doubled if ESC_MBCHAR == TRUE.
 */

/*
 * 1998/09      Psycho
 *      Updated post_preproc() to concatenate wide-character-string and
 *          multibyte-character-string generating a wide-character-string.
 */

#if ! CONCAT_STRINGS

#if (MODE != STANDARD || !OK_DIGRAPHS || !CONV_DIGRAPHS)    \
        && ! (BSL_IN_MBCHAR && ESC_MBCHAR) && HAVE_ALERT
    /* No post_preproc()    */
#else

static int
#if PROTO
post_preproc( char * out)
#else
post_preproc( out)
    char *  out;
#endif
/*
 * Convert digraphs and double '\\' of the second byte of SJIS.
 */
{
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
    int     di_count = 0;
#endif
    int     token_type;
    register int    c;
    char *  str;
    char *  cp = out;

    ungetstring( out, NULLST);
    while ((c = get()) != '\n') {   /* Not to read over to next line    */
        if (c == ' ') {
            *cp++ = ' ';
            continue;
        }
        str = cp;
        token_type = scantoken( c, &cp, out_end);
        switch (token_type) {
#if (BSL_IN_MBCHAR && ESC_MBCHAR) || ! HAVE_ALERT
#if MODE >= STANDARD
        case WSTR   :
        case WCHR   :
            str++;                          /* Skip prefix 'L'      */
            /* Fall through */
#endif
        case STR    :
        case CHR    :
#if ! HAVE_ALERT
            cp = conv_esc( str);
#endif
            cp = esc_mbchar( str, cp);
            break;
#endif
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
        case OPE    :
            if (openum & OP_DIGRAPH) {
                cp = conv_a_digraph( cp);   /* Convert a digraph    */
                di_count++;
            }
#endif
        }
    }
    *cp++ = '\n';
    *cp = EOS;
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
    if (di_count && (warn_level & 8))
        cwarn( "%.0s%ld digraph(s) converted"               /* _W8_ */
                , NULLST, (long) di_count, NULLST);
#endif
    return  0;
}

#endif

#else   /* CONCAT_STRINGS   */

static int
#if PROTO
post_preproc( char * out)
#else
post_preproc( out)
    char *  out;
#endif
/*
 * Concatenate adjacent character string literals, concatenate adjacent wide
 * string literals.
 */
{
    static D09 char *   catenated = "String literals %s are concatenated";
    static D09 char *   catp;           /* Current output pointer   */
    static D09 char *   prev_token;     /* Previous token pointer   */
    static D09 int      prev_type = 0;  /* Type of previous token   */
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
    int     di_count = 0;               /* Count of digraphs        */
#endif
    int     c;                          /* Value of the character   */
    int     token_type;                 /* Type of current token    */
    char *  token;                      /* Top of current token     */
    register char *     cp;

    if (catbufp)                        /* Output newer line        */
        catp = catbufp;
    if (catflag != FLUSH) {
#if DEBUG
        if (debug & TOKEN)
            dumpstring( "catstr enter", out);
#endif
        ungetstring( out, NULLST);      /* To re-read               */
    }

    while ((c = get()) != '\n') {
        if (c == ' ') {
            *catp++ = ' ';
            continue;
        }
        token = cp = catp;
        token_type = scantoken( c, &catp, catbuf_end);

        switch (token_type) {
#if MODE >= STANDARD
        case WCHR   :   cp++;           /* Wide-character constant  */
            /* Fall through */
#endif
        case CHR    :                   /* Character constant       */
#if ! HAVE_ALERT
            catp = conv_esc( cp);
#endif
#if BSL_IN_MBCHAR && ESC_MBCHAR
            catp = esc_mbchar( cp, catp);
#endif
            break;
#if MODE >= STANDARD
        case WSTR   :   cp++;           /* Wide string literal      */
            /* Fall through */
#endif
        case STR    :                   /* Character string literal */
            if (prev_type == token_type) {      /* Diagnose prior   */
                if (warn_level & 4)             /*   to conversion. */
                    cwarn( catenated, prev_token, 0L, NULLST);      /* _W4_ */
            }
#if MODE >= STANDARD
            else if (prev_type == WSTR) {
                if (warn_level & 4)
                    cwarn( catenated, prev_token, 0L, NULLST);      /* _W4_ */
                token_type = WSTR;
            }
#endif
#if ! HAVE_ALERT
            catp = conv_esc( cp);
#endif
            if (prev_type == token_type) {      /* Adjacent strings */
                char    *catp1, *catp2;

                catp1 = token - 1;      /* Just before the token    */
                catp2 = cp + 1;         /* To the content of token  */
                while (type[ *catp1 & UCHARMAX] & SPA)
                    --catp1;            /* Back to closing quote    */
                if (is_last_esc( prev_token)) {
                    catp = conv2oct( catp2, catp1);
                } else {
                    memmove( catp1, catp2, (size_t)(catp - cp));
                    catp -= (size_t)(catp2 - catp1);    /* End of string    */
                }
#if MODE >= STANDARD
                if (token_type == WSTR && *prev_token != 'L') {
                    memmove( prev_token + 1, prev_token
                            , (size_t)(catp - prev_token));
                    *prev_token = 'L';
                    catp++;
                }
#endif
                token = prev_token;     /* Preserve prev_token      */
            }
            break;
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
        case OPE    :
            if (openum & OP_DIGRAPH) {                  /* Digraph  */
                catp = conv_a_digraph( catp);
                di_count++;
            }
            break;
#endif
        default :
            break;
        }

#if BSL_IN_MBCHAR && ESC_MBCHAR
        if ((prev_type == STR
#if MODE >= STANDARD
                    || prev_type == WSTR
#endif
                ) && prev_type != token_type)
            catp = esc_mbchar( prev_token, catp);
#endif
        if (catflag == SUSPEND      /* The line has been suspended  */
                && token_type != prev_type) {   /* Next non-string  */
            catflag = FLUSH;        /* To output the pending line   */
            ungetstring( token, NULLST);        /* To re-read later */
            prev_type = 0;          /* To re-read the same token    */
            catp = token;
            while (type[ *(catp - 1) & UCHARMAX] & SPA)
                catp--;
            goto  ret;              /* Output the pending line      */
        }
        prev_type = token_type;     /* Remember the token           */
        prev_token = token;
    }

    if (token_type == STR
#if MODE >= STANDARD
            || token_type == WSTR
#endif
            )
        catflag = SUSPEND;          /* Suspend the line             */
    else
        catflag = DONE;             /* The line has been processed  */
ret:
    *catp++ = '\n';
    *catp = EOS;
#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
    if (di_count && (warn_level & 8))
        cwarn( "%.0s%ld digraph(s) converted"               /* _W8_ */
                , NULLST, (long) di_count, NULLST);
#endif
#if DEBUG
    if (catflag != FLUSH && (debug & TOKEN))
        dumpstring( "catstr exit", NULLST);
#endif
    return  catflag;
}

#if COMPILER == TURBOC
#define MAXOCT  8   /* To work around a bug of tcc, bcc */
#else
#define MAXOCT  3
#endif

static int
#if PROTO
is_last_esc( char * cp)
#else
is_last_esc( cp)
    char *  cp;
#endif
/*
 * Is the last character of the string a hex-or-octal escape sequence ?
 */
{
    int     i;
    register int    c;

#if MODE >= STANDARD
    if (*cp++ == 'L')
#endif
        cp++;                           /* The content of string    */
    while ((c = *cp++ & UCHARMAX) != '"') {
#if BSL_IN_MBCHAR
        if (type[ c] & MB1) {
            if (type[ *cp & UCHARMAX] & MB2)
                cp++;                   /* Not an escape sequence   */
            continue;
        }
#endif
        if (c != '\\')
            continue;

        switch (c = *cp++) {            /* An escape sequence       */
        case 'x' :                  /* Hexadecimal-escape-sequence  */
            while (isxdigit( *cp & UCHARMAX))
                cp++;
            goto  is_last;
        case '0': case '1': case '2': case '3':     /* Octal-escape */
        case '4': case '5': case '6': case '7':     /*  -sequence   */
            for (i = 1; i < MAXOCT && isdigit( *cp & UCHARMAX) && *cp < '8';
                    i++)    /* MAXOCT is bug-to-bug implementation  */
                cp++;
is_last:    if (*cp == '"')             /* String ends with hex or  */
                return  TRUE;           /*   octal escape sequence. */
            break;
        default  :
#if BSL_IN_MBCHAR
            if (type[ c & UCHARMAX] & MB1)
                cp--;
#endif
            break;
        }
    }
    return  FALSE;
}

static char *
#if PROTO
conv2oct( char * cp, char * out)
#else
conv2oct( cp, out)
    char *  cp;
    char *  out;
#endif
/*
 * If the former string ends with hex or oct escape sequence and the current
 * string starts with a hex char, convert the char to octal sequence.
 */
{
    char *  save;
    char *  in;

    in = save = savestring( cp);        /* Content of the quote     */
    if (isxdigit( *in & UCHARMAX)) {
        sprintf( out, "\\%03o", *in++);
        out += 4;
#if MAXOCT > 3
        while (isdigit( *in & UCHARMAX) && *in != 8 && *in != 9) {
            sprintf( out, "\\%03o", *in++);
            out += 4;
        }
#endif
    }
    out = stpcpy( out, in);
    free( save);
    return  out;
}

#endif      /* CONCAT_STRINGS   */

#if ! HAVE_ALERT

#ifdef  ALERT_STR
static const char * const   alert = ALERT_STR;
#endif
#ifdef  VT_STR
static const char * const   vt = VT_STR;
#endif

static char *
#if PROTO
conv_esc( char * cp)
#else
conv_esc( cp)
    char *  cp;
#endif
/*
 * Convert \a, \v to 3 digits octal escape sequence.
 */
{
    char *  in;
    char *  save;
    register int    c;

    in = save = savestring( ++cp);      /* Content of the quote     */

    while ((c = *in++ & UCHARMAX) != EOS) {
        *cp++ = c;
#if BSL_IN_MBCHAR
        if (type[ c] & MB1) {
            if (type[ *in & UCHARMAX] & MB2)
                *cp++ = *in++;          /* Not an escape sequence   */
            continue;
        }
#endif
        if (c != '\\')
            continue;

        switch (c = *in++) {            /* An escape sequence       */
#ifdef  ALERT_STR
        case 'a' :
            cp = stpcpy( cp, alert + 1);
            break;
#endif
#ifdef  VT_STR
        case 'v' :
            cp = stpcpy( cp, vt + 1);
            break;
#endif
        default  :
#if BSL_IN_MBCHAR
            if (type[ c & UCHARMAX] & MB1)
                in--;
            else
#endif
                *cp++ = c;
            break;
        }

    }

    *cp = EOS;
    free( save);
    return  cp;
}
#endif  /* ! HAVE_ALERT */

#if MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS
static char *
#if PROTO
conv_a_digraph( char * cp)
#else
conv_a_digraph( cp)
    char *  cp;                     /* The end of the digraph token */
#endif
/*
 * Convert a digraph to usual token in place.
 */
{
    cp -= 2;
    switch (openum) {
    case OP_LBRACE_D    :
        *cp++ = '{';
        break;
    case OP_RBRACE_D    :
        *cp++ = '}';
        break;
    case OP_LBRCK_D     :
        *cp++ = '[';
        break;
    case OP_RBRCK_D     :
        *cp++ = ']';
        break;
    case OP_SHARP_D     :                       /* Error of source  */
        *cp++ = '#';
        break;
    case OP_DSHARP_D    :                       /* Error of source  */
        cp -= 2;
        *cp++ = '#';
        *cp++ = '#';
        break;
    }
    return  cp;
}
#endif  /* MODE == STANDARD && OK_DIGRAPHS && CONV_DIGRAPHS */

#if BSL_IN_MBCHAR && ESC_MBCHAR
static char *
#if PROTO
esc_mbchar( char * str,  char * str_end)
#else
esc_mbchar( str, str_end)
    char *  str;        /* String literal or character constant without 'L' */
    char *  str_end;    /* The end of the token */
#endif
/*
 * Insert \ before \ of the second byte of the (shift-JIS) MBCHAR code in
 * string literal or character constant.
 * esc_mbchar() does in-place insertion.
 */
{
    int     delim;
    register int    c;

#if MODE >= STANDARD
    if ((delim = *str++) == 'L')
#endif
        delim = *str++;                     /* The quote character  */
    while ((c = *str++ & UCHARMAX) != delim) {
        if (type[ c] & MB1) {           /* The first byte of MBCHAR */
            c = *str & UCHARMAX;
            if (type[ c] & MB2) {           /* The second byte      */
                str++;
                if (c == '\\') {            /* Insert \ before 0x5c */
                    memmove( str, str - 1
                            , (size_t) (str_end - str) + 2);
                    str++;
                    str_end++;
                }
            }
        } else if (c == '\\' && ! (type[ *str & UCHARMAX] & MB1)) {
            str++;                          /* Escape sequence      */
        }
    }
    return  str_end;
}
#endif  /* BSL_IN_MBCHAR && ESC_MBCHAR  */

