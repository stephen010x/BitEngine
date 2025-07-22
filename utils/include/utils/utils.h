#ifndef UTILS_H
#define UTILS_H

//#include <time.h>
#include <stdint.h>

//#include "bitwin/macros.h"
//#include "bitwin/debug.h"


//#define TICK_TO_MILLI(__n) ((uint32_t)((((uint64_t)__n)*1000)/CLOCKS_PER_SEC))



//extern uint32_t start_time;


const char* filename_from_path(const char* path);

uint32_t millis_usage_total(void);
uint32_t millis_usage(void);
uint32_t millis(void);


// use to reset millisecond clock
// now we just ignore this, because glock_gettime guarentees program execution time
/*__FORCE_INLINE__ void sync_milli(void) {
    start_time = TICK_TO_MILLI(clock());
    debugf("sync_milli() called at %u ms", start_time);
    debug(
        static int i = 0;
        if (i >= 1)
            warn("start_timekeeping function called more than once");
    );
}*/







// TODO: add more from this list
//       or maybe just use a standard library. That might just be better
// https://gcc.gnu.org/onlinedocs/gcc/Built-in-Functions.html
// https://gcc.gnu.org/onlinedocs/gcc/Numeric-Builtins.html
// swaps the order of bytes
// returns uint<n>_t, parameter is uint<n>_t
#define bswap16(__n)  __builtin_bswap16(__n)
#define bswap32(__n)  __builtin_bswap32(__n)
#define bswap64(__n)  __builtin_bswap64(__n)
#define bswap128(__n) __builtin_bswap128(__n)





 
#endif
