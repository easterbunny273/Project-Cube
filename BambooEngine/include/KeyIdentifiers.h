#ifndef KEYIDENTIFIERS_H
#define KEYIDENTIFIERS_H

namespace BambooGraphics
{
    enum TKey
    {
        KEY_UNKNOWN,
        KEY_A = (int) 'A', KEY_B = (int) 'B', KEY_C = (int) 'C',
        KEY_D = (int) 'D', KEY_E = (int) 'E', KEY_F = (int) 'F',
        KEY_G = (int) 'G', KEY_H = (int) 'H', KEY_I = (int) 'I',
        KEY_J = (int) 'J', KEY_K = (int) 'K', KEY_L = (int) 'L',
        KEY_M = (int) 'M', KEY_N = (int) 'N', KEY_O = (int) 'O',
        KEY_P = (int) 'P', KEY_Q = (int) 'Q', KEY_R = (int) 'R',
        KEY_S = (int) 'S', KEY_T = (int) 'T', KEY_U = (int) 'U',
        KEY_V = (int) 'V', KEY_W = (int) 'W', KEY_X = (int) 'X',
        KEY_Y = (int) 'Y', KEY_Z = (int) 'Z',
        KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
        KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
        KEY_SPACE,
        KEY_TAB,
        KEY_ESCAPE, KEY_ENTER, KEY_BACKSPACE,
        KEY_CONTROL, KEY_SHIFT, KEY_CAPSLOCK,
        KEY_ALT, KEY_ALT_GR,
        KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
        KEY_NUM_0, KEY_NUM_1, KEY_NUM_2, KEY_NUM_3, KEY_NUM_4, KEY_NUM_5, KEY_NUM_6, KEY_NUM_7, KEY_NUM_8, KEY_NUM_9,
        KEY_NUM_DIV, KEY_NUM_MULT, KEY_NUM_MINUS, KEY_NUM_PLUS, KEY_NUM_ENTER, KEY_NUM_COMMA
    };

    enum TMouseButton
    {
        BUTTON_UNKNOWN = 0,
        BUTTON_LEFT,
        BUTTON_MIDDLE,
        BUTTON_RIGHT
    };

    enum TInputEvent
    {
        EVENT_PRESSED,
        EVENT_RELEASED
    };
}

#endif // KEYIDENTIFIERS_H
