/*
 *                          C O N T R O L . C
 *              P r o c e s s   C o n t r o l   L i n e s
 *
 * Edit history of DECUS CPP / cpp2.c
 * 13-Nov-84    MM      Split from cpp1.c
 * 06-Jun-85    KR      Latest revision
 */

/*
 * CPP Version 2.0 / control.c
 * 1998/08      Psycho
 *      Renamed cpp2.c control.c.
 *      Moved dodefine(), is_formal(), mtokensave(), stparmscan(), doundef()
 *          from cpp4.c to control.c.
 *      Moved lookid(), defendel(), dumpadef() from cpp6.c to control.c.
 *      Moved doinclude(), openinclude(), vmsparse()
 *          from cpp2.c to system.c.
 *      Split doline() from control().
 *      Split getparm(), getrepl(), def_stringization() from dodefine().
 *      Split dumprepl() from dumpadef().
 *      Devided defendel() to install(), undefine(), lookprev().
 *      Removed textput(), charput(), checkparm() from cpp4.c.
 *      Expanded specification of stparmscan().
 *      Revised most of the functions.
 */

/*
 * CPP Version 2.2 / control.c
 * 1998/11      Psycho
 *      Revised several functions.
 */

#if PREPROCESSED
#include    "cpp.H"
#else
#include    "system.H"
#include    "internal.H"
#endif

#if PROTO

static int      doif( int hash);
static long     doline( void);
static int      dodefine( void);
static int      getparm( void);
static int      getrepl( const char * macroname);
static char *   is_formal( const char * name, int conv);
#if MODE != CON_FALSE
static char *   def_stringization( char * repl_cur);
static char *   mtokensave( const char * macroname);
#endif
#if STRING_FORMAL
static char *   stparmscan( char * string_end);
#endif
static void     doundef( void);
static DEFBUF **    lookprev( const char * name);
static void     dumprepl( const DEFBUF * dp);

#else   /* ! PROTO  */

static int      doif();         /* #if, #elif, #ifdef, #ifndef      */
static long     doline();       /* Process #line directive          */
static int      dodefine();     /* Process #define directive        */
static int      getparm();      /* Get parm., its nargs, names, lens*/
static int      getrepl();      /* Get replacement embedded parm-no.*/
static char *   is_formal();    /* If formal param., save the num.  */
#if MODE != CON_FALSE
static char *   def_stringization();    /* Define stringization     */
static char *   mtokensave();   /* Prefix DEF_MAGIC to macro name   */
#endif
#if STRING_FORMAL
static char *   stparmscan();   /* Scan the param. in string        */
#endif
static void     doundef();      /* Process #undef directive         */
static DEFBUF **    lookprev(); /* Look for the place to insert def.*/
static void     dumprepl();     /* Dump replacement text.           */

#endif  /* PROTO    */

/*
 * Generate (by hand-inspection) a set of unique values for each directive.
 * Note that this is not guaranteed to work for non-Ascii machines.
 * CPP won't compile if there are hash conflicts.
 */

#define L_if            ('i' ^ (EOS << 1))
#define L_ifdef         ('i' ^ ('e' << 1))
#define L_ifndef        ('i' ^ ('d' << 1))
#if MODE != CON_FALSE
#define L_elif          ('e' ^ ('f' << 1))
#endif
#define L_else          ('e' ^ ('e' << 1))
#define L_endif         ('e' ^ ('i' << 1))
#define L_define        ('d' ^ ('i' << 1))
#define L_undef         ('u' ^ ('e' << 1))
#define L_line          ('l' ^ ('e' << 1))
#define L_include       ('i' ^ ('l' << 1))
#if MODE >= STANDARD
#define L_error         ('e' ^ ('o' << 1))
#define L_pragma        ('p' ^ ('g' << 1))
#endif

static const char * const   not_ident
            = "Not an identifier \"%s\"";               /* _E_      */
static const char * const   no_arg = "No argument";     /* _E_      */
#if ! OK_IF_JUNK
static const char * const   excess
            = "Excessive token sequence \"%s\"";        /* _E_ _W1_ */
#endif

int
#if PROTO
control( int newlines)
#else
control( newlines)
    int         newlines;                       /* Pending newlines */
