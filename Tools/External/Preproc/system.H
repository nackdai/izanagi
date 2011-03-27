/*
 *                          S Y S T E M . H
 *                  S y s t e m   D e p e n d e n t
 *              D e f i n i t i o n s   f o r   C P P
 *
 * Definitions in this file may be edited to configure CPP for particular
 * operating systems and compiler configurations.
 *
 * NOTE: cpp assumes it is compiled by a compiler that supports macros
 * with arguments.
 *   cpp also assumes the system implement the Ascii character set.
 * If this is not the case, you will have to do some editing here and there.
 *
 * 1984/05 - 1985/06    CPP / cppdef.h
 *              Created and released on USENET          by Martin Minow
 * 1998/08              CPP Version 2.0 / system.H
 *              Re-written and renamed from cppdef.h    by Psycho
 * 1998/09              CPP Version 2.1 / system.H
 *              Updated according C9X-1998/08 draft     by Psycho
 * 1998/11              CPP Version 2.2 / system.H
 *              Updated according C++ Standard (ISO/IEC 14882)  by Psycho
 */

#define SYSTEM_H

/*
 *  'Target' means the O.S. and the compiler to which cpp is implemented.
 *  'Host' means the O.S. and the compiler with which cpp is compiled.
 */

#ifdef WIN32
/* Define target operating-system.  */
#define SYSTEM              SYS_WIN32
/* Define target compiler.          */
#define COMPILER            CMP_UNKNOWN
#else
/* Define target operating-system.  */
#define SYSTEM              SYS_LINUX
/* Define target compiler.          */
#define COMPILER            CMP_UNKNOWN
#endif

#define NINCLUDE	0x80
#define ENV_C_INCLUDE ""

/* Define host operating-system.    */
#define HOST_SYSTEM         SYSTEM

/* Define host compiler.            */
#define HOST_COMPILER       COMPILER

/* Version message.                 */
#define VERSION     \
"cpp v.2.2 (1998/11) STANDARD mode for GNU C compiled by GNU C v.2.7.2.1\n"
#if     0
"cpp v.2.2 (1998/11) STANDARD mode for GNU C compiled by GNU C v.2.7.2.1\n"
"CPP V.2.2 (1998/11) STANDARD mode for DJGPP compiled by DJGPP (GNU C 2.7.2)\n"
"CPP V.2.2 (1998/11) STANDARD mode for Borland C compiled by BCC V.4.0J\n"
"CPP V.2.2 (1998/11) STANDARD mode for Turbo C compiled by Turbo C V.2.0\n"
"CPP V.2.2 (1998/11) STANDARD mode for LSI C-86 compiled by LSI C-86 V.3.3\n"
"CPP V.2.2 (1998/11) STANDARD mode for MWC09 compiled by MWC09\n"
#endif

#define TRUE                1
#define FALSE               0

/*
 *      P A R T   1     Configurations for target-operating-system
 *                      and target-compiler.
 */

/*
 * Names of the SYSTEM (i.e. target operating system).  This is needed so that
 * cpp can use appropriate filename conventions.
 */
#define SYS_UNKNOWN         0
#define SYS_UNIX            0x1000
#define SYS_ULTRIX          0x1100  /* (SYS_ULTRIX & 0xF000) == SYS_UNIX    */
#define SYS_LINUX           0x1800  /* (SYS_LINUX & 0xF000) == SYS_UNIX     */
#define SYS_FREEBSD         0x1C00  /* (SYS_FREEBSD & 0xF000) == SYS_UNIX   */
#define SYS_OS9             0x2000
#define SYS_OSK             0x2100  /* (SYS_OSK & 0xF000)   == SYS_OS9      */
#define SYS_OS9000          0x2200  /* (SYS_OS9000 & 0xF000) == SYS_OS9     */
#define SYS_VMS             0x4000
#define SYS_MAC             0x6000
#define SYS_MSDOS           0x7000
#define SYS_DJGPP           0x7400  /* (SYS_DJGPP & 0xF000) == SYS_MSDOS    */
#define SYS_HUMAN           0x7600  /* (SYS_HUMAN & 0xF000) == SYS_MSDOS    */
#define SYS_OS2             0x7800  /* (SYS_OS2 & 0xF000)   == SYS_MSDOS    */
#define SYS_WIN32           0x7C00  /* (SYS_WIN32 & 0xF000) == SYS_MSDOS    */

/* COMPILER */
#define CMP_UNKNOWN         0
#define PCC                 0x1000  /* Portable CC  */
#define LINUX_GCC           0x18F0  /* (LINUX_GCC & 0xF0)   == GNUC */
#define FREEBSD_CC          0x1CF0  /* (FREEBSD_CC & 0xF0)  == GNUC */
#define MWC09               0x2000
#define MWC68K              0x2100
#define ULTRAC              0x2200
#define VAX11C              0x4000
#define VAXC                0x4010
#define DECC                0x4020
#define MWPC                0x6000
#define MSC                 0x7000  /* Microsoft C, Visual C++      */
#define QUICKC              0x7010
#define TURBOC              0x7040  /* Turbo C, Borland C   */
#define LSIC                0x7060
#define DOS_SYMANTECC       0x7070
#define LATTICEC            0x7080
#define DJGPP               0x74F0  /* (DJGPP & 0xF0) == GNUC       */
#define XC                  0x7600
#define XGCC                0x76F0  /* (XGCC & 0xF0)  == GNUC       */
#define GNUC                0x00F0

#define SYS_FAMILY          (SYSTEM & 0xF000)
#define CMP_FAMILY          (COMPILER & 0xF0)
#define HOST_SYS_FAMILY     (HOST_SYSTEM & 0xF000)

