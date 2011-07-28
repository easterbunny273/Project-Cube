/*
 * header file for IInputEventListener interface
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 07/2011, Project "Project Cube"
 */

#pragma once
#ifndef __IINPUTEVENTLISTENER_PROJECTCUBE_HEADER
#define __IINPUTEVENTLISTENER_PROJECTCUBE_HEADER

class IInputEventListener
{
public:
    /*! \name Public methods */
    //@{
        enum TKey
        {
            KEY_UNKNOWN = 0,

            // key modifiers
            KEY_LSHIFT, KEY_RSHIFT, KEY_LCTRL, KEY_RCTRL, KEY_LALT, KEY_RALT,

            // F1..F12
            KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

            // cursor keys
            KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,

            // special keys like SPACE and ESC
            KEY_ESC, KEY_SPACE, KEY_TAB, KEY_ENTER, KEY_BACKSPACE,

            // numbers 0..9 (mapped to ascii-conform values 48..57)
            KEY_0 = 48, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,

            // chars A..Z (mapped to ascii-conform values 65..90
            KEY_A = 65, KEY_B, KEY_C, KEY_D, KEY_E,
            KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
            KEY_K, KEY_L, KEY_M, KEY_N, KEY_O,
            KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
            KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

            // numbers on numpad
            KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4,
            KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9

            // blabla. not finished yet, but you got the idea :)
        };

        enum TMouseButton
        {
            BUTTON_LEFT,
            BUTTON_MIDDLE,
            BUTTON_RIGHT
        };

    //@}

    /*! \name Public signals for key handling */
    //@{
        /// fired when a key is down
        virtual void OnKeyDown(TKey eKey) = 0;

        /// fired when a key goes up
        virtual void OnKeyUp(TKey eKey) = 0;
    //@}

    /*! \name Public signals for mouse handling */
    //@{
        /// fired when a key is pressed
        virtual void OnMouseMove(int iX, int iY) = 0;

        /// fired when a mouse button is pressed
        virtual void OnMouseButtonPressed(TMouseButton eButton) = 0;

        /// fired when a mouse button is released
        virtual void OnMouseButtonReleased(TMouseButton eButton) = 0;
    //@}

};

#endif // __IINPUTEVENTLISTENER_PROJECTCUBE_HEADER
