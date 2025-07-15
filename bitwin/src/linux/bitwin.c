#ifdef __LINUX__
#include <stdbool.h>
#include <time.h>
//#include <stdatomic.h>
#include <X11/Xlib.h>

#include "../macros.h"
#include "../debug.h"
#define LINUX_WINDOW_C
#include "../bitwin.h"
#include "../utils.h"
#include "../keycode.h"

// https://tronche.com/gui/x/xlib/
// https://tronche.com/gui/x/xlib/window/XCreateWindow.html
// https://x.z-yx.cc/libX11/libX11/04-chapter-3-window-functions.html#Creating_Windows
// https://www.x.org/docs/X11/xlib.pdf
// https://en.cppreference.com/w/c/23.html


// TODO: I am heavily considering renaming all "bitwin_p" to "bitwin"


#define DEFAULT_WINDOW_NAME "BitWindow"


typeof(*(XSetErrorHandler)NULL) bitwin_error_handler;



// NOTE:
// New style rule, all variables declared must appear at the top of their scope




//int bitwin_new(bitwin_t *bitwin_p, int x, int y, unsigned int width, unsigned int height, char *title) {
// TODO: add a way to set background color
int bitwin_new(bitwin_t *restrict bitwin, bitwin_prop_t *restrict prop) {
    int      xscreen;
    Display *xdisp;
    Window   xrootwin;
    Window   xwin;

    debug( XSetErrorHandler(&bitwin_error_handler); );
    
    xdisp = XOpenDisplay(NULL);
    assert(("XOpenDisplay(NULL)", xdisp != NULL), 1);

    xscreen = DefaultScreen(xdisp);

    xrootwin = RootWindow(xdisp, xscreen);

    // geez, the documentation is nonexistant for detecting whether or not this worked
    // so I guess we simply just have to assume it worked.
    xwin = XCreateSimpleWindow(
        xdisp, 
        xrootwin, 
        prop->x, prop->y, 
        prop->width, prop->height, 
        0, BlackPixel(xdisp, xscreen),     // border width and border color
        //WhitePixel(xdisp, xscreen)         // background color
        BlackPixel(xdisp, xscreen)         // background color
    );

    // set window title
    XStoreName(xdisp, xwin, (prop->title != NULL) ? (prop->title) : (DEFAULT_WINDOW_NAME));

    // set callbacks
    bitwin->prop.callback_block = prop->callback_block;

    bitwin->linux.xdisp = xdisp;
    bitwin->linux.xscreen = xscreen;
    bitwin->linux.xwin = xwin;

    return 0;
}





void bitwin_close(bitwin_t *bitwin) {
    debug( XSetErrorHandler(NULL); );
    XDestroyWindow(bitwin->linux.xdisp, bitwin->linux.xwin);
    XCloseDisplay(bitwin->linux.xdisp);
    debug( *bitwin = (bitwin_t){0}; );
}


// TODO: I would like to make these inline, but it would require including the xlib header
// in the header for this file, which I am wanting to avoid.
void bitwin_show(bitwin_t *bitwin) {
    XMapWindow(bitwin->linux.xdisp, bitwin->linux.xwin);
}

void bitwin_hide(bitwin_t *bitwin) {
    XUnmapWindow(bitwin->linux.xdisp, bitwin->linux.xwin);
}