#if     SYS_FAMILY == SYS_VMS || HOST_SYS_FAMILY == SYS_VMS
#if     SYSTEM != HOST_SYSTEM
    error  <<cpp for DEC O.S. should be compiled on the same O.S.>>
#endif
#endif

/* MBCHAR (multi-byte character) encoding.  */
#define SJIS            0x10    /* Shift-JIS encoding of JIS X 0208 */
#define EUC             0x20    /* Extended UNIX code of JIS X 0208 */
#define KS_C_5601       0x30    /* EUC-like encoding of Korean KS C 5601    */
#define GB_2312         0x40    /* EUC-like encoding of Chinese GB 2312-80  */
#define BIGFIVE         0x50    /* EUC-like encoding of Taiwan Big Five     */

/*
 * In order to predefine target-machine-dependent macros,
 * several macros are defined here:
 *
 * CPU_OLD,_STD     define the target cpu (by name)
 * SYSTEM_OLD,_STD  define the target operating system (by name)
 * SYSTEM_EXTRA     define the target operating system (by another name)
 * CMP_OLD,_STD     define the target compiler (by name)
 * CMP_EXTRA        define the target compiler (by another name)
 * CMP_CPLUS        define the target compiler as C++
 *      The above nine may not be defined, if they are not needed.
 *      They should not be #defined to no token or to "".
 *
 * SYS_EXT_VAL, CMP_STD_VAL, CMP_EXT_VAL, CMP_CPLUS_VAL
 *      specify the values of SYSTEM_EXTRA, CMP_STD, CMP_EXTRA, CMP_CPLUS.
 *      If not specified, these values default to "1".
 *      To define the value of no-token, specify as "" rather than no-token.
 */

/*
 * target-compiler-dependent definitions:
 *
 * LINE_PREFIX  defines the # output line prefix, if not "#line 123".
 *              This should be defined as " " to represent "# 123" format
 *              ("line " represents "#line 123" format).
 *
 * PRAGMA       should be set TRUE, if COMPILER can handle the
 *              #pragma lines.
 *
 * C_INCLUDE    may be defined if you have a site-specific include directory
 *              which is to be searched *before* the operating-system
 *              specific directories.
 * CPP_INCLUDE  is for C++ include directory which exist other than C_INCLUDE.
 * ENV_C_INCLUDE    may be defined to the name of environment-variable for
 *              C include directory.
 * ENV_CPP_INCLUDE  is name of environment-variable for C++ include directory
 *              which exists other than ENV_C_INCLUDE.
 * ENV_SEP      is the separator (other than space) of include-paths in an
 *              environment-variable.  e.g. the ':' in
 *              "/usr/abc/include:/usr/xyz/include"
 * EMFILE       should be defined to the macro to represent errno of 'too many
 *              open files' if the macro is different from EMFILE.
 */

#if     SYSTEM == SYS_UNIX
#define SYSTEM_OLD          "izix"
#define SYSTEM_STD          "__unix__"
#define SYSTEM_EXTRA        "__unix"
#endif

#if     SYSTEM == SYS_ULTRIX
#define CPU_OLD             "vax"       /* "mips"       */
#define CPU_STD             "__VAX"     /* "__mips", "__MIPSEL", "__MIPSEB" */
#define SYSTEM_OLD          "ultrix"
#define SYSTEM_STD          "__ULTRIX"
#endif

#if     SYSTEM == SYS_FREEBSD
#define CPU_STD             "__i386__"
#define SYSTEM_OLD          "izix"
#define SYSTEM_STD          "__unix__"
#define SYSTEM_EXTRA        "__FreeBSD__"
#define SYS_EXT_VAL         "2"         /* V.1.*: 1, V.2.*: 2, V.3.*: 3 */
#define C_INCLUDE           "/usr/local/include"
#define CPP_INCLUDE         "/usr/include/g++"
#endif

#if     SYS_FAMILY == SYS_UNIX
#define MBCHAR              EUC
#endif

#if     SYSTEM == SYS_VMS
#define CPU_OLD             "vax"
#define CPU_STD             "__VAX"     /* "__ALPHA"    */
#define SYSTEM_OLD          "vms"
#define SYSTEM_STD          "__VMS"
#endif

#if     COMPILER == DECC
#define CMP_STD             "__DECC"
#define CMP_EXTRA           "__DECCXX"  /* for DEC C++  */
#endif

#if     COMPILER == VAX11C
#define CMP_OLD             "vax11c"
#define CMP_STD             "__VAX11C"
#endif

#if     COMPILER == VAXC
#define CMP_OLD             "vaxc"
#define CMP_STD             "__VAXC"
#endif

#if     SYSTEM == SYS_MSDOS || SYSTEM == SYS_WIN32
/*
 *      Here we assume that the operating system is MS-DOS or WIN32, the
 * processor is an Intel 8086 (or i386, etc.) and #pragma directive, unsigned
 * long and long double data types are available for the compilers.
 */
#define FILENAMEMAX         FILENAME_MAX

#if     SYSTEM == SYS_MSDOS
#define CPU_STD             "__8086__"
#define SYSTEM_OLD          "MSDOS"
#define SYSTEM_STD          "__MSDOS__"
#else
#define CPU_STD             "__i386__"
#define SYSTEM_OLD          "WIN32"
#define SYSTEM_STD          "__WIN32__"
#define SYSTEM_EXTRA        "__FLAT__"
#endif

