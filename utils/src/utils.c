#ifdef __LINUX__

#include <stdint.h>
#include <string.h>
#include <time.h>

#include "utils/utils.h"
#include "utils/debug.h"


__noinit static uint32_t start_time;


// no allocations needed, it just shifts the pointer.
// can be used directly on string literals without any extra
// allocation.
__share const char* filename_from_path(const char* path) {
    for (path += strlen(path); *path != '/'; path--);
    return path + 1;
}



// will run before main() gets called
__construct static void utils_init(void) {
    start_time = 0;
    start_time = millis();
    //debugf("CLOCKS_PER_SEC = %ld", CLOCKS_PER_SEC);
    debugf("start_time set to %u ms", start_time);

    debug(
        struct timespec ts;
    
        clock_getres(CLOCK_PROCESS_CPUTIME_ID, &ts);
        debugf("milli_usage_total() resolution is %f ms", ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0);
        
        clock_getres(CLOCK_THREAD_CPUTIME_ID, &ts);
        debugf("milli_usage() resolution is %f ms", ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0);
        
        clock_getres(CLOCK_MONOTONIC_RAW, &ts);
        debugf("milli() resolution is %f ms", ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0);
    );
}



// get milliseconds of cpu time the program has taken up in total, counting all threads
__share uint32_t millis_usage_total(void) {
    //debugf("clock() = %ld", clock());
    // the clock() function was misbehaving, and was incrementing arbitrary values
    // every time called rather than keeping time
    //uint32_t time = TICK_TO_MILLI(clock()) - start_time;

    struct timespec ts;
    uint32_t time;

    // cpu time consumed by all threads in this process
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);

    time = (uint32_t)((uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000);
    
    /*debug( once(
        debugf("CLOCKS_PER_SEC = %ld", CLOCKS_PER_SEC);
        debugf("first call to milli() returned %u ms", time);
    ););*/
    return time;
}


// total milliseconds of cpu time that the current thread has taken
// useful for virtual time
__share uint32_t millis_usage(void) {
    struct timespec ts;
    
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    return (uint32_t)((uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000);
}


// returns ms since program start
// The really smart thing about this is that it doesn't matter if the cast to the 
// 64 bit time to 32 bit time overflows, because subtracting the start_time alongside
// casual assembly overflow magic means that my program is guarenteed 2^32 milliseconds
// before this function returns an overflowed value
__share uint32_t millis(void) {
    struct timespec ts;
    uint32_t time;

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    time = (uint32_t)((uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000);
    return time - start_time;
}

#endif
