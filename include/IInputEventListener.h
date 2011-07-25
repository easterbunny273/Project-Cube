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
            KEY_A,
            KEY_B,
            KEY_C,
            KEY_D,
            KEY_E,
            KEY_F
            /// blabla. not finished yet, but you got the idea :)

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
        /// fired when a key is pressed
        virtual void OnKeyPressed(TKey eKey) = 0;

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
        virtual void OnMouseButtonPressed(TMouseButton eButton, int iX, int iY) = 0;

        /// fired when a mouse button is released
        virtual void OnMouseButtonReleased(TMouseButton eButton, int iX, int iY) = 0;
    //@}

};

#endif // __IINPUTEVENTLISTENER_PROJECTCUBE_HEADER
