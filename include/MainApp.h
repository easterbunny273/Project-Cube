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
#include "Logger.h"
#include "EventManager.h"
#include "Graphics/Graphic.h"
#include "DummyGame/Game.h"

// include lua libs as explicit C-functions, because
// the lib is compiled as C-lib (-> other naming conventions)
extern "C"
{
#include <lua.h>
#include <lualib.h>
}

#include <luabind/luabind.hpp>

class MainApp
{
public:
    /*! \name Access to the subsystems */
    //@{
	/// returns ptr to the graphic subsystem
        Graphic * GetGraphic();

	/// returns ptr to the core settings
        Settings * GetCoreSettings();

	/// returns ptr to the event manager
	EventManager * GetEventManager();

        // dummy
	DummyGame * GetGame();

	// get app wide lua state
	lua_State * GetLuaState();
    //@}

    /*! \name Run methods */
    //@{
        void Run();
    //@}

    /*! \name Testing stuff */
    //@{
	void LuaTest();
    //@}

    /*! \name Get singelton instance */
    //@{
        static MainApp *GetInstance();
    //@}

private:
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

	Graphic *m_pGraphic;
	DummyGame *m_pGame;
	lua_State * m_pLuaState;
    //@}

    /*! \name Static members */
    //@{
	static MainApp *s_pInstance;
    //@}
};

#endif
