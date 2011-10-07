/*
* source file for Graphic class
* written by: christian moellinger <ch.moellinger@gmail.com>
* 10/2010
*/

#include <string>
#include <sstream>
#include <memory>

//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/common_gl.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/TextureManager.h"
#include "Logger.h"
#include "Graphics/SceneObject.h"
#include "Graphics/SceneObjects/SceneObject_PostEffect.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"
#include "Graphics/SceneObjects/SceneObject_EmptyNode.h"
#include "Graphics/SceneObjects/SceneObject_Camera.h"
#include "Graphics/SceneObjects/SceneObject_FBO.h"
#include "Graphics/SceneObjects/SceneObject_Cube.h"

#include "Graphics/SceneObjects/SceneObject_AssimpImport.h"

#include "Graphics/Graphic.h"

#include "Events.h"

#include "MainApp.h"

using namespace std;

#define PI 3.14159265f

int Graphic::s_iInstances = 0;
Graphic *Graphic::s_pInstance = NULL;

/****************************************************************
  *************************************************************** */
Graphic::Graphic()
    : m_bWindowOpenened(false),
      m_iFramesInThisSecondYet(0),
      m_iFramesPerSecond(0),
      m_fTimeOfLastRenderCall(0),
      m_bIsMouseLocked(false),
      m_bUseOpenGL_4_1(false),
      m_pSettings(NULL)
{
    // check if this is the first instance
    assert (s_iInstances == 0);

    // if not (and we are in release mode, so that the assertion did not fire),
    // print an error message
    if (s_iInstances > 0)
    {
        Logger::error() << "It seems that more than 1 instance of the Graphics engine was created." << Logger::endl;
        Logger::error() << "Please note that only one instance is supported by the event handling of glfw" << Logger::endl;
        Logger::error() << "Contact the developers or reinstall if you have any problems with event handling" << Logger::endl;
    }

    // inc counter
    s_iInstances ++;

    // set instance ptr
    s_pInstance = this;

}

/****************************************************************
  *************************************************************** */
Graphic::~Graphic()
{
    // check if the window is closed
    assert (m_bWindowOpenened == false);

    // dec instances counter
    s_iInstances--;
}

/****************************************************************
  *************************************************************** */
bool Graphic::InitializeOpenGL()
{
    // first, set settings group
    ItlLoadSettings();

    // create window
    ItlCreateOpenGLWindow();

    m_bWindowOpenened = true;
    m_bIsMouseLocked = false;

    // initialize opengl state variables
    ItlInitializeOpenGLStates();

    // initialize shader programs
    ItlLoadShaderPrograms();

    // create basic render path
    ItlCreateBaseRenderPath();

    // set main camera values
    assert (m_pSettings != NULL);
    Settings::TSettingsGroup *pCameraSettings = m_pSettings->GetGroup("camera");
    float fFieldOfView = pCameraSettings->GetValueOrDefault("fov", 45.0f);
    float fRatio = pCameraSettings->GetValueOrDefault("ratio", 1.33f);
    float fNearPlane = pCameraSettings->GetValueOrDefault("near-plane", 0.02f);
    float fFarPlane = pCameraSettings->GetValueOrDefault("far-plane", 100.0f);

    GetCamera()->SetPerspectiveProjection(fFieldOfView, fRatio, fNearPlane, fFarPlane);
    GetDebugCamera()->SetPerspectiveProjection(45.0f, 1.33f, 0.02f, 500.0f);

    GetCamera()->SetActive(true);

    return true;
}

/****************************************************************
  *************************************************************** */
void Graphic::SetActiveRenderPath(std::string sRenderPath)
{
    // todo: for perfomance reasons, it would be better to store the starting node directly (or the index in the list)
    // and update it every time when a new render path is added

    m_sActiveRenderPath = sRenderPath;
}

/****************************************************************
  *************************************************************** */
void Graphic::AddRenderPath(std::shared_ptr<SceneObject> spRoot, std::string sRenderPath)
{
    unsigned int nOldSize = m_vRenderPaths.size();

    m_vRenderPaths[sRenderPath] = spRoot;

    if (nOldSize == m_vRenderPaths.size())
	Logger::error() << "New renderpath " << sRenderPath << " uses an already existing name" << Logger::endl;
}

/****************************************************************
  *************************************************************** */
