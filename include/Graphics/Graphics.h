/*
 * header file for Graphics class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __GRAPHICS_PROJECT_CUBE_HEADER
#define __GRAPHICS_PROJECT_CUBE_HEADER

#include <list>

class Graphics
{
public:
    /*! \name Public interfaces */
    //@{
        class IInputEventListener
        {
            // this interface methods will change,
            // I wrote just some dummy methods.
            // my plan is to wrote generic methods and own types
            // to provide an interface, which can be used without using some
            // part of glfw (event not the structs which describe the keys)
        public:
            void OnKeyPressed(int iKey);
            void OnKeyDown (int iKey);
            void OnKeyUp (int iKey);

            void OnMouseMove(int iX, int Y);
            void OnMouseButtonPressed(int iButton);
            void OnMouseButtonReleased(int iButton);
        };

    //@}

    /*! \name Construction / Destruction */
    //@{
        /// constructor
        Graphics();

        /// destructor
        ~Graphics();
    //@}

    /*! \name Public methods */
    //@{
        /// initialize graphics subsystem, creates window, initializes opengl context
        /// returns true if successful
        bool StartUp();

        /// shuts down the graphics subsystem
        bool ShutDown();

        /// registers an input event listener
        void RegisterInputListener(IInputEventListener *pListener);

        /// unregister an input event listener
        void UnRegisterInputListener(IInputEventListener *pListener);
    //@}

private:
    /*! \name Private members */
    //@{
        /// A list which holds all currently registered input event listeners
        /// this list uses naked ptrs instead of shared_ptrs because no new/delete
        /// calls are involved
        std::list<IInputEventListener *>    m_lInputListeners;
    //@}
};

#endif //__GRAPHICS_PROJECT_CUBE_HEADER
