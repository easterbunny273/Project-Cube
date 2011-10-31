#include "MainApp.h"
#include "Logger.h"
#include "Graphics/Graphic-GlfwWindow.h"
#include "Graphics/SceneObjects/LoadedModel.h"
#include "Graphics/SceneObjects/Cube.h"
#include "GL/glfw.h"

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

    // create an input event listener
    m_spInputEventListener = std::shared_ptr<InputEventListener>(new InputEventListener());
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

void MainApp::StartGraphic_Test()
{
    // create glfw window
    std::shared_ptr<Bamboo::GlfwWindow> spWindow = Bamboo::GlfwWindow::Create(1024, 768, "Test");
    // set input event listener
    spWindow->SetInputEventListener(m_spInputEventListener);

    // create camera
    std::shared_ptr<Bamboo::Camera> spCamera(new Bamboo::Camera());
    // set perspective projection
    spCamera->SetPerspectiveProjection(45.0f, 1.33f, 0.01f, 100.0f);

    // create scene
    std::shared_ptr<Bamboo::Scene> spScene = Bamboo::Scene::Create(spCamera);

    // workaround - triggers initializing of graphic engine
    // todo: fix this.
    GetGraphic();

    // create objects
    std::shared_ptr<Bamboo::ISceneObject> spCube = Bamboo::SO_Cube::Create();
    std::shared_ptr<Bamboo::ISceneObject> spTable = Bamboo::SO_LoadedModel::Create("models/freepool-ng-table.3ds");
    spTable->SetTransformMatrix(glm::scale(glm::mat4(), glm::vec3(0.01, 0.01, 0.01)));

    // add objects to scene
    spScene->AttachObject(spCube);
    spScene->AttachObject(spTable);

    // add render loop
    GetGraphic()->AddRenderLoop(spWindow, spCamera, spScene);
}

void MainApp::Run()
{
    // init game logic, graphics, do main loop, all this nasty stuff.

    GetEventManager()->Initialize();

    StartGraphic_Test();

    // main loop
    while(GetGame()->GetStop() == false)
    {
	GetEventManager()->ProcessEvents();

	LuaTest();

        GetGraphic()->Render();
    }
}

Bamboo * MainApp::GetGraphic()
{
    if (m_pGraphic == NULL)
        m_pGraphic = new Bamboo();

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

void MainApp::InputEventListener::ItlHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState)
{
    InputKeyEvent::TKey eKey = InputKeyEvent::KEY_UNKNOWN;
    bool bKeyRecognized = true;

    // first, map ranges KEY_A .. KEY_Z
    if (iKeyIdentifier >= 'A' && iKeyIdentifier <= 'Z')
    {
    eKey = static_cast<InputKeyEvent::TKey>(InputKeyEvent::KEY_A + (iKeyIdentifier - 'A'));
    }
    // map range KEY_0 .. KEY_9
    else if (iKeyIdentifier >= '0' && iKeyIdentifier <= '9')
    {
    eKey = static_cast<InputKeyEvent::TKey>(InputKeyEvent::KEY_0 + (iKeyIdentifier - '0'));
    }
    // map range KEY_KP_0 .. KEY_KP_9
    else if (iKeyIdentifier >= GLFW_KEY_KP_0 && iKeyIdentifier <= GLFW_KEY_KP_9)
    {
    eKey = static_cast<InputKeyEvent::TKey>(InputKeyEvent::KEY_KP_0 + (iKeyIdentifier - GLFW_KEY_KP_0));
    }
    // map range KEY_F1 .. KEY_F12
    else if (iKeyIdentifier >= GLFW_KEY_F1 && iKeyIdentifier <= GLFW_KEY_F12)
    {
    eKey = static_cast<InputKeyEvent::TKey>(InputKeyEvent::KEY_F1 + (iKeyIdentifier - GLFW_KEY_F1));
    }
    else // map other single keys
    switch (iKeyIdentifier)
    {
    case GLFW_KEY_LSHIFT: eKey = InputKeyEvent::KEY_LSHIFT; break;
    case GLFW_KEY_RSHIFT: eKey = InputKeyEvent::KEY_RSHIFT; break;
    case GLFW_KEY_LCTRL: eKey = InputKeyEvent::KEY_LCTRL; break;
    case GLFW_KEY_RCTRL: eKey = InputKeyEvent::KEY_RCTRL; break;
    case GLFW_KEY_LALT: eKey = InputKeyEvent::KEY_LALT; break;
    case GLFW_KEY_RALT: eKey = InputKeyEvent::KEY_RALT; break;
    case GLFW_KEY_UP: eKey = InputKeyEvent::KEY_UP; break;
    case GLFW_KEY_DOWN: eKey = InputKeyEvent::KEY_DOWN; break;
    case GLFW_KEY_LEFT: eKey = InputKeyEvent::KEY_LEFT; break;
    case GLFW_KEY_RIGHT: eKey = InputKeyEvent::KEY_RIGHT; break;
    case GLFW_KEY_SPACE: eKey = InputKeyEvent::KEY_SPACE; break;
    case GLFW_KEY_ESC: eKey = InputKeyEvent::KEY_ESC; break;
    case GLFW_KEY_TAB: eKey = InputKeyEvent::KEY_TAB; break;
    case GLFW_KEY_ENTER: eKey = InputKeyEvent::KEY_ENTER; break;
    case GLFW_KEY_BACKSPACE: eKey = InputKeyEvent::KEY_BACKSPACE; break;

    default:
        Logger::error() << "keycode " << iKeyIdentifier << " not recognized" << Logger::endl;
        bKeyRecognized = false;
    }

    // call methods of listener if key was recognized
    if (bKeyRecognized)
    {
    if (iNewKeyState == GLFW_PRESS)
        MainApp::GetInstance()->GetEventManager()->QueueEvent(InputKeyEvent::Create(eKey,InputKeyEvent::EVENT_DOWN));
    else
        MainApp::GetInstance()->GetEventManager()->QueueEvent(InputKeyEvent::Create(eKey,InputKeyEvent::EVENT_UP));
    }
}

