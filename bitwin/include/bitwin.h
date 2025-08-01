#ifndef WINDOW_H
#define WINDOW_H

// NOTE: Don't include <GL/glx.h> after this file, or type errors will occur

#include <stdint.h>
#include <limits.h>

// TODO: find a way to remove the macros dependancy from this header file
// I want people to only have access to this if they explicitly include it themselves
#include "utils/macros.h"
//#include "bitwin/debug.h"
#include "utils/utils.h"
#include "bitwin/keycode.h"


#define BITWIN_DEFAULT_X 0
#define BITWIN_DEFAULT_Y 0


#ifdef LINUX_BITWIN_C
#elifdef WIN32_BITWIN_C
#elifndef _X11_XLIB_H_
typedef unsigned long XID;
typedef void Display;         // is struct, so only to be used as a pointer in the header
typedef XID Window;
typedef void *GLXContext;
#endif


// alright, so I think the philosophy I am leaning towards is to not hide anything from the
// programmer. So all elements in the struct must be the same, but I can substitute equivalent
// types, so as to avoid file inclusions, which I will allow to be privatized.
// The reason is mostly because it isn't worth the effort nor ugliness to try to obscure
// values that are meant to be private.


typedef struct _bitwin bitwin_t;
typedef struct _bitwin_event bitwin_event_t;
typedef struct _bitwin_prop bitwin_prop_t;



#define BITWIN_VOID_MASK    (0<<0)
#define BITWIN_X_MASK       (1<<0)
#define BITWIN_Y_MASK       (1<<1)
#define BITWIN_WIDTH_MASK   (1<<2)
#define BITWIN_HEIGHT_MASK  (1<<3)
#define BITWIN_TITLE_MASK   (1<<4)
//#define BITWIN_CALL_MASK    (1<<5)
#define BITWIN_POS_MASK     (BITWIN_X_MASK     | BITWIN_Y_MASK     )
#define BITWIN_SIZE_MASK    (BITWIN_WIDTH_MASK | BITWIN_HEIGHT_MASK)
#define BITWIN_ALL_MASK     (BITWIN_POS_MASK   | BITWIN_SIZE_MASK   | BITWIN_TITLE_MASK)
/*#define BITWIN_EVENT_MASK_START     5
#define BITWIN_KEY_EVENT_MASK       (1<<(BITWIN_EVENT_MASK_START+0))
#define BITWIN_MOUSE_EVENT_MASK     (1<<(BITWIN_EVENT_MASK_START+1))
#define BITWIN_FOCUS_EVENT_MASK     (1<<(BITWIN_EVENT_MASK_START+2))
#define BITWIN_RESIZE_EVENT_MASK    (1<<(BITWIN_EVENT_MASK_START+3))
#define BITWIN_CHANGE_EVENT_MASK    (1<<(BITWIN_EVENT_MASK_START+4))
#define BITWIN_CLOSE_EVENT_MASK     (1<<(BITWIN_EVENT_MASK_START+5))*/
#define BITWIN_KEY_EVENT_MASK       (1<<5)
#define BITWIN_MOUSE_EVENT_MASK     (1<<6)
#define BITWIN_FOCUS_EVENT_MASK     (1<<7)
//#define BITWIN_RESIZE_EVENT_MASK    (1<<8)
#define BITWIN_CHANGE_EVENT_MASK    (1<<9)
#define BITWIN_CLOSE_EVENT_MASK     (1<<10)
#define BITWIN_EXPOSE_EVENT_MASK    (1<<11)
#define BITWIN_SHOW_EVENT_MASK      (1<<12)
#define BITWIN_INPUT_EVENT_MASK     (BITWIN_KEY_EVENT_MASK    | BITWIN_MOUSE_EVENT_MASK )
#define BITWIN_WINDOW_EVENT_MASK    (BITWIN_FOCUS_EVENT_MASK  | /*BITWIN_RESIZE_EVENT_MASK |*/ \
                                     BITWIN_CHANGE_EVENT_MASK | BITWIN_CLOSE_EVENT_MASK  | \
                                     BITWIN_EXPOSE_EVENT_MASK)
#define BITWIN_ALL_EVENT_MASK       (BITWIN_INPUT_EVENT_MASK  | BITWIN_WINDOW_EVENT_MASK)


typedef uint16_t bitwin_mask_t;
// note you can, and are encouraged to add restrict to these function parameters, 
// even if the predeclaration doesn't include them.
typedef int (*bitwin_event_callback_t)(const bitwin_t *, const bitwin_event_t *);


struct _bitwin_prop {
    int16_t x, y;
    int16_t width, height;
    char* title;
    //BITWIN_EVENT_CALLBACK bitwin_mask_t update_mask;
    //BITWIN_EVENT_CALLBACK keyup_callback;
    //BITWIN_EVENT_CALLBACK keydown_callback;
    //BITWIN_EVENT_CALLBACK mmotion_callback;
    //BITWIN_EVENT_CALLBACK mbuttonup_callback;
    //BITWIN_EVENT_CALLBACK mbuttondown_callback;
    union {
        struct {
            bitwin_event_callback_t key_callback;
            bitwin_event_callback_t mouse_callback;
            bitwin_event_callback_t focus_callback;
            // for when the window is resized
            // it is worth noting that for both the resize and the change events, 
            // resizing the window doesn't change the client area automatically
            // return 0 to let that scale automatically, and return nonzero to handle 
            // it yourself
            //bitwin_event_callback_t resize_callback;
            // for when a geometry property changes on the window
            bitwin_event_callback_t change_callback;
            bitwin_event_callback_t close_callback;
            bitwin_event_callback_t show_callback;
            // for when portion of window is obscured
            bitwin_event_callback_t expose_callback;
        };
        struct {
            bitwin_event_callback_t _cb[7];
        } callback_block;
    };
};