#define MBCHAR              SJIS    /* Multi-byte char is encoded in SJIS   */
#define PRAGMA              TRUE    /* Compiler proper can handle #pragma   */
#define HAVE_ULONG          TRUE    /* Compiler proper has unsigned long    */
#define HAVE_LDBL           TRUE    /* Compiler proper has long double      */

#if     COMPILER == MSC
#define CMP_STD             "_MSC_VER"
#define CMP_STD_VAL         "700"   /* MSC 6.0: "600", MSC 7.0: "700"       */
#define C_INCLUDE           "/MSC7/INCLUDE"
/* #define CPP_INCLUDE         "/MSC7/CPP_INCLUDE"    */
#define ESC_MBCHAR          FALSE   /* I don't know, define by yourself.    */
#else

#if     COMPILER == QUICKC
#define CMP_STD             "_MSC_VER"
#define CMP_EXTRA           "_QC"
#define C_INCLUDE           "/QC/INCLUDE"
/* #define CPP_INCLUDE         "/QC/CPP_INCLUDE"  */
#define ESC_MBCHAR          FALSE   /* I don't know, define by yourself.    */
#else

#if     COMPILER == TURBOC
#define CMP_STD             "__TURBOC__"
#if     __BORLANDC__        /* Borland C    */
#define CMP_STD_VAL         "0x0452"
    /* BC(C++) 2.0: "0x0297", BC(C++) 3.0: "0x0400", BC(C++) 4.0: "0x0452"  */
#define CMP_EXTRA           "__BORLANDC__"
#define CMP_EXT_VAL         "0x0452"
    /* BC(C++) 2.0: "0x0200", BC(C++) 3.0: "0x0400", BC(C++) 4.0: "0x0452"  */
#define CMP_CPLUS           "__BCPLUSPLUS__"
#define CMP_CPLUS_VAL       "0x0320"
    /* BC++ 2.0: "0x0200, BC++ 3.0: "0x0300", BC++ 4.0: "0x0320"    */
#define C_INCLUDE           "/BC4/INCLUDE"
#else
#if     __TCPLUSPLUS__      /* Turbo C++    */
#define CMP_STD_VAL         "0x0296"        /* TC++ 1.0: "0x0296"   */
#define CMP_CPLUS           "__TCPLUSPLUS__"
#define CMP_CPLUS_VAL       "0x0100"        /* TC++ 1.0: "0x0100"   */
#define C_INCLUDE           "/TCPP1/INCLUDE"
#else                       /* Turbo C      */
#define CMP_STD_VAL         "0x0200"        /* TC 2.0: "0x0200"     */
#define C_INCLUDE           "/TC2/INCLUDE"
#endif
#endif
#define ESC_MBCHAR          FALSE
#else

#if     COMPILER == LSIC
#define CMP_STD             "__LSIC__"
#define CMP_OLD             "LSI_C"
#define LINE_PREFIX         " "
#define C_INCLUDE           "/LSIC86/INCLUDE"
#define ESC_MBCHAR          FALSE
#endif
#endif
#endif
#endif

#endif  /* SYSTEM == SYS_MSDOS || SYSTEM == SYS_WIN32   */

#if     SYSTEM == SYS_DJGPP
#define CPU_STD             "__i386__"
#define SYSTEM_STD          "__GO32__"
#define MBCHAR              SJIS
#define FILENAMEMAX         FILENAME_MAX
#define CMP_STD             "__DJGPP__"
#define CMP_STD_VAL         "1"     /* "1" for DJGPP V.1.*, "2" for V.2.*   */
#define HAVE_LLONG          FALSE
#endif  /* SYSTEM == SYS_DJGPP  */

#if     CMP_FAMILY == GNUC
#define CMP_EXTRA           "__GNUC__"
#define CMP_EXT_VAL         "2"     /* "1" for GNU C V.1.*, "2" for V.2.*   */
#define CMP_CPLUS           "__GNUG__"
#define CMP_CPLUS_VAL       "2"     /* "1" for GNU C++ V.1.*, "2" for V.2.* */
#define PRAGMA              TRUE
#define HAVE_ULONG          TRUE
#ifndef HAVE_LLONG
#define HAVE_LLONG          TRUE
#define LL_FORM             "q"
#endif
#define HAVE_LDBL           TRUE
#ifndef ENV_C_INCLUDE
#define ENV_C_INCLUDE       "C_INCLUDE_PATH"
#define ENV_CPP_INCLUDE     "CPLUS_INCLUDE_PATH"
#endif
#endif

#if     SYS_FAMILY == SYS_OS9
#define SYSTEM_OLD          "OS9"
#define MBCHAR              SJIS
#define C_INCLUDE           "/D0/INCLUDE"
#if     COMPILER == MWC09
#define CPU_STD             /* "__MC6809__" */  "__HD63C09__"
#define SYSTEM_STD          "__OS9__"
#define CMP_STD             "__MWC09__"
#define LINE_PREFIX         "5\n"
#define HAVE_ALERT          FALSE
#else
#if     COMPILER == MWC68K
#define CPU_STD             "__m68k__"
#define SYSTEM_STD          "__OSK__"
#define CMP_STD             "__MWC68K__"
#else
#if     COMPILER == ULTRA_C
#define CPU_STD             "__m68k__"      /* "__i386__", ...      */
#define SYSTEM_STD          "__OSK__"       /* "__OS9000__"         */
#define CMP_STD             "__ULTRAC__"
#define PRAGMA              TRUE
#define HAVE_ULONG          TRUE
#define HAVE_LDBL           TRUE
#endif
#endif
#endif
#endif  /* SYSTEM == SYS_OS9    */

/*
 * defaults
 */

