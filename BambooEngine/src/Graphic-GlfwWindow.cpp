#include "Graphic.h"
#include "Graphic-GlfwWindow.h"

#ifdef GLFW

#include <string>
#include <sstream>
#include <memory>
#include <vector>

//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common_gl.h"
#include "Logger.h"

Bamboo::GlfwWindow *Bamboo::GlfwWindow::s_pInstance = NULL;

/****************************************************************
  *************************************************************** */
Bamboo::GlfwWindow::GlfwWindow()
{
    // construction is done in Create()
}

/****************************************************************
  *************************************************************** */
std::shared_ptr<Bamboo::GlfwWindow> Bamboo::GlfwWindow::Create(int iWidth, int iHeight, std::string sWindowTitle)
{
    // todo: try only versions which are acceptable for the used rendernodes, else something strange will happen :-)
    std::vector<int> viMajorVersions = { 4, 4, 3, 3, 3, 2, 2, 1 };
    std::vector<int> viMinorVersions = { 2, 1, 3, 2, 0, 1, 0, 5 };
    std::vector<int> viCoreProfileFlag = { GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_COMPAT_PROFILE, 0 };

    std::shared_ptr<GlfwWindow> spNewWindow(new GlfwWindow());
    spNewWindow->m_iWidth = iWidth;
    spNewWindow->m_iHeight = iHeight;
    spNewWindow->m_sWindowTitle = sWindowTitle;

    // assert that only one instance of a glfw window is created (glfw does not support multiple windows)
    assert(GlfwWindow::s_pInstance == NULL);

    if (s_pInstance != NULL)
    {
        Logger::error() << "It seems that more than 1 instance of the Graphics engine was created." << Logger::endl;
        Logger::error() << "Please note that only one instance is supported by the event handling of glfw" << Logger::endl;
        Logger::error() << "Contact the developers or reinstall if you have any problems with event handling" << Logger::endl;
    }

    GlfwWindow::s_pInstance = spNewWindow.get();

    // initialize glfw
    if (glfwInit() == GL_TRUE)
            Logger::debug() << "glfw sucessfully initialized" << Logger::endl;
    else
            Logger::fatal() << "glfw initialization failed" << Logger::endl;


    bool bWindowCreated = false;

    for (unsigned int iProfile = 0; iProfile < viCoreProfileFlag.size(); iProfile++)
    for (unsigned int i=0; !bWindowCreated && i < viMajorVersions.size(); i++)
    {
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, viMajorVersions[i]);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, viMinorVersions[i]);

        glfwOpenWindowHint(GLFW_OPENGL_PROFILE, viCoreProfileFlag[iProfile]);

        std::string sProfileName;

        if (viCoreProfileFlag[iProfile] == GLFW_OPENGL_CORE_PROFILE)
            sProfileName = "CORE";
        else if (viCoreProfileFlag[iProfile] == GLFW_OPENGL_COMPAT_PROFILE)
            sProfileName = "COMPAT";
        else
            sProfileName = "*";


        Logger::debug() << "Try opening context " <<
                           viMajorVersions[i] << "." << viMinorVersions[i]
                        << " with " << sProfileName << " profile" << Logger::endl;

        bWindowCreated = (glfwOpenWindow(iWidth, iHeight, 8,8,8,8, 24, 8, GLFW_WINDOW) == GL_TRUE);
    }

    if (!bWindowCreated)
        Logger::fatal() << "No opengl context could be created" << Logger::endl;

    //Activate 4x antialiasing
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);


    // set input handling callback methods
    glfwSetKeyCallback(Bamboo::GlfwWindow::ItlStaticHandleKeyboardEvent);
    glfwSetMousePosCallback(Bamboo::GlfwWindow::ItlStaticHandleMousePos);
    glfwSetMouseWheelCallback(Bamboo::GlfwWindow::ItlStaticHandleMouseWheel);
    glfwSetMouseButtonCallback(Bamboo::GlfwWindow::ItlStaticHandleMouseButton);

    // set window title
    glfwSetWindowTitle(sWindowTitle.c_str());

    // Disable the mouse cursor
    //glfwDisable(GLFW_MOUSE_CURSOR);

    // set experimental flag to true, needed for correct importing of function pointers when using core-profile
    glewExperimental = GL_TRUE;

    // start up GLEW
    if (glewInit() != GLEW_OK)
            Logger::fatal() << "glew initialization failed" << Logger::endl;

    Logger::debug() << "Got context with OpenGL Version " << (char *) glGetString(GL_VERSION) << Logger::endl;

    GLint profile;

    // check if we have a core-profile
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

    if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
            Logger::debug() << "with core profile" << Logger::endl;
    else
            Logger::debug() << "with compatibility profile" << Logger::endl;

    return spNewWindow;
}

/****************************************************************
  *************************************************************** */
Bamboo::GlfwWindow::~GlfwWindow()
{
    glfwCloseWindow();
    glfwTerminate();

    GlfwWindow::s_pInstance = NULL;
}

/****************************************************************
  *************************************************************** */
void Bamboo::GlfwWindow::ItlStaticHandleKeyboardEvent(int iKeyIdentifier, int iNewKeyState)
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
    Bamboo::GlfwWindow *pThis = s_pInstance;

    // call method of listener
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleKeyboardEvent(iKeyIdentifier, iNewKeyState);
}

/****************************************************************
  *************************************************************** */
void Bamboo::GlfwWindow::ItlStaticHandleMousePos(int iX, int iY)
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
    Bamboo::GlfwWindow *pThis = s_pInstance;

    // call member method
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleMousePos(iX, iY);
}

/****************************************************************
  *************************************************************** */
void Bamboo::GlfwWindow::ItlStaticHandleMouseWheel(int iPosition)
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
    Bamboo::GlfwWindow *pThis = s_pInstance;

    // call member method
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleMouseWheel(iPosition);
}

/****************************************************************
  *************************************************************** */
void Bamboo::GlfwWindow::ItlStaticHandleMouseButton(int iButton, int iAction)
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
    Bamboo::GlfwWindow *pThis = s_pInstance;

    // call member method
    if (pThis->m_spInputEventListener)
        pThis->m_spInputEventListener->ItlHandleMouseButton(iButton, iAction);
}

/****************************************************************
  *************************************************************** */
void Bamboo::GlfwWindow::SwapBuffers()
{
    glfwSwapBuffers();
}

/****************************************************************
  *************************************************************** */
void Bamboo::GlfwWindow::ClearBuffers()
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif
