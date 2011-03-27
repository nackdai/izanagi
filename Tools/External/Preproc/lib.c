/*
 *                              L I B . C
 *                  L i b r a r y   R o u t i n e s
 *
 * Some variation of standard library functions.
 * Some standard functions for the library which has not those or the library
 *      which has only non-conforming ones.
 *
 * 1998/08      created     Psycho
 */

#if 1
#if PREPROCESSED
#include    "cpp.H"
#else
#include    "system.H"  /* For PROTO, UCHARMAX and function declarations    */
#endif
#endif

#if 1

/* Based on the public-domain-software released by AT&T.    */

#define OPTERR( s, c)   if (opterr) {   \
    fputs( argv[0], stderr);    \
    fputs( s, stderr);          \
    putc( c, stderr);           \
    putc( '\n', stderr);        \
    }

int     optind = 1;
int     opterr = 1;
int     optopt;
char *  optarg;

#if PROTO
int     getopt( int argc, char * const * argv, const char * opts)
#else
int     getopt( argc, argv, opts)
    int         argc;
    char **     argv;
    char *      opts;
#endif
/*
 * Get the next option (and it's argument) from the command line.
 */
{
    const char * const   error1 = ": option requires an argument --";
    const char * const   error2 = ": illegal option --";
    static int      sp = 1;
    register int    c;
    register char *     cp;

    if (sp == 1) {
        if (argc <= optind ||
                argv[ optind][ 0] != '-' || argv[ optind][ 1] == '\0') {
            return  EOF;
        } else if (strcmp( argv[ optind], "--") == 0) {
            optind++;
            return  EOF;
        }
    }
/*  optopt = c = (unsigned char) argv[ optind][ sp];    */
    optopt = c = argv[ optind][ sp] & UCHARMAX;
    if (c == ':' || (cp = strchr( opts, c)) == NULL) {
        OPTERR( error2, c);
        if (argv[ optind][ ++sp] == '\0') {
            optind++;
            sp = 1;
        }
        return  '?';
    }
    if (*++cp == ':') {
        if (argv[ optind][ sp+1] != '\0') {
            optarg = &argv[ optind++][ sp+1];
        } else if (argc <= ++optind) {
            OPTERR( error1, c);
            sp = 1;
            return  '?';
        } else {
            optarg = argv[ optind++];
        }
        sp = 1;
    } else {
        if (argv[ optind][ ++sp] == '\0') {
            sp = 1;
            optind++;
        }
        optarg = NULL;
    }
    return  c;
}

#endif

#if 0

#if PROTO
char *  fgets( char * buf, int size, FILE * fp)
#else
char *  fgets( buf, size, fp)
    char *  buf;
    int     size;
    FILE *  fp;
#endif
{
    int     c;
    char *  cp = buf;

    if (size <= 1)
        return  NULL;
    while (--size && (c = getc( fp)) != EOF && c != '\n')
        *cp++ = c;
    if (c != EOF || buf < cp) {
        if (c == '\n')
            *cp++ = '\n';
        *cp = '\0';
        return  buf;
    } else {
        return  NULL;
    }
}

#endif

#if 1

#if PROTO
char *  stpcpy( char * dest, const char * src)
#else
char *  stpcpy( dest, src)
    char *  dest;
    char *  src;
#endif
/*
 * Copy the string and return the advanced pointer.
 */
{
    size_t  size;

    size = strlen( src);
    memcpy( dest, src, size + 1);
    return  dest + size;
}

#endif

#if 0

#if PROTO
char *  memmove( char * dest, const char * src, size_t size)
#else
char *  memmove( dest, src, size)
    char *  dest;
    char *  src;
    size_t  size;
#endif
{
    char *  cp = dest;

    if (dest < src) {
        while (size--)
            *cp++ = *src++;
    } else {
        cp += size - 1;
        src += size - 1;
        while (size--)
            *cp-- = *src--;
    }
    return  dest;
}

#endif

#if 0

#if PROTO
char *  memcpy( char * dest, const char * src, size_t size)
#else
char *  memcpy( dest, src, size)
    char *  dest;
    char *  src;
    size_t  size;
#endif
{
    char *  p = dest;

    while (size-- > 0)
        *p++ = *src++;
    return  dest;
}

#endif

#if 0

#if PROTO
int     memcmp( const char * s1, const char * s2, size_t size)
#else
int     memcmp( s1, s2, size)
    char *  s1;
    char *  s2;
    size_t  size;
#endif
{
    for ( ; size; size--, s1++, s2++) {
        if (*s1 != *s2)
            return  ((*s1 & UCHARMAX) < (*s2 & UCHARMAX)) ? -1 : 1;
    }
    return  0;
}

#endif

