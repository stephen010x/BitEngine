#ifndef MACROS_H
#define MACROS_H

#include <stdbool.h>

#define NOP() ((void)0)
#define lenof(__0) (sizeof(__0)/sizeof(*(__0)))

#define stringify(__0) _stringify(__0)
#define _stringify(__0) #__0

#define __FORCE_INLINE__ __attribute__((always_inline))
#define __INLINE__ __ALWAYS_INLINE__

#define once(__statement) do {      \
        static bool ran = false;    \
        if (!ran) {                 \
            __statement             \
            NOP();                  \
            ran = true;             \
        }                           \
    } while(0);

#endif
