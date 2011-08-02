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
    GetGraphic()->GetCamera()->SetPerspectiveProjection(45.0f, 1.3f, 0.1f, 100.0f);
    GetGraphic()->RegisterInputHandler(GetGame());
    GetGraphic()->SetActiveRenderPath("default");

    while(GetGame()->GetStop() == false)
    {
        GetGraphic()->Render();
    }

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
