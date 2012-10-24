/*
 * header file for MainApp class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __MAINAPP_PROJECT_CUBE_HEADER
#define __MAINAPP_PROJECT_CUBE_HEADER

#include "Settings.h"
#include "BambooLib/include/Logger.h"
#include "EventManager.h"
#include "Graphic.h"
#include "Graphic-GlfwWindow.h"
#include "DummyGame/Game.h"

class QWidget;
class QApplication;

class MainApp : public EventManager::IEventListener
{
public:
    /*! \name Access to the subsystems */
    //@{
	/// returns ptr to the graphic subsystem
        BambooGraphics::GraphicsCore * GetGraphic();

	/// returns ptr to the core settings
        Settings * GetCoreSettings();

	/// returns ptr to the event manager
	EventManager * GetEventManager();

        // dummy
	DummyGame * GetGame();

    //@}

    /*! \name The MainApp gets the camera events and call the camera methods */
    //@{
        virtual bool OnEvent(std::shared_ptr<EventManager::IEvent> spEvent);
    //@}

    /*! \name Run methods */
    //@{
        void Run(QApplication *pApp, QWidget *pWidget);
    //@}

    /*! \name Testing stuff */
    //@{
        void StartGraphic_Test2(QWidget *pWidget);
    //@}

    /*! \name Get singelton instance */
    //@{
        static MainApp *GetInstance();
    //@}

private:
    /*! \name InputEventListener for the Graphic engine */
    //@{
        class InputEventListener : public BambooGraphics::GraphicsCore::GlfwWindow::IInputEventListener
        {
        public:
            /// handles keyboard events and sends signals to listener
            virtual void ItlHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState);

            /// handles mouse movements and sends signals to the listener
            virtual void ItlHandleMousePos(int iX, int iY);

            /// handles mouse wheel input events and sends signals to the listener
            virtual void ItlHandleMouseWheel(int iPosition);

            /// handles mouse button events and sends signals to the listener
            virtual void ItlHandleMouseButton(int iButton, int iAction);
        };
    //@}

    /*! \name Construction / Destruction */
    //@{
        /// constructor
        MainApp();

        /// destructor
        ~MainApp();
    //@}

    /*! \name Internal helper methods */
    //@{
    //@}

    /*! \name Private members */
    //@{
        EventManager m_EventManager;
        Settings m_CoreSettings;

        BambooGraphics::GraphicsCore *m_pGraphic;
        DummyGame *m_pGame;
        std::shared_ptr<InputEventListener> m_spInputEventListener;
        std::shared_ptr<BambooGraphics::GraphicsCore::ICamera> m_spCamera;
    //@}

    /*! \name Static members */
    //@{
	static MainApp *s_pInstance;
    //@}
};

#endif