#endif
/*
 * Process #directive lines.  Each directive have their own subroutines.
 */
{
    const char * const  many_nesting =
"More than %.0s%ld nesting of #if (#ifdef) sections%s"; /* _F_ _W2_ _W4_    */
    const char * const  not_in_section
    = "Not in a #if (#ifdef) section in a source file"; /* _E_ _W1_ */
    const char * const  unknown
            = "Unknown #directive \"%s\"%.0ld%s";       /* _E_ _W4_ */
    const char * const  illeg_dir
            = "Illegal #directive \"%s%.0ld%s\"";       /* _E_ _W4_ */
    const char * const  in_skipped = " (in skipped block)"; /* _W4_ */
    int     token_type;
    register int    c;
    register int    hash;
    char *      tp;

    in_directive = TRUE;
    if (keepcomments) {
        fputc( '\n', stdout);       /* Possibly flush out comments  */
        newlines--;
    }
    c = skipws();
    if (c == '\n')                              /* 'null' directive */
        goto  ret;
#if MODE < STANDARD
    token_type = scantoken( c, (workp = work, &workp), work_end);
    if (in_asm && (token_type != NAM            /* In #asm block    */
            || (! str_eq( identifier, "asm")    /* Ignore #anything */
                && ! str_eq( identifier, "endasm"))))   /*   other  */
        goto  skip_line;                /*    than #asm or #endasm  */
    if (token_type != NAM) {
#if MODE != CON_FALSE
        if (token_type == NUM) {                /* # 123 [fname]    */
            strcpy( identifier, "line");
        } else
#endif
        {
            if (compiling)
                cerror( illeg_dir, work, 0L, NULLST);
            else if (warn_level & 4)
                cwarn( illeg_dir, work, 0L, in_skipped);
            goto  skip_line;
        }
    }
#else
    token_type = scantoken( c, (workp = work, &workp), work_end);
    if (token_type != NAM) {
        if (token_type == NUM) {                /* # 123 [fname]    */
			ungetstring( work, identifier );
            strcpy( identifier, "line");
        } else
        {
            if (compiling)
                cerror( illeg_dir, work, 0L, NULLST);
            else if (warn_level & 4)
                cwarn( illeg_dir, work, 0L, in_skipped);
            goto  skip_line;
        }
    }
#endif
    if (identifier[2] == EOS)
        identifier[3] = EOS;                    /* Diddle           */
    hash = (identifier[1] == EOS) ? identifier[0]
            : (identifier[0] ^ (identifier[3] << 1));

    switch (hash) {
    case L_if:      tp = "if";      break;
    case L_ifdef:   tp = "ifdef";   break;
    case L_ifndef:  tp = "ifndef";  break;
#if MODE != CON_FALSE
    case L_elif:    tp = "elif";    break;
#endif
    case L_else:    tp = "else";    break;
    case L_endif:   tp = "endif";   break;
    case L_define:  tp = "define";  break;
    case L_undef:   tp = "izdef";   break;
    case L_line:    tp = "line";    break;
    case L_include: tp = "include"; break;
#if MODE >= STANDARD
    case L_error:   tp = "error";   break;
    case L_pragma:  tp = "pragma";  break;
#endif
    default:        tp = NULL;      break;
    }

    if (tp != NULL && ! str_eq( identifier, tp)) {  /* Hash conflict*/
        if (compiling)
            cerror( unknown, identifier, 0L, NULLST);
        else if (warn_level & 4)
            cwarn( unknown, identifier, 0L, in_skipped);
        goto  skip_line;                    /* Skip the line        */
    }

    /*
     * hash is set to a unique value corresponding to the control directive.
     */
    if (! compiling) {                      /* Not compiling now    */
        switch (hash) {
        case L_else :                       /* Test the #if's nest, */
#if MODE != CON_FALSE
        case L_elif :                       /*   if 0, compile.     */
#endif
        case L_endif:                       /* Un-nest #if          */
            break;
        case L_if   :                       /* These can't turn     */
        case L_ifdef:                       /*  compilation on, but */
        case L_ifndef:                      /*   we must nest #if's.*/
            if (&ifstack[ BLK_NEST] < ++ifptr)
                goto  if_nest_err;
#if MODE >= STANDARD && BLK_NEST > BLK_NEST90MIN
            if ((warn_level & 4) && &ifstack[ blk_nestmin + 1] == ifptr)
                cwarn( many_nesting, NULLST, (long) blk_nestmin, in_skipped);
#endif
            ifptr->stat = 0;                /* !WAS_COMPILING       */
            ifptr->ifline = line;           /* Line at section start*/
            goto  skip_line;
        default     :                       /* Other directives     */
            if (tp == NULL && (warn_level & 4))
#if MODE >= STANDARD
                cwarn( unknown, identifier, 0L, in_skipped);
#else
                doold();                    /* Unknown directive ?  */
#endif
            goto  skip_line;                /* Skip the line        */
        }
    }
    macro_line = 0;                         /* Reset error flag     */

    switch (hash) {

    case L_if:
    case L_ifdef:
    case L_ifndef:
        if (&ifstack[ BLK_NEST] < ++ifptr)
            goto  if_nest_err;
#if MODE >= STANDARD && BLK_NEST > BLK_NEST90MIN
        if ((warn_level & 2) &&
                &ifstack[ blk_nestmin + 1] == ifptr)
            cwarn( many_nesting, NULLST , (long) blk_nestmin, NULLST);
#endif
        ifptr->stat = WAS_COMPILING;
        ifptr->ifline = line;
#if MODE != CON_FALSE
        goto  ifdo;

    case L_elif:
        if (ifptr == &ifstack[0])
            goto  nest_err;
        if (ifptr == infile->initif) {
#if MODE >= STANDARD
            goto  in_file_nest_err;
#else
            if (warn_level & 1)
                cwarn( not_in_section, NULLST, 0L, NULLST);
#endif
        }
        if (ifptr->stat & ELSE_SEEN)
            goto  else_seen_err;
        if ((ifptr->stat & (WAS_COMPILING | TRUE_SEEN)) != WAS_COMPILING) {
            compiling = FALSE;              /* Done compiling stuff */
            goto  skip_line;                /* Skip this group      */
        }
        hash = L_if;
ifdo:
#endif  /* MODE != CON_FALSE    */
        c = doif( hash);
#if DEBUG
        if (debug & IF) {
            printf( "#if (#elif, #ifdef, #ifndef) evaluate to %s.\n"
                    , compiling ? "TRUE" : "FALSE");
            printf( "line %ld: %s", line, infile->buffer);
        }
#endif
        if (c == FALSE) {                   /* Error                */
            compiling = FALSE;              /* Skip this group      */
            goto  skip_line;    /* Prevent an extra error message   */
        }
        break;

    case L_else:
        if (ifptr == &ifstack[0])
            goto  nest_err;
        if (ifptr == infile->initif) {
#if MODE >= STANDARD
            goto  in_file_nest_err;
#else
            if (warn_level & 1)
                cwarn( not_in_section, NULLST, 0L, NULLST);
#endif
        }
        if (ifptr->stat & ELSE_SEEN)
            goto  else_seen_err;
        ifptr->stat |= ELSE_SEEN;
        ifptr->elseline = line;
        if (ifptr->stat & WAS_COMPILING) {
            if (compiling || (ifptr->stat & TRUE_SEEN) != 0)
                compiling = FALSE;
            else
                compiling = TRUE;
        }
        break;

    case L_endif:
        if (ifptr == &ifstack[0])
            goto  nest_err;
        if (ifptr <= infile->initif) {
#if MODE >= STANDARD
            goto  in_file_nest_err;
#else
            if (warn_level & 1)
                cwarn( not_in_section, NULLST, 0L, NULLST);
#endif
        }
        if (! compiling && (ifptr->stat & WAS_COMPILING))
            wrongline = TRUE;
        compiling = (ifptr->stat & WAS_COMPILING);
        --ifptr;
        break;

    case L_define:
        in_define = TRUE;
        dodefine();
        in_define = FALSE;
        break;

    case L_undef:
        doundef();
        break;

    case L_line:
        if ((c = doline()) > 0) {
            line = c;
            sharp();    /* Putout the new line number and file name */
            infile->line = --line;  /* Next line number is 'line'   */
            newlines = -1;
        } else {            /* Error already diagnosed by doline()  */
            skipnl();
        }
        break;

    case L_include:
        if (doinclude() == TRUE)
            newlines = -1;                  /* To clear line number */
        break;

#if MODE >= STANDARD
    case L_error:
        cerror( "Preprocessing error directive"             /* _E_  */
                , NULLST, 0L, NULLST);
        break;

    case L_pragma:
        dopragma();
        break;

    default:
        cerror( unknown, identifier, 0L, NULLST);
        goto  skip_line;
#else
    default:                /* Non-Standard or unknown directives   */
        doold();
        break;
#endif

    }

    switch (hash) {
    case L_if       :
    case L_define   :
#if MODE >= STANDARD
    case L_error    :
#endif
        goto  skip_line;    /* To prevent duplicate error message   */
    case L_include  :
    case L_line     :
#if MODE >= STANDARD
    case L_pragma   :
#endif
        break;              /* Already read over the line           */
    default :               /* L_else, L_endif, L_undef, etc.       */
#if OK_IF_JUNK
        /*
         * Ignore the rest of the #control line so you can write
         *          #if     foo
         *          #endif  foo
         */
        skipnl();
#else   /* ! OK_IF_JUNK */
        if ((c = skipws()) != '\n') {
#if MODE >= STANDARD
            cerror( excess, infile->bptr-1, 0L, NULLST);
#else
            if (warn_level & 1)
                cwarn( excess, infile->bptr-1, 0L, NULLST);
#endif
            skipnl();
        }
#endif
    }
    goto  ret;

#if MODE >= STANDARD
in_file_nest_err:
    cerror( not_in_section, NULLST, 0L, NULLST);
    goto  skip_line;
#endif
nest_err:
    cerror( "Not in a #if (#ifdef) section", NULLST, 0L, NULLST);   /* _E_  */
    goto  skip_line;
else_seen_err:
    cerror( "Already seen #else at line %.0s%ld"            /* _E_  */
            , NULLST, ifptr->elseline, NULLST);
skip_line:
    skipnl();                               /* Ignore rest of line  */
    goto  ret;

if_nest_err:
    cfatal( many_nesting, NULLST, (long) BLK_NEST, NULLST);
ret:
    in_directive = FALSE;
    keepcomments = cflag && compiling && !no_output;
    return  (wrongline ? 0 : ++newlines);
}

