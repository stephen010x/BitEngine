#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdint.h>

#include "macros.h"
#include "debug.h"


//#define TICK_TO_MILLI(__n) ((uint32_t)((((uint64_t)__n)*1000)/CLOCKS_PER_SEC))



//extern uint32_t start_time;


const char* filename_from_path(const char* path);


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


// get milliseconds since program start
// TODO: may be a linux specific implementation, so consider moving this to linux dir
__force_inline uint32_t milli(void) {
    //debugf("clock() = %ld", clock());
    // the clock() function was misbehaving, and was incrementing arbitrary values
    // every time called rather than keeping time
    //uint32_t time = TICK_TO_MILLI(clock()) - start_time;

    struct timespec tp;

    // cpu time consumed by all threads in this process
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);

    uint32_t time = (uint32_t)((uint64_t)tp.tv_sec * 1000 + (uint64_t)tp.tv_nsec / 1000000);
    
    debug( once(
        debugf("CLOCKS_PER_SEC = %ld", CLOCKS_PER_SEC);
        debugf("first call to milli() returned %u ms", time);
    ););
    return time;
}




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