#ifdef  SYSTEM_EXTRA
#ifndef SYS_EXT_VAL
#define SYS_EXT_VAL         "1"
#endif
#endif
#ifdef  CMP_STD
#ifndef CMP_STD_VAL
#define CMP_STD_VAL         "1"
#endif
#endif
#ifdef  CMP_EXTRA
#ifndef CMP_EXT_VAL
#define CMP_EXT_VAL         "1"
#endif
#endif
#ifdef  CMP_CPLUS
#ifndef CMP_CPLUS_VAL
#define CMP_CPLUS_VAL       "1"
#endif
#endif

#ifndef LINE_PREFIX
#define LINE_PREFIX         "line "
#define STD_LINE_PREFIX     TRUE    /* C source format              */
#else
#define STD_LINE_PREFIX     FALSE   /* Compiler-dependent format    */
#endif

#ifndef PRAGMA
#define PRAGMA              FALSE
#endif

#ifndef ENV_C_INCLUDE
#define ENV_C_INCLUDE       "INCLUDE"
#endif
#ifndef ENV_CPP_INCLUDE
#define ENV_CPP_INCLUDE     "CPP_INCLUDE"
#endif

#ifndef ENV_SEP
#if     SYS_FAMILY == SYS_MSDOS
#define ENV_SEP             ';'
#else
#define ENV_SEP             ':'
#endif
#endif

/*
 * These three definitions should be set TRUE, if *both* of the target
 * and the host compilers have the specified data type.
 * The latter two are used only by #if sizeof evaluation routine.
 * These definitions are written in PART 1 for convenience.
 */
/* The target and host compilers both have the type unsigned long ? */
#ifndef HAVE_ULONG
#define HAVE_ULONG          FALSE
#endif
/* The target and host compilers both have the type long long int ? */
#ifndef HAVE_LLONG
#define HAVE_LLONG          FALSE
#endif
/* The target and host compilers both have the type long double ?   */
#ifndef HAVE_LDBL
#define HAVE_LDBL           FALSE
#endif

/* Define the format specifier of long long for printf().   */
#if     HAVE_LLONG
#ifndef LL_FORM
#define LL_FORM             "ll"        /* "ll": C9X 1998/08 draft  */
#endif
#endif

/*
 * CHARBIT, UCHARMAX are respectively CHAR_BIT, UCHAR_MAX of target compiler.
 * CHARBIT should be defined to the number of bits per character.
 * It is needed only for processing of multi-byte character constants.
 * UCHARMAX should be defined to the maximum value of type unsigned char
 * or maximum value of unsigned int which is converted from type (signed)
 * char.
 *
 * LONGMAX      should be defined to the LONG_MAX in <limits.h>.
 * ULONGMAX     should be defined to the ULONG_MAX in <limits.h> or LONG_MAX
 *              for the compiler which lacks of unsigned long.
 */

#ifndef CHARBIT
#define CHARBIT             8
#endif
#ifndef UCHARMAX
#define UCHARMAX            255
#endif
#ifndef LONGMAX
#define LONGMAX             2147483647L
#endif
#ifndef ULONGMAX
#define ULONGMAX            4294967295UL
                            /* or 2147483647L in case of !HAVE_ULONG    */
#endif

/* FILENAMEMAX should be defined to FILENAME_MAX of target system.  */
#ifndef FILENAMEMAX
#define FILENAMEMAX         BUFSIZ
#endif

/*
 * FOLD_CASE    should be set TRUE if upper-case and lower-case letters are
 *              not distinguished by O.S. in the options handed to cpp.
 */
#ifndef FOLD_CASE
#define FOLD_CASE           FALSE
#endif

/*
 * Define MBCHAR (multi-byte character encoding) to SJIS, EUC or other.
 * We suppose that any multi-byte character other than single-byte character
 * in source text is 2 bytes long.  The encodings which have shift-states are
 * not supported.
 */
#ifndef MBCHAR
#define MBCHAR              0
#endif

/*
 * BSL_IN_MBCHAR (backslash in multi-byte character) shoud be set TRUE if
 * the second byte of MBCHAR encoding, which has no shift-states, may has a
 * code identical to '\\'.
 */
#if     MBCHAR == SJIS || MBCHAR == BIGFIVE
#define BSL_IN_MBCHAR       TRUE
#else
#define BSL_IN_MBCHAR       FALSE
#endif

/*
 * ESC_MBCHAR (escape '\\' in multi-byte character) enables cpp to insert '\\'
 * before '\\' of the 2nd byte of MBCHAR code in literal.  This insertion is
 * done for the compiler-proper which can't recognize MBCHAR literal.
 */
#if     BSL_IN_MBCHAR
#ifndef ESC_MBCHAR
#define ESC_MBCHAR          TRUE    /* or FALSE following your compiler     */
#endif
#else
#define ESC_MBCHAR          FALSE
#endif

#ifndef HAVE_ALERT
#define HAVE_ALERT          TRUE
#endif

/*
 * These are used to implement the Standard C escape sequences '\a' and '\v'.
 */
#if     ! HAVE_ALERT
        /* 1: for the target compiler-proper which can't recognize \a, \v   */
#define ALERT_STR           "\\007"
#define VT_STR              "\\013"
#endif

/*
 *      P A R T   2     Configurations for host-compiler.
 */

#if     0
        /* 1: for the host compiler-proper which can't recognize \a, \v     */
#define ALERT               '\007'          /* '\a' is "Bell"       */
#define VT                  '\013'          /* Vertical Tab CTRL/K  */
#endif

/*
 * PROTO    means that host-compiler permits prototype declarations.
 */
#define PROTO               TRUE

#if     PROTO
#define register
#else
#define const
#endif