static int
#if PROTO
doif( int hash)
#else
doif( hash)
    int     hash;
#endif
/*
 * Process an #if (#elif), #ifdef or #ifndef.  The latter two are straight-
 * forward, while #if needs a subroutine of its own to evaluate the
 * expression.
 * doif() is called only if compiling is TRUE.  If false, compilation is
 * always supressed, so we don't need to evaluate anything.  This supresses
 * unnecessary warnings.
 */
{
    register int    c;
    register int    found;

    if ((c = skipws()) == '\n') {
        unget();
        cerror( no_arg, NULLST, 0L, NULLST);
        return  FALSE;
    }
    if (hash == L_if) {                 /* #if or #elif             */
        unget();
        found = (eval() != 0L);         /* Evaluate expression      */
        hash = L_ifdef;                 /* #if is now like #ifdef   */
    } else {                            /* #ifdef or #ifndef        */
        if (scantoken( c, (workp = work, &workp), work_end) != NAM) {
            cerror( not_ident, work, 0L, NULLST);
            return  FALSE;      /* Next token is not an identifier  */
        }
        found = (lookid( identifier) != NULL);  /* Look in table    */
    }
    if (found == (hash == L_ifdef)) {
        compiling = TRUE;
        ifptr->stat |= TRUE_SEEN;
    } else {
        compiling = FALSE;
    }
    return  TRUE;
}

static long
#if PROTO
doline( void)
#else
doline()
#endif
/*
 * Parse the line to update the line number and "filename" field for the next
 * input line.
 * Values returned are as follows:
 *  -1: syntax error or out-of-range error (diagnosed by doline(), evalnum()).
 *  [1,32767]: legal line number for C90, [1,0x7FFFFFFF] for C99.
 * doline() always absorbs the whole line.
 */
{
    const char * const  not_digits
        = "Line number \"%s\" isn't a decimal digits sequence"; /* _E_ _W1_ */
#if MODE >= STANDARD
    const char * const  out_of_range
        = "Line number \"%s\" is out of range of [1,%ld]";      /* _E_ _W1_ */
#endif
#if MODE != CON_FALSE
    register int    token_type;
#endif
    VAL_SIGN *      valp;
    char *  save;
    int     c;

    if ((c = skipws()) == '\n') {
        cerror( no_arg, NULLST, 0L, NULLST);
        unget();                            /* Push back <newline>  */
        return  -1L;                /* Line number is not changed   */
    }

#if MODE != CON_FALSE
    token_type = get_unexpandable( c, FALSE);
#if MODE >= STANDARD
    if (macro_line == MACRO_ERROR)          /* Unterminated macro   */
        return  -1L;                        /*   already diagnosed. */
#endif
    if (token_type == NO_TOKEN)     /* Macro expanded to 0 token    */
        goto  no_num;
    if (token_type != NUM && scantoken( c, (workp = work, &workp), work_end) != NUM)
#else   /* MODE == CON_FALSE    */
    if (scantoken( c, (workp = work, &workp), work_end) != NUM)
#endif
        goto  illeg_num;
    for (workp = work; *workp != EOS; workp++) {
        if (! isdigit( *workp & UCHARMAX)) {
#if MODE >= STANDARD
            cerror( not_digits, work, 0L, NULLST);
            return  -1L;
#else
            if (warn_level & 1)
                cwarn( not_digits, work, 0L, NULLST);
#endif
        }
    }
    valp = evalnum( work);                  /* Evaluate number      */
    if (valp->sign == VAL_ERROR) {  /* Error diagnosed by evalnum() */
        return  -1;
#if MODE >= STANDARD
    } else if (line_limit < valp->val || valp->val <= 0L) {
        cerror( out_of_range, work, line_limit, NULLST);
        return  -1L;
#endif
    }

#if MODE != CON_FALSE
    token_type = get_unexpandable( skipws(), FALSE);
#if MODE >= STANDARD
    if (macro_line == MACRO_ERROR)
        return  -1L;
#endif
    if (token_type != STR) {
        if (token_type == NO_TOKEN) {       /* Filename is absent   */
            return  (long) valp->val;
        } else {
            goto  not_fname;    /* Expanded macro should be a quoted string */
        }
    }
#else   /* MODE == CON_FALSE    */
    if ((c = skipws()) == '\n') {
        unget();
        return  (long) valp->val;
    }
    if (scantoken( c, (workp = work, &workp), work_end) != STR)
        goto  not_fname;
#endif
    *(workp - 1) = EOS;                     /* Ignore right '"'     */
    save = savestring( &work[ 1]);          /* Ignore left '"'      */

#if ! OK_IF_JUNK
#if MODE != CON_FALSE
    if (get_unexpandable( skipws(), FALSE) != NO_TOKEN) {
#if MODE >= STANDARD
        cerror( excess, work, 0L, NULLST);
        free( save);
        return  -1L;
#else   /* MODE < STANDARD  */
        if (warn_level & 1)
            cwarn( excess, work, 0L, NULLST);
        skipnl();                           /* Clear stacked text   */
        unget();
#endif
    }
#else   /* MODE == CON_FALSE    */
    if ((c = skipws()) == '\n') {
        unget();
    } else {
        if (warn_level & 1) {
            scantoken( c, (workp = work, &workp), work_end);
            cwarn( excess, work, 0, NULLST);
        }
        skipnl();
        unget();
    }
#endif
#else   /* OK_IF_JUNK   */
    skipnl();
    unget();
#endif

    free( infile->filename);                /* Give up the old name */
    infile->filename = save;                /* New file name        */
    return  (long) valp->val;               /* New line number      */

#if MODE != CON_FALSE
no_num:
    cerror( "No line number", NULLST, 0L, NULLST);          /* _E_  */
    return  -1L;
#endif
illeg_num:
    cerror( "Not a line number \"%s\"", work, 0L, NULLST);  /* _E_  */
    return  -1L;
not_fname:
    cerror( "Not a file name \"%s\"", work, 0L, NULLST);    /* _E_  */
    return  -1L;
}

