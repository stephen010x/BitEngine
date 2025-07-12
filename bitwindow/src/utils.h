#ifndef UTILS_H
#define UTILS_H



#define TICK_TO_MILLI(__n) ((uint32_t)((((uint64_t)__n)*1000)/CLOCKS_PER_SEC))



extern uint32_t start_time = 0;



// use to reset millisecond clock
__FORCE_INLINE__ void sync_milli(void) {
    start_time = TICK_TO_MILLI(clock());
    debugf("sync_milli() called at %u ms", start_time);
    debug(
        static int i = 0;
        if (i >= 1)
            warn("start_timekeeping function called more than once");
    );
}


// get milliseconds since program start
__FORCE_INLINE__ uint32_t milli(void) {
    clock_t time = TICK_TO_MILLI(clock()) - start_time;
    debug( once(
        debugf("first call to milli() returned %d", time);
    ););
}


#endif