#if     HOST_COMPILER == LSIC
#define const               /* To work around some bugs of LSIC     */
#endif

/*
 * HAVE_ENV should be defined TRUE, if the HOST_COMPILER has the function
 * getenv().
 */
#define HAVE_ENV            TRUE

#if     HOST_COMPILER == MWC09
/*
 * Qualifier to variables which are located at "direct page" and accessible
 * by 8 bits offset from base register.
 */
#define D09                 direct
#else
#define D09
#endif

/*
 * Declaration of standard library functions and macros.
 */

/* stdin, stdout, stderr, FILE, NULL, putchar(), putc() and other functions.*/
#include    "stdio.h"

/* islower(), isupper(), toupper(), isdigit(), isxdigit(), iscntrl()        */
#include    "ctype.h"

/* errno    */
#include    "errno.h"

#if     0
extern FILE *   fopen();
extern FILE *   freopen();
extern int      fclose();
extern int      fseek();
extern long     ftell();
extern int      fprintf();
extern int      printf();
extern int      sprintf();
extern int      fputs();
extern char *   fgets();
extern int      ferror();
#if     HOST_SYSTEM == SYS_VMS
extern void     perror();
#endif
#endif

#if     1

#include    "string.h"
#include    "stdlib.h"
#include    "stddef.h"
#include    "time.h"        /* For MODE != CON_FALSE    */

#if     0
#ifdef  __cplusplus
extern "C" char *   memmove( char * dest, const char * src, size_t size);
#else
extern char *   memmove( char * dest, const char * src, size_t size);
#endif
#endif

#else   /* for the COMPILER which has not the standard header files */

#if     0
typedef unsigned    size_t;
typedef long        time_t;
#endif

#if     0
#define strchr( s, c)       index( (s), (c))
#define strrchr( s, c)      rindex( (s), (c))
#define memcpy( d, s, n)    bcopy( (s), (d), (n))
#define memcmp( s1, s2, n)  bcmp( (s1), (s2), (n))
extern char *   index();
extern char *   rindex();
extern char *   bcopy();
extern int      bcmp();
#else
extern char *   strchr();
extern char *   strrchr();
extern char *   memcpy();
extern int      memcmp();
#endif

extern void     exit();
extern char *   malloc();
extern void     free();
extern char *   realloc();
extern int      strcmp();
extern size_t   strlen();
extern char *   strcpy();
extern char *   strcat();
extern size_t   strcspn();
extern char *   memmove();
extern time_t   time();
extern char *   ctime();

#if     HAVE_ENV
extern char *   getenv();
#endif

#endif  /* declaration of standard library functions    */

/* Functions other than standard.   */
#if     PROTO
#ifdef  __cplusplus
extern "C" int  getopt( int argc, char * const * argv, const char * opts);
extern "C" char *   stpcpy( char * dest, const char * src);
#else   /* #ifndef __cplusplus  */
extern int      getopt( int argc, char * const * argv, const char * opts);
extern char *   stpcpy( char * dest, const char * src);
#endif
#else   /* ! PROTO  */
extern int      getopt();
extern char *   stpcpy();
#endif

/* For debugging malloc systems */
#if     __cplusplus && ((PSYCHO && _MEM_DEBUG) || DB_MALLOC || DMALLOC)
extern "C" {
#endif
#if     PSYCHO && _MEM_DEBUG
#include        "xalloc.h"
#else
#if     DB_MALLOC
#include        "malloc.h"
#else
#if     DMALLOC
#include        "dmalloc.h"
#endif
#endif
#endif
#if     __cplusplus && ((PSYCHO && _MEM_DEBUG) || DB_MALLOC || DMALLOC)
}
#endif

/*
 *      P A R T   3     Configurations for mode of macro expansion, mode of
 *                      tokenization, available preprocessing directives and
 *                      translation limits.
 */

