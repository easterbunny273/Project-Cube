#include "MainApp.h"
#include "Logger.h"

MainApp::MainApp()
{
    // todo: move in an own init method, because in constructor error handling is shitty

    // load core settings
    m_CoreSettings.RestoreSettingsFromXMLFile("config/core-settings.xml");
}

MainApp::~MainApp()
{
    // save core settings
    m_CoreSettings.StoreSettingsAsXMLFile("config/core-settings.xml");
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

void MainApp::Run()
{
    // init game logic, graphics, do main loop, all this nasty stuff.
    Logger::debug() << "hello!" << Logger::endl;

    GetGraphic()->InitializeOpenGL();
    GetGraphic()->RegisterInputHandler(GetGame());
    GetGraphic()->SetActiveRenderPath("default");

    while(true)
        GetGraphic()->Render();

    GetGraphic()->ShutDown();
}

Graphic * MainApp::GetGraphic()
{
    return &m_Graphic;
}

DummyGame * MainApp::GetGame()
{
    return &m_Game;
}