/*
 *                  M a c r o  D e f i n i t i o n s
 *
 * Edit History (cpp4.c) of original version
 * 31-Aug-84    MM      USENET net.sources release
 *  2-May-85    MM      Latest revision
 */

/*
 * lookid()     Looks for the name in the defined symbol table.  Returns a
 *              pointer to the definition if found, or NULL if not present.
 * install()    Installs the definition.  Updates the symbol table.
 * undefine()   Deletes the definition from the symbol table.
 */

/*
 * Global work[] are used to store #define parameter lists and parlist[]
 * point to them.
 * 'nargs' contains the actual number of parameters stored.
 */
static char *   parlist[ NMACPARS]; /* -> Start of each parameter   */
static int      parlen[ NMACPARS];  /* Length of parameter name     */
static D09 int      nargs;          /* Number of parameters         */
static D09 char *   token_p;        /* Pointer to the token scanned */
static D09 char *   repl_base;      /* Base of buffer for repl-text */
static D09 char *   repl_end;       /* End of buffer for repl-text  */
static const char * const   no_ident = "No identifier";     /* _E_  */

static int
#if PROTO
dodefine( void)
#else
dodefine()
#endif
/*
 * Called from control() when a #define is scanned.  This module parses formal
 * parameters by getparm() and the replacement text by getrepl().
 *
 * There is some special case code to distinguish
 *      #define foo     bar     --  object-like macro
 * from #define foo()   bar     --  function-like macro with no parameter
 *
 * Also, we make sure that
 *      #define foo     foo
 * expands to "foo" but doesn't put cpp into an infinite loop.
 *
 * An error message is printed if you redefine a symbol with a non-identical
 * text.  I.e,
 *      #define foo     123
 *      #define foo     123
 * is ok, but
 *      #define foo     123
 *      #define foo     +123
 * is not.
 *
 * The following subroutines are called from dodefine():
 * getparm()    parsing and remembering parameter names.
 * getrepl()    parsing and remembering replacement text.
 *
 * The following subroutines are called from getrepl():
 * is_formal()  is called when an identifier is scanned.  It checks through
 *              the array of formal parameters.  If a match is found, the
 *              identifier is replaced by a control byte which will be used
 *              to locate the parameter when the macro is expanded.
 * def_stringization()  is called when '#' operator is scanned.  It surrounds
 *              the token to stringize with magic-codes.
 *
 * MODEs other than STANDARD ignore difference of parameter names in macro
 * redefinition.
 */
{
#if MODE != CON_FALSE
    const char * const  predef = "\"%s\" shouldn't be redefined";   /* _E_  */
#endif
    char    repl_list[ NMACWORK + IDMAX];   /* Replacement text     */
    char    macroname[ IDMAX + 1];  /* Name of the macro defining   */
    register DEFBUF *   defp;       /* -> old definition            */
    int     c;
    int     redefined;                      /* TRUE if redefined    */
    int     dnargs;                         /* defp->nargs          */

    repl_base = repl_list;
    repl_end = & repl_list[ NMACWORK];
    if ((c = skipws()) == '\n') {
        cerror( no_ident, NULLST, 0L, NULLST);
        unget();
        return  FALSE;
    } else if (scantoken( c, (workp = work, &workp), work_end) != NAM) {
        cerror( not_ident, work, 0L, NULLST);
        return  FALSE;
    } else if ((defp = lookid( identifier)) == NULL) {  /* Not known*/
#if MODE != CON_FALSE
        if (str_eq( identifier, "defined")
#if MODE >= STANDARD
                || (stdc_ver >= 199901L && str_eq( identifier, "__VA_ARGS__"))
#endif
                ) {
            cerror(
            "\"%s\" shouldn't be defined", identifier, 0L, NULLST); /* _E_  */
            return  FALSE;
        }
#endif
        redefined = FALSE;                  /* Quite new definition */
    } else {                                /* It's known:          */
        dnargs = (defp->nargs == DEF_NOARGS-1) ? DEF_NOARGS : defp->nargs;
#if MODE != CON_FALSE
        if (dnargs < DEF_NOARGS - 1         /* Standard predefined  */
#if MODE >= STANDARD && OK_PRAGMA_OP
                || dnargs == DEF_PRAGMA     /* _Pragma() pseudo-macro       */
#endif
                ) {
            cerror( predef, identifier, 0L, NULLST);
            return  FALSE;
        } else
#endif
            redefined = TRUE;               /* Remember this fact   */
    }
    strcpy( macroname, identifier);         /* Remember the name    */

    if (getparm() == FALSE)                 /* Get parameter list   */
        return  FALSE;                      /* Syntax error         */
    if (getrepl( macroname) == FALSE)       /* Get replacement text */
        return  FALSE;                      /* Syntax error         */

    if (redefined) {                        /* Error if redefined   */
        if (dnargs != nargs || ! str_eq( defp->repl, repl_list)
#if MODE == STANDARD
                || ! str_eq( defp->parmnames, work)
#endif
                ) {
            cerror( "Invalid redefinition", NULLST, 0L, NULLST);    /* _E_  */
            return  FALSE;
        }
    } else {                                /* New definition       */
        defp = install( macroname, nargs, work, repl_list);
    }
    return  TRUE;
}