/*
 * MODE     specifies the mode of preprocessing -- i.e. handling of trans-
 *      lation phases; handling of some preprocessing tokens; availability
 *      of some directives; mode of macro expansion; -- as follows.
 *
 * pre-Standard modes (CON_FALSE, CON_NOEXPAND, CON_EXPAND)     Actual
 *      arguments of a macro are expanded (after substitution) with rest of
 *      the replacement text and the subsequent source text.
 *          Intermediately recursive macro call causes an error.
 * CON_FALSE        <backslash><newline> sequence is deleted only in string
 *      literals and in #define directive lines.
 *          ## has no significance to cpp.  The tokens are macro-expanded
 *      separately.
 *          # has no significance to cpp.  The following token is expanded.
 *          Directly or intermediately recursive macro call causes an error.
 *          CON_FALSE corresponds to the "K&R 1st." preprocessing.
 * CON_NOEXPAND, CON_EXPAND     <backslash><newline> sequence is deleted only
 *      in string literals and in directive lines.
 * CON_NOEXPAND ## concatenates tokens.  The tokens are macro-expanded prior
 *      to concatenation.  The concatenated token is not expanded.
 *          # stringizes the following argument.  The argument is expanded
 *      prior to stringizing.
 *          Intermediately recursive macro call causes an error.
 * CON_EXPAND   ## concatenates tokens.  The tokens are macro-expanded prior
 *      to concatenation.  The concatenated token is re-expanded.
 *          # stringizes the following argument.  The argument is expanded
 *      prior to stringizing.
 *          Intermediately recursive macro call causes an error.
 *
 * STANDARD     <backslash><newline> sequence is always deleted after trigraph
 *      conversion and before tokenization.
 *          Digraph sequences are recognized as tokens.
 *          Actual arguments of a macro are expanded separately and prior to
 *      re-scanning of the replacement text.
 *          The name in the replacement text once expanded is not re-expanded,
 *      thus preventing recursive death.
 *          ## concatenates tokens.  The tokens are not expanded.  The con-
 *      catenated token is expanded by rescanning.
 *          # stringizes the following argument.  The argument is not
 *      expanded.  \ is inserted before " and \ in or surrounding the string
 *      literal or character constant (except in MBCHAR literals).  White-
 *      spaces between the tokens are squeezed to a space, \n is converted to
 *      a space.
 *          An expanded macro is surrounded by spaces to prevent unintended
 *      token merging.
 * POST_STANDARD    Simplifies the behavior of STANDARD mode as follows.
 *          1. Digraph sequences are treated as another trigraphs in
 *      translation phase 1, rather than as tokens.
 *          2. A space is inserted as a token separator between any tokens in
 *      a source (except a macro name and the next '(' in macro definition) :
 *      thus simplifying tokenization, test of macro redefinition and macro
 *      expansion especially "stringization".
 *          3. Test of macro redefinition ignores difference of parameter
 *      names, test of which has little utility and not a little overhead.
 *          4. #if expression forbids character constants, which have little
 *      portability, little utility and not a little overhead.
 *          5. Rescanning of a macro expansion is limited in the replacement
 *      text, rest of the source file is not scanned, thus making the syntax
 *      of "function-like" macro call more similar to that of function call.
 *          6. Argument of #include directive in <header.h> form is an obso-
 *      lescent feature.
 *          7. '$' or so are not treated specially in #define directive.
 *
 * The following specifications are available when MODE >= STANDARD.
 *      preprocessing number token, digraphs,
 *      #pragma (#pragma __put_defines, #pragma __once, #pragma __debug)
 *          directive,
 *      #error directive,
 *      predefined macro __STDC__, __STDC_VERSION__,
 *      wide character constant, wide character string literal,
 *      '+' option (C++ preprocessing),
 *      'S<n>' option (pre-defines __STDC__ to <n>, unpredefine some macros).
 * The following specifications are available in the modes other than
 * CON_FALSE.
 *      trigraphs,
 *      #if defined operator, #elif directive,
 *      macro as an argument of #include, #line directives,
 *      predefined macro __FILE__, __LINE__, __DATE__, __TIME__,
 *      escape sequences \x[hex-digits], \a, \v.
 *      concatenation of string literals.
 * The following specifications are available when MODE < STANDARD.
 *      #assert, #asm, #endasm, #put_defines, #debug
 *          and some other older directives,
 *      argument of #line directive other than dicimal-digits or out of range
 *          of [1,32767].
 * The following specifications are available in CON_NOEXPAND, CON_EXPAND.
 *      integer number token with suffix of 'u' or 'U',
 *      floating point number token with suffix of 'l', 'L', 'f' or 'F',
 *      # integer [file-name] directive as a synonym of #line directive.
 */
#define CON_FALSE           0
#define CON_NOEXPAND        1
#define CON_EXPAND          2
#define STANDARD            3
#define POST_STANDARD       9

/* MODE         Specify the preprocessing mode. */
#define MODE                STANDARD

#if     MODE >= STANDARD
#define COMMENT_INVISIBLE   FALSE
#define STRING_FORMAL       FALSE
#define OK_IF_JUNK          TRUE
#define OK_SIZE             FALSE
#define OK_UNTERM_STRING    FALSE
#endif

#if     MODE != CON_FALSE
#define OLD_PREPROCESSOR    FALSE
#endif

#if     MODE == CON_FALSE
#define CONCAT_STRINGS      FALSE
#endif

#if     SYS_FAMILY == SYS_VMS
#define DOLLAR_IN_NAME      TRUE
#endif

/*
 * OLD_PREPROCESSOR Forces several parameters to a state consistant
 *              with the Reiser cpp preprocessor.
 */
#ifndef OLD_PREPROCESSOR
#define OLD_PREPROCESSOR    FALSE   /* TRUE for "Reiser" model cpp  */
#endif
#if     OLD_PREPROCESSOR
#define OK_IF_JUNK          TRUE
#define COMMENT_INVISIBLE   TRUE
#define STRING_FORMAL       TRUE
#define OK_UNTERM_STRING    TRUE
#endif

/*
 * COMMENT_INVISIBLE may be defined to allow "old-style" comment
 * processing, whereby the comment becomes a zero-length token
 * separator.  This permitted tokens to be concatenated in macro
 * expansions.  This is disallowed by the Standard.
 */
#ifndef COMMENT_INVISIBLE
#define COMMENT_INVISIBLE   FALSE
#endif

/*
 * OK_IF_JUNK   should be set TRUE to allow text after #else and
 *              #endif.  This is needed for compatiblity with old
 *              preprocessors, but is disallowed by the Standard.
 */
#ifndef OK_IF_JUNK
#define OK_IF_JUNK          FALSE
#endif

/*
 * STRING_FORMAL may be defined to allow recognition of macro parameters in
 * string literals and character constant in replacement text.
 * This was removed from the Standard.
 */
#ifndef STRING_FORMAL
#define STRING_FORMAL       FALSE
#endif

/*
 * OK_UNTERM_STRING ebables implicit closing of quote by end of line.
 */
#ifndef OK_UNTERM_STRING
#define OK_UNTERM_STRING    FALSE
#endif

/*
 * OK_SIZE permits sizeof operator in #if expression.
 * See definitions of S_CHAR, etc. in eval.c.
 * This is disallowed by the Standard.
 */
#ifndef OK_SIZE
#define OK_SIZE             TRUE
#endif

