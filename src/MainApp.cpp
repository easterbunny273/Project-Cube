#include "MainApp.h"
#include "PC_Logger.h"
#include "Graphic-GlfwWindow.h"
#include "Graphic.h"
#include "Camera.h"
#include "AssimpWrapper.h"
#include "GeometryData.h"
#include "LuaManager.h"

#include "SemanticSceneNodes/ISemanticSceneNode.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"
#include "SemanticSceneNodes/Camera_SemSceneNode.h"
#include "SemanticSceneNodes/Cube_SemSceneNode.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"
#include "SemanticSceneNodes/FractalTerrain_SemSceneNode.h"

#include "DeferredNodeTranslator/DeferredNodeTranslator.h"

#include "Gamelogic/Level.h"

bool g_bLockedMouse = true;

// initialize singelton ptr to NULL
MainApp * MainApp::s_pInstance = NULL;

MainApp::MainApp()
{
    // todo: move in an own init method, because in constructor error handling is shitty

    // load core settings
    m_CoreSettings.RestoreSettingsFromXMLFile("config/core-settings.xml");

    // set subsystems to NULL, lazy initialisation
    m_pLuaState = NULL;
    m_pGraphic = new Bamboo();
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

void MainApp::StartGraphic_Test2()
{
  // this method uses the new SemanticSceneNodes
  //

  // create camera
  m_spCamera = Bamboo::PerspectiveCamera::Create(45.0f, 1.33f, 0.01f, 100.0f, glm::vec3(-0.2f, 0.2f, 0.0f), 90.0f, -50.0f);

  // register itself as listener for camera events
  GetEventManager()->RegisterEventListener(this, CameraMovementEvent::EventType());

  // load level
  LuaManager::GetInstance()->ExecuteFile("lua/test.lua");

  Level level = LuaManager::GetInstance()->CallLuaFunction<Level>("GetLevel");

  std::shared_ptr<ISemanticSceneNode> spCube = Cube_SemSceneNode::Create(level.GetCubeByPosition(0,0,0));
  //spCube->SetTransformMatrix(glm::scale(glm::mat4(), glm::vec3(0.01, 0.01, 0.01)));

  std::shared_ptr<FractalTerrain_SemSceneNode> spTerrain = FractalTerrain_SemSceneNode::Create("textures/fractal_terrain_test1.png");
  std::shared_ptr<Light_SemSceneNode> spTestLight1 = Light_SemSceneNode::Create(glm::vec3(-0.5f, 0.2f, 0.3f), glm::vec3(1.0f, -0.4f, -1.0f), 50.0f, glm::vec3(1.0, 1.0, 1.0), 0.1, 50.0f);
  std::shared_ptr<Light_SemSceneNode> spTestLight2 = Light_SemSceneNode::Create(glm::vec3(-0.2f, 0.2f, -0.14f), glm::vec3(1.0f, -1.1f, 0.62f), 50.0f, glm::vec3(1.0, 1.0, 1.0), 0.1, 50.0f);

  std::shared_ptr<Camera_SemSceneNode> spCamera = Camera_SemSceneNode::Create(m_spCamera);

  // link scene graph
//  spCamera->AddChild(spCube);
  spCamera->AddChild(spTerrain);
  spCamera->AddChild(spTestLight1);
 // spCamera->AddChild(spTestLight2);

  // create node translator
  std::shared_ptr<INodeTranslator> spDeferredTranslator(new DeferredNodeTranslator(m_pGraphic));

  // create glfw window
  std::shared_ptr<Bamboo::GlfwWindow> spWindow = Bamboo::GlfwWindow::Create(1024, 768, "Test");
  spWindow->SetInputEventListener(m_spInputEventListener);

  // add render loop
  m_pGraphic->AddRenderLoop(spWindow, spCamera, spDeferredTranslator);
}

void MainApp::Run()
{
    // init game logic, graphics, do main loop, all this nasty stuff.

    GetEventManager()->Initialize();

    StartGraphic_Test2();

    // main loop
    while(GetGame()->GetStop() == false)
    {
	GetEventManager()->ProcessEvents();

	LuaTest();

        static int i=0;
        i++;

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

lua_State * MainApp::GetLuaState()
{
    /*
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
	*/
    return m_pLuaState;
}

void MainApp::LuaTest()
{
    /*luaL_dostring(
    m_pLuaState,
    "new_event = InputKeyEvent::Create(87, 1)\n"
    "eventmanager::QueueEvent(new_event)\n"
                );
				*/
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
    if (g_bLockedMouse)
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
      {
        g_bLockedMouse = true;
        glfwSetMousePos(1024 / 2, 768 / 2);

        pEventManager->QueueEvent(InputMouseButtonEvent::Create(eMouseButton, InputMouseButtonEvent::EVENT_DOWN));
      }
    else
      {
        g_bLockedMouse = false;
        pEventManager->QueueEvent(InputMouseButtonEvent::Create(eMouseButton, InputMouseButtonEvent::EVENT_UP));
      }
    }
}

bool MainApp::OnEvent(std::shared_ptr<EventManager::IEvent> spEvent)
{
    std::shared_ptr<Bamboo::ICamera> spCamera = m_spCamera;

    assert (spCamera);

    // only accept CameraMovementEvent atm
    assert (spEvent->GetEventType() == CameraMovementEvent::EventType());

    std::shared_ptr<CameraMovementEvent> spMovementEvent = CameraMovementEvent::Cast(spEvent);
    assert(spMovementEvent);

    //if (m_bActive)
    {
        CameraMovementEvent::TMovementType eMovementType = spMovementEvent->GetMovementType();
        float fValue = spMovementEvent->GetValue();

        if (eMovementType == CameraMovementEvent::CAMERA_MOVE_X)
            spCamera->AddToMoveVector(glm::vec3(fValue * 5, 0.0f, 0.0f));
        else if (eMovementType == CameraMovementEvent::CAMERA_MOVE_Y)
            spCamera->AddToMoveVector(glm::vec3(0.0f, fValue * 5, 0.0f));
        else if (eMovementType == CameraMovementEvent::CAMERA_MOVE_Z)
            spCamera->AddToMoveVector(glm::vec3(0.0f, 0.0f, fValue * 5));
        else if (eMovementType == CameraMovementEvent::CAMERA_ROTATE_X)
            spCamera->RotateHorizontal(fValue);
        else if (eMovementType == CameraMovementEvent::CAMERA_ROTATE_Y)
            spCamera->RotateVertical(fValue);
        else
            assert (!"no if condition fired");
    }

    return true;
}