static int
#if PROTO
getparm( void)
#else
getparm()
#endif
/*
 *   Get parameters i.e. numbers into nargs, name into work[], name-length
 * into parlen[].
 *   Return TRUE if the parameters are legal, else return FALSE.
 *   In STANDARD mode cpp must remember the parameter names, only for checking
 * the validity of macro redefinitions.  This is required by the Standard
 * (what an overhead !).
 */
{
    const char * const  many_parms
            = "More than %.0s%ld parameters";       /* _E_ _W2_    */
    int     token_type;
    register int    c;

    parlist[ 0] = workp = work;
    work[ 0] = EOS;
#if MODE == POST_STANDARD
    insert_sep = NO_SEP;    /* Clear the inserted token separator   */
#endif
    c = get();

    if (c == '(') {                         /* With arguments?      */
        nargs = 0;                          /* Init parms counter   */
        if ((c = skipws()) == ')')
            return  TRUE;                   /* Macro with 0 parm    */
        else
            unget();

        do {                                /* Collect parameters   */
            if (nargs >= NMACPARS) {
                cerror( many_parms, NULLST, (long) NMACPARS, NULLST);
                return  FALSE;
            }
            parlist[ nargs] = workp;        /* Save its start       */
            if ((token_type = scantoken( c = skipws(), &workp, work_end))
                    != NAM) {
                if (c == '\n') {
                    break;
                } else if (c == ',' || c == ')') {
                    cerror( "Empty parameter", NULLST, 0L, NULLST); /* _E_  */
                    return  FALSE;
#if MODE >= STANDARD
                } else if (stdc_ver >= 199901L && token_type == OPE
                        && openum == OP_ELL) {
                    if ((c = skipws()) != ')') {
                        cerror( "\"...\" isn't the last parameter", /* _E_  */
                                NULLST, 0L, NULLST);
                        return  FALSE;
                    }
                    parlen[ nargs++] = 3;
                    nargs |= VA_ARGS;
                    goto  ret;
#endif
                } else {
                    cerror( "Illegal parameter \"%s\""      /* _E_  */
                            , parlist[ nargs], 0L, NULLST);
                    return  FALSE;          /* Bad parameter syntax */
                }
            }
            if (is_formal( parlist[ nargs], FALSE)) {
                cerror( "Duplicate parameter name \"%s\""   /* _E_  */
                        , parlist[ nargs], 0L, NULLST);
                return  FALSE;
            }
            parlen[ nargs] = (int)(workp - parlist[ nargs]);
                                            /* Save length of param */
            *workp++ = ',';
            nargs++;
        } while ((c = skipws()) == ',');    /* Get another parameter*/

        *--workp = EOS;                     /* Remove excessive ',' */
        if (c != ')') {                     /* Must end at )        */
            unget();                        /* Push back '\n'       */
            cerror( "Unterminated parameter list \"(%s\""   /* _E_  */
                    , work, 0L, NULLST);
            return  FALSE;
        }
#if MODE == STANDARD
    } else if ((type[ c] & (LET | DIG | DOT | PUNC | QUO | SPA)) == 0) {
        char    ch[ 2];
        sprintf( ch, "%c", c);
        cerror(
"No space before repl-text beginning with non-basic character '%s'" /* _E_  */
                , ch, 0L, NULLST);
#endif
    } else {
        /*
         * DEF_NOARGS is needed to distinguish between
         * "#define foo" and "#define foo()".
         */
        nargs = DEF_NOARGS;                 /* Object-like macro    */
        unget();
    }
#if MODE >= STANDARD
ret:
#if NMACPARS > NMACPARS90MIN
    if ((warn_level & 2) && (nargs & ~VA_ARGS) > nmacparsmin)
        cwarn( many_parms, NULLST , (long) nmacparsmin , NULLST);
#endif
#endif
    return  TRUE;
}

static int
#if PROTO
getrepl( const char * macroname)
#else
getrepl( macroname)
    char *  macroname;
