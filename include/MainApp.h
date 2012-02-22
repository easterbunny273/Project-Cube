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
#include "PC_Logger.h"
#include "EventManager.h"
#include "Graphic.h"
#include "Graphic-GlfwWindow.h"
#include "DummyGame/Game.h"

#include "lua_include.h"

class MainApp : public EventManager::IEventListener
{
public:
    /*! \name Access to the subsystems */
    //@{
	/// returns ptr to the graphic subsystem
        Bamboo * GetGraphic();

	/// returns ptr to the core settings
        Settings * GetCoreSettings();

	/// returns ptr to the event manager
	EventManager * GetEventManager();

        // dummy
	DummyGame * GetGame();

	// get app wide lua state
	lua_State * GetLuaState();
    //@}

    /*! \name The MainApp gets the camera events and call the camera methods */
    //@{
        virtual bool OnEvent(std::shared_ptr<EventManager::IEvent> spEvent);
    //@}

    /*! \name Run methods */
    //@{
        void Run();
    //@}

    /*! \name Testing stuff */
    //@{
	void LuaTest();
        void StartGraphic_Test();
        void StartGraphic_Test2();
    //@}

    /*! \name Get singelton instance */
    //@{
        static MainApp *GetInstance();
    //@}

private:
    /*! \name InputEventListener for the Graphic engine */
    //@{
        class InputEventListener : public Bamboo::GlfwWindow::IInputEventListener
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
	void ItlCreateSceneGraphs();
    //@}

    /*! \name Private members */
    //@{
        EventManager m_EventManager;
        Settings m_CoreSettings;

        Bamboo *m_pGraphic;
        DummyGame *m_pGame;
        lua_State * m_pLuaState;
        std::shared_ptr<InputEventListener> m_spInputEventListener;
        std::shared_ptr<Bamboo::ICamera> m_spCamera;
    //@}

    /*! \name Static members */
    //@{
	static MainApp *s_pInstance;
    //@}
};

#endif
