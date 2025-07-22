#ifdef ENABLE_BITWIN_TEST_C

#include <stdio.h>
#include <GL/gl.h>
//#include <GL/glx.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

//#include "bitwin/macros.h"
//#ifndef __DEBUG__
//#   define __DEBUG__
//#endif
#include "utils/debug.h"
#include "bitwin.h"


//typeof(*(bitwin_event_callback_t)NULL) key_handler;
//typeof(*(bitwin_event_callback_t)NULL) mouse_handler;

DEREF_TYPE(bitwin_event_callback_t) key_handler;
DEREF_TYPE(bitwin_event_callback_t) mouse_handler;
DEREF_TYPE(bitwin_event_callback_t) misc_handler;


void draw_triangle(void);



// Simple vertex shader code
/*const char *vertex_shader_source = "     \
    #version 120\n                              \
    attribute vec2 position;                    \
    void main() {                               \
        gl_Position = vec4(position, 0.0, 1.0); \
    }";

// Simple fragment shader code
const char *fragment_shader_source = "       \
    #version 120\n                                  \
    void main() {                                   \
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);    \
    }";

const float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
*/


int main() {
    bitwin_t bitwin;
    pthread_t thread;

    bitwin_prop_t prop = (typeof(prop)){
        .x = 0,
        .y = 0,
        .width  = 300,
        .height = 300,
        .title  = "bitwin test",
        .key_callback   = key_handler,
        .mouse_callback = mouse_handler,
        //.resize_callback = misc_handler,
        .change_callback = misc_handler,
    };

    // test window creation
    int err = bitwin_new(&bitwin, &prop);
    assert(("bitwin_new(...)", !err), 1);

    bitwin_show(&bitwin);

    // test the get function
    prop = (typeof(prop)){0};
    bitwin_get(&bitwin, &prop, BITWIN_ALL_MASK);
    msgf("bitwin_get test\n\tx: %hd\n\ty: %hd\n\twidth: %hd\n\theight:%hd",
        prop.x, prop.y, prop.width, prop.height);

    // test the set function
    prop.x += 100;
    prop.y += 100;
    prop.width  += 100;
    prop.height += 100;
    prop.callback_block = (typeof(prop.callback_block)){0};
    msgf("bitwin_set test");
    bitwin_get(&bitwin, &prop, BITWIN_POS_MASK | BITWIN_SIZE_MASK);


    // testing gcc's nested function extension
    // also this is going to be super unsafe
    // lets see if nested functions trampolines can handle being called
    // from a new thread
    void *async_handler(void *arg) {
        (void)arg;
        // test mouse and key handling, as well as the handler itself
        // parameters are read only, so it is probably perfectly thread safe
        bitwin_handler(&bitwin);
        return NULL;
    }

    pthread_create(&thread, NULL, &async_handler, NULL);

    // bind graphics context
    bitwin_bind(&bitwin);

    for (;;) {
        // draw triangle
        draw_triangle();
        bitwin_swap(&bitwin);
        // sched_yield();
        sleep(1);
    }

    // gate so that the parent function never exits until the thread finishes.
    // it is a nested function, so bad stuff will happen if the parent leaves
    // before the thread finishes
    pthread_join(thread, NULL);

    return 0;
}





int key_handler(const bitwin_t *restrict bitwin, const bitwin_event_t *restrict event) {
    char* type_str;
    (void)bitwin;
    
    switch (event->type) {
        case BITWIN_KEYUP:
            type_str = "keyup";
            break;
        case BITWIN_KEYDOWN:
            type_str = "keydown";
            break;
        default:
            warnf("unknown key event type (%hu)", event->type);
            return 0;
    }

    msgf("%s %d @%ums", type_str, (int)event->key.keycode, event->time_ms);

    return 0;
}




int mouse_handler(const bitwin_t *restrict bitwin, const bitwin_event_t *restrict event) {
    (void)bitwin;
    
    switch (event->type) {
    
        case BITWIN_MMOTION:
            msgf("mouse motion xy:(%d, %d) @%ums", (int)event->mouse.x, (int)event->mouse.y, 
                event->time_ms);
            break;
        
        case BITWIN_MENTER:
            msgf("mouse enter window @%ums", event->time_ms);
            break;
        
        case BITWIN_MLEAVE:
            msgf("mouse leave window @%ums", event->time_ms);
            break;
        
        case BITWIN_BTNUP:
            msgf("mouse button %d up @%ums", (int)event->mouse.button, event->time_ms);
            break;
        
        case BITWIN_BTNDOWN:
            msgf("mouse button %d down @%ums", (int)event->mouse.button, event->time_ms);
            break;
        
        default:
            warnf("unknown mouse event type (%d) @%ums", (int)event->type, event->time_ms);
            return 0;
    }

    return 0;
}



int misc_handler(const bitwin_t *restrict bitwin, const bitwin_event_t *restrict event) {
    (void)bitwin;

    switch (event->type) {
        /*case BITWIN_RESIZE:
            msgf("window resize width,height:(%d, %d) @%ums", 
                event->resize.width, event->resize.height, event->time_ms);
            break;*/

        case BITWIN_CHANGE:
            msgf(
                "window change event\n"
                "\tmask   : %04x\n"
                "\tx      : %d\n" "\ty      : %d\n"
                "\twidth  : %d\n" "\theight : %d",
                event->change.change_mask,
                event->change.x, event->change.y,
                event->change.width, event->change.height
            )
            break;
    }

    return 0;
}


void draw_triangle(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0.0, 1.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(-1.0, -1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(1.0, -1.0);
    glEnd();
}



#endif
