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

#include "IInputEventListener.h"

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
      m_pInputEventListener(NULL)
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

    m_Camera.Move(0.05f);

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
    if (m_pInputEventListener != NULL)
    {
        IInputEventListener::TKey eKey = IInputEventListener::KEY_UNKNOWN;
        bool bKeyRecognized = true;

        // first, map ranges KEY_A .. KEY_Z
        if (iKeyIdentifier >= 'A' && iKeyIdentifier <= 'Z')
        {
            eKey = static_cast<IInputEventListener::TKey>(IInputEventListener::KEY_A + (iKeyIdentifier - 'A'));
        }
        // map range KEY_0 .. KEY_9
        else if (iKeyIdentifier >= '0' && iKeyIdentifier <= '9')
        {
            eKey = static_cast<IInputEventListener::TKey>(IInputEventListener::KEY_0 + (iKeyIdentifier - '0'));
        }
        // map range KEY_KP_0 .. KEY_KP_9
        else if (iKeyIdentifier >= GLFW_KEY_KP_0 && iKeyIdentifier <= GLFW_KEY_KP_9)
        {
            eKey = static_cast<IInputEventListener::TKey>(IInputEventListener::KEY_KP_0 + (iKeyIdentifier - GLFW_KEY_KP_0));
        }
        // map range KEY_F1 .. KEY_F12
        else if (iKeyIdentifier >= GLFW_KEY_F1 && iKeyIdentifier <= GLFW_KEY_F12)
        {
            eKey = static_cast<IInputEventListener::TKey>(IInputEventListener::KEY_F1 + (iKeyIdentifier - GLFW_KEY_F1));
        }
        else // map other single keys
            switch (iKeyIdentifier)
            {
            case GLFW_KEY_LSHIFT: eKey = IInputEventListener::KEY_LSHIFT; break;
            case GLFW_KEY_RSHIFT: eKey = IInputEventListener::KEY_RSHIFT; break;
            case GLFW_KEY_LCTRL: eKey = IInputEventListener::KEY_LCTRL; break;
            case GLFW_KEY_RCTRL: eKey = IInputEventListener::KEY_RCTRL; break;
            case GLFW_KEY_LALT: eKey = IInputEventListener::KEY_LALT; break;
            case GLFW_KEY_RALT: eKey = IInputEventListener::KEY_RALT; break;
            case GLFW_KEY_UP: eKey = IInputEventListener::KEY_UP; break;
            case GLFW_KEY_DOWN: eKey = IInputEventListener::KEY_DOWN; break;
            case GLFW_KEY_LEFT: eKey = IInputEventListener::KEY_LEFT; break;
            case GLFW_KEY_RIGHT: eKey = IInputEventListener::KEY_RIGHT; break;
            case GLFW_KEY_SPACE: eKey = IInputEventListener::KEY_SPACE; break;
            case GLFW_KEY_ESC: eKey = IInputEventListener::KEY_ESC; break;
            case GLFW_KEY_TAB: eKey = IInputEventListener::KEY_TAB; break;
            case GLFW_KEY_ENTER: eKey = IInputEventListener::KEY_ENTER; break;
            case GLFW_KEY_BACKSPACE: eKey = IInputEventListener::KEY_BACKSPACE; break;

            default:
                Logger::error() << "keycode " << iKeyIdentifier << " not recognized" << Logger::endl;
                bKeyRecognized = false;
            }

        // call methods of listener if key was recognized
        if (bKeyRecognized)
        {
            if (iNewKeyState == GLFW_PRESS)
                m_pInputEventListener->OnKeyDown(eKey);
            else
                m_pInputEventListener->OnKeyUp(eKey);
        }
    }
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlHandleMousePos(int iX, int iY)
{
    if (m_pInputEventListener != NULL)
    {
	// only deliver relative mouse position to center
	int iRelX = iX - m_iWidth / 2;
	int iRelY = iY - m_iHeight / 2;

	// issue 7 - workaround
	iRelX = -iRelX;
	iRelY = -iRelY;

	m_pInputEventListener->OnMouseMove(iRelX, iRelY);
    }

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
    if (m_pInputEventListener != NULL)
    {
        IInputEventListener::TMouseButton eMouseButton;
        bool bButtonRecognized = true;

        switch (iButton)
        {
        case GLFW_MOUSE_BUTTON_LEFT: eMouseButton = IInputEventListener::BUTTON_LEFT; break;
        case GLFW_MOUSE_BUTTON_MIDDLE: eMouseButton = IInputEventListener::BUTTON_MIDDLE; break;
        case GLFW_MOUSE_BUTTON_RIGHT: eMouseButton = IInputEventListener::BUTTON_RIGHT; break;
        default:
            Logger::error() << "mouse button " << iButton << " not recognized" << Logger::endl;
            bButtonRecognized = false;
        }

        if (bButtonRecognized)
        {
            if (iAction == GLFW_PRESS)
                m_pInputEventListener->OnMouseButtonPressed(eMouseButton);
            else
                m_pInputEventListener->OnMouseButtonReleased(eMouseButton);
        }

    }
}

/****************************************************************
  *************************************************************** */
