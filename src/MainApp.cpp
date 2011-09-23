#include "MainApp.h"
#include "Logger.h"

MainApp::MainApp()
{
    // todo: move in an own init method, because in constructor error handling is shitty

    // load core settings
    m_CoreSettings.RestoreSettingsFromXMLFile("config/core-settings.xml");

    // set lua state to NULL, lazy evaluation
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
    static MainApp instance;

    return &instance;
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
    //GetGraphic()->RegisterInputHandler(GetGame());
    GetGraphic()->SetActiveRenderPath("default");

    while(GetGame()->GetStop() == false)
    {
	GetEventManager()->ProcessEvents();

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
    }

    assert (m_pLuaState != NULL);

    return m_pLuaState;
}
