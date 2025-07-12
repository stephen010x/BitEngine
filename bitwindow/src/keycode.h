#ifndef KEYCODE_H

// https://github.com/D-Programming-Deimos/libX11/blob/master/c/X11/keysymdef.h
// https://ss64.com/ascii.html

// TODO:
// I got lazy, and didn't add all the keypresses.
// Also I don't know if xlib will differentiate between uppercase and lowercase letters
// but it sounds like a problem for another time.

#ifdef __LINUX__



#define KEY_NULL        0


#define _KEY_MASK       0x00ff

// misc keys
#define KEY_BSPACE      0x08
#define KEY_TAB         0x09
#define KEY_ENTER       0x0d
#define KEY_PAUSE       0x13
#define KEY_ESC         0x1b
#define KEY_DEL         0xff

// cursor control
#define KEY_LEFT        0x51
#define KEY_UP          0x52
#define KEY_RIGHT       0x53
#define KEY_DOWN        0x54

// modifiers
#define KEY_SHIFT_L     0xe1
#define KEY_SHIFT_R     0xe2
#define KEY_CTL_L       0xe3
#define KEY_CTL_R       0xe4
#define KEY_CAPS        0xe5
#define KEY_SHIFT       0xe6
#define KEY_META_L      0xe7
#define KEY_META_R      0xe8
#define KEY_ALT_L       0xe9
#define KEY_ALT_R       0xea
#define KEY_SUPER_L     0xeb
#define KEY_SUPER_R     0xec
#define KEY_HYPER_L     0xed
#define KEY_HYPER_L     0xee

// latin
#define KEY_SPACE       0x20
#define KEY_APOST       0x27
#define KEY_COMMA       0x2c
#define KEY_MINUS       0x2d
#define KEY_PERIOD      0x2e
#define KEY_SLASH       0x2f
#define KEY_0           0x30
#define KEY_1           0x31
#define KEY_2           0x32
#define KEY_3           0x33
#define KEY_4           0x34
#define KEY_5           0x35
#define KEY_6           0x36
#define KEY_7           0x37
#define KEY_8           0x38
#define KEY_9           0x39
#define KEY_EQUAL       0x3d
#define KEY_SEMIC       0x3b

// these are all uppercase
/*
#define KEY_A           0x41
#define KEY_B           0x42
#define KEY_C           0x43
#define KEY_D           0x44
#define KEY_E           0x45
#define KEY_F           0x46
#define KEY_G           0x47
#define KEY_H           0x48
#define KEY_I           0x49
#define KEY_J           0x4a
#define KEY_K           0x4b
#define KEY_L           0x4c
#define KEY_M           0x4d
#define KEY_N           0x4e
#define KEY_O           0x4f
#define KEY_P           0x50
#define KEY_Q           0x51
#define KEY_R           0x52
#define KEY_S           0x53
#define KEY_T           0x54
#define KEY_U           0x55
#define KEY_V           0x56
#define KEY_W           0x57
#define KEY_X           0x58
#define KEY_Y           0x59
#define KEY_Z           0x5a
*/

#define KEY_A           0x61
#define KEY_B           0x62
#define KEY_C           0x63
#define KEY_D           0x64
#define KEY_E           0x65
#define KEY_F           0x66
#define KEY_G           0x67
#define KEY_H           0x68
#define KEY_I           0x69
#define KEY_J           0x6a
#define KEY_K           0x6b
#define KEY_L           0x6c
#define KEY_M           0x6d
#define KEY_N           0x6e
#define KEY_O           0x6f
#define KEY_P           0x70
#define KEY_Q           0x71
#define KEY_R           0x72
#define KEY_S           0x73
#define KEY_T           0x74
#define KEY_U           0x75
#define KEY_V           0x76
#define KEY_W           0x77
#define KEY_X           0x78
#define KEY_Y           0x79
#define KEY_Z           0x7a


// mouse buttons too
#define BUTTON_1    1
#define BUTTON_2    2
#define BUTTON_3    3



#endif



#endif
