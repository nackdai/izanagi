/*
 *                          S Y S T E M . C
 *          S y s t e m   D e p e n d e n t   R o u t i n e s
 *
 * Routines dependent on character set, O.S., compiler or compiler-driver.
 * To implement cpp for the systems not yet implemented, you must
 *      1. specify the constants in "system.H",
 *      2. append the system-dependent routines in this file.
 *
 * Edit history of DECUS CPP / cpp1.c, cpp2.c, cpp3.c
 * 08-Nov-85            Latest revision
 */

/*
 * 24-Jun-88    BM      taught cpp about multiple include directoried in
 *                      INCLUDE environment variable (Blake McBride)
 */

/*
 * 1989/05      Gigo & Adek     Implemented for OS-9 / Microware C/09.
 *      Split sharpsub(), curfile() from sharp().
 *      Created comsrc().
 *      Revised some system-dependent functions.
 */

/*
 * CPP Version 2.0 / system.c
 * 1998/08      Psycho
 *      Created setoptlist(), setcplus(), version(), bsl2sl(), put_depend(),
 *          dopragma(), doonce(), included(), dumppath(), is_junk(),
 *          alloc_mem(), print_heap();
 *      Split reopen_stdout(), setstdin() from main(),
 *          put_start_file(), putfname() from cppmain(),
 *          setfiles(), usage(), mem_model(), is_id() from dooptions(),
 *          doold(), dodebug(), doasm(), put_asm() from control().
 *      Split setincdirs() to setsysdirs(), setenvdirs(), parse_env(),
 *              set_a_dir().
 *      Moved sharp(), sharpsub(), getredirection()
 *              from cpp1.c to system.c,
 *          doinclude(), openinclude(), hasdirectory(), vmsparse()
 *              from cpp2.c to system.c,
 *          setincdirs(), dooptions(), zap_uc() from cpp3.c to system.c,
 *          comsrc() from cpp1.c to system.c (renamed put_source()),
 *      Moved type[] from cpp6.c to system.c.
 *      Revised most of the functions.
 */

/*
 * CPP Version 2.2 / system.c
 * 1998/11      Psycho
 *      Revised several functions.
 *      Removed alloc_mem().
 */

#if PREPROCESSED
#include    "cpp.H"
#else
#include    "system.H"
#include    "internal.H"
#endif

/*
 * PATH_DELIM is defined for the O.S. which has single byte path-delimiter.
 * Note: '\\' or any other character identical to second byte of MBCHAR should
 * not be used for PATH_DELIM for convenience of path-list parsing.
 */
#if SYS_FAMILY == SYS_UNIX || SYS_FAMILY == SYS_OS9 || SYS_FAMILY == SYS_MSDOS
#define PATH_DELIM      '/'
#else
#if SYSTEM == SYS_MAC
#define PATH_DELIM      ':'
#endif
#endif

/*
 * SEARCH_INIT  specifies the default value of 'search_rule'.
 * 'search_rule' holds searching rule of #include "header.h" to search first
 * before searching user specified or system-specific include directories.
 * CURRENT means to search the directory relative to "current directory" which
 * is current at cpp invocation.
 * SOURCE means to search the directory relative to that of the source file
 * (i.e. "includer").
 * 'search_rule' is initialized to SEARCH_INIT.  It can be changed by -I1, -I2
 * or -I3 option.  -I1 specifies CURRENT, -I2 SOURCE and -I3 both.
 */
#if SYS_FAMILY == SYS_UNIX || CMP_FAMILY == GNUC || COMPILER == LSIC
#define SEARCH_INIT     SOURCE
#else
#define SEARCH_INIT     SOURCE
#endif
#define CURRENT         1
#define SOURCE          2

/*
 * EXT_OBJ is the suffix to denote "object" file.
 */
#if SYS_FAMILY == SYS_UNIX || SYSTEM == SYS_DJGPP
#define EXT_OBJ     "o"
#else
#if SYS_FAMILY == SYS_MSDOS
#define EXT_OBJ     "obj"
#else
#if SYS_FAMILY == SYS_OS9
#define EXT_OBJ     "r"
#else
/* Add here appropriate definitions for other systems.  */
#define EXT_OBJ     "o"
#endif
#endif
#endif

#if PROTO

static void     usage( void);
static void     version( void);
static void     setoptlist( char * optlist);
#if MODE >= STANDARD
static void     setcplus( char * val, int nflag);
#endif
static void     setfiles( int argc, char ** argv, char ** in_pp
        , char ** out_pp);
static int      is_id( char * ap);
static void     setsysdirs( void);
#if HAVE_ENV
static void     setenvdirs( void);
static void     parse_env( const char * env);
#endif
static void     set_a_dir( const char * dirname);
static int      openinclude( char * filename, int searchlocal);
static int      hasdirectory( char * source, char * directory);
#if ! OK_IF_JUNK
static int      is_junk( void);
#endif
#if MODE >= STANDARD
static void     doonce( const char * filename);
static int      included( const char * filename);
#endif
#if DEBUG || DEBUG_EVAL
static int      dodebug( int set);
#endif
#if DEBUG
static void     dumppath( void);
#endif
#if MODE < STANDARD && COMPILER == MWC09
static void     doasm( int asm);
#endif

#else   /* ! PROTO  */

static void     usage();        /* Putout usage of CPP              */
static void     version();      /* Putout version message           */
static void     setoptlist();   /* Set list of legal option chars   */
#if MODE >= STANDARD
static void     setcplus();     /* Set the macro __cplusplus        */
#endif
static void     setfiles();     /* Set input, output, diagnostic    */
static int      is_id();        /* Validate argument of -D, -U opts */
static void     setsysdirs();   /* Set system-specific include dirs */
#if HAVE_ENV
static void     setenvdirs();   /* Set user-defined include dirs    */
static void     parse_env();    /* Parse environment variables      */
#endif
static void     set_a_dir();    /* Append an include directory      */
static int      openinclude();  /* Open the included file           */
static int      hasdirectory(); /* Get directory part of filename   */
#if ! OK_IF_JUNK
static int      is_junk();      /* The directive has trailing junk? */
#endif
#if MODE >= STANDARD
static void     doonce();       /* Process #pragma __once           */
static int      included();     /* The file has been included ?     */
#endif
#if DEBUG || DEBUG_EVAL
static int      dodebug();      /* Process #pragma __debug, #debug  */
#endif
#if DEBUG
static void     dumppath();     /* Print include search path        */
#endif
#if MODE < STANDARD && COMPILER == MWC09
static void     doasm();        /* Process #asm, #endasm            */
#endif

#endif

#if HOST_SYSTEM == SYS_VMS
/*
 * Include the rms stuff.  (We can't just include rms.h as it uses the
 * VaxC-specific library include syntax that Decus CPP doesn't support.
 * By including things by hand, we can CPP ourself.)
 */
#include        <nam.h>
#include        <fab.h>
#include        <rab.h>
#include        <rmsdef.h>

extern int      access();
extern int      creat();
extern int      dup2();

static int      vmsparse();             /* Parse source file name   */

#endif  /* HOST_SYSTEM == SYS_VMS   */

#if FOLD_CASE
static void     zap_uc();               /* Toupper option arguments */
#endif

#if SYSTEM == SYS_MSDOS || SYSTEM == SYS_WIN32
static int      mem_model( int model);  /* Specify memory-model     */
#endif

#if SYS_FAMILY == SYS_MSDOS
static char *   bsl2sl( char * filename);       /* Convert \ to /   */
#endif

#if COMPILER == MWC09
static void     sharpsub();                     /* Convert . to _   */
static void     printlong();    /* Enable printf() to print long    */
#endif

/*
 * Table of character types.
 * This table must be rewritten for a non-Ascii machine and for set of
 * multi-byte characters other than KANJI of Japan, China or Korea.
 */

#if DOLLAR_IN_NAME
#define DOL     LET
#else
#define DOL     000
#endif

#if MBCHAR
#define MB12    MB1 | MB2
#define LET2    LET | MB2
#define PNC2    PUNC| MB2
#endif

