

#ifdef __debug__
// TODO: relocate debug.h to new library or something
#include "bitwin/debug.h"


enum {
    SHADER_VOID_T = 0,
    SHADER_INT_T,
};


typedef struct {
    uint8_t loc;
    #ifdef __debug__
    int type;
    char *name;
    shader_t *shader;
    #endif
} shader_type_t;

typedef shader_type_t shader_int_t;


__force_inline shader_type_t shader_init_int(char *name) {
    shader_int16_t self;
    debug(
        shader_t *current_shader = get_current_shader();
        self.name = name;
    );

    
}
