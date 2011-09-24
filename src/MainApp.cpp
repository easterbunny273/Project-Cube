#include "MainApp.h"
#include "Logger.h"

// initialize singelton ptr to NULL
MainApp * MainApp::s_pInstance = NULL;

MainApp::MainApp()
{
    // todo: move in an own init method, because in constructor error handling is shitty

    // load core settings
    m_CoreSettings.RestoreSettingsFromXMLFile("config/core-settings.xml");

    // set subsystems to NULL, lazy initialisation
    m_pLuaState = NULL;
    m_pGraphic = NULL;
    m_pGame = NULL;

}

MainApp::~MainApp()
{
    // save core settings
    m_CoreSettings.StoreSettingsAsXMLFile("config/core-settings.xml");

    // close lua state, if open
    if (m_pLuaState != NULL)
    {
	lua_close(m_pLuaState);
	m_pLuaState = NULL;
    }

    if (m_pGraphic != NULL)
    {
	delete m_pGraphic;
	m_pGraphic = NULL;
    }

    if (m_pGame != NULL)
    {
	delete m_pGame;
	m_pGame = NULL;
    }
}

MainApp * MainApp::GetInstance()
{
    static bool bInitializing = false;

    if (s_pInstance == NULL)
    {
	// when this assertion fails, it means the following:
	// --
	// MainApp is just initializing and a subsystem is created by the constructor of MainApp. Now,
	// the subsystem accesses the instance of MainApp, but the instance is not initialized yet.
	// It's a infinite loop which causes serious problems.
	assert(bInitializing == false);

	bInitializing = true;
	s_pInstance = new MainApp();
    }

    assert (s_pInstance != NULL);
    return s_pInstance;
}

Settings * MainApp::GetCoreSettings()
{
    return &m_CoreSettings;
}

EventManager * MainApp::GetEventManager()
{
    return &m_EventManager;
}

void MainApp::Run()
{
    // init game logic, graphics, do main loop, all this nasty stuff.
    Logger::debug() << "hello!" << Logger::endl;

    GetGraphic()->InitializeOpenGL();
    GetGraphic()->SetActiveRenderPath("default");
    GetEventManager()->Initialize();


    while(GetGame()->GetStop() == false)
    {
	GetEventManager()->ProcessEvents();

	LuaTest();

        GetGraphic()->Render();
    }

    GetGraphic()->ShutDown();
}

Graphic * MainApp::GetGraphic()
{
    if (m_pGraphic == NULL)
	m_pGraphic = new Graphic();

    assert (m_pGraphic != NULL);
    return m_pGraphic;
}

DummyGame * MainApp::GetGame()
{
    if (m_pGame == NULL)
	m_pGame = new DummyGame();

    assert (m_pGame != NULL);
    return m_pGame;
}

void MainApp::ItlCreateSceneGraphs()
{

}

lua_State * MainApp::GetLuaState()
{
    // lazy evaluation
    if (m_pLuaState == NULL)
    {
	// open lua state
	m_pLuaState = lua_open();

	// load lua libs
	luaL_openlibs(m_pLuaState);

	luabind::open(m_pLuaState);
    }

    assert (m_pLuaState != NULL);
    return m_pLuaState;
}

void MainApp::LuaTest()
{
    luaL_dostring(
    m_pLuaState,
    "new_event = InputKeyEvent::Create(87, 1)\n"
    "eventmanager::QueueEvent(new_event)\n"
    );
}