void MainApp::InputEventListener::ItlHandleMousePos(int iX, int iY)
{
    //TODO
    int m_iWidth = 1024;
    int m_iHeight = 768;

    // only deliver relative mouse position to center
    int iRelX = iX - m_iWidth / 2;
    int iRelY = iY - m_iHeight / 2;

    // issue 7 - workaround
    iRelX = -iRelX;
    iRelY = -iRelY;

    EventManager *pEventManager = MainApp::GetInstance()->GetEventManager();
    assert (pEventManager != NULL);

    pEventManager->QueueEvent(InputMouseMoveEvent::Create(iRelX, iRelY));

    glfwSetMousePos(m_iWidth / 2, m_iHeight / 2);
}

void MainApp::InputEventListener::ItlHandleMouseWheel(int iPosition)
{
    Logger::error() << "mouse wheel handling not implemented yet" << Logger::endl;
}

void MainApp::InputEventListener::ItlHandleMouseButton(int iButton, int iAction)
{
    InputMouseButtonEvent::TMouseButton eMouseButton;
    bool bButtonRecognized = true;

    switch (iButton)
    {
    case GLFW_MOUSE_BUTTON_LEFT: eMouseButton = InputMouseButtonEvent::BUTTON_LEFT; break;
    case GLFW_MOUSE_BUTTON_MIDDLE: eMouseButton = InputMouseButtonEvent::BUTTON_MIDDLE; break;
    case GLFW_MOUSE_BUTTON_RIGHT: eMouseButton = InputMouseButtonEvent::BUTTON_RIGHT; break;
    default:
    Logger::error() << "mouse button " << iButton << " not recognized" << Logger::endl;
    bButtonRecognized = false;
    }

    if (bButtonRecognized)
    {
    EventManager *pEventManager = MainApp::GetInstance()->GetEventManager();
    assert (pEventManager != NULL);

    if (iAction == GLFW_PRESS)
        pEventManager->QueueEvent(InputMouseButtonEvent::Create(eMouseButton, InputMouseButtonEvent::EVENT_DOWN));
    else
        pEventManager->QueueEvent(InputMouseButtonEvent::Create(eMouseButton, InputMouseButtonEvent::EVENT_UP));
    }
}
