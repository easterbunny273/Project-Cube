#include <string>
#include <sstream>
#include <memory>

//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Graphic-GlfwWindow.h"
#include "Graphics/Graphic.h"
#include "Graphics/common_gl.h"
#include "Logger.h"

Graphic::GlfwWindow *Graphic::GlfwWindow::s_pInstance = NULL;

/****************************************************************
  *************************************************************** */
Graphic::GlfwWindow::GlfwWindow()
{
    // construction is done in Create()
}

/****************************************************************
  *************************************************************** */
std::shared_ptr<Graphic::GlfwWindow> Graphic::GlfwWindow::Create(int iWidth, int iHeight, std::string sWindowTitle)
{
    bool m_bUseOpenGL_4_1 = false; //TODO: remove

    std::shared_ptr<GlfwWindow> spNewWindow(new GlfwWindow());
    spNewWindow->m_iWidth = iWidth;
    spNewWindow->m_iHeight = iHeight;
    spNewWindow->m_sWindowTitle = sWindowTitle;

    // assert that only one instance of a glfw window is created (glfw does not support multiple windows)
    assert(GlfwWindow::s_pInstance == NULL);
    GlfwWindow::s_pInstance = spNewWindow.get();

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

    if (glfwOpenWindow(iWidth, iHeight, 8,8,8,8, 24, 8, GLFW_WINDOW) != GL_TRUE)
            Logger::fatal() << "failed to initialize OpenGL window" << Logger::endl;
    else
            Logger::debug() << "OpenGL window initialized" << Logger::endl;

    // set input handling callback methods
    glfwSetKeyCallback(Graphic::GlfwWindow::ItlStaticHandleKeyboardEvent);
    glfwSetMousePosCallback(Graphic::GlfwWindow::ItlStaticHandleMousePos);
    glfwSetMouseWheelCallback(Graphic::GlfwWindow::ItlStaticHandleMouseWheel);
    glfwSetMouseButtonCallback(Graphic::GlfwWindow::ItlStaticHandleMouseButton);

    // set window title
    glfwSetWindowTitle(sWindowTitle.c_str());

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

    return spNewWindow;
}

/****************************************************************
  *************************************************************** */
Graphic::GlfwWindow::~GlfwWindow()
{
    glfwCloseWindow();
    glfwTerminate();

    GlfwWindow::s_pInstance = NULL;
}

/****************************************************************
  *************************************************************** */
void Graphic::GlfwWindow::ItlStaticHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState)
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
    Graphic::GlfwWindow *pThis = s_pInstance;

    // call method of listener
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleKeyboardEvent(iKeyIdentifier, iNewKeyState);
}

/****************************************************************
  *************************************************************** */
void Graphic::GlfwWindow::ItlStaticHandleMousePos(int iX, int iY)
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
    Graphic::GlfwWindow *pThis = s_pInstance;

    // call member method
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleMousePos(iX, iY);
}

/****************************************************************
  *************************************************************** */
void Graphic::GlfwWindow::ItlStaticHandleMouseWheel(int iPosition)
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
    Graphic::GlfwWindow *pThis = s_pInstance;

    // call member method
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleMouseWheel(iPosition);
}

/****************************************************************
  *************************************************************** */
void Graphic::GlfwWindow::ItlStaticHandleMouseButton(int iButton, int iAction)
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
    Graphic::GlfwWindow *pThis = s_pInstance;

    // call member method
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleMouseButton(iButton, iAction);
}

/****************************************************************
  *************************************************************** */
void Graphic::GlfwWindow::SwapBuffers()
{
    glfwSwapBuffers();
}

/****************************************************************
  *************************************************************** */
void Graphic::GlfwWindow::ClearBuffers()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
