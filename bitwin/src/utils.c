#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "debug.h"


//uint32_t start_time;


// no allocations needed, it just shifts the pointer.
// can be used directly on string literals without any extra
// allocation.
const char* filename_from_path(const char* path) {
    for (path += strlen(path); *path != '/'; path--);
    return path + 1;
}