void Graphic::Render()
{
    // check if any render path is set to active
    assert (m_sActiveRenderPath.empty() == false);

    //clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Camera.Move(0.0001f);

    // draw the scene
    if (m_vRenderPaths.find(m_sActiveRenderPath) != m_vRenderPaths.end())
        m_vRenderPaths[m_sActiveRenderPath]->Render();
    else
        Logger::error() << "RenderPath " << m_sActiveRenderPath << " does not exist!" << Logger::endl;

    // swap back and front buffers
    glfwSwapBuffers();

    // increase frames counter
    m_iFramesInThisSecondYet ++ ;

    // print frames per second each second
    if (glfwGetTime() - m_fTimeOfLastRenderCall >= 1.0)
    {
        m_fTimeOfLastRenderCall = glfwGetTime();

        std::stringstream ssConversionBuffer;
        std::string sFramesPerSecond;
        ssConversionBuffer << m_iFramesInThisSecondYet;
        ssConversionBuffer >> sFramesPerSecond;

        std::string sWindowTitle = std::string("Project CUBE, frames per second: ") + sFramesPerSecond;
        glfwSetWindowTitle(sWindowTitle.data());

        m_iFramesPerSecond = m_iFramesInThisSecondYet;
        m_iFramesInThisSecondYet = 0;
    }

}

/****************************************************************
  *************************************************************** */