int bitwin_set(bitwin_t *restrict bitwin, bitwin_prop_t *restrict prop, bitwin_mask_t update_mask) {

    if (update_mask & (~(BITWIN_POS_MASK | BITWIN_SIZE_MASK))) { 
        Window root;
        int x, y;
        unsigned int width, height, border_width, depth;
        
        XGetGeometry(bitwin->linux.xdisp, bitwin->linux.xwin, 
            &root, &x, &y, &width, &height, &border_width, &depth);

        if (update_mask & (~BITWIN_X_MASK)) x = prop->x;
        if (update_mask & (~BITWIN_Y_MASK)) y = prop->y;
        if (update_mask & (~BITWIN_WIDTH_MASK))  width  = prop->width;
        if (update_mask & (~BITWIN_HEIGHT_MASK)) height = prop->height;

        XMoveResizeWindow(bitwin->linux.xdisp, bitwin->linux.xwin, x, y, width, height);
    }

    if (update_mask & (~BITWIN_TITLE_MASK))
        XStoreName(bitwin->linux.xdisp, bitwin->linux.xwin, 
            (prop->title != NULL) ? (prop->title) : (DEFAULT_WINDOW_NAME));


    // as long as the callback members of the bitwin are 64 bit aligned, these should be thread safe
    // this is the only place that should set them, and it is fine if whatever is reading them
    // lapses one or two frames before it begins calling the right one.
    if (update_mask & (~BITWIN_ALL_EVENT_MASK)) {
        if (update_mask & (~BITWIN_KEY_EVENT_MASK))    
            bitwin->prop.key_callback = prop->key_callback;
        if (update_mask & (~BITWIN_MOUSE_EVENT_MASK))
            bitwin->prop.mouse_callback = prop->mouse_callback;
        if (update_mask & (~BITWIN_FOCUS_EVENT_MASK))
            bitwin->prop.focus_callback = prop->focus_callback;
        //if (update_mask & (~BITWIN_RESIZE_EVENT_MASK))
        //    bitwin->prop.resize_callback = prop->resize_callback;
        if (update_mask & (~BITWIN_CHANGE_EVENT_MASK))
            bitwin->prop.change_callback = prop->change_callback;
        if (update_mask & (~BITWIN_CLOSE_EVENT_MASK))
            bitwin->prop.close_callback = prop->close_callback;
        if (update_mask & (~BITWIN_EXPOSE_EVENT_MASK))
            bitwin->prop.expose_callback = prop->expose_callback;
        if (update_mask & (~BITWIN_CLOSE_EVENT_MASK))
            bitwin->prop.close_callback = prop->close_callback;
        if (update_mask & (~BITWIN_SHOW_EVENT_MASK))
            bitwin->prop.show_callback = prop->show_callback;
    }

    return 0;
}






// I am just going to be lazy with the efficiency of this function.
int bitwin_get(bitwin_t *restrict bitwin, bitwin_prop_t *restrict prop, bitwin_mask_t update_mask) {

    if (update_mask & (~(BITWIN_POS_MASK | BITWIN_SIZE_MASK))) {
    
        Window root;
        int x, y;
        unsigned int width, height, border_width, depth;
        
        XGetGeometry(bitwin->linux.xdisp, bitwin->linux.xwin, 
            &root, &x, &y, &width, &height, &border_width, &depth);

        prop->x = (int16_t)x;
        prop->y = (int16_t)y;
        prop->width = (uint16_t)width;
        prop->height = (uint16_t)height;
    }

    // too lazy to add support for this. Nobody will ever use it anyway.
    if (update_mask & (~BITWIN_TITLE_MASK))
        warn("support not yet implemented for getting title with bitwin_get");
    //assert(!(update_mask & (~BITWIN_TITLE_MASK)), 1);

    if (update_mask & (~BITWIN_ALL_EVENT_MASK))
        prop->callback_block = bitwin->prop.callback_block;

    return 0;
}







#ifdef __DEBUG__
int bitwin_error_handler(Display *restrict xdisp, XErrorEvent *restrict xerr) {
    char xerror_text[256];
    
    XGetErrorText(xdisp, xerr->error_code, xerror_text, sizeof(xerror_text));
    warnf("xlib error: '%s'", xerror_text);
    return 0;   // the docs dont explain what value to return, so whatever.
}
#endif






