#ifndef ASSERT_H
#define ASSERT_H

#ifdef __DEBUG__
#include <stdlib.h>
#endif
// TODO: copy some of the macros from this to remove it as a header dependancy
#include <assert.h>
// https://github.com/lattera/glibc/blob/master/assert/assert.h

#include "macros.h"


#ifndef __DEBUG__


#   define fassert(__expr) do {             \
        if (!(__expr)) abort();             \
    } while(0)                              \

#   define assert(__expr, __rcode) do {     \
        if (!(__expr)) return (__rcode);    \
    } while(0)                              \

#   define vassert(__expr) do {             \
        if (!(__expr)) return;              \
    } while(0)                              \

#   define wassert(__expr)  NOP()
//#   define error(__msg)     abort()
#   define warn(__msg)      NOP()
//#   define ffatal(...)      abort()
//#   define ferror(...)      abort()
#   define warnf(...)       NOP()

#   define debugf(...)      NOP()


#   define debug(__statement) NOP()


#else


#   include <stdio.h>

//extern char *assert_format;
#define ANSI_DEFAULT   "\033[0m"
#define ANSI_BLACK     "\033[0;30m"
#define ANSI_RED       "\033[0;31m"
#define ANSI_YELLOW    "\033[0;33m"
#define ANSI_REDBG     "\033[0;41m"

#define STR_FATAL   ANSI_BLACK ANSI_REDBG "[fatal]"   ANSI_DEFAULT
#define STR_ERROR   ANSI_RED              "[error]"   ANSI_DEFAULT
#define STR_WARN    ANSI_YELLOW           "[warn] "   ANSI_DEFAULT
#define STR_DEBUG                         "[debyg]"

#define STRINGIFY(__0) _STRINGIFY(__0)
#define _STRINGIFY(__0) #__0


#   define fassert(__expr) do {                                             \
        if (!(__expr)) {                                                    \
            printf(STR_FATAL "\t%s:%s: %s(...): assertion '%s' failed.\n"   \
                __FILE__, __LINE__, __ASSERT_FUNCTION, #__expr);            \
            fflush(stdout);                                                 \
            abort();                                                        \
        }                                                                   \
    } while(0)

#   define assert(__expr, __rcode) do {                                     \
        if (!(__expr)) {                                                    \
            printf(STR_ERROR "\t%s:%s: %s(...): "                           \
                "assertion '%s' failed. return code %x.\n"                  \
                __FILE__, __ASSERT_FUNCTION, __LINE__, #__expr, (__rcode)); \
            return (__rcode);                                               \
        }                                                                   \
    } while(0)

#   define vassert(__expr) do {                                             \
        if (!(__expr)) {                                                    \
            printf(STR_ERROR "\t%s:%s: %s(...): assertion '%s' failed.\n"   \
                __FILE__, __ASSERT_FUNCTION, __LINE__, #__expr);            \
            return;                                                         \
        }                                                                   \
    } while(0)

#   define wassert(__expr) do {                                             \
        if (!(__expr)) {                                                    \
            printf(STR_WARN "\t%s:%s: %s(...): assertion '%s' failed\n"     \
                __FILE__, __ASSERT_FUNCTION, __LINE__, #__expr);            \
        }                                                                   \
    } while(0)

/*#   define fatal(__msg) do {            \
        printf(STR_FATAL "\t" __msg);   \
        abort();                        \
    } while(0)

#   define error(__msg, ...) _error(__msg, __VA_ARGS__, ((void)0))
#   define _error(__msg, __rcode, ...) do {     \
        printf(STR_ERROR "\t" __msg);           \
        return __rcode;                         \
    } while(0)*/

#   define warn(__msg) do {                 \
        printf(STR_WARN "\t%s\n", __msg);   \
    } while(0)

//#   define ferror(...) do {} while(0)

    // WARNING: Requires static literal for __format
#   define warnf(__format, ...) do {                                    \
        printf(STR_WARN "\t" __format "\n" __VA_OPT__(,) __VA_ARGS__);  \
    } while(0)


#   define debug(__statement) do {      \
        __statement                     \
    } while(0)


#   define debugf(__format, ...) do {                                   \
        printf(STR_DEBUG "\t" __format "\n" __VA_OPT__(,) __VA_ARGS__); \
    } while(0)


#endif


#endif