#ifdef __cplusplus
extern const char   type[ UCHARMAX + 1] = {
#else
const char          type[ UCHARMAX + 1] = {
#endif

/* Character type codes */
/*   0,     1,     2,     3,     4,     5,     6,     7,                    */
/*   8,     9,     A,     B,     C,     D,     E,     F,       Hex          */

   000,   000,   000,   000,   000,   000,   000,   000,    /* 00           */
   000,   SPA,   SPA,   SPA,   SPA,   SPA,   000,   000,    /* 08           */
   000,   000,   000,   000,   000,   000,   000,   000,    /* 10           */
   000,   000,   000,   000,   LET,   000,   000,   SPA,    /* 18           */
   SPA,   PUNC,  QUO,   PUNC,  DOL,   PUNC,  PUNC,  QUO,    /* 20  !"#$%&'  */
   PUNC,  PUNC,  PUNC,  PUNC,  PUNC,  PUNC,  DOT,   PUNC,   /* 28 ()*+,-./  */
   DIG,   DIG,   DIG,   DIG,   DIG,   DIG,   DIG,   DIG,    /* 30 01234567  */
   DIG,   DIG,   PUNC,  PUNC,  PUNC,  PUNC,  PUNC,  PUNC,   /* 38 89:;<=>?  */

#if MBCHAR == SJIS || MBCHAR == BIGFIVE
   MB2,   LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,   /* 40 @ABCDEFG  */
   LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,   /* 48 HIJKLMNO  */
   LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,   /* 50 PQRSTUVW  */
   LET2,  LET2,  LET2,  PNC2,  MB2,   PNC2,  PNC2,  LET2,   /* 58 XYZ[\]^_  */
   MB2,   LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,   /* 60 `abcdefg  */
   LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,   /* 68 hijklmno  */
   LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,  LET2,   /* 70 pqrstuvw  */
   LET2,  LET2,  LET2,  PNC2,  PNC2,  PNC2,  PNC2,  000,    /* 78 xyz{|}~   */
#else
/* MBCHAR == EUC || MBCHAR == KS_C_5601 || MBCHAR == GB_2312 || MBCHAR == 0 */
   000,   LET,   LET,   LET,   LET,   LET,   LET,   LET,    /* 40 @ABCDEFG  */
   LET,   LET,   LET,   LET,   LET,   LET,   LET,   LET,    /* 48 HIJKLMNO  */
   LET,   LET,   LET,   LET,   LET,   LET,   LET,   LET,    /* 50 PQRSTUVW  */
   LET,   LET,   LET,   PUNC,  000,   PUNC,  PUNC,  LET,    /* 58 XYZ[\]^_  */
   000,   LET,   LET,   LET,   LET,   LET,   LET,   LET,    /* 60 `abcdefg  */
   LET,   LET,   LET,   LET,   LET,   LET,   LET,   LET,    /* 68 hijklmno  */
   LET,   LET,   LET,   LET,   LET,   LET,   LET,   LET,    /* 70 pqrstuvw  */
   LET,   LET,   LET,   PUNC,  PUNC,  PUNC,  PUNC,  000,    /* 78 xyz{|}~   */
#endif

#if MBCHAR == SJIS
   MB2,   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   80 .. 87   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   88 .. 8F   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   90 .. 97   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   98 .. 9F   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   A0 .. A7   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   A8 .. AF   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   B0 .. B7   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   B8 .. BF   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   C0 .. C7   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   C8 .. CF   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   D0 .. D7   */
   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,   MB2,    /*   D8 .. DF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   E0 .. E7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   E8 .. EF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   F0 .. F7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  000,   000,   000,    /*   F8 .. FF   */
#else
#if MBCHAR == EUC
   000,   000,   000,   000,   000,   000,   000,   000,    /*   80 .. 87   */
   000,   000,   000,   000,   000,   000,   MB1,   000,    /*   88 .. 8F   */
   000,   000,   000,   000,   000,   000,   000,   000,    /*   90 .. 97   */
   000,   000,   000,   000,   000,   000,   000,   000,    /*   98 .. 9F   */
   000,   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   A0 .. A7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   A8 .. AF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   B0 .. B7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   B8 .. BF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   C0 .. C7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   C8 .. CF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   D0 .. D7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   D8 .. DF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   E0 .. E7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   E8 .. EF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   F0 .. F7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  000,    /*   F8 .. FF   */
#else
#if MBCHAR == KS_C_5601 || MBCHAR == GB_2312 || MBCHAR == BIGFIVE
   000,   000,   000,   000,   000,   000,   000,   000,    /*   80 .. 87   */
   000,   000,   000,   000,   000,   000,   000,   000,    /*   88 .. 8F   */
   000,   000,   000,   000,   000,   000,   000,   000,    /*   90 .. 97   */
   000,   000,   000,   000,   000,   000,   000,   000,    /*   98 .. 9F   */
   000,   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   A0 .. A7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   A8 .. AF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   B0 .. B7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   B8 .. BF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   C0 .. C7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   C8 .. CF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   D0 .. D7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   D8 .. DF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   E0 .. E7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   E8 .. EF   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,   /*   F0 .. F7   */
   MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  MB12,  000,    /*   F8 .. FF   */
/* Else 0 cleared   */
#endif
#endif
#endif

};

/*
 * incdir[] stores the -I directories (and the system-specific #include <...>
 * directories).
 */
static const char *   incdir[ NINCLUDE];        /* Include directories      */
static const char **  incend = incdir;          /* -> active end of incdir  */
static int      search_rule = SEARCH_INIT;  /* Rule to search include file  */
static const char * const   argerr = "Incorrect argument for -%c\n";

#if HOST_SYSTEM == SYS_MSDOS && HOST_COMPILER == TURBOC
extern unsigned     _stklen = NMACWORK + (NEXP * 30) + (sizeof (int) * 1280)
#if MODE >= STANDARD
                        + (sizeof (char *) * 12 * RESCAN_LIMIT)
#endif
                        + 0x800;
#endif

#if COMPILER == MWC09
static char *   edition = "0";
#endif

#define LINE90LIMIT     32767
#define LINE99LIMIT     0x7FFFFFFF
#define LINE_CPP_LIMIT  32767

void
#if PROTO
dooptions( int argc, char ** argv, char ** in_pp, char ** out_pp)
#else
dooptions( argc, argv, in_pp, out_pp)
    int         argc;
    char **     argv;
    char **     in_pp;                      /* Input file name      */
    char **     out_pp;                     /* Output file name     */
#endif
/*
 * Process command line arguments (-D, etc.), called only at cpp startup.
 */
{
    extern int      optind;                 /* Defined in getopt()  */
    extern char *   optarg;                 /*      ditto           */
    char            optlist[ 50];
#if MODE >= STANDARD
    char            tmp[ 16];
    const char *    stdc_v_name = "__STDC_VERSION__";
    VAL_SIGN        *valp;
    int             sargs;                  /* -S option            */
#endif
#if SYSTEM == SYS_MSDOS
    int             memmodel = FALSE;
#endif
    int             i;
    int             vflag, nflag;           /* -V, -N options       */
    int             unsetsysdirs;
    /* Unset system-specific and site-specific include directories ?*/
    DEFBUF *        defp;
    register int    opt;
    char *          cp;                     /* Temporary pointer    */

    setoptlist( optlist);

#if MODE >= STANDARD
    sargs = 0;
#endif
    vflag = nflag = unsetsysdirs = FALSE;

    while (optind < argc
            && (opt = getopt( argc, argv, optlist)) != EOF) {

#if FOLD_CASE
        if (islower( opt))
            opt = toupper( opt);            /* Normalize the case   */
        if (optarg)
            zap_uc( optarg);                /* Touppers argument    */
#endif

        switch (opt) {          /* Command line option character    */

#if CMP_FAMILY == GNUC
        case 'A':       /* Ignore -A system(gnu), -A cpu(vax) or so */
            break;
#endif

#if COMPILER == MWC09
        case 'c':
#endif
        case 'C':                           /* Keep comments        */
            cflag = TRUE;
            break;

#if MODE >= STANDARD && CMP_FAMILY == GNUC
        case 'd':
            if (str_eq( optarg, "igraphs")) {       /* -digraphs    */
                digraphs = TRUE;
            } else {
                usage();
            }
            break;
#endif

#if COMPILER == LSIC
        case 'd':
#endif
        case 'D':                           /* Define symbol        */
#if MODE != CON_FALSE && OK_TRIGRAPHS
            /* Convert trigraphs for the environment which need trigraphs   */
            if (tflag)
                trigraph( optarg);
#endif
#if MODE == POST_STANDARD && OK_DIGRAPHS
            if (digraphs)   /* Convert prior to installing macro    */
                cnv_digraph( optarg);
#endif
            /*
             * Check for incorrect argument -- don't allow
             * -D foo(bar)=something
             */
            if (! is_id( optarg)) {
                fprintf( stderr, argerr, opt);
                usage();
            }
            /* If the option is just "-D foo", make it -D foo=1     */
            /* -D foo= defines foo to no token.                     */
            cp = optarg;
            while (*cp != EOS && *cp != '=')
                cp++;
            if (*cp == EOS)
                cp = "1";
            else
                *cp++ = EOS;                /* Remove '='           */
            if ((defp = lookid( optarg)) != NULL) { /* Defined      */
                if (defp->nargs == DEF_NOARGS - 1) {
                    undef_a_predef( optarg);
                    /* Remove the non-standard predefined macro     */
                }
#if MODE != CON_FALSE
                else if (defp->nargs < DEF_NOARGS - 1) {
                    fprintf( stderr, "\"%s\" shouldn't be redefined\n"
                            , optarg);      /* Standard predefined  */
                    usage();
                }
#endif
                undefine( optarg);
            }
#if MODE != CON_FALSE
            if (str_eq( optarg, "defined")) {
                fputs( "\"defined\" shouldn't be defined\n", stderr);
                usage();
            }
#endif
            /* Now, save the name and its definition.   */
            install( optarg, DEF_NOARGS, "", cp);
            break;

#if COMPILER == MWC09
        case 'e':
            if (*optarg == '=')
                optarg++;
            for (cp = optarg; *cp; cp++) {
                if (! isdigit( *cp & UCHARMAX)) {
                    fprintf( stderr, argerr, opt);
                    usage();
                }
            }
            edition = optarg;
            break;
#endif
        case 'E':                       /* Ignore non-fatal errors  */
            eflag = TRUE;
            break;

#if CMP_FAMILY == GNUC
        case 'g':
            break;                          /* Ignore this option   */
#endif

        case 'I':                           /* Include directory    */
#if CMP_FAMILY != GNUC
            if (str_eq( optarg, "-")) {
                unsetsysdirs = TRUE;        /* Unset pre-specified  */
                break;                      /*   include directories*/
            }
#endif
            if (*(optarg + 1) == EOS && isdigit( *optarg)
                    && (i = *optarg - '0') != 0
                    && (i & ~(CURRENT | SOURCE)) == 0) {
                search_rule = i;            /* -I1, -I2 or -I3      */
                break;
            }
#if SYS_FAMILY == SYS_MSDOS
            set_a_dir( bsl2sl( optarg));
#else
            set_a_dir( optarg);             /* User-defined direct. */
#endif
            break;

#if ! FOLD_CASE
        case 'i':                       /* No output of included    */
#else
        case 'Z':
#endif
            iflag = TRUE;
            break;

#if COMPILER == MWC09
        case 'l':                   /* Output source lines as       */
            lflag = TRUE;           /*  comments to complier-proper.*/
            break;
#endif

#if CMP_FAMILY == GNUC
        case 'l':
            if (str_eq( optarg, "ang-c")) {     /* -lang-c          */
                break;                      /* Ignore this option   */
#if MODE >= STANDARD
            } else if (str_eq( optarg, "ang-c99")) {    /* -lang-c99*/
                if (! sargs) {
                    i = 1;                  /* Define __STDC__ to 1 */
                    stdc_ver = 199901L;
                    goto  stdc;
                }
            } else if (str_eq( optarg, "ang-c89")) {    /* -lang-c89*/
                if (! sargs) {
                    i = 1;                  /* Define __STDC__ to 1 */
                    goto  stdc;
                }
            } else if (str_eq( optarg, "ang-c++")) {    /* -lang-c++*/
                goto  plus;
#endif
            } else if (str_eq( optarg, "ang-asm")) {    /* -lang-asm*/
                break;                      /* Ignore this option   */
            } else {
                usage();
            }
            break;
#endif

#if FOLD_CASE
        case 'm':
#endif
#if OK_MAKE
        case 'M':           /* Output source file dependency line   */
            if (str_eq( optarg, "M"))                       /* -MM  */
                ;
            else if (str_eq( optarg, "D"))                  /* -MD  */
                mkdep = MD_SYSHEADER | MD_FILE;
            else if (str_eq( optarg, "MD"))                 /* -MMD */
                mkdep = MD_FILE;
            else if (argv[ optind - 1] == optarg) {         /* -M   */
                mkdep = MD_SYSHEADER;
                optind--;
            } else
                usage();
            mkdep |= MD_MKDEP;
            break;
#endif

#if SYSTEM == SYS_MSDOS
        case 'm':                       /* Specify a memory model   */
            if (*(optarg + 1) != EOS)
                usage();
            else if (mem_model( toupper( *optarg & UCHARMAX)) == FALSE) {
                fprintf( stderr, "Invalid memory model '%s'\n", optarg);
                usage();
            }
            memmodel = TRUE;
            break;
#endif

#if CMP_FAMILY == GNUC
        case 'u':
            if (! str_eq( optarg, "ndef"))  /* -undef               */
                usage();
            /* Fall through */
#endif
        case 'N':                           /* No predefineds       */
            /* cpp -N           remove "vax", "__VAX" and friends.  */
            if (! nflag) {
                nflag = TRUE;
                unpredefine( TRUE);
            }
            break;

#if CMP_FAMILY == GNUC
        case 'n':
            if (str_eq( optarg, "ostdinc")) {       /* -nostdinc    */
                unsetsysdirs = TRUE;    /* Unset pre-specified dirs */
            } else if (str_eq( optarg, "oprecomp")) {   /* -noprecomp   */
                break;                      /* Ignore this option   */
            } else {
                usage();
            }
            break;
#endif

#if FOLD_CASE
        case 'O':
#endif
        case 'o':
            *out_pp = optarg;               /* Output file name     */
            break;

        case 'P':                           /* No #line output      */
            pflag = TRUE;
            break;

#if ! STD_LINE_PREFIX
#if FOLD_CASE
        case 'a':
#else
        case 'p':
#endif
            std_line_prefix = TRUE; /* Putout line and file infor-  */
            break;                  /*   mation in C source style.  */
#endif

#if CMP_FAMILY == GNUC
        case 'p':
            if (str_eq( optarg, "edantic")          /* -pedantic    */
                    || str_eq( optarg, "edantic-errors")) {
                                            /* -pedantic-errors     */
#if MODE >= STANDARD
                if (! sargs && ! cplus) {
                    i = 1;
                    goto  stdc;             /* Set -S1 option       */
                }
#else
                fputs( "Use STANDARD MODE cpp for -pedantic option\n"
                        , stderr);
                usage();
#endif  /* MODE >= STANDARD */
            } else {
                usage();
            }
            break;
#endif  /* GNUC */

        case 'Q':
            qflag = TRUE;
            break;

#if MODE >= STANDARD
        case 'S':
            if (cplus) {                /* Re-define __cplusplus    */
                sargs = 1;              /* Clear previous -S<n>     */
                valp = evalnum( optarg);
                if (valp->sign == VAL_ERROR)
                    usage();
                cplus = valp->val;
                strcpy( tmp, optarg);
                break;
            }
            if (sargs > 1)              /* The third time or more   */
                break;
            if (! sargs) {      /* The first time of this option    */
                i = *optarg;
                if (! isdigit( i) || *(optarg + 1) != EOS)
                    usage();
                i = i - '0';
#if CMP_FAMILY == GNUC
stdc:
#endif
                defp = lookid( "__STDC__");
                sprintf( defp->repl, "%d", i);      /* Redefine     */
                stdc_val = i;
            }
            if ((defp = lookid( stdc_v_name)) != NULL) {
                defp->nargs = DEF_NOARGS;
                undefine( stdc_v_name);
            }
            if (stdc_val) {
                if (! sargs) {                      /* First time   */
                    if (! stdc_ver)
                        stdc_ver = i == 1 ? STDC1_VERSION
                                 : i == 2 ? STDC2_VERSION : 0L;
                    sprintf( tmp, "%ldL", stdc_ver);
                } else {                            /* Second time  */
                    valp = evalnum( optarg);
                    if (valp->sign == VAL_ERROR)
                        usage();
                    stdc_ver = valp->val;
                    strcpy( tmp, optarg);
                }
                install( stdc_v_name, DEF_NOARGS - 2, "", tmp);
                                    /* Redefine __STDC_VERSION__    */
            }
            sargs++;
            break;
#endif  /* MODE >= STANDARD */

#if COMPILER == LSIC
        case 's':
            pflag = TRUE;
            break;
#endif

#if CMP_FAMILY == GNUC
        case 't':
            if (str_eq( optarg, "raditional")) {    /* -traditional */
#if ! COMMENT_INVISIBLE || ! STRING_FORMAL
                fputs( "Use OLD_PREPROCESSOR for -traditional\n", stderr);
                usage();
#else
                break;
#endif
#if MODE != CON_FALSE && OK_TRIGRAPHS
            } else if (str_eq( optarg, "rigraphs")) {
                tflag = TRUE;                       /* -trigraphs   */
#endif
            } else {
                usage();
            }
            break;
#endif  /* GNUC */

        case 'U':                           /* Undefine symbol      */
            /*
             * We don't need to map trigraphs as they can't be part of a
             * symbol name. (_ isn't trigraphable).
             */
            if (! is_id( optarg)) {
                fprintf( stderr, argerr, opt);
                usage();
            }
            if ((defp = lookid( optarg)) != NULL) {
                if (defp->nargs == DEF_NOARGS - 1) {
                    undef_a_predef( optarg);
                }
#if MODE != CON_FALSE
                else if (defp->nargs < DEF_NOARGS - 1) {
                    fprintf( stderr, "\"%s\" shouldn't be undefined\n"
                            , optarg);
                    usage();
                }
#endif
                undefine( optarg);
            } else {
                fprintf( stderr, "\"%s\" wasn't defined\n", optarg);
            }
            break;

        case 'V':
        case 'v':
            vflag = TRUE;
            break;

        case 'W':                           /* Warning level        */
            if (warn_level == -1)           /* Have to initialize   */
                warn_level = 0;
#if CMP_FAMILY == GNUC
            if (str_eq( optarg, "comment") || str_eq( optarg, "comments")) {
                warn_level |= 1;
                break;
            }
#if MODE > CON_FALSE && OK_TRIGRAPHS
            else if (str_eq( optarg, "trigraphs")) {
                warn_level |= 8;
                break;
            }
#endif
            else if (str_eq( optarg, "all")) {
                warn_level |= 9;            /* Convert -Wall to -W9 */
                break;
            }
#endif
            if ((warn_level |= atoi( optarg)) > 15 || warn_level < 0)
                usage();
            break;

#if COMPILER == LSIC
        case 'w':
            warn_level = 0;
            break;
#endif

#if MODE >= STANDARD && OK_DIGRAPHS
        case '2':                   /* Revert digraphs recognition  */
            digraphs = ! digraphs;
            break;
#endif

#if MODE != CON_FALSE && OK_TRIGRAPHS
        case '3':                   /* Revert trigraph recogniion   */
            tflag = ! tflag;
            break;
#endif

#if MODE >= STANDARD
        case '+':
#if CMP_FAMILY == GNUC
plus:
#endif
            if (cplus)
                break;
            cplus = CPLUS;
            unpredefine( FALSE);            /* Remove "izix", etc.  */
            if ((defp = lookid( "__STDC__")) != NULL) {
                defp->nargs = DEF_NOARGS;
                undefine( "__STDC__");      /* Remove __STDC__      */
                stdc_val = 0;
            }
            if ((defp = lookid( stdc_v_name)) != NULL) {
                defp->nargs = DEF_NOARGS;
                undefine( stdc_v_name); /* Remove __STDC_VERSION__  */
            }
            sargs = 0;          /* Clear preceding -S<n> options    */
            break;
#endif

#if CMP_FAMILY == GNUC && ! DOLLAR_IN_NAME
        case '$':                       /* Forbid '$' in identifier */
            break;                          /* Ignore this option   */
#endif

        default:                            /* What is this one?    */
            usage();
            break;
        }                               /* Switch on all options    */

    }                                   /* For all arguments        */

    if (warn_level == -1)                   /* No -W option         */
        warn_level = 1;                 /* Default warning level    */
    setfiles( argc, argv, in_pp, out_pp);

#if MODE >= STANDARD
    if (!nflag && stdc_val)
        unpredefine( FALSE);            /* Undefine "izix" or so    */
    if (cplus && !sargs)
        sprintf( tmp, "%ldL", (long) CPLUS);
    setcplus( tmp, nflag);
    stdc2 = cplus || stdc_ver >= 199901L;
    /* Set translation limits specified by the Standards    */
    if (cplus) {                /* Specified by C++ 1996/12 draft   */
        slenmin = SLEN_CPP_MIN;
        idlenmin = IDLEN_CPP_MIN;
        nmacparsmin = NMACPARS_CPP_MIN;
        exp_nestmin = EXP_NEST_CPP_MIN;
        blk_nestmin = BLK_NEST_CPP_MIN;
        inc_nestmin = INCLUDE_NEST_CPP_MIN;
        nmacromin = NMACRO_CPP_MIN;
        line_limit = LINE_CPP_LIMIT;
    } else if (stdc_ver >= 199901L) {   /* Specified by C99-1998/08 draft   */
        slenmin = SLEN99MIN;
        idlenmin = IDLEN99MIN;
        nmacparsmin = NMACPARS99MIN;
        exp_nestmin = EXP_NEST99MIN;
        blk_nestmin = BLK_NEST99MIN;
        inc_nestmin = INCLUDE_NEST99MIN;
        nmacromin = NMACRO99MIN;
        line_limit = LINE99LIMIT;
    } else {                        /* Specified by C90 Standard    */
        slenmin = SLEN90MIN;
        idlenmin = IDLEN90MIN;
        nmacparsmin = NMACPARS90MIN;
        exp_nestmin = EXP_NEST90MIN;
        blk_nestmin = BLK_NEST90MIN;
        inc_nestmin = INCLUDE_NEST90MIN;
        nmacromin = NMACRO90MIN;
        line_limit = LINE90LIMIT;
    }
#if OK_PRAGMA_OP
    if (stdc_ver >= 199901L) {
        /*
         *      #define _Pragma(a)  _Pragma ( a )
         * Define _Pragma() operator as a special macro so as to be searched
         * easily.  The unusual 'DEF_PRAGMA' is a marker of this psuedo
         * macro.
         */
        char *  name = "_Pragma";
        if (lookid( name) == NULL) {        /* Not defined by -D    */
            sprintf( tmp, "%c%s ( %c%c )", DEF_MAGIC, name, MAC_PARM, 1);
                                                /* Replacement text */
            install( name, DEF_PRAGMA, "a", tmp);
        }
    }
#endif  /* OK_PRAGMA_OP     */
#endif  /* MODE >= STANDARD */

#if SYSTEM == SYS_MSDOS
    if (memmodel == FALSE)
        mem_model( 'S');            /* Default memory model : SMALL */
#else
#endif
#if HAVE_ENV
    setenvdirs();
#endif
    if (! unsetsysdirs)
        setsysdirs();
    if (vflag)
        version();
}

static void
#if PROTO
usage( void)
#else
usage()
#endif
/*
 * Print usage.
 */
{
#if 0	// BuildShader が Usage を出すのでカット.
	static const char * const   mes[] = {

"Usage:      cpp [-<opts> [-<opts>]] [<infile> [<outfile>]]\n",
"    <infile> defaults to stdin and <outfile> defaults to stdout.\n",
"The following options are valid:\n",

#if COMPILER == MWC09
"-C, -c  ",
#else
"-C      ",
#endif
        "Output also comments.           -E      Ignore non-fatal errors.\n",

#if COMPILER == LSIC
"-D, -d <macro>[=<value>]    ",
#else
"-D <macro>[=<value>]    ",
#endif
                        "Define <macro> to <value> (default:1).\n",

#if COMPILER == MWC09
"-e <edition>    Set edition number to <edition> (in decimal).\n",
#endif

"-I <directory>  Add <directory> to the #include search list.\n",
#if CMP_FAMILY == GNUC
"-nostdinc   ",
#else
"-I-     ",
#endif
        "Unset system or site specific include directories.\n",

#if ! FOLD_CASE
"-i      ",
#else
"-Z      ",
#endif
        "Don't output the included file, only defining macros.\n",

#if COMPILER == MWC09
"-l      Output source lines as comments for compiler.\n",
#endif

#if OK_MAKE
"-M, -MM, -MD, -MMD     Output source file dependency line for makefile.\n",
#endif

#if SYSTEM == SYS_MSDOS
"-m <x>  Specify memory-model as <x> ",
#if COMPILER == LSIC || COMPILER == LATTICEC
                                    "(t,s,d,p,l,h).\n",
#else
                                    "(t,s,c,m,l,h).\n",
#endif
#endif

#if CMP_FAMILY == GNUC
"-N, -undef  ",
#else
"-N      ",
#endif
        "Don't predefine any non-standard macros.\n",

#if FOLD_CASE
"-O <file>   ",
#else
"-o <file>   ",
#endif
            "Output to <file>.           ",

#if COMPILER == LSIC
"-P, -s  ",
#else
"-P      ",
#endif
        "Don't output #line lines.\n",

#if ! STD_LINE_PREFIX
#if FOLD_CASE
"-A      ",
#else
"-p      ",
#endif
        "Output #line lines in C source style.\n",
#endif

"-Q      Output diagnostics to \"cpp.err\" (default:stderr).\n",

#if MODE >= STANDARD
#if CMP_FAMILY == GNUC
"-pedantic, -pedantic-errors, -lang-c89      Same as -S1.\n",
#endif
"-S <n>  ",
        "Redefine __STDC__ to <n>, undefine old style macros.\n",
"        The second -S <n> redefine __STDC_VERSION__ to <n>.\n",
#if OK_PRAGMA_OP
"            -S 199901L enables also // comments and _Pragma() operator.\n",
#else
"            -S 199901L enables also // comments.\n",
#endif
"        For C++ -S <n> redefine __cplusplus to <n>.\n",
#endif

"-U <macro>  Undefine <macro>.           -V, -v  Show version of cpp.\n",

"-W <level>  Set warning level to <level> (OR of {0,1,2,4,8}, default:1).\n",

#if COMPILER == LSIC
"-w      Same as -W0.\n",
#endif

#if MODE >= STANDARD && OK_DIGRAPHS
#if CMP_FAMILY == GNUC
"-digraphs   Enable digraphs.            ",

#endif
"-2      ",
#if DIGRAPHS_INIT
        "Dis",
#else
        "En",
#endif
           "able digraphs.\n",
#endif  /* MODE >= STANDARD */

#if MODE != CON_FALSE && OK_TRIGRAPHS
#if CMP_FAMILY == GNUC
"-trigraphs  Enable trigraphs.           ",
#endif
"-3      ",
#if TFLAG_INIT
        "Dis",
#else
        "En",
#endif
            "able trigraphs.\n",
#endif  /* MODE != CON_FALSE    */
/*
#if MODE >= STANDARD
#if CMP_FAMILY == GNUC
"-+, -lang-c++   ",
#else
"-+      ",
#endif
        "Process C++ source.\n",
#endif
*/
        NULL,
    };

    const char * const *    mpp = & mes[ 0];

    version();
    while (*mpp)
        fputs( *mpp++, stderr);
#endif
    exit( 1);
}

static void
#if PROTO
version( void)
#else
version()
#endif
/*
 * Print version.
 */
{
    fputs( VERSION, stderr);
}

static void
#if PROTO
setoptlist( char * optlist)
#else
setoptlist( optlist)
    char *  optlist;
#endif
/*
 * Set list of legal option characters.
 */
{
    strcpy( optlist, "o:vCD:EI:NPQU:VW:");

#if MODE != CON_FALSE && OK_TRIGRAPHS
    strcat( optlist, "3");
#endif
#if MODE >= STANDARD
#if OK_DIGRAPHS
    strcat( optlist, "2");
#endif
    strcat( optlist, "+S:");
#endif
#if OK_MAKE
#if FOLD_CASE
    strcat( optlist, "m:M:");
#else
    strcat( optlist, "M:");
#endif
#endif
#if FOLD_CASE
    strcat( optlist, "cd:ei:npqs:u:w:zOZ");
#else
    strcat( optlist, "i");
#endif
#if ! STD_LINE_PREFIX
#if FOLD_CASE
    strcat( optlist, "aA");
#else
    strcat( optlist, "p");
#endif
#endif
#if SYSTEM == SYS_MSDOS
    strcat( optlist, "m:");
#endif
#if CMP_FAMILY == GNUC
    strcat( optlist, "$A:d:gl:n:t:u:p:");
#endif
#if COMPILER == LSIC
    strcat( optlist, "d:sw");
#endif
#if COMPILER == MWC09
    strcat( optlist, "ce:l");
#endif
}

static void
#if PROTO
setfiles( int argc, char ** argv, char ** in_pp, char ** out_pp)
#else
setfiles( argc, argv, in_pp, out_pp)
    int     argc;
    char ** argv;
    char ** in_pp;
    char ** out_pp;
#endif
/*
 * Set input, output and diagnostic files.
 */
{
    extern int  optind;

    if (optind < argc)
#if SYS_FAMILY == SYS_MSDOS
        *in_pp = bsl2sl( argv[ optind++]);
#else
        *in_pp = argv[ optind++];
#endif
    if (optind < argc && *out_pp == NULL)
        *out_pp = argv[ optind++];
    if (optind < argc) {
        fprintf( stderr, "Excessive file argument \"%s\"\n", argv[ optind]);
        usage();
    }
}

#if MODE >= STANDARD
static void
#if PROTO
setcplus( char * val, int nflag)
#else
setcplus( val, nflag)
    char *  val;
    int     nflag;
#endif
/*
 * Set the macro "__cplusplus".
 */
{
    const char * const  cplus_name = "__cplusplus";

    if (! cplus) {
#ifdef  CMP_CPLUS
        if (! nflag)        /* Undefine pre-defined macro for C++   */
            undefine( CMP_CPLUS);
#endif
    } else {
        if (lookid( cplus_name) != NULL)
            undefine( cplus_name);
        install( cplus_name, DEF_NOARGS - 2, "", val);
    }
}
#endif

static int
#if PROTO
is_id( char * ap)
#else
is_id( ap)
    char *  ap;
#endif
/*
 * Check validity of specified identifier.
 */
{
    register char *     cp;

    if ((type[ *ap & UCHARMAX] & LET) == 0)
        return  FALSE;
    for (cp = ap; *cp != EOS && *cp != '='; cp++) {
        if ((type[ *cp & UCHARMAX] & (LET | DIG)) == 0)
            return  FALSE;
    }
    return  TRUE;
}

#if FOLD_CASE
static void
zap_uc(ap)
    register char *     ap;
/*
 * Dec operating systems mangle upper-lower case in command lines.
 * This routine forces the arguments of option to uppercase.
 * It is called only on cpp startup by dooptions().
 */
{
    while (*ap != EOS) {
        if (islower( *ap & UCHARMAX))
            *ap = toupper( *ap & UCHARMAX);
        ap++;
    }
}
#endif

#if SYSTEM == SYS_MSDOS || SYSTEM == SYS_WIN32

#define LARGE   4
#define SMALL   2

static int
mem_model( int model)
/*
 * Support the selection of different memory models for the i8086 family.
 */
{
#if OK_SIZE
    SIZES *         sizp = size_table;      /* Get start of table   */
#endif
#if COMPILER == MSC || COMPILER == QUICKC
    char            name[ 8] = "M_I86SM";
#else
    const char *    name;
#endif

    switch (model) {
#if ! OK_SIZE
    case 'T' :
    case 'S' :
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'D' :
#else
    case 'C' :
#endif
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'P' :
#else
    case 'M' :
#endif
    case 'L' :
    case 'H' :
        break;
#else   /* OK_SIZE  */
    case 'T' :                  /* Tiny model...    */
    case 'S' :                  /* Small model...   */
        while (sizp->bits != T_FPTR)
            (sizp++)->psize = SMALL;
        sizp->psize = SMALL;
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'D' :
#else
    case 'C' :                  /* Compact model... small code, large data  */
#endif
        while (sizp->bits != T_FPTR)
            (sizp++)->psize = LARGE;
        sizp->psize = SMALL;
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'P' :
#else
    case 'M' :                  /* Medium model... large code, small data   */
#endif
        while (sizp->bits != T_FPTR )
            (sizp++)->psize = SMALL;    /* All data pointers are short.     */
        sizp->psize = LARGE;    /* Function pointers are long.  */
        break;

    case 'L' :                  /* Large model...   */
    case 'H' :                  /* Huge model... large code, large data     */
        while (sizp->bits)
            (sizp++)->psize = LARGE;
        break;
#endif  /* OK_SIZE  */

    default  :                  /* Invalid model    */
        return  FALSE;
    }

#if COMPILER == MSC || COMPILER == QUICKC

    name[ 5] = model;

#else

    switch (model) {
    case 'T' :
        name = "__TINY__";
        break;
    case 'S' :
        name = "__SMALL__";
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'D' :
#else
    case 'C' :
#endif
        name = "__COMPACT__";
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'P' :
#else
    case 'M' :
#endif
        name = "__MEDIUM__";
        break;
    case 'L' :
        name = "__LARGE__";
        break;
    case 'H' :
        name = "__HUGE__";
        break;
    }

#endif

    if (lookid( name))
        undefine( name);
    install( name, DEF_NOARGS, "", "1");
    return  TRUE;
}

#endif  /* SYS_MSDOS    */

int
#if PROTO
reopen_stdout( const char * name)
#else
reopen_stdout( name)
    char *  name;
#endif
/*
 * Connect stdout to output-file.
 */
{
#if SYSTEM == SYS_VMS
    /*
     * On vms, reopen stdout with "vanilla rms" attributes.
     */
    if ((i = creat( name, 0, "rat=cr", "rfm=var")) == -1
            || dup2( i, fileno(stdout)) == -1)
#else
    if (freopen( name, "w", stdout) == NULL)
#endif
        return  FALSE;
    else
        return  TRUE;
}

void
#if PROTO
setstdin( char * cp)
#else
setstdin( cp)
    char *  cp;
#endif
/*
 * Get the name of stdin.
 */
{
#if SYSTEM == SYS_VMS
    fgetname( stdin, cp);           /* Vax-11C, Decus C know name   */
#else
    strcpy( cp, "stdin");           /* Unix can't find stdin name   */
#endif
}

void
#if PROTO
put_start_file( char * name)
#else
put_start_file( name)
    char *  name;
#endif
/*
 * Explicitly output a #line at the start of cpp output so that lint (etc.)
 * knows the name of the original sourcefile.  If we don't do this explicitly,
 * we may get the name of the first #include file instead.
 *
 * The extra fputc() and sharp() works around a bug in pcc.
 */
{
    if (no_output)
        return;
#if COMPILER == MWC09
    if (! pflag) {
        if (std_line_prefix) {
            line++;
            sharp();
            line--;
        } else {
            curfile();
            fputs( "#P\n", stdout);
            sharpsub( name);
            fputs( edition, stdout);
            fputc( '\n', stdout);
        }
    }
#else
    line++;
    sharp();
#if COMPILER == PCC                 /* To work around a bug of PCC  */
    fputc( '\n', stdout);
    sharp();
#endif
    line--;
#endif
}

void
#if PROTO
sharp( void)
#else
sharp()
#endif
/*
 * Output a line number line.
 */
{
    if (no_output || pflag)
        goto  sharp_exit;
    if (keepcomments)
        fputc( '\n', stdout);           /* Ensure to be on line top */
#if COMPILER != MWC09
    printf( "#%s%ld", std_line_prefix ? "line " : LINE_PREFIX, line);
    curfile();
#else
    if (std_line_prefix) {
        printf( "#line %ld", line);
        curfile();
    } else {
        curfile();
        printf( "#%s%ld", LINE_PREFIX, line - 1);
    }
#endif
    fputc( '\n', stdout);
sharp_exit:
    wrongline = FALSE;
}

void
#if PROTO
putfname( char * name)
#else
putfname( name)
    char *  name;
#endif
/*
 * Output the name of included file.
 */
{
    if (no_output)
        return;
#if COMPILER == MWC09
    if (std_line_prefix) {
        printf( " \"%s\"", name);
    } else {
        printf( "#7\n%s\n", name);
        sharpsub( name);
    }
#else
    printf( " \"%s\"", name);
#endif
}

#if COMPILER == MWC09
static void
sharpsub( name)
    register char *     name;
/*
 * Output the normalized file name.
 */
{
    register char *     p;

    if (no_output)
        return;
    if ((p = strrchr( name, PATH_DELIM)) != NULL)
        name = ++p;
    if ((p = strchr( name, '.')) != NULL)
        *p = '_';
    fputs( name, stdout);
    fputc( '\n', stdout);
    if (p != NULL)
        *p = '.';
}
#endif

/*
 * getredirection() is intended to aid in porting C programs to VMS (Vax-11 C)
 * which does not support '>' and '<' I/O redirection.  With suitable
 * modification, it may useful for other portability problems as well.
 */
int
#if PROTO
getredirection( int argc, char ** argv)
#else
getredirection(argc, argv)
    int     argc;
    char    **argv;
#endif
{
#if SYSTEM == SYS_VMS
/*
 * Process vms redirection arg's.  Exit if any error is seen.
 * If getredirection() processes an argument, it is erased
 * from the vector.  getredirection() returns a new argc value.
 *
 * Warning: do not try to simplify the code for vms.  The code
 * presupposes that getredirection() is called before any data is
 * read from stdin or written to stdout.
 *
 * Normal usage is as follows:
 *
 *  main(argc, argv)
 *  int             argc;
 *  char            *argv[];
 *  {
 *      argc = getredirection(argc, argv);
 *  }
 */
    register char           *ap;    /* Argument pointer     */
    int                     i;      /* argv[] index         */
    int                     j;      /* Output index         */
    int                     file;   /* File_descriptor      */

    for (j = i = 1; i < argc; i++) {   /* Do all arguments  */
        switch (*(ap = argv[i])) {
        case '<':                   /* <file                */
            if (freopen(++ap, "r", stdin) == NULL) {
                perror(ap);         /* Can't find file      */
                exit(IO_ERROR);     /* Is a fatal error     */
            }
            break;

        case '>':                   /* >file or >>file      */
            if (*++ap == '>') {     /* >>file               */
                /*
                 * If the file exists, and is writable by us,
                 * call freopen to append to the file (using the
                 * file's current attributes).  Otherwise, create
                 * a new file with "vanilla" attributes as if the
                 * argument was given as ">filename".
                 * access(name, 2) returns zero if we can write on
                 * the specified file.
                 */
                if (access(++ap, 2) == 0) {
                    if (freopen(ap, "a", stdout) != NULL)
                        break;      /* Exit case statement  */
                    perror(ap);     /* Error, can't append  */
                    exit(IO_ERROR); /* After access test    */
                }                   /* If file accessable   */
            }
            /*
             * On vms, we want to create the file using "standard"
             * record attributes.  creat(...) creates the file
             * using the caller's default protection mask and
             * "variable length, implied carriage return"
             * attributes.  dup2() associates the file with stdout.
             */
            if ((file = creat(ap, 0, "rat=cr", "rfm=var")) == -1
                    || dup2(file, fileno(stdout)) == -1) {
                perror(ap);         /* Can't create file    */
                exit(IO_ERROR);     /* is a fatal error     */
            }                       /* If '>' creation      */
            break;                  /* Exit case test       */

        default:
            argv[j++] = ap;         /* Not a redirector     */
            break;                  /* Exit case test       */
        }
    }                               /* For all arguments    */
    argv[j] = NULL;                 /* Terminate argv[]     */
    return (j);                     /* Return new argc      */

#else   /* SYSTEM != SYS_VMS    */

    return (argc);

#endif  /* SYSTEM == SYS_VMS    */
}

#if HAVE_ENV

static void
#if PROTO
setenvdirs( void)
#else
setenvdirs()
#endif
/*
 * Add to include path those specified by environment variables.
 */
{
    const char *    env;
#if MODE >= STANDARD
    if (cplus) {
        if ((env = getenv( ENV_CPP_INCLUDE)) != NULL) {
            parse_env( env);
            return;
        }
    }
#endif
    if ((env = getenv( ENV_C_INCLUDE)) != NULL)
        parse_env( env);

#if SYSTEM == SYS_VMS
    if (getenv( "C$LIBRARY") != NULL)
        set_a_dir( "C$LIBRARY:");
#endif
}

static void
#if PROTO
parse_env( const char * env)
#else
parse_env( env)
    const char *    env;
#endif
/*
 * Parse environmental variable and append the path to include-dir-list.
 */
{
    int     c;
    char *  save;
    char *  p;

    save = savestring( env);
    while (*save) {
        p = save;
        while (*p && *p != ENV_SEP && *p != ' ')
            p++;
        if (p != save)  {                   /* Variable separator   */
            c = *p;
            *p = EOS;
#if SYS_FAMILY == SYS_MSDOS
            set_a_dir( bsl2sl( save));
#else
            set_a_dir( save);
#endif
            *p = c;
            save = p;
        }
        while (*save == ENV_SEP || *save == ' ')
            ++save;
    }
}

#endif  /* HAVE_ENV */

static void
#if PROTO
setsysdirs( void)
#else
setsysdirs()
#endif
/*
 * Set site-specific system-specific directories to the include directory
 * list.
 */
{
#if MODE >= STANDARD
#ifdef  CPP_INCLUDE
    if (cplus)
        set_a_dir( CPP_INCLUDE);
#endif
#endif
#ifdef  C_INCLUDE
    set_a_dir( C_INCLUDE);
#endif

#if SYS_FAMILY == SYS_UNIX
    set_a_dir( "/usr/include");
#endif  /* SYS_FAMILY == SYS_UNIX   */

#if SYSTEM == SYS_VMS
    set_a_dir( "SYS$LIBRARY:");
#endif  /* SYSTEM == SYS_VMS    */

#if SYS_FAMILY == SYS_OS9
    set_a_dir( "/DD/DEFS");
#endif  /* SYS_FAMILY == SYS_OS9    */
}

static void
#if PROTO
set_a_dir( const char * dirname)
#else
set_a_dir( dirname)
    const char *    dirname;                /* The path-name        */
#endif
/*
 * Append an include directory, checking array boundary.
 * This routine is called from the following routines (in this order).
 * 1. doooptions() by -I option.
 * 2. setenvdirs() by environment variables.
 * 3. setsysdirs() by CPP_INCLUDE, C_INCLUDE and system-specifics (unless
 *      -I- or -nostdinc option is specified).
 */
{
    if (& incdir[ NINCLUDE - 1] < incend) {
        fprintf( stderr, "Too many include directories \"%s\" (max %d).\n", dirname, NINCLUDE);
    } else {
        *incend++ = dirname;
    }
}

#if OK_MAKE
void
#if PROTO
put_depend( const char * filename)
#else
put_depend( filename)
    char *  filename;
#endif
/*
 * Append a header name to the source file dependency line.
 */
{
    static FILE *   fp;         /* Path to output dependency line   */
    static size_t   llen;       /* Length of current physical output line   */

    if (fp == NULL) {   /* Main source file.  Have to initialize.   */
        char    prefix[ FILENAMEMAX];
        const char  *target = filename;
        const char  *cp0;
        char    *cp;
        size_t  len;

#ifdef  PATH_DELIM
        if ((cp0 = strrchr( target, PATH_DELIM)) != NULL)
            target = cp0 + 1;
#endif
        if ((cp0 = strrchr( target, '.')) == NULL) {
            len = strlen( target);
        } else {
            len = (size_t) (cp0 - target);
        }
        memcpy( prefix, target, len);
        cp = prefix + len;
        *cp++ = '.';
        if (mkdep & MD_FILE) {
            strcpy( cp, "d");
            fp = fopen( prefix, "w");
        } else {
            fp = stdout;        /* Output dependency line to stdout */
            no_output++;        /* Without normal output            */
        }
        strcpy( cp, EXT_OBJ);
        fprintf( fp, "%s :", prefix);
        llen = strlen( prefix) + 2;
    } else if (filename == NULL) {              /* End of input     */
        fputc( '\n', fp);
        return;
    } else {            /* Any header.  Append its name to output.  */
        if (llen + strlen( filename) > 76) {    /* Line is long     */
            fputs( " \\\n ", fp);               /* Fold it          */
            llen = 1;
        }
    }
    fputc( ' ', fp);
    fputs( filename, fp);
    llen += strlen( filename) + 1;
}
#endif  /* OK_MAKE  */

#if MODE != CON_FALSE
static char * toolong_fname = "Too long header name \"%s%.0ld%s\""; /* _F_  */
#endif
#if ! OK_IF_JUNK
static char * excess_token =
        "Excessive token sequence \"%s\"";          /* _E_, _W1_    */
#endif

int
#if PROTO
doinclude( void)
#else
doinclude()
#endif
/*
 * Process the #include line.
 * There are three variations:
 *      #include "file"         search somewhere relative to the
 *                              current directory, if not found,
 *                              treat as #include <file>.
 *      #include <file>         Search in an implementation-dependent
 *                              list of places.
 *      #include macro-call     Expand the macro call, it must be one of
 *                              "file" or <file>, process as such.
 * On success : return TRUE;
 * On failure of syntax : return FALSE;
 * On failure of file opening : exit processing.
 * doinclude() always absorbs the line.
 */
{
    static const char * const   no_name = "No header name"; /* _E_  */
#if MODE != CON_FALSE
    char            header[ FILENAMEMAX + 16];
    char            *hp;
    int             c;
    int             token_type;
#endif
#if SYSTEM == SYS_VMS
    char            def_filename[NAM$C_MAXRSS + 1];
#endif
    char *          fname;
    register int    delim;                  /* " or <, >            */

    if ((delim = skipws()) == '\n') {       /* No argument          */
        cerror( no_name, NULLST, 0L, NULLST);
        return  FALSE;
    }
    fname = infile->bptr - 1;       /* Current token for diagnosis  */

#if MODE != CON_FALSE
    if (type[ delim] & LET) {               /* Maybe a macro        */
        if ((token_type = get_unexpandable( delim, FALSE)) == NO_TOKEN) {
            cerror( no_name, NULLST, 0L, NULLST);   /* Expanded to  */
            return  FALSE;                          /*   0 token.   */
        }
#if MODE >= STANDARD
        if (macro_line == MACRO_ERROR)      /* Unterminated macro   */
            return  FALSE;                  /*   already diagnosed. */
#endif
        delim = work[ 0];                   /* Expanded macro       */
        if (token_type == STR) {            /* String literal form  */
            goto found_name;
#if 1 || MODE != POST_STANDARD
        } else if (token_type == OPE && openum == OP_LT) {  /* '<'  */
            hp = header;
            while ((c = skipws()) != '\n') {    /* Eliminate spaces */
                token_type = get_unexpandable( c, FALSE);
                if (header + FILENAMEMAX + 15 < hp + (int) (workp - work))
                    cfatal( toolong_fname, header, 0L, work);
                hp = stpcpy( hp, work);     /* Expand any macros    */
                if (token_type == OPE && openum == OP_GT)   /* '>'  */
                    break;
            }
            ungetstring( header, NULLST);   /* To re-read           */
            workp = scanquote( delim, work, work + FILENAMEMAX, TRUE);
                                        /* Re-construct or diagnose */
            goto scanned;
        } else {                            /* Any other token in-  */
            goto not_header;                /*   cluding <=, <<, <% */
#endif
        }
    }
#endif

    if (delim == '"') {                     /* String literal form  */
        workp = scanquote( delim, work, work + FILENAMEMAX, FALSE);
#if 1 || MODE != POST_STANDARD
    } else if (delim == '<'
            && scantoken( delim, (workp = work, &workp), work_end) == OPE
            && openum == OP_LT) {           /* Token '<'            */
        workp = scanquote( delim, work, work + FILENAMEMAX, TRUE);
                                            /* Don't decompose      */
#endif
    } else {                                /* Any other token      */
        goto not_header;
    }

scanned:
    if (workp == NULL)                      /* Missing closing '>'  */
        goto  syntax_error;

found_name:
    *--workp = EOS;                     /* Remove the closing and   */
    fname = savestring( &work[ 1]);     /*  the starting delimiter. */

#if OK_IF_JUNK
    skipnl();
#else   /* ! OK_IF_JUNK */
#if MODE != CON_FALSE
    if (get_unexpandable( skipws(), FALSE) != NO_TOKEN) {
#if MODE >= STANDARD
        cerror( excess_token, work, 0L, NULLST);
        goto  error;
#else   /* MODE == CON_NOEXPAND || MODE == CON_EXPAND   */
        if (warn_level & 1)
            cwarn( excess_token, work, 0L, NULLST);
        skipnl();
#endif
    }
    get();                              /* Skip the newline         */
#else   /* MODE == CON_FALSE    */
    if (skipws() != '\n') {
        if (warn_level & 1)
            cwarn( excess_token, infile->bptr-1, 0L, NULLST);
        skipnl();
    }
#endif
#endif

#if SYSTEM == SYS_VMS
    /*
     * Assume the default .h filetype.
     */
    if (!vmsparse(fname, ".H", def_filename)) {
        perror(fname);                      /* Oops.                */
        goto error;
    } else if (openinclude(def_filename, (delim == '"'))) {
        goto opened;
    }
#else
    if (openinclude( fname, (delim == '"'))) {
        goto opened;
    }
#endif
    /*
     * No sense continuing if #include file isn't there.
     */
    cerror( "Can't open include file \"%s\"", fname, 0L, NULLST);   /* _E_  */
    goto error;

not_header:
    cerror( "Not a header name \"%s\"", fname, 0L, NULLST);         /* _E_  */
syntax_error:
    skipnl();
    return  FALSE;
error:
    free( fname);
    skipnl();
    return  FALSE;
opened:
    free( fname);
    errno = 0;      /* Clear errno possibly set by path searching   */
    return  TRUE;
}

static int
#if PROTO
openinclude( char * filename, int searchlocal)
#else
openinclude( filename, searchlocal)
    char *  filename;               /* File name to include         */
    int     searchlocal;            /* TRUE if #include "file"      */
#endif
/*
 * Open an include file.  This routine is only called from doinclude()
 * above, but was written as a separate subroutine for portability.
 * It searches the list of directories and opens the file via openfile(),
 * linking it into the list of active files.
 * Returns TRUE if the file * was opened, FALSE if opfile() fails.
 */
{
    char    tmpname[ FILENAMEMAX + 1];          /* Filename buffer  */
    int     full_path;              /* Filename is full-path-list   */
    int     has_dir = FALSE;        /* Includer has directory part  */
    register const char **      incptr;     /* -> inlcude directory */

#if SYS_FAMILY == SYS_UNIX || SYS_FAMILY == SYS_OS9
    if (filename[0] == PATH_DELIM)
#else
#if SYS_FAMILY == SYS_MSDOS
    bsl2sl( filename);
    if (filename[1] == ':')
#else
#if SYSTEM == SYS_MAC           /* I don't know.  Write by yourself */
    if (filename[0] != PATH_DELIM && strchr( filename, PATH_DELIM))
#else
#if SYS_FAMILY == SYS_VMS
    if (strchr( filename, ':'))
#else
/* For other systems you should write code here.    */
    if (0)
#endif
#endif
#endif
#endif
        full_path = TRUE;
    else
        full_path = FALSE;

    if (! full_path && searchlocal && (search_rule & SOURCE))
        has_dir = hasdirectory( infile->filename, tmpname);

    if ((searchlocal && ((search_rule & CURRENT) || !has_dir)) || full_path) {
        /*
         * Look in local directory first.
         * Try to open filename relative to the "current directory".
         */
#if DEBUG
        if ((debug & PATH) && searchlocal)
            fputs( "Searching .\n", stdout);
#endif
#if MODE >= STANDARD
        if (included( filename))            /* Once included        */
            return  TRUE;
#endif
        if (openfile( filename, searchlocal && !full_path))
            return  TRUE;
    }
    if (full_path)
        return  FALSE;

    if (searchlocal && (search_rule & SOURCE) && has_dir) {
        /*
         * Look in local directory of source file.
         * Try to open filename relative to the "source directory".
         */
#if DEBUG
        if (debug & PATH)
            printf( "Searching %s\n", tmpname);
#endif
        strcat( tmpname, filename);
#if MODE >= STANDARD
        if (included( tmpname))             /* Once included        */
            return  TRUE;
#endif
        if (openfile( tmpname, TRUE))
            return  TRUE;
    }

    /*
     * Look in any directories specified by -I command line arguments,
     * specified by environment variable, then in the builtin search list.
     */
    for (incptr = incdir; incptr < incend; incptr++) {
        if (strlen( *incptr) + strlen( filename) >= FILENAMEMAX) {
#ifdef PATH_DELIM
            char    *path = xmalloc( strlen( *incptr) + strlen( filename) + 4);
            sprintf( path, "\"%s%c%s\"", *incptr, PATH_DELIM, filename);
            cfatal( "Too long header name %s"               /* _F_  */
                    , path, 0L, NULLST);
#else
            cfatal( toolong_fname, *incptr, 0L, filename);
#endif
        }
#if DEBUG
        if (debug & PATH)
            printf( "Searching %s\n", *incptr);
#endif
#ifdef  PATH_DELIM
        sprintf( tmpname, "%s%c%s", *incptr, PATH_DELIM, filename);
#else
        sprintf( tmpname, "%s%s", *incptr, filename);
#endif
#if MODE >= STANDARD
        if (included( tmpname))             /* Once included        */
            return  TRUE;
#endif
        if (openfile( tmpname, FALSE))
            return  TRUE;
    }

    return  FALSE;
}

static int
#if PROTO
hasdirectory( char * source, char * directory)
#else
hasdirectory( source, directory)
    char *  source;            /* Directory to examine             */
    char *  directory;         /* Put directory stuff here         */
#endif
/*
 * If a device or directory is found in the 'source' filename string (i.e.
 * "includer"), the node/device/directory part of the string is copied to
 * 'directory' and hasdirectory() returns TRUE.
 * Else, nothing is copied and it returns FALSE.
 */
{
#ifdef  PATH_DELIM
    register char * tp;
    size_t          len;

    if ((tp = strrchr( source, PATH_DELIM)) == NULL) {
        return  FALSE;
    } else {
        len = (size_t)(tp - source) + 1;
        memcpy( directory, source, len);
        directory[ len] = EOS;
        return  TRUE;
    }
#else
#if SYSTEM == SYS_VMS
    if (vmsparse( source, NULLST, directory)
            && directory[ 0] != EOS) {
        return  TRUE;
    } else {
        return  FALSE;
    }
#endif
#endif
}

#if SYSTEM == SYS_VMS

static int
vmsparse(source, defstring, result)
char            *source;
char            *defstring;     /* non-NULL -> default string.      */
char            *result;        /* Size is at least NAM$C_MAXRSS + 1*/
/*
 * Parse the source string, applying the default (properly, using
 * the system parse routine), storing it in result.
 * TRUE if it parsed, FALSE on error.
 *
 * If defstring is NULL, there are no defaults and result gets
 * (just) the node::[directory] part of the string (possibly "")
 */
{
    struct FAB      fab = cc$rms_fab;       /* File access block    */
    struct NAM      nam = cc$rms_nam;       /* File name block      */
    char            fullname[NAM$C_MAXRSS + 1];
    register char   *rp;                    /* Result pointer       */

    fab.fab$l_nam = &nam;                   /* fab -> nam           */
    fab.fab$l_fna = source;                 /* Source filename      */
    fab.fab$b_fns = strlen(source);         /* Size of source       */
    fab.fab$l_dna = defstring;              /* Default string       */
    if (defstring != NULLST)
        fab.fab$b_dns = strlen(defstring);  /* Size of default      */
    nam.nam$l_esa = fullname;               /* Expanded filename    */
    nam.nam$b_ess = NAM$C_MAXRSS;           /* Expanded name size   */
    if (sys$parse(&fab) == RMS$_NORMAL) {   /* Parse away           */
        fullname[nam.nam$b_esl] = EOS;      /* Terminate string     */
        result[0] = EOS;                    /* Just in case         */
        rp = &result[0];
        /*
         * Remove stuff added implicitly, accepting node names and
         * dev:[directory] strings (but not process-permanent files).
         */
        if ((nam.nam$l_fnb & NAM$M_PPF) == 0) {
            if ((nam.nam$l_fnb & NAM$M_NODE) != 0) {
                strncpy(result, nam.nam$l_node, nam.nam$b_node);
                rp += nam.nam$b_node;
                *rp = EOS;
            }
            if ((nam.nam$l_fnb & NAM$M_EXP_DEV) != 0) {
                strncpy(rp, nam.nam$l_dev, nam.nam$b_dev);
                rp += nam.nam$b_dev;
            }
            if ((nam.nam$l_fnb & NAM$M_EXP_DIR) != 0) {
                strncpy(rp, nam.nam$l_dir, nam.nam$b_dir);
                rp += nam.nam$b_dir;
                *rp = EOS;
            }
        }
        if (defstring != NULLST) {
            strncpy(rp, nam.nam$l_name, nam.nam$b_name + nam.nam$b_type);
            rp += nam.nam$b_name + nam.nam$b_type;
            *rp = EOS;
            if ((nam.nam$l_fnb & NAM$M_EXP_VER) != 0) {
                strncpy(rp, nam.nam$l_ver, nam.nam$b_ver);
                rp[nam.nam$b_ver] = EOS;
            }
        }
        return (TRUE);
    }
    return (FALSE);
}
#endif

#if SYS_FAMILY == SYS_MSDOS

static char *
bsl2sl( char * filename)
/*
 * Convert '\\' in the path-list to '/'.
 */
{
    static int  diagnosed = FALSE;
    char *  cp;

    cp = filename;

    while (*cp) {
#if BSL_IN_MBCHAR
        if (type[ *cp & UCHARMAX] & MB1) {
            cp++;                           /* First byte of MBCHAR */
            if (type[ *cp & UCHARMAX] & MB2)
                cp++;                       /* Second byte of MBCHAR*/
            continue;
        }
#endif
        if (*cp == '\\') {
            *cp++ = PATH_DELIM;
#if 0	// '\\' 使ったっていいじゃん!
            if (diagnosed == FALSE && (warn_level & 2)) {
                cwarn(
        "Converted \\ to %s%.0ld%.0s", "/", 0L, NULLST);    /* _W2_ */
                diagnosed = TRUE;           /* Diagnose only once   */
            }
#endif
		} else {
            cp++;
        }
    }

    return  filename;
}

#endif

static const char * const   unknown_arg =
        "Unknown argument \"%s\"";      /*_W1_*/
#if MODE >= STANDARD || DEBUG || DEBUG_EVAL
static const char * const   not_ident =
        "Not an identifier \"%s\"";     /*_W1_*/
#endif

static int
#if PROTO
is_junk( void)
#else
is_junk()
#endif
{
#if ! OK_IF_JUNK
    int     c;

    c = skipws();
    unget();
    if (c != '\n') {                        /* Trailing junk        */
        if (warn_level & 1)
            cwarn( unknown_arg, infile->bptr, 0L, NULLST);
        return TRUE;
    } else {
        return FALSE;
    }
#else
	return FALSE;
#endif
}

#if MODE >= STANDARD

void
#if PROTO
dopragma( void)
#else
dopragma()
#endif
/*
 * Process the #pragma lines.
 * 1. Process the sub-directive for cpp.
 * 2. Pass the line to the compiler-proper who understands #pragma.
 *      #pragma __put_defines and #pragma __once are, however, not put out
 *      so as not to duplicate output when re-preprocessed.
 * 3. Warn and skip the line for the compiler-proper who can't understand
 * #pragma.
 *    When '__STDC_VERSION__' >= 199901L, the line is subject to macro
 * expansion unless the next to 'pragma' token is 'STDC'.
 */
{
    register int    c;
    int             warn = FALSE;       /* Necessity of warning for !PRAGMA */
    int             token_type;
    char *          bp;                     /* Pointer to argument  */
    char *          tp;
    char *          mp;
    char *          mp_end;
    FILEINFO *      file;
    DEFBUF *        defp;

    c = skipws();
    bp = infile->bptr - 1;  /* Remember token to pass to compiler   */
    if (c == '\n') {
        if (warn_level & 1)
            cwarn( "No sub-directive", NULLST, 0L, NULLST); /* _W1_ */
        unget();
        goto  parse_end;
    }
    token_type = scantoken( c, (tp = work, &tp), work_end);
#if EXPAND_PRAGMA
    if (stdc_ver >= 199901L && token_type == NAM
            && !str_eq( identifier, "STDC")) {
        bp = mp = xmalloc( (size_t)(NMACWORK + IDMAX));
                                    /* Buffer for macro expansion   */
        mp_end = mp + NMACWORK;
        tp = stpcpy( mp, identifier);
        do {                /* Expand all the macros in the line    */
            if (token_type == NAM && (defp = is_macro( &tp)) != NULL)
                tp = expand( defp, bp, mp_end);
            token_type = scantoken( c = get(), (bp = tp, &tp), mp_end);
        } while (c != '\n');
        ungetstring( mp, NULL);                     /* To re-read   */
        free( mp);
        c = skipws();
        bp = infile->bptr - 1;
        token_type = scantoken( c, (tp = work, &tp), work_end);
    }
#endif
    if (token_type != NAM) {
        if (warn_level & 1)
            cwarn( not_ident, work, 0L, NULLST);
    } else if (str_eq( identifier, "__put_defines")) {
        if (! is_junk()) {
            dumpdef();                      /* #pragma __put_defines*/
            goto  skipnl;
        }
    } else if (str_eq( identifier, "__once")) {
        if (! is_junk()) {
            file = infile;
            if (stdc_ver >= 199901L && file->fp == NULL)
                file = file->parent;
            doonce( file->filename);        /* #pragma __once       */
            goto  skipnl;
        }
#if DEBUG || DEBUG_EVAL
    } else if (str_eq( identifier, "__debug")) {
        warn = ! dodebug( TRUE);            /* #pragma __debug      */
    } else if (str_eq( identifier, "__end_debug")) {
        warn = ! dodebug( FALSE);           /* #pragma __end_debug  */
#endif
    } else {
        warn = TRUE;                        /* Need to warn         */
    }

#if COMPILER == CMP_UNKNOWN
    /*
     * Write here any compiler-specific #pragma sub-directive which should
     * be processed by preprocessor.
     */
#endif

parse_end:
    /*
     * If PRAGMA == TRUE, #pragma lines are handed to the compiler proper.
     * Else #pragma lines are warned and skipped.
     */
#if PRAGMA
#if COMPILER == LSIC
    if (!no_output) {
        if (std_line_prefix)
            fputs( "#pragma ", stdout);
        else
            fputs( "#p ", stdout);
    }
#else
    if (!no_output)
        fputs( "#pragma ", stdout);
#endif
    if (!no_output)
        fputs( bp, stdout);             /* Line is put out, though  */
    wrongline = TRUE;                   /*   it is a directive line */
#else
    if (warn && (warn_level & 1))
        cwarn( unknown_arg, identifier, 0L, NULLST);
    /* Else already warned by is_junk() or dodebug().   */
#endif
skipnl: /* Don't use skipnl() which skips to the newline in source file     */
    while (get() != '\n')
        ;
}

typedef struct inc_list {               /* List of #pragma once file*/
    struct inc_list *   next;           /* Next file                */
    char            fname[ 1];          /* Filename                 */
} INC_LIST;

static INC_LIST *   start_inc = NULL;   /* The first file in list   */
static INC_LIST *   last_inc;           /* The last file in list    */

void
#if PROTO
doonce( const char * filename)
#else
doonce( filename)
    char *  filename;
#endif
/*
 * Process #pragma __once so as not to re-include the file in future.
 * This directive has been imported from GNU C V.1.* / cpp as an extension.
 */
{
    register INC_LIST *     inc;
    size_t      fnamlen;

    fnamlen = strlen( filename);
    inc = (INC_LIST *) xmalloc( sizeof (INC_LIST) + fnamlen);
    memcpy( inc->fname, filename, fnamlen + 1);
    inc->next = NULL;
    if (start_inc == NULL)
        start_inc = last_inc = inc;         /* The first file       */
    else
        last_inc = last_inc->next = inc;    /* Append the file to the list  */
}

int
#if PROTO
included( const char * filename)
#else
included( filename)
    char *  filename;
#endif
/*
 * Has the file been once included ?
 */
{
    register INC_LIST *     inc;

    for (inc = start_inc; inc; inc = inc->next) {
        if (str_eq( inc->fname, filename)) {    /* Already included */
#if DEBUG
            if (debug & PATH)
                printf( "Once included \"%s\"\n", filename);
#endif
            return  TRUE;
        }
    }
    return  FALSE;                          /* Not yet included     */
}

#else   /* MODE < STANDARD  */

void
#if PROTO
doold( void)
#else
doold()
#endif
/*
 * Process the out-of-standard directives.
 */
{
    static const char * const   unknown
            = "Unknown #directive \"%s\"%.0ld%s";       /* _E_ _W2_ */

    if (str_eq( identifier, "assert")) {    /* #assert              */
        if (! compiling)                    /* Only validity check  */
            return;
        if (eval() == 0L) {                 /* Assert expression    */
            cerror( "Preprocessing assertion failed"        /* _E_  */
                    , NULLST, 0L, NULLST);
            skipnl();
            unget();
        }
        return;
    } else if (str_eq( identifier, "put_defines")) {
        if (! compiling)                    /* Only validity check  */
            return;
#if ! OK_IF_JUNK
        if (! is_junk())                    /* Warn at junk         */
#endif
            dumpdef();
        skipnl();
        unget();
        return;
    }

#if DEBUG || DEBUG_EVAL
    if (str_eq( identifier, "debug")) {     /* #debug <args>        */
        if (! compiling)                    /* Only validity check  */
            return;
        dodebug( TRUE);
        return;
    } else if (str_eq( identifier, "end_debug")) {
        if (! compiling)
            return;
        dodebug( FALSE);                    /* #end_debug <args>    */
        return;
    }
#endif

#if COMPILER == MWC09
    if (str_eq( identifier, "asm")) {       /* #asm                 */
        doasm( TRUE);
        return;
    }
    if (str_eq( identifier, "endasm")) {    /* #endasm              */
        doasm( FALSE);
        skipnl();                           /* Skip comments, etc.  */
        unget();
        return;
    }
#endif

    if (compiling)
        cerror( unknown, identifier, 0L, NULLST);
    else if (warn_level & 4)
        cwarn( unknown, identifier, 0L, " (in skipped block)");
    skipnl();
    unget();
    return;
}

#if COMPILER == MWC09

static void
#if PROTO
doasm( int asm)
#else
doasm( asm)
    int     asm;                            /* #asm ?               */
#endif
/*
 * #asm, #endasm
 */
{
    if (! compiling)
        return;
    if (asm == (in_asm != 0L)) {
        if (in_asm)
            cerror( "In #asm block started at line %.0s%ld" /* _E_  */
                    , NULLST, in_asm, NULLST);
        else
            cerror( "Without #asm", NULLST, 0L, NULLST);    /* _E_  */
        skipnl();
        unget();
        return;
    }
    in_asm = asm ? line : 0L;
}

#endif  /* MWC09    */

#endif  /* MODE >= STANDARD */

#if DEBUG || DEBUG_EVAL

static int
#if PROTO
dodebug( int set)
#else
dodebug( set)
    int     set;                        /* TRUE to set debugging    */
#endif
/*
 * #pragma __debug, #pragma __end_debug, #debug, #end_debug
 * Return TRUE when diagnostic is issued else return FALSE.
 */
{
    struct Debug_arg {
        const char *    arg_name;               /* Name of option   */
        int     arg_num;                        /* Value of 'debug' */
    };
    static struct Debug_arg     debug_args[] = {
#if DEBUG
#if MODE >= STANDARD
        { "__path",     PATH    },
        { "__token",    TOKEN   },
        { "__expand",   EXPAND  },
        { "__if",       IF      },
        { "__getc",     GETC    },
        { "__memory",   MEMORY  },
#else
        { "path",   PATH    },
        { "token",  TOKEN   },
        { "expand", EXPAND  },
        { "if",     IF      },
        { "getc",   GETC    },
        { "memory", MEMORY  },
#endif
#endif
#if DEBUG_EVAL
#if MODE >= STANDARD
        { "__expression",   EXPRESSION  },
#else
        { "expression", EXPRESSION  },
#endif
#endif
        { NULL,     0       },
    };
    struct Debug_arg    *argp;
    int     num;
    int     c;

    c = skipws();
    if (c == '\n') {
        unget();
        if (set) {
            if (warn_level & 1)
                cwarn( "No argument", NULLST, 0L, NULLST);  /* _W1_ */
            return TRUE;
        } else {
            debug = 0;                      /* Clear all the flags  */
            return FALSE;
        }
    }
    while (scantoken( c, (workp = work, &workp), work_end) == NAM) {
        argp = debug_args;
        while (argp->arg_name) {
            if (str_eq( argp->arg_name, work))
                break;
            argp++;
        }
        if (argp->arg_name == NULL) {
            if (warn_level & 1)
                cwarn( unknown_arg, work, 0L, NULLST);
            return TRUE;
        } else {
            num = argp->arg_num;
            if (set) {
                debug |= num;
#if DEBUG
                if (num == PATH)
                    dumppath();
                else if (num == MEMORY)
                    print_heap();
#endif
            } else {
                debug &= ~num;
            }
        }
        c = skipws();
    }
    if (c != '\n') {
        if (warn_level & 1)
            cwarn( not_ident, work, 0L, NULLST);
        skipnl();
        unget();
        return TRUE;
    }
    unget();
    return FALSE;
}

#endif

#if MODE < STANDARD

void
#if PROTO
put_asm( void)
#else
put_asm()
#endif
/*
 * Put out source line as it is.
 */
{
#if COMPILER == MWC09
    fputs( "#2\n", stdout);
    fputs( infile->buffer, stdout);
    skipnl();
#endif
}

#endif

void
#if PROTO
put_source( const char * src)
#else
put_source( src)
    char *  src;
#endif
/*
 * Output a physical source line as a comment to compiler-proper.
 */
{
#if COMPILER == MWC09
    sharp();
    fputs( "#6\n", stdout);
#if ! COMMENT_INVISIBLE
    fputs( src, stdout);
#else
    put_line( src, stdout);
#endif
#endif
}

#if DEBUG

static void
#if PROTO
dumppath( void)
#else
dumppath()
#endif
{
    const char **   incptr;

    fputs( "Include paths are as follows --\n", stdout);
    for (incptr = incdir; incptr < incend; incptr++)
        printf( "    %s\n", *incptr);
}

/*
 * list_heap() is a function to print out information of heap-memory.
 * See "memory23.lzh" by Psycho.
 */
#if PSYCHO
#ifdef  __cplusplus
    extern "C"  int     list_heap( int);
#else
#if PROTO
    int     list_heap( int);
#else
    int     list_heap();
#endif
#endif
#endif
#if BSD_MALLOC
    int     list_heap( char *);
#else
#if DB_MALLOC || DMALLOC
    int     list_heap( FILE *);
#endif
#endif

void
#if PROTO
print_heap( void)
#else
print_heap()
#endif
{
#if PSYCHO
    list_heap( 1);
#else
#if BSD_MALLOC
    list_heap( ":cpp");
#else
#if DB_MALLOC || DMALLOC || PHK_MALLOC || DLMALLOC
    list_heap( stdout);
#endif
#endif
#endif
}

#endif  /* DEBUG    */

#if COMPILER == MWC09
static void
printlong()
{
    pflinit();      /* Enable printf() to print long    */
}
#endif