/*
 * TFLAG_INIT   Initial value for the -3 option.  If TRUE -3
 *              disables trigraphs, if FALSE -3 enables them.
 */
#if     MODE != CON_FALSE
#define OK_TRIGRAPHS        TRUE
#if     OK_TRIGRAPHS
#define TFLAG_INIT          FALSE
#endif
#endif

/*
 * DIGRAPHS_INIT    Initial value for the -2 option.  If TRUE -2
 *              disables digraphs, if FALSE -2 enables them.
 * CONV_DIGRAPHS    Convert digraphs to usual tokens in the supplementary
 *              phase of preprocessing.
 *
 * OK_PRAGMA_OP     Enable _Pragma() operator by -S199901L option.
 * OK_UCN       Enable recognition of Universal-Character-Name sequence
 *              by -S199901L option.
 * OK_MBIDENT   Enable multi-byte characters in identifier by -S199901L
 *              option.
 * EXPAND_PRAGMA    Enable macro expansion of #pragma line.
 * expr_t, uexpr_t      Type of maximum integer. long (unsigned long) for C90
 *              and long long (unsigned long long) for C99.
 * EXPR_MAX should be defined to the maximum value of uexpr_t,
 *              or maximum value of expr_t in case of !HAVE_ULONG.
 */
#if     MODE >= STANDARD
#define OK_DIGRAPHS         TRUE
#if     OK_DIGRAPHS
#define DIGRAPHS_INIT       FALSE
#if     MODE == STANDARD
#define CONV_DIGRAPHS       FALSE
#endif
#endif
#define OK_PRAGMA_OP        TRUE
#define OK_UCN              FALSE/*TRUE*/
#define OK_MBIDENT          FALSE
#define EXPAND_PRAGMA       TRUE
#endif

#if     HAVE_LLONG
#if     MODE >= STANDARD
typedef long long   expr_t;
typedef unsigned long long  uexpr_t;
#else
typedef long    expr_t;
typedef unsigned long   uexpr_t;
#endif
#else
typedef long    expr_t;
#if     HAVE_ULONG
typedef unsigned long   uexpr_t;
#else
typedef long    uexpr_t;
#endif
#endif

#if     HAVE_LLONG
#if MODE >= STANDARD
#define EXPR_MAX            0xFFFFFFFFFFFFFFFFULL
#else
#define EXPR_MAX            4294967295UL
#endif
#else
#if     HAVE_ULONG
#define EXPR_MAX            4294967295UL
#else
#define EXPR_MAX            2147483647L
#endif
#endif

/*
 * CONCAT_STRINGS   When this is set TRUE, cpp will replace some escape
 *              sequences with an equivalent sequence and concatenate
 *              adjacent string literals in the supplementary phase of
 *              preprocessing.  When set FALSE, cpp will leave the escape
 *              sequence conversion and string concatenation for the COMPILER.
 */
#ifndef CONCAT_STRINGS
#define CONCAT_STRINGS      FALSE   /* TRUE for pre-Standard compiler   */
#endif

/*
 * DOLLAR_IN_NAME   Should be set TRUE if $ is a valid letter in identifiers
 *              or FALSE if $ is invalid.
 */
#ifndef DOLLAR_IN_NAME
#define DOLLAR_IN_NAME      FALSE
#endif

/*
 * TOP_SPACE    specifies handling of line top white spaces (including
 *              comment) as follows:
 *      TRUE:  squeeze to one space
 *      FALSE: delete
 */
#if     MODE >= STANDARD
#define TOP_SPACE           TRUE
#endif
#ifndef TOP_SPACE
#define TOP_SPACE           TRUE
#endif

/*
 * OK_MAKE      implement -M* options to put source file dependency line
 *              for 'make'.
 */
#define OK_MAKE             TRUE

/*
 * For #pragma __debug, #debug.
 */
#define DEBUG               TRUE    /* for debugging of cpp */
#define DEBUG_EVAL          TRUE    /* for debug of cpp #if expression      */

/*
 * Translation limits.
 * The following definitions are used to allocate memory for work buffers.
 *
 * NWORK        Output buffer size.  Set this size according to your compiler-
 *              proper.  Length of string literal should be less than NWORK
 *              - 1.
 * NBUFF        Input buffer size after line concatenation by <backslash>
 *              <newline>.
 * NMACWORK     Internal work buffer size for macro definition and expansion.
 * IDMAX        The longest identifier length.
 * NMACPARS     The maximum number of #define parameters.
 *              NOTE: Must be NMACPARS <= UCHARMAX.
 * NEXP         The maximum nesting depth of #if expressions.
 * BLK_NEST     The number of nested #if's permitted.
 * NINCLUDE     The number of directories that may be specified on a per-
 *              system basis, or by the -I option.
 * RESCAN_LIMIT The maximum rescan times of macro expansion.
 *
 * NBUFF, NMACWORK should not be smaller than NWORK.
 *
 * SBSIZE defines the number of hash-table slots for the macro symbol table.
 * It must be a power of 2.
 */

#if     COMPILER == MWC09
#define IDMAX               0x20
#define NMACPARS            0x3F
#define NEXP                0x20
#define BLK_NEST            0x10
#define NINCLUDE            8
#define RESCAN_LIMIT        0x10
#define NWORK               0x100
#define NBUFF               0x400
#define SBSIZE              0x100
#else
#if     SYSTEM == SYS_MSDOS
#if     __TINY__ || __SMALL__ || __MEDIUM__ || M_I86TM || M_I86SM || M_I86MM
#define IDMAX               0x3F
#define NMACPARS            0x7F
#define NEXP                0x40
#define BLK_NEST            0x40
#define RESCAN_LIMIT        0x20
#define NBUFF               0x800
#define SBSIZE              0x200
#else
#define IDMAX               0xFF
#define NBUFF               0x1000
#define SBSIZE              0x400
#endif
#endif
#endif