#endif
/*
 *   Get replacement text i.e. names of formal parameters are converted to
 * the magic numbers, and operators #, ## is converted to magic characters.
 *   Return TRUE if replacement list is legal, else return FALSE.
 *   Any token separator in the text is converted to a single space, no token
 * sepatator is inserted by CPP.  Those are required by the Standard for
 * stringizing of an argument by # operator.
 *   POST_STANDARD inserts a space between any tokens in source (except a
 * macro name and the next '(' in macro definition), hence presence or absence
 * of token separator makes no difference.
 */
{
#if MODE != CON_FALSE
    const char * const  mixed_ops
    = "Macro with mixing of ## and # operators isn't portable";     /* _W1_ */
    const char * const  multiple_cats
    = "Macro with multiple ## operators isn't portable";    /* _W1_ */
    char *  prev_token = NULL;              /* Preceding token      */
    char *  prev_prev_token = NULL;         /* Pre-preceding token  */
    int     multi_cats = FALSE;             /* Multiple ## operators*/
#endif
    register int    c;
    int     token_type;                     /* Type of token        */
    char *  temp;
    char *  repl_cur = repl_base;   /* Pointer into repl-text buffer*/

    *repl_cur = EOS;
    token_p = NULL;
    c = skipws();                           /* Get to the body      */

    while (c != CHAR_EOF && c != '\n') {
#if MODE != CON_FALSE
        prev_prev_token = prev_token;
        prev_token = token_p;
#endif
        token_p = repl_cur;                 /* Remember the pointer */
        token_type = scantoken( c, &repl_cur, repl_end);

        switch (token_type) {
#if MODE != CON_FALSE
        case OPE:                   /* Operator or punctuator       */
            switch (openum) {
            case OP_CAT:                    /* ##                   */
                if (prev_token == NULL) {
                    cerror( "No token before ##"            /* _E_  */
                            , NULLST, 0L, NULLST);
                    return  FALSE;
                } else if (*prev_token == CAT) {
                    cerror( "## after ##", NULLST, 0L, NULLST);     /* _E_  */
                    return  FALSE;
                } else if (prev_prev_token && *prev_prev_token == CAT) {
                    multi_cats = TRUE;
                } else if (prev_prev_token && *prev_prev_token == ST_QUOTE
                        && (warn_level & 1)) {      /* # parm ##    */
                    cwarn( mixed_ops, NULLST, 0L, NULLST);
                }
                repl_cur = token_p;
                *repl_cur++ = CAT;          /* Convert to CAT       */
                break;
            case OP_STR:                    /* #                    */
                if (nargs < 0)              /* In object-like macro */
                    break;                  /* '#' is an usual char */
                if (prev_token && *prev_token == CAT
                        && (warn_level & 1))        /* ## #         */
                    cwarn( mixed_ops, NULLST, 0L, NULLST);
                repl_cur = token_p;         /* Overwrite on #       */
                if ((temp = def_stringization( repl_cur)) == NULL) {
                    return  FALSE;          /* Error                */
                } else {
                    repl_cur = temp;
                }
                break;
            default:                    /* Any operator as it is    */
                break;
            }
            break;
#endif
        case NAM:
        /*
         * Replace this name if it's a parm.  Note that the macro name is a
         * possible replacement token.  We stuff DEF_MAGIC in front of the
         * token which is treated as a LETTER by the token scanner and eaten
         * by the macro expanding routine.  This prevents the macro expander
         * from looping if someone writes "#define foo foo".
         */
            temp = is_formal( identifier, TRUE);
            if (temp == NULL) {             /* Not a parameter name */
#if MODE != CON_FALSE
                if ((temp = mtokensave( macroname)) != NULL)
                    repl_cur = temp;        /* Macro name           */
#endif
            } else {                        /* Parameter name       */
                repl_cur = temp;
            }
            break;

#if STRING_FORMAL
        case STR:                           /* String in mac. body  */
        case CHR:                           /* Character constant   */
            repl_cur = stparmscan( repl_cur);
            break;
#endif
        default:                            /* Any token as it is   */
            break;
        }

        c = get();
        if (c == ' ') {
            *repl_cur++ = ' ';
            c = get();
        }
    }

    unget();                                /* For control check    */
#if MODE != CON_FALSE
    if (token_p && *token_p == CAT) {
        cerror( "No token after ##", NULLST, 0L, NULLST);   /* _E_  */
        return  FALSE;
    }
    if (multi_cats && (warn_level & 1))
        cwarn( multiple_cats, NULLST, 0L, NULLST);
#endif
    *repl_cur = EOS;                        /* Terminate work       */

    return  TRUE;
}

static char *
#if PROTO
is_formal( const char * name, int conv)
#else
is_formal( name, conv)
    char *      name;
    int         conv;                   /* Convert to magic number? */
#endif
/*
 * If the identifier is a formal parameter, save the MAC_PARM and formal
 * offset, returning the advanced pointer into the replacement text.
 * Else, return NULL.
 */
{
    register char *   repl_cur;
    int     i;

    for (i = 0; i < (nargs & ~VA_ARGS); i++) { /* For each parameter   */
        if ((strlen( name) == parlen[ i]
                            /* Note: parlist[] are comma separated  */
                    && memcmp( name, parlist[ i], (size_t) parlen[ i]) == 0)
#if MODE >= STANDARD
                || ((nargs & VA_ARGS) && i == (nargs & ~VA_ARGS) - 1 && conv
                    && memcmp( name, "__VA_ARGS__", 12) == 0)
#endif
            ) {
                                            /* If it's known        */
            if (conv) {
                repl_cur = token_p;         /* Overwrite on the name*/
                *repl_cur++ = MAC_PARM;     /* Save the signal      */
                *repl_cur++ = i + 1;        /* Save the parm number */
                return  repl_cur;           /* Return "gotcha"      */
            } else {
                return  parlist[ i];        /* Duplicate parm name  */
            }
        }
    }

    return  NULL;                           /* Not a formal param   */
}

#if MODE != CON_FALSE

static char *
#if PROTO
def_stringization( char * repl_cur)
#else
def_stringization( repl_cur)
    char *  repl_cur;
#endif
/*
 * Define token stringization.
 * We store a magic cookie (which becomes " on output) preceding the
 * parameter as an operand of # operator.
 * Return the current pointer into replacement text if the token following #
 * is a parameter name, else return NULL.
 */
{
    int     c;
    register char *     temp;

    *repl_cur++ = ST_QUOTE;                 /* prefix               */
    if ((c = get()) == ' ') {
        *repl_cur++ = ' ';
        c = get();
    }
    token_p = repl_cur;                     /* Remember the pointer */
    if (scantoken( c, &repl_cur, repl_end) == NAM) {
        if ((temp = is_formal( identifier, TRUE)) != NULL) {
            repl_cur = temp;
#if MODE < STANDARD
            *repl_cur++ = ST_QUOTE;         /* suffix               */
#endif
            return  repl_cur;
        }
    }
    cerror( "Not a formal parameter \"%s\"", token_p, 0L, NULLST);  /* _E_  */
    return  NULL;
}

static char *
#if PROTO
mtokensave( const char * macroname)
#else
mtokensave( macroname)
    char *  macroname;
#endif
/*
 * A magic cookie is inserted if the token is identical to the macro name,
 * so the expansion doesn't recurse.
 * Return the advanced pointer into the replacement text or NULL.
 */
{
    register char *   repl_cur;

    if (str_eq( macroname, identifier)) {   /* Macro name in body   */
        repl_cur = token_p;                 /* Overwrite on token   */
        *repl_cur++ = DEF_MAGIC;            /* Save magic marker    */
        repl_cur = stpcpy( repl_cur, identifier);
                                            /* And save the token   */
        return  repl_cur;
    } else {
        return  NULL;
    }
}