// TODO: this is a big boy. Maybe try to find a way to reduce it's size
// especially since it is handling all window messages
int bitwin_handler(bitwin_t *restrict bitwin) {

    XEvent xevent;
    bitwin_event_t event;
    //struct State state;
    
    // instanced to the handler, so information is retained. Good to know
    event = (bitwin_event_t){
        .bitwin = bitwin,
    };

    // TODO: use XQueryPointer to get initial positions for mouse
    // to prevent initial jitter
    // nevermind. dx and dy are being relocated to bitinput
    struct State {
        struct {
            /*int16_t x;
            int16_t y;
            int16_t dx;
            int16_t dy;*/
            uint8_t button;
        } mouse;
        struct {
            int16_t x, y;
            int16_t width, height;
        } change;
    } state = {0}; // TODO: this should not be initilized to zero, but I am lazy. 
                   //       It should start with actual win properties

    XSelectInput(bitwin->linux.xdisp, bitwin->linux.xwin, 
        KeyPressMask       | KeyReleaseMask       | 
        ButtonPressMask    | ButtonReleaseMask    | PointerMotionMask |
        EnterWindowMask    | LeaveWindowMask      | FocusChangeMask   | 
        /*ExposureMask   |*/ VisibilityChangeMask |
        /*ResizeRedirectMask |*/ StructureNotifyMask
        // turns out resizeredirect is not good for me to do
        // and I only ever want to use it if I am making a window manager
        // in other words I can probably only resize child windows with it
        // https://stackoverflow.com/questions/60493878/x11-in-c-cropping-happens-whenever-resizeredirectmask-is-set-when-the-window
    );

    //bitwin_p->run_event_handler = true;
    //while (bitwin_p->run_event_handler) {
    for(;;) {
        int retval;
    
        XNextEvent(bitwin->linux.xdisp, &xevent);   // blocking if event queue empty

        if (xevent.xany.window != bitwin->linux.xwin)
            continue;

        // first pass for individual variables
        switch (xevent.type) {
            case ButtonPress:       event.type = BITWIN_BTNDOWN; break;
            case ButtonRelease:     event.type = BITWIN_BTNUP;   break;
            case MotionNotify:      event.type = BITWIN_MMOTION; break;
            case EnterNotify:       event.type = BITWIN_MENTER;  break;
            case LeaveNotify:       event.type = BITWIN_MLEAVE;  break;
            //case Expose:            event.type = ;   break;
            //case NoExpose:          event.type = ;   break;
            case KeyPress:          event.type = BITWIN_KEYDOWN; break;
            case KeyRelease:        event.type = BITWIN_KEYUP;   break;
            //case ResizeRequest:     event.type = BITWIN_RESIZE;  break;
            case ConfigureNotify:   event.type = BITWIN_CHANGE;  break;
            case DestroyNotify:     event.type = BITWIN_CLOSE;   break;
            case MapNotify:         event.type = BITWIN_SHOW;    break;
            case UnmapNotify:       event.type = BITWIN_HIDE;    break;
            case FocusIn:           event.type = BITWIN_FOCUS;   break;
            case FocusOut:          event.type = BITWIN_UNFOCUS; break;
            
            default: event.type = BITWIN_NULL;
        }

        // set time of event
        //event.time_ms = milli();
        event.time_ms = 1000;

        // second pass for shared variables and function calls
        // the state mouse needed so that the dx is only updated once per frame
        // aaaand already we are at a bad start. I guess dx and dy are not meant for
        // the window handler. Maybe they should only be an bitinput construct, which will actually be
        // frame limited
        switch (xevent.type) {

        
            case ButtonPress:
            case ButtonRelease:
                //event.time = xevent.xbutton.time;
                //event.mouse = (typeof(((bitwin_event_t*)NULL)->mouse)){
                event.mouse = (MEMBER_TYPE(bitwin_event_t, mouse)){
                    .mod = KEY_NULL,    // TODO: xevent.xbutton.state
                    .button = (uint8_t)xevent.xbutton.button,
                    .x = xevent.xmotion.x,
                    .y = xevent.xmotion.y,
                };
                state.mouse.button = (uint8_t)xevent.xbutton.button;
                break;

                
            case MotionNotify:
                //event.time = xevent.xbutton.time;
                event.mouse = (MEMBER_TYPE(bitwin_event_t, mouse)){
                    .mod = KEY_NULL,    // TODO: xevent.xbutton.state
                    .button = state.mouse.button,
                    .x = xevent.xmotion.x,
                    .y = xevent.xmotion.y,
                };
                break;

                
            case EnterNotify:
            case LeaveNotify:
                //event.time = xevent.xbutton.time;
                event.mouse = (MEMBER_TYPE(bitwin_event_t, mouse)){
                    .mod = KEY_NULL,    // TODO: xevent.xbutton.state
                    .button = state.mouse.button,
                    .x = xevent.xcrossing.x,
                    .y = xevent.xcrossing.y,
                };
                break;

                
            /*case Expose:
                //event.time = milli();
                event.expose = {
                    .count = 
                    .x
                    .y
                    .width
                    .height
                };
                break;*/

                
            case KeyPress:
            case KeyRelease:
                event.key = (MEMBER_TYPE(bitwin_event_t, key)){
                    .mod = KEY_NULL,    // TODO: for my own sanity, fix the mod and the keycodes later
                    .keycode = (uint8_t)xevent.xkey.keycode,
                };
                break;

                
            /*case ResizeRequest:
                event.resize = (MEMBER_TYPE(bitwin_event_t, resize)){
                    .width  = xevent.xresizerequest.width,
                    .height = xevent.xresizerequest.height,
                };
                break;*/

            
            case ConfigureNotify:
                //TODO: maybe use typeof(event.change) rather than MEMBER_TYPE(bitwin_event_t, change)
                event.change = (MEMBER_TYPE(bitwin_event_t, change)){
                    .change_mask = 0,
                    .x = xevent.xconfigurerequest.x,
                    .y = xevent.xconfigurerequest.y,
                    .width  = xevent.xconfigurerequest.width,
                    .height = xevent.xconfigurerequest.height,
                };
                
                // if change occurred between new and old states, then add it to change mask
                if (event.change.x != state.change.x) event.change.change_mask |= BITWIN_X_MASK;
                if (event.change.y != state.change.y) event.change.change_mask |= BITWIN_Y_MASK;
                if (event.change.width  != state.change.width)  event.change.change_mask |= BITWIN_WIDTH_MASK;
                if (event.change.height != state.change.height) event.change.change_mask |= BITWIN_HEIGHT_MASK;
                
                // update state
                state.change = (typeof(state.change)){
                    .x = event.change.x,
                    .y = event.change.y,
                    .width  = event.change.width,
                    .height = event.change.height,
                };
                break;

                
            case DestroyNotify:
                break;

                
            case MapNotify:
            case UnmapNotify:
                break;

                
            case FocusIn:
            case FocusOut:
                break;
        }


        retval = 0;

        
        // TODO: optimize these so that there arent three whole switch case statements for this
        // I guess we are doing this a third time for the function calls
        switch (xevent.type) {
        
            case ButtonPress:
            case ButtonRelease:
            case MotionNotify:
            case EnterNotify:
            case LeaveNotify:
                if (bitwin->prop.mouse_callback != NULL)
                    retval = bitwin->prop.mouse_callback(bitwin, &event);
                break;

            
            case KeyPress:
            case KeyRelease:
                if (bitwin->prop.key_callback != NULL)
                    retval = bitwin->prop.key_callback(bitwin, &event);

                if (retval == 0) {
                    // TODO: add a ctrl+q keypress here that will close window
                }
                break;

            
            /*case ResizeRequest:
                // TODO: I am actually not sure if this intercepts the resize
                // or if it would just happen anyway
                //debugf("prop.resize_callback = %p", (void*)(uintptr_t)bitwin->prop.resize_callback);
                if (bitwin->prop.resize_callback != NULL)
                    retval = bitwin->prop.resize_callback(bitwin, &event);
                
                if (retval == 0) {
                    XResizeWindow(bitwin->linux.xdisp, bitwin->linux.xwin,
                        500, 500);
                }
                break;*/

            
            case ConfigureNotify:
                if (bitwin->prop.change_callback != NULL)
                    retval = bitwin->prop.change_callback(bitwin, &event);
                break;

            
            case DestroyNotify:
                if (bitwin->prop.close_callback != NULL)
                    retval = bitwin->prop.close_callback(bitwin, &event);

                if (retval == 0) {
                    return 0;
                }
                break;

            
            case MapNotify:
            case UnmapNotify:
                if (bitwin->prop.show_callback != NULL)
                    retval = bitwin->prop.show_callback(bitwin, &event);
                break;

            
            case FocusIn:
            case FocusOut:
                if (bitwin->prop.focus_callback != NULL)
                    retval = bitwin->prop.focus_callback(bitwin, &event);
                break;
                
        }
    }

    // this should never happen
    return 1;
}


//int bitwin_exit_handler_block(bitwin_t *bitwin_p) {}





#endif