void Graphic::ItlStaticHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState)
{
    // this method is a static method (glfw cannot call class methods)
    // which redirects the event to the member-method of the Graphic class
    // therefore, we need a pointer to the instance. Thus, only one instance
    // of Graphic can exist without having problems with the input events
    // (because with more instances, this method would not know which instance
    // should receive the events).

    // make sure we know the address of the instance
    assert (s_pInstance != NULL);

    // get "this" to call member methods
    Graphic *pThis = s_pInstance;

    // call member method
    pThis->ItlHandleKeyboardEvent(iKeyIdentifier, iNewKeyState);
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlStaticHandleMousePos(int iX, int iY)
{
    // this method is a static method (glfw cannot call class methods)
    // which redirects the event to the member-method of the Graphic class
    // therefore, we need a pointer to the instance. Thus, only one instance
    // of Graphic can exist without having problems with the input events
    // (because with more instances, this method would not know which instance
    // should receive the events).

    // make sure we know the address of the instance
    assert (s_pInstance != NULL);

    // get "this" to call member methods
    Graphic *pThis = s_pInstance;

    // call member method
    pThis->ItlHandleMousePos(iX, iY);
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlStaticHandleMouseWheel(int iPosition)
{
    // this method is a static method (glfw cannot call class methods)
    // which redirects the event to the member-method of the Graphic class
    // therefore, we need a pointer to the instance. Thus, only one instance
    // of Graphic can exist without having problems with the input events
    // (because with more instances, this method would not know which instance
    // should receive the events).

    // make sure we know the address of the instance
    assert (s_pInstance != NULL);

    // get "this" to call member methods
    Graphic *pThis = s_pInstance;

    // call member method
    pThis->ItlHandleMouseWheel(iPosition);
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlStaticHandleMouseButton(int iButton, int iAction)
{
    // this method is a static method (glfw cannot call class methods)
    // which redirects the event to the member-method of the Graphic class
    // therefore, we need a pointer to the instance. Thus, only one instance
    // of Graphic can exist without having problems with the input events
    // (because with more instances, this method would not know which instance
    // should receive the events).

    // make sure we know the address of the instance
    assert (s_pInstance != NULL);

    // get "this" to call member methods
    Graphic *pThis = s_pInstance;

    // call member method
    pThis->ItlHandleMouseButton(iButton, iAction);
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState)
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

/****************************************************************
  *************************************************************** */
void Graphic::ItlHandleMousePos(int iX, int iY)
{
    // only deliver relative mouse position to center
    int iRelX = iX - m_iWidth / 2;
    int iRelY = iY - m_iHeight / 2;

    // issue 7 - workaround
    iRelX = -iRelX;
    iRelY = -iRelY;

    EventManager *pEventManager = MainApp::GetInstance()->GetEventManager();
    assert (pEventManager != NULL);

    pEventManager->QueueEvent(InputMouseMoveEvent::Create(iRelX, iRelY));

    if (m_bIsMouseLocked)
	glfwSetMousePos(m_iWidth / 2, m_iHeight / 2);
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlHandleMouseWheel(int iPosition)
{
    Logger::error() << "mouse wheel handling not implemented yet" << Logger::endl;
}


/****************************************************************
  *************************************************************** */
void Graphic::ItlHandleMouseButton(int iButton, int iAction)
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

/****************************************************************
  *************************************************************** */
void Graphic::ItlCreateOpenGLWindow()
{
    assert (m_pSettings != NULL);

    m_iWidth = m_pSettings->GetGroup("window")->GetValueOrDefault("width", 800);
    m_iHeight = m_pSettings->GetGroup("window")->GetValueOrDefault("height", 600);
    m_bFullscreen = m_pSettings->GetGroup("window")->GetValueOrDefault("fullscreen", false);
    m_bUseOpenGL_4_1 = m_pSettings->GetGroup("window")->GetValueOrDefault("use_opengl_4.1", false);

    int iFlags = (m_bFullscreen) ? GLFW_FULLSCREEN : GLFW_WINDOW;	//if fullscreen is true, flags is set to GLFW_FULLSCREEN, else to GLFW_WINDOW

    // initialize glfw
    if (glfwInit() == GL_TRUE)
            Logger::debug() << "glfw sucessfully initialized" << Logger::endl;
    else
            Logger::fatal() << "glfw initialization failed" << Logger::endl;

    // Set flags so GLFW creates the desired OpenGL context
    if (m_bUseOpenGL_4_1)
    {
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
    }
    else
    {
	// else we want opengl 3.3
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    }

    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Activate 4x antialiasing
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

    if (glfwOpenWindow(m_iWidth, m_iHeight, 8,8,8,8, 24, 8, iFlags) != GL_TRUE)
            Logger::fatal() << "failed to initialize OpenGL window" << Logger::endl;
    else
            Logger::debug() << "OpenGL window initialized" << Logger::endl;

    // set input handling callback methods
    glfwSetKeyCallback(Graphic::ItlStaticHandleKeyboardEvent);
    glfwSetMousePosCallback(Graphic::ItlStaticHandleMousePos);
    glfwSetMouseWheelCallback(Graphic::ItlStaticHandleMouseWheel);
    glfwSetMouseButtonCallback(Graphic::ItlStaticHandleMouseButton);

    // set window title
    glfwSetWindowTitle("Project CUBE");

    // Disable the mouse cursor
    //glfwDisable(GLFW_MOUSE_CURSOR);

    // set experimental flag to true, needed for correct importing of function pointers when using core-profile
    glewExperimental = GL_TRUE;

    // start up GLEW
    if (glewInit() != GLEW_OK)
            Logger::fatal() << "glew initialization failed" << Logger::endl;
    else
            Logger::debug() << "glew initialized" << Logger::endl;

    Logger::debug() << "Opened context with OpenGL Version " << (char *) glGetString(GL_VERSION) << Logger::endl;

    // check if we got the right version
    if ((GLEW_VERSION_3_3 && !m_bUseOpenGL_4_1) ||
	(GLEW_VERSION_4_1 && m_bUseOpenGL_4_1))
    {
            GLint profile;

            // check if we have a core-profile
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

	    if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
                    Logger::debug() << "got rendering context with core profile" << Logger::endl;
            else
                    Logger::fatal() << "got rendering context with compatibility profile instead of core profile" << Logger::endl;
    }
    else
    {
	if (m_bUseOpenGL_4_1)
	    Logger::info() << "The graphics engine was configured to use OpenGL 4. Maybe you should change the configuration to 3.3, see config/core_config.xml" << Logger::endl;

	Logger::fatal() << "The window could not be created. The wanted OpenGL version was not supported by the system." << Logger::endl;
    }
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlInitializeOpenGLStates()
{
    // Enable sRGB gamma correction for framebuffer output.
    glEnable(GL_FRAMEBUFFER_SRGB);

    //Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Enable face culling (default: backface culling)
    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);

    glPolygonOffset(1.1f, 4.0f);

    //set viewport
    glViewport(0,0, m_iWidth, m_iHeight);

    //clear error status
    glGetError();

    Logger::debug() << "graphic initialized" << Logger::endl;
}

/****************************************************************
  ** Nestes class Graphic::Camera **
  *************************************************************** */
Graphic::Camera::Camera()
{
    m_bInitialized = false;
    m_bActive = false;
    m_m4ProjectionMatrix = glm::mat4();

    m_fRotationHorizontal = 180;
    m_fRotationVertical = 0;

    // register for camera movement events
    EventManager *pEventManager = MainApp::GetInstance()->GetEventManager();
    assert (pEventManager != NULL);

    pEventManager->RegisterEventListener(this, CameraMovementEvent::EventType());
}

/****************************************************************
  *************************************************************** */
void Graphic::Camera::SetOrthoProjection(float fLeft,
                                         float fRight,
                                         float fTop,
                                         float fBottom,
                                         float fNear,
                                         float fFar)
{
    assert(!"not implemented yet");
}

/****************************************************************
  *************************************************************** */
void Graphic::Camera::SetPerspectiveProjection(float fFieldOfView,
                                               float fAspectRatio,
                                               float fNearPlane,
                                               float fFarPlane)
{
    m_m4ProjectionMatrix = glm::perspective(fFieldOfView, fAspectRatio, fNearPlane, fFarPlane);

    m_bInitialized = true;
}

/****************************************************************
  *************************************************************** */
glm::mat4 Graphic::Camera::GetProjectionMatrix() const
{
    assert (m_bInitialized);

    return m_m4ProjectionMatrix;
}

/****************************************************************
  *************************************************************** */
glm::mat4 Graphic::Camera::GetViewMatrix() const
{
    return m_m4ViewMatrix;
}

/****************************************************************
  *************************************************************** */
void Graphic::Camera::RotateVertical(float fValue)
{
    // lock vertical rotation to ]-90, 90[ because else
    // the view vector changes the direction if it exceeds 90 or -90

    m_fRotationVertical += fValue;

    if (m_fRotationVertical < -89.99f)
	m_fRotationVertical = -89.99f;
    else
    if (m_fRotationVertical > 89.99f)
	m_fRotationVertical = 89.99f;
}

/****************************************************************
  *************************************************************** */
void Graphic::Camera::RotateHorizontal(float fValue)
{
    m_fRotationHorizontal += fValue;

    while (m_fRotationHorizontal < 0.0f)
	  m_fRotationHorizontal += 360.0f;

    while (m_fRotationHorizontal >= 360.0f)
	m_fRotationHorizontal -= 360.0f;
}

/****************************************************************
  *************************************************************** */
void Graphic::Camera::Move(float fFactor)
{
    //Logger::debug() << m_fRotationHorizontal << ":" << m_fRotationVertical << Logger::endl;

    glm::vec3 v3MoveX;
    glm::vec3 v3MoveZ;

    if (m_v3MoveVector.x != 0)
    {
        //if camera should move in x-direction, we must calculate the look-at point
        //on the camera-sphere and use this vector as reference, and move normal to this vector

	v3MoveX.x = sin(((m_fRotationHorizontal+270) / 180.0) * PI);
        v3MoveX.y = 0.0;
	v3MoveX.z = cos(((m_fRotationHorizontal+270) / 180.0) * PI);

        //normalize to project point on our sphere
        v3MoveX /= sqrt(v3MoveX.x*v3MoveX.x + v3MoveX.y*v3MoveX.y + v3MoveX.z*v3MoveX.z);

        //and multiplicate with given x-factor to adjust movement length
        v3MoveX *= m_v3MoveVector.x;
    }

    if (m_v3MoveVector.y != 0)
    {
        //if camera should move in y-direction, we can use the given value directly

        v3MoveX.y = m_v3MoveVector.y;
    }

    if (m_v3MoveVector.z != 0)
    {
        //if camera should move in z-direction, we must calculate the look-at point
        //on the camera-sphere and use this vector as reference, and move along this vector

        v3MoveZ.x = sin((m_fRotationHorizontal / 180.0) * PI);
        v3MoveZ.y = tan((m_fRotationVertical / 180.0) * PI);
        v3MoveZ.z = cos((m_fRotationHorizontal / 180.0) * PI);

        //normalize to project point on our sphere
        v3MoveZ /= sqrt(v3MoveZ.x*v3MoveZ.x + v3MoveZ.y*v3MoveZ.y + v3MoveZ.z*v3MoveZ.z);

        //and multiplicate with given z-factor to adjust movement length
        v3MoveZ *= m_v3MoveVector.z;
    }

    m_v3CameraPosition += (v3MoveX + v3MoveZ) * fFactor;

    glm::vec3 v3LookAt;

    v3LookAt.x = sin(((m_fRotationHorizontal) / 180.0) * PI);
    v3LookAt.y = tan((m_fRotationVertical / 180.0) * PI);
    v3LookAt.z = cos(((m_fRotationHorizontal) / 180.0) * PI);

    m_m4ViewMatrix = glm::lookAt(m_v3CameraPosition, m_v3CameraPosition + v3LookAt, glm::vec3(0,1,0));
}

/****************************************************************
  *************************************************************** */
bool Graphic::ShutDown()
{
    glfwCloseWindow();
    glfwTerminate();

    m_bWindowOpenened = false;

    return true;
}

/****************************************************************
  *************************************************************** */
void Graphic::Camera::AddToMoveVector(glm::vec3 vVector)
{
    m_v3MoveVector += vVector;
}

/****************************************************************
  *************************************************************** */
void Graphic::HideAndLockMouseToWindowCenter()
{
    assert (m_bWindowOpenened);

    glfwDisable(GLFW_MOUSE_CURSOR);

    m_bIsMouseLocked = true;

    glfwSetMousePos(m_iWidth / 2, m_iHeight / 2);
}

/****************************************************************
  *************************************************************** */
void Graphic::UnHideAndUnLockMouse()
{
    assert (m_bWindowOpenened);

    glfwEnable(GLFW_MOUSE_CURSOR);

    m_bIsMouseLocked = false;
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlLoadShaderPrograms()
{
    ShaderManager *pShaderManager = ShaderManager::instance();

    assert (pShaderManager != NULL);

    pShaderManager->AddShader("basic_shading", new Shader("shaders/basic_shading.vs", "shaders/basic_shading.fs"));
    pShaderManager->AddShader("camera-debug", new Shader("shaders/camera-debug.vs", "shaders/camera-debug.fs"));
    pShaderManager->AddShader("bounding_shader", new Shader("shaders/bounding-box.vs", "shaders/bounding-box.fs"));
    pShaderManager->AddShader("simple_shading", new Shader("shaders/simple-shading.vs", "shaders/simple-shading.fs"));

}

/****************************************************************
  *************************************************************** */
void Graphic::ItlCreateBaseRenderPath()
{
    // only a dummy method, should be refactored

    std::shared_ptr<SceneObject> spRootNode (new SceneObject_EmptyNode());

    std::shared_ptr<SceneObject> spDebugCameraNode (new SceneObject_Camera(GetDebugCamera(), false));
    std::shared_ptr<SceneObject> spCameraNode (new SceneObject_Camera(GetCamera()));

    std::shared_ptr<SceneObject> spCubeNode (new SceneObject_Cube());
    std::shared_ptr<SceneObject> spTreppe (new SceneObject_AssimpImport("models/freepool-ng-table.3ds"));


    spRootNode->AddChild(spCameraNode);
    spCameraNode->AddChild(spDebugCameraNode);
    spDebugCameraNode->AddChild(spTreppe);
    //spCubeNode->AddChild(spTreppe);

    spTreppe->SetTransformMatrix(glm::scale(glm::mat4(), glm::vec3(0.01f)));

    AddRenderPath(spRootNode, "default");
}

/****************************************************************
  *************************************************************** */
ShaderManager * Graphic::GetShaderManager()
{
    // TODO: should we use the shader manager as a singelton or as a member of the graphics class?

    return ShaderManager::instance();
}

/****************************************************************
  *************************************************************** */
TextureManager * Graphic::GetTextureManager()
{
    // TODO: should we use the texture manager as a singelton or as a member of the graphics class?
    return TextureManager::instance();
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlLoadSettings()
{
    m_pSettings = MainApp::GetInstance()->GetCoreSettings()->GetGroup("graphics");
}

/****************************************************************
  *************************************************************** */
bool Graphic::OnEvent(std::shared_ptr<EventManager::IEvent> spEvent)
{
    assert(!"not implemented yet, should not registered for any event");
}

/****************************************************************
  *************************************************************** */
bool Graphic::Camera::OnEvent(std::shared_ptr<EventManager::IEvent> spEvent)
{
    // only accept CameraMovementEvent atm
    assert (spEvent->GetEventType() == CameraMovementEvent::EventType());

    std::shared_ptr<CameraMovementEvent> spMovementEvent = CameraMovementEvent::Cast(spEvent);
    assert(spMovementEvent);

    if (m_bActive)
    {
	CameraMovementEvent::TMovementType eMovementType = spMovementEvent->GetMovementType();
	float fValue = spMovementEvent->GetValue();

	if (eMovementType == CameraMovementEvent::CAMERA_MOVE_X)
	    AddToMoveVector(glm::vec3(fValue, 0.0f, 0.0f));
	else if (eMovementType == CameraMovementEvent::CAMERA_MOVE_Y)
	    AddToMoveVector(glm::vec3(0.0f, fValue, 0.0f));
	else if (eMovementType == CameraMovementEvent::CAMERA_MOVE_Z)
	    AddToMoveVector(glm::vec3(0.0f, 0.0f, fValue));
	else if (eMovementType == CameraMovementEvent::CAMERA_ROTATE_X)
	    RotateHorizontal(fValue);
	else if (eMovementType == CameraMovementEvent::CAMERA_ROTATE_Y)
	    RotateVertical(fValue);
	else
	    assert (!"no if condition fired");
    }
}
