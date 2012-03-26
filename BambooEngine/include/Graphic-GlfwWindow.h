#include "Graphic.h"

#ifdef GLFW

#ifndef __GRAPHIC_GLFWWINDOW_HEADER
#define __GRAPHIC_GLFWWINDOW_HEADER

#include <string>

class Bamboo::GlfwWindow : public IRenderTarget
{

public:
    /*! \name Public interfaces */
    //@{
        /// Interface for event listener, which receives the input events which happened in a glfw window
        class IInputEventListener
        {
        public:
            /// handles keyboard events and sends signals to listener
            virtual void ItlHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState) = 0;

            /// handles mouse movements and sends signals to the listener
            virtual void ItlHandleMousePos(int iX, int iY) = 0;

            /// handles mouse wheel input events and sends signals to the listener
            virtual void ItlHandleMouseWheel(int iPosition) = 0;

            /// handles mouse button events and sends signals to the listener
            virtual void ItlHandleMouseButton(int iButton, int iAction) = 0;
        };
    //@}

    /*! \name Construction / Destruction */
    //@{
        static std::shared_ptr<GlfwWindow> Create(int iWidth, int iHeight, std::string sWindowTitle);
        virtual ~GlfwWindow();
    //@}

    /*! \name Implementing the IRenderTarget interface */
    //@{
        virtual void ClearBuffers();
        virtual void SwapBuffers();
    //@}

    /*! \name Methods for input event handling */
    //@{
        void SetInputEventListener(std::shared_ptr<IInputEventListener> spListener) { m_spInputEventListener = spListener; }
        void UnsetInputEventListener(std::shared_ptr<IInputEventListener> spListener) { assert(m_spInputEventListener == spListener); m_spInputEventListener = NULL; }
    //@}

private:
    /*! \name Construction / Destruction */
    //@{
        /// private constructor (only the static method Glfw::Create() is allowed to create a new instance of an GlfwWindow)
        GlfwWindow();
    //@}

    /*! \name Static helper methods */
    //@{
        /// called by glfw when a key is pressed or released
        static void ItlStaticHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState);

        /// called by glfw when the mouse position has changed
        static void ItlStaticHandleMousePos(int iX, int iY);

        /// called by glfw when the mouse wheel has scrolled
        static void ItlStaticHandleMouseWheel(int iPosition);

        /// called by glfw when a mouse button is pressed or released
        static void ItlStaticHandleMouseButton(int iButton, int iAction);
    //@}

    /*! \name Private members */
    //@{
        std::shared_ptr<IInputEventListener>   m_spInputEventListener;
        int                                    m_iWidth;
        int                                    m_iHeight;

        int                                    m_iFrames;
        int                                    m_iFPS;
        double                                 m_dLastTimeStamp;

        std::string                            m_sWindowTitle;
    //@}

    /*! \name Static members */
    //@{
        static GlfwWindow *                             s_pInstance;
    //@}
};

#endif //__GRAPHIC_GLFWWINDOW_HEADER

#endif // GLFW