#endif

#if STRING_FORMAL

static char *
#if PROTO
stparmscan( char * string_end)
#else
stparmscan( string_end)
    char *  string_end;
#endif
/*
 * String parameter scan.
 * This code -- if enabled -- recognizes a formal parameter in a string
 * literal or in a character constant.
 *      #define foo(bar, v) printf("%bar\n", v)
 *      foo( d, i)
 * expands to:
 *      printf("%d\n", i)
 * stparmscan() return the advanced pointer into the replacement text.
 * This has been superceded by # stringizing and string concatenation.
 */
{
    size_t  len;
    int     pos;
    int     i;
    char *  temp;
    char *  wp_end;                         /* Limit of scanning    */
    register char *     wp;     /* Pointer into the quoted literal  */

    for (i = 0; i < (nargs & ~VA_ARGS); i++) {
        len = parlen[ i];
        wp_end = string_end - len;
        for (wp = token_p + 1; wp < wp_end; wp++) {
            if (memcmp( wp, parlist[ i], len) == 0) {   /* Matched  */
                temp = savestring( token_p);
                pos = wp - token_p;
                *wp++ = MAC_PARM;       /* Convert string formal    */
                *wp++ = i + 1;                      /*   to MAGIC.  */
                strcpy( wp--, temp + pos + len);    /* Latter text  */
                free( temp);
                string_end = wp_end + 2;
                wp_end -= len - 2;
            }
        }
    }

    return  string_end;
}

#endif  /* STRING_FORMAL    */

static void
#if PROTO
doundef( void)
#else
doundef()
#endif
/*
 * Remove the symbol from the defined list.
 * Called from control().
 */
{
    register DEFBUF *   defp;
    int     c;

    if ((c = skipws()) == '\n') {
        cerror( no_ident, NULLST, 0L, NULLST);
        unget();
        return;
    }
    if (scantoken( c, (workp = work, &workp), work_end) != NAM) {
        cerror( not_ident, work, 0L, NULLST);
        skipnl();
        unget();
    } else {
        if ((defp = lookid( identifier)) == NULL) {
            if (warn_level & 4)
                cwarn( "\"%s\" wasn't defined"              /* _W4_ */
                        , identifier, 0L, NULLST);
#if MODE != CON_FALSE
        } else if (defp->nargs < DEF_NOARGS - 1 /* Standard predef  */
#if MODE >= STANDARD && OK_PRAGMA_OP
                || defp->nargs == DEF_PRAGMA    /* _Pragma() pseudo-macro   */
#endif
                ) {
            cerror( "\"%s\" shouldn't be undefined"         /* _E_  */
                    , identifier, 0L, NULLST);
#endif
        } else {
#if MODE >= STANDARD
            c = skipws();
            unget();
            if (c != '\n')                      /* Trailing junk    */
                return;
            else
                undefine( identifier);
#else
            undefine( identifier);
#endif
        }
    }
}

/*
 *                  C P P   S y m b o l   T a b l e s
 *
 * SBSIZE defines the number of hash-table slots for the symbol table.
 * It must be a power of 2.
 */

/* Symbol table queue headers.  */
static DEFBUF *     symtab[ SBSIZE];
#if MODE >= STANDARD
static int      num_of_macro;
#endif

DEFBUF *
#if PROTO
lookid( const char * name)
#else
lookid( name)
    char *          name;
#endif
/*
 * Look for the identifier in the symbol table.
 * If found, return the table pointer;  Else return NULL.
 */
{
    register const char *   np;
    DEFBUF *    dp;
    size_t      s_name;
    int         temp = -1;
    int         hash;

    for (hash = 0, np = name; *np != EOS; )
        hash += *np++;
    hash += s_name = (size_t)(np - name);
    s_name++;
    dp = symtab[ hash & SBMASK];            /* Starting bucket      */

    for ( ; dp != NULL; dp = dp->link) {    /* Search symbol table  */
        if ((temp = memcmp( dp->name, name, s_name)) >= 0)
            break;
    }

    return ((temp == 0) ? dp : (DEFBUF *)NULL);
}

static DEFBUF **
#if PROTO
lookprev( const char * name)
#else
lookprev( name)
    char *  name;                           /* Name of the macro    */
#endif
/*
 * Look for the place to insert the macro definition.
 * Return a pointer to the previous member in the linked list.
 * Return NULL when the macro is Standard predefined.
 */
{
    register const char *   np;
    DEFBUF **   prevp;
    DEFBUF *    dp;
    size_t      s_name;
    int         temp;
    int         hash;

    for (hash = 0, np = name; *np != EOS; )
        hash += *np++;
    hash += s_name = (size_t)(np - name);
    s_name++;
    prevp = & symtab[ hash & SBMASK];

    while ((dp = *prevp) != NULL) {
        if ((temp = memcmp( dp->name, name, s_name)) >= 0) {
#if MODE != CON_FALSE
            if (temp == 0 && dp->nargs < DEF_NOARGS - 1)
                return  NULL;               /* Standard predefined  */
#endif
            break;
        }
        prevp = &dp->link;
    }

    return  prevp;
}

DEFBUF *
#if PROTO
install( const char * name, int nargs, const char * parmnames
        , const char * repl)
#else
install( name, nargs, parmnames, repl)
    char *  name;                           /* Name of the macro    */
    int     nargs;                          /* The numbers of parms */
    char *  parmnames;          /* Names of parameters concatenated */
    char *  repl;                           /* Replacement text     */