void Graphic::ItlCreateOpenGLWindow()
{
    m_iWidth = 800;
    m_iHeight = 600;

    int flags = (false) ? GLFW_FULLSCREEN : GLFW_WINDOW;	//if fullscreen is true, flags is set to GLFW_FULLSCREEN, else to GLFW_WINDOW

    // initialize glfw
    if (glfwInit() == GL_TRUE)
            Logger::debug() << "glfw sucessfully initialized" << Logger::endl;
    else
            Logger::fatal() << "glfw initialization failed" << Logger::endl;

    // Set flags so GLFW creates the desired OpenGL context
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Activate 4x antialiasing
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

    if (glfwOpenWindow(m_iWidth, m_iHeight, 0,0,0,0, 24, 8, flags) != GL_TRUE)
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
    if (GLEW_VERSION_3_3)
    {
            GLint profile;

            // check if we have a core-profile
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

	    /*if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
                    Logger::debug() << "got rendering context with core profile" << Logger::endl;
            else
                    Logger::fatal() << "got rendering context with compatibility profile instead of core profile" << Logger::endl;
*/
	    int iMayor, iMinor, iRev;

	    glfwGetVersion(&iMayor, &iMinor, &iRev);

	    Logger::error() << iMayor << ":" << iMinor << ":" << iRev << Logger::endl;
    }
    else
            Logger::fatal() << "OpenGL version 3.3 is needed but not supported" << Logger::endl;
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
   // glEnable(GL_CULL_FACE);

    glPolygonOffset(1.1f, 4.0f);

    //set viewport
    glViewport(0,0, m_iWidth, m_iHeight);

    //clear error status
    glGetError();

    /*
    m_spMainCamera = shared_ptr<Camera> (new Camera(glm::vec3(4,3,6), 210, -15, 45.0, 1.33, 0.1, 100.0));
    m_spDebugCamera = shared_ptr<Camera> (new Camera(glm::vec3(0,5,10), 180, 0, 45.0, 1.33, 0.1, 100.0));
    m_spLightCamera = shared_ptr<Camera> (new Camera(glm::vec3(0,6,0), 180, -90, 50.0, 1.0, 2.5, 10.0));
    */

    Logger::debug() << "graphic initialized" << Logger::endl;
}

/****************************************************************
  ** Nestes class Graphic::Camera **
  *************************************************************** */
Graphic::Camera::Camera()
{
    m_bInitialized = false;
    m_m4ProjectionMatrix = glm::mat4();

    m_fRotationHorizontal = 0;
    m_fRotationVertical = 0;
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
    m_fRotationVertical += fValue;

    while (fValue < 0.0f)
          fValue += 360.0f;

    while (fValue >= 360.0f)
        fValue -= 360.0f;
}

/****************************************************************
  *************************************************************** */
void Graphic::Camera::RotateHorizontal(float fValue)
{
    m_fRotationHorizontal += fValue;

    while (fValue < 0.0f)
          fValue += 360.0f;

    while (fValue >= 360.0f)
        fValue -= 360.0f;
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

    v3LookAt.x = sin((m_fRotationHorizontal / 180.0) * PI);
    v3LookAt.y = tan((m_fRotationVertical / 180.0) * PI);
    v3LookAt.z = cos((m_fRotationHorizontal / 180.0) * PI);

    m_m4ViewMatrix = glm::lookAt(m_v3CameraPosition, m_v3CameraPosition + v3LookAt, glm::vec3(0,1,0));
}

bool Graphic::ShutDown()
{
    glfwCloseWindow();
    glfwTerminate();

    m_bWindowOpenened = false;

    return true;
}

void Graphic::RegisterInputHandler(IInputEventListener *pListener)
{
    assert (pListener != NULL);

    m_pInputEventListener = pListener;
}

void Graphic::Camera::AddToMoveVector(glm::vec3 vVector)
{
    m_v3MoveVector += vVector;
}

void Graphic::HideAndLockMouseToWindowCenter()
{
    assert (m_bWindowOpenened);

    glfwDisable(GLFW_MOUSE_CURSOR);

    m_bIsMouseLocked = true;

    glfwSetMousePos(m_iWidth / 2, m_iHeight / 2);
}

void Graphic::UnHideAndUnLockMouse()
{
    assert (m_bWindowOpenened);

    glfwEnable(GLFW_MOUSE_CURSOR);

    m_bIsMouseLocked = false;
}

void Graphic::ItlLoadShaderPrograms()
{
    ShaderManager *pShaderManager = ShaderManager::instance();

    assert (pShaderManager != NULL);

    pShaderManager->AddShader("basic_shading", new Shader("shaders/basic_shading.vs", "shaders/basic_shading.fs"));
    pShaderManager->AddShader("camera-debug", new Shader("shaders/camera-debug.vs", "shaders/camera-debug.fs"));
}

void Graphic::ItlCreateBaseRenderPath()
{
    std::shared_ptr<SceneObject> spRootNode (new SceneObject_EmptyNode());

    std::shared_ptr<SceneObject> spCameraNode (new SceneObject_Camera(GetCamera()));

    std::shared_ptr<SceneObject> spCubeNode (new SceneObject_Cube());


    spRootNode->AddChild(spCameraNode);
    spCameraNode->AddChild(spCubeNode);

    AddRenderPath(spRootNode, "default");
}