enum {
    /*BITWIN_EVENT_KEYUP = 0,
    BITWIN_EVENT_KEYDOWN,
    BITWIN_EVENT_MOTION,
    BITWIN_EVENT_BUTTONUP,
    BITWIN_EVENT_BUTTONDOWN,*/

    BITWIN_NULL = 0,
    // struct key
    BITWIN_KEYUP,
    BITWIN_KEYDOWN,
    // struct mouse
    BITWIN_MMOTION,
    BITWIN_MENTER,
    BITWIN_MLEAVE,
    BITWIN_BTNUP,
    BITWIN_BTNDOWN,
    // struct resize
    //BITWIN_RESIZE,
    // struct change
    BITWIN_CHANGE,
    // struct expose
    BITWIN_EXPOSE,
    BITWIN_NOEXPOSE,
    // no struct
    BITWIN_FOCUS,
    BITWIN_UNFOCUS,
    BITWIN_SHOW,
    BITWIN_HIDE,
    BITWIN_CLOSE,
};


struct _bitwin_event {
    bitwin_t *bitwin;
    uint32_t time_ms;
    uint16_t type;
    union {
    
        struct {
            uint8_t mod;
            uint8_t keycode;
        } key;
        
        struct {
            uint8_t mod;
            uint8_t button;
            int16_t x, y;
            // the event handler wont be frame limited, so dx and dy are being
            // moved to the bitinput api
            //int16_t dx, dy;
            //uint16_t x_root, y_root;
        } mouse;
        
        //struct {} focus;

        // TODO:
        // consolidate resize and change structs
        // really they should be the same thing, and changes
        // should be interceptable with all of them
        /*struct {
            int16_t width, height;
        } resize;*/
        
        struct {
            bitwin_mask_t change_mask;
            int16_t x, y;
            int16_t width, height;
        } change;

        // expose event handling removed until the expose event is better understood by me
        // also look into GraphicsExpose and NoExpose
        // https://tronche.com/gui/x/xlib/events/exposure/graphics-expose-and-no-expose.html
        // until then, it probably isn't a very important one to implement
        /*struct {
            int16_t count;          // how many expose events are left
            int16_t x, y;
            int16_t width, height;
        } expose;*/                   // only evaluate if count=0 for simple expose handling
        
    };
};



// I guess the gnu23 standard defines linux as a macros somewhere,
// and it was causing a whole lot of compiler errors
#undef linux

struct _bitwin {
    union {
    
        struct {            // for linux
            Display *xdisp;
            int xscreen;
            Window xwin;
            GLXContext glx_context;
        } linux;
        
        struct {
            // ...
            void* none; // to satiate the pedantic gods
        } win32;
        
        struct {
            // ...
            void* none; // to satiate the pedantic gods
        } wasm;
    };
    //_Atomic bool run_event_handler;
    //bool run_event_handler;

    // this is used to store callbacks, and last known properties
    // in order to detect property changes
    // also, I wanted to reuse the property masks for the event handling, 
    // which requires using this struct that they were meant for, for consistancy
    // TODO: nevermind, I need to get rid of this. The properties are no longer being used
    // except for the callbacks
    bitwin_prop_t prop;

    // I am putting this in the window class because:
    // 1. lets be honest, there will only ever be one window instance, so storage isn't an issue
    // 2. events are going to be called a lot, so having members that don't need to be set
    //    every time would probably be a good idea.
    // Actually, I suppose that last point is what kills it too. There will only really be one member
    // that doesn't need to be set every time. So nevermind.
    // also, the handler doesnt return until window close, so it is instanced anyway.
    //bitwin_event_t event;
};



int bitwin_new(bitwin_t *restrict bitwin, bitwin_prop_t *restrict prop);
void bitwin_close(bitwin_t *bitwin);
void bitwin_show(bitwin_t *bitwin);
void bitwin_hide(bitwin_t *bitwin);
int bitwin_set(bitwin_t *restrict bitwin, bitwin_prop_t *restrict prop, bitwin_mask_t update_mask);
int bitwin_get(bitwin_t *restrict bitwin, bitwin_prop_t *restrict prop, bitwin_mask_t update_mask);
// runs event handler. only returns when window is destroyed
int bitwin_handler(bitwin_t *restrict bitwin);
// binds graphics context
int bitwin_bind(bitwin_t *bitwin);
int bitwin_unbind(bitwin_t *bitwin);
// swap buffers, basically update the screen with the new buffer
// TODO: consider different name
void bitwin_swap(bitwin_t *bitwin);


#endif