#endif
/*
 * We know by lookid() that the macro is not defined.
 * Enter this name in the lookup table.
 * Returns a pointer to the definition block.
 * Returns NULL if the symbol was Standard-predefined.
 */
{
    register DEFBUF *   dp;
    register DEFBUF **  prevp;
    size_t              s_name, s_parmnames, s_repl;

    if ((prevp = lookprev( name)) == NULL)  /* Standard predefined  */
        return  NULL;
#if DEBUG
    if (parmnames == NULL || repl == NULL)      /* Shouldn't happen */
        cfatal( "Bug: Illegal macro installation of \"%s\"" /* _F_  */
                , name, 0L, NULLST);    /* Use "" instead of NULL   */
#endif
    s_name = strlen( name);
#if MODE == STANDARD
    s_parmnames = strlen( parmnames) + 1;
#else
    s_parmnames = 0;
#endif
    s_repl = strlen( repl) + 1;
    dp = (DEFBUF *) xmalloc( sizeof (DEFBUF) + s_name + s_parmnames + s_repl);
    dp->link = *prevp;                  /* Insert to linked list    */
    *prevp = dp;
    dp->nargs = nargs;
#if MODE == STANDARD
    dp->parmnames = (char *)dp + sizeof (DEFBUF) + s_name;
    dp->repl = dp->parmnames + s_parmnames;
    memcpy( dp->parmnames, parmnames, s_parmnames);
#else
    dp->repl = (char *)dp + sizeof (DEFBUF) + s_name;
#endif
    memcpy( dp->name, name, s_name + 1);
    memcpy( dp->repl, repl, s_repl);
#if MODE >= STANDARD
    if (nmacromin && (warn_level & 2) && ++num_of_macro == nmacromin + 1)
        cwarn( "More than %.0s%ld macros defined"           /* _W2_ */
                , NULLST , (long) nmacromin , NULLST);
#endif
    return  dp;
}

int
#if PROTO
undefine( const char * name)
#else
undefine( name)
    char *  name;                           /* Name of the macro    */
#endif
/*
 * Delete the macro definition from the symbol table.
 * Returns TRUE, if deleted;
 * Else returns FALSE (when the macro was not defined or was predefined).
 */
{
    register DEFBUF **  prevp;  /* Preceding definition in list     */
    DEFBUF *            dp;             /* Definition to delete     */

    if ((prevp = lookprev( name)) == NULL)  /* Standard predefined  */
        return  FALSE;
    dp = *prevp;                        /* Definition to delete     */
    *prevp = dp->link;          /* Link the previous and the next   */
    free( dp);                          /* Delete the definition    */
#if MODE >= STANDARD
    num_of_macro--;
#endif
    return  TRUE;
}

static void
#if PROTO
dumprepl( const DEFBUF * dp)
#else
dumprepl( dp)
    const DEFBUF *  dp;
#endif
/*
 * Dump replacement text.
 */
{
#if MODE == STANDARD
    char *  cp1;
    int     i;
#endif
    int     c;
    register const char *   cp;

    for (cp = dp->repl; (c = *cp++ & UCHARMAX) != EOS; ) {

        switch (c) {
        case MAC_PARM:                              /* Parameter    */
            c = (*cp++ & UCHARMAX) - 1;
#if MODE == STANDARD
            for (i = 0, cp1 = parlist[ c]; i < parlen[ c]; i++)
                fputc( *cp1++, stdout);
#else
            fputc( 'a' + c % 26, stdout);
            if (c > 26)
                fputc( '0' + c / 26, stdout);
#endif
#if MODE < STANDARD && MODE != CON_FALSE
            if (*(cp + 1) == ST_QUOTE)
                cp++;               /* Skip the suffixed ST_QUOTE   */
#endif
            break;
#if COMMENT_INVISIBLE
        case COM_SEP:
            fputs( "/**/", stdout);
            break;
#endif
#if MODE != CON_FALSE
        case DEF_MAGIC:
            break;
        case CAT:
            fputs( "##", stdout);
            break;
        case ST_QUOTE:
            fputs( "#", stdout);
            break;
#endif
        default:
            fputc( c, stdout);
            break;
        }
    }
}

void
#if PROTO
dumpadef( const char * why, const DEFBUF * dp, int newdef)
#else
dumpadef( why, dp, newdef)
    const char *    why;
    DEFBUF *        dp;
    int     newdef;         /* TRUE if parmnames are currently in parlist[] */
#endif
/*
 * Dump a macro definition.
 */
{
#if MODE == STANDARD
    char *  cp, * cp1;
#endif
    int     nargs = dp->nargs & ~VA_ARGS;
    register int    i;

#if MODE >= STANDARD && OK_PRAGMA_OP
    if (nargs == DEF_PRAGMA)                /* _Pragma pseudo-macro */
        return;
#endif
    if (why)
        printf( "%s ==>\n", why);
    printf( "%s#define %s", (nargs < DEF_NOARGS - 1) ? "/* " : "",
            dp->name);                      /* Macro name           */
    if (nargs >= 0) {                       /* Parameter list       */
#if MODE == STANDARD
        printf( "(%s)", dp->parmnames);
        if (! newdef) {
            for (i = 0, cp = dp->parmnames; i < nargs; i++, cp = cp1 + 1) {
                if ((cp1 = strchr( cp, ',')) == NULL)
                    parlen[ i] = strlen( cp);
                else
                    parlen[ i] = (int) (cp1 - cp);
                parlist[ i] = cp;
            }
        }
#else
        if (newdef) {
            printf( "(%s)", parlist[0]);
        } else if (nargs == 0) {
            fputs( "()", stdout);
        } else {
            fputc( '(', stdout);
            for (i = 0; i < nargs; i++) {   /* Make parameter list  */
                fputc( 'a' + i % 26, stdout);
                if (i >= 26)
                    fputc( '0' + i / 26, stdout);
                if (i + 1 < nargs)
                    fputc( ',', stdout);
            }
            fputc( ')', stdout);
        }
#endif
    }
    if (*dp->repl) {
        fputs( "  ", stdout);
        dumprepl( dp);                      /* Replacement text     */
    }
    if (nargs < DEF_NOARGS - 1)             /* Standard predefined  */
        fputs( " */", stdout);
    fputc( '\n', stdout);
}

void
#if PROTO
dumpdef( void)
#else
dumpdef()
#endif
/*
 * Dump all the current macro definitions to output stream.
 */
{
    register DEFBUF *   dp;
    DEFBUF **   syp;

    sharp();            /* Report the current source file & line    */
    fputs( "\n/* Currently defined macros. */\n", stdout);
    for (syp = symtab; syp < &symtab[ SBSIZE]; syp++) {
        if ((dp = *syp) != NULL) {
            do {
                dumpadef( NULLST, dp, FALSE);
            } while ((dp = dp->link) != NULL);
        }
    }
    wrongline = TRUE;               /* Line number is out of sync   */
}

