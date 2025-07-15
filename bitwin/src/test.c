#ifdef ENABLE_BITWIN_TEST_C

#include <stdio.h>

#include "macros.h"
#ifndef __DEBUG__
#   define __DEBUG__
#endif
#include "debug.h"
#include "bitwin.h"


//typeof(*(bitwin_event_callback_t)NULL) key_handler;
//typeof(*(bitwin_event_callback_t)NULL) mouse_handler;

DEREF_TYPE(bitwin_event_callback_t) key_handler;
DEREF_TYPE(bitwin_event_callback_t) mouse_handler;
DEREF_TYPE(bitwin_event_callback_t) misc_handler;



int main() {
    bitwin_t bitwin;

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
    debugf("bitwin_get test\n\tx: %hd\n\ty: %hd\n\twidth: %hd\n\theight:%hd",
        prop.x, prop.y, prop.width, prop.height);

    // test the set function
    prop.x += 100;
    prop.y += 100;
    prop.width  += 100;
    prop.height += 100;
    prop.callback_block = (typeof(prop.callback_block)){0};
    debugf("bitwin_set test");
    bitwin_get(&bitwin, &prop, BITWIN_POS_MASK | BITWIN_SIZE_MASK);

    // test mouse and key handling, as well as the handler itself
    bitwin_handler(&bitwin);

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

    debugf("%s %d @%ums", type_str, (int)event->key.keycode, event->time_ms);

    return 0;
}




int mouse_handler(const bitwin_t *restrict bitwin, const bitwin_event_t *restrict event) {
    (void)bitwin;
    
    switch (event->type) {
    
        case BITWIN_MMOTION:
            debugf("mouse motion xy:(%d, %d) @%ums", (int)event->mouse.x, (int)event->mouse.y, 
                event->time_ms);
            break;
        
        case BITWIN_MENTER:
            debugf("mouse enter window @%ums", event->time_ms);
            break;
        
        case BITWIN_MLEAVE:
            debugf("mouse leave window @%ums", event->time_ms);
            break;
        
        case BITWIN_BTNUP:
            debugf("mouse button %d up @%ums", (int)event->mouse.button, event->time_ms);
            break;
        
        case BITWIN_BTNDOWN:
            debugf("mouse button %d down @%ums", (int)event->mouse.button, event->time_ms);
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
            debugf("window resize width,height:(%d, %d) @%ums", 
                event->resize.width, event->resize.height, event->time_ms);
            break;*/

        case BITWIN_CHANGE:
            debugf(
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



#endif