#if     CMP_FAMILY == GNUC || COMPILER == TURBOC || COMPILER == LSIC
#define NWORK               NBUFF
#endif

/* For the systems of 32 bits or more.  */
#ifndef IDMAX
#define IDMAX               0x400
#endif
#ifndef NMACPARS
#define NMACPARS            0xFF
#endif
#ifndef NEXP
#define NEXP                0x100
#endif
#ifndef BLK_NEST
#define BLK_NEST            0x100
#endif
#ifndef NINCLUDE
#define NINCLUDE            0x10
#endif
#ifndef RESCAN_LIMIT
#define RESCAN_LIMIT        0x80
#endif
#ifndef NBUFF
#define NBUFF               0x10000     /* Must be NWORK <= NBUFF   */
#endif
#ifndef NWORK
#define NWORK               NBUFF       /* 256, 512, 1024, 4096, .. */
#endif
#ifndef NMACWORK
#define NMACWORK            NBUFF       /* Must be NWORK <= NMACWORK*/
#endif
#ifndef SBSIZE
#define SBSIZE              0x1000
#endif

#if     UCHARMAX < NMACPARS
    error  <<NMACPARS should not be greater than UCHARMAX>>
#endif

#if     HOST_SYSTEM == SYS_VMS
#if     NWORK < (NAM$C_MAXRSS + 1)
    error  <<NWORK must be greater than NAM$C_MAXRSS>>
#endif
#endif

#if     NBUFF < NWORK
    error   <<NBUFF must be same or greater than NWORK>>
#endif
#if     NMACWORK < NWORK
    error   <<NMACWORK must be same or greater than NWORK>>
#endif

#define SBMASK  (SBSIZE - 1)
#if     (SBSIZE ^ SBMASK) != ((SBSIZE * 2) - 1)
    error  <<SBSIZE must be a power of 2 !>>
#endif

#if     MODE >= STANDARD
/*
 * Translation limits required by the Standard.
 *
 * *90MIN   limits specified by C90.
 * *99MIN   limits specified by C99.
 * *_CPP_MIN    limits recommended by C++ (ISO 1997/02 draft Standard).
 *
 * SLEN*MIN     Characters in a logical source line
 *              and characters in a string literal or wide string literal
 *              (after concatenation).
 * IDLEN*MIN    Significant initial characters in an internal identifier
 *              or a macro name.
 * NMACPARS*MIN     Parameters in one macro definition.
 *              Arguments in one macro invocation.
 * EXP_NEST*MIN     Nesting levels of parenthesized expressions in a full
 *              expression.
 * BLK_NEST*MIN     Nesting levels of conditional inclusion.
 * INCLUDE_NEST*MIN     Nesting levels for #include files.
 * NMACRO*MIN   Macro identifiers simultaneously defined in one translation
 *              unit.
 */
#define SLEN90MIN           0x1FD
#define IDLEN90MIN          0x1F
#define NMACPARS90MIN       0x1F
#define EXP_NEST90MIN       0x20
#define BLK_NEST90MIN       8
#define INCLUDE_NEST90MIN   8
#define NMACRO90MIN         0x400

#define SLEN99MIN           0xFFF
#define IDLEN99MIN          0x3F
#define NMACPARS99MIN       0x7F
#define EXP_NEST99MIN       0x3F
#define BLK_NEST99MIN       0x3F
#define INCLUDE_NEST99MIN   0xF
#define NMACRO99MIN         0xFFF

#define SLEN_CPP_MIN        0x10000
#define IDLEN_CPP_MIN       0x400
#define NMACPARS_CPP_MIN    0x100
#define EXP_NEST_CPP_MIN    0x100
#define BLK_NEST_CPP_MIN    0x100
#define INCLUDE_NEST_CPP_MIN    0x100
#define NMACRO_CPP_MIN      0x10000

/*
 * STDC     might be set to 1, if COMPILER conforms to the Standard, else
 *      be set to 0.
 *      This macro is used for the predefined __STDC__, when MODE >=
 *      STANDARD.
 *          (STDC >= 1) disables predefined macros not beginning with '_'.
 *          Note:   -S<n> option redefines __STDC__ to <n> and redefines
 *      __STDC_VERSION__ to STDC<n>_VERSION even if STDC is 0.
 */
#if     ! HAVE_ULONG || IDMAX < IDLEN90MIN || NBUFF < SLEN90MIN + 3
#define STDC                0
#else
#if     NWORK < SLEN90MIN + 2 || NMACPARS < NMACPARS90MIN
#define STDC                0
#else
#if     NEXP < EXP_NEST90MIN || BLK_NEST < BLK_NEST90MIN
#define STDC                0
#endif
#endif
#endif
#ifndef STDC
#define STDC                1       /* 1 : for ISO 9899:1990 or later   */
#endif
#define STDC1_VERSION       199409L     /* 199409L  : For conforming
            implementation to ISO 9899:1990 / Amendment 1:1995
            199901L : For C9X   */
#define STDC2_VERSION       0L          /* Used when STDC == 2  */
#endif  /* MODE >= STANDARD */

/*
 * CPLUS specifies the default value of the pre-defined macro __cplusplus
 * for C++ processing.
 * The value can be changed by -S<n> option.
 */
#define CPLUS               1       /* 1 or 199711L for C++ Standard    */

