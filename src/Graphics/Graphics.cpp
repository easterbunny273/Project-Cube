#include "Graphics/Graphics.h"
#include "Graphics/common_gl.h"

#include "Error.h"
#include "Settings.h"
#include "Logger.h"

Graphics::Graphics()
{
    // nothing to do so far
}

Graphics::~Graphics()
{
    if (m_lInputListeners.size())
        assert(!"Graphics() subsystem should be destroyed but there are still registered IInputEventListeners!");
}

bool Graphics::Initialize()
{
    bool bOk = true;

    bool bFullscreen;
    bool bAntiAliasing;
    int iWidth = -1;
    int iHeight = -1;

    Settings::instance()->GetGroup("graphics")->GetValueOrDefault("window_width", 800, iWidth);
    Settings::instance()->GetGroup("graphics")->GetValueOrDefault("window_height", 600, iHeight);
    Settings::instance()->GetGroup("graphics")->GetValueOrDefault("fullscreen", false, bFullscreen);
    Settings::instance()->GetGroup("graphics")->GetValueOrDefault("antialiasing", false, bAntiAliasing);

    int iFlags = (bFullscreen) ? GLFW_FULLSCREEN : GLFW_WINDOW;

    if (glfwInit() == GL_TRUE)
            Logger::debug() << "glfw sucessfully initialized" << Logger::endl;
    else
            Logger::fatal() << "glfw initialization failed" << Logger::endl;

    // Set flags so GLFW creates the desired OpenGL context
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (bAntiAliasing)
    {
        //Activate 4x antialiasing
        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    }


    if (glfwOpenWindow(iWidth, iHeight, 0, 0, 0, 0, 24, 8, iFlags) != GL_TRUE)
            Logger::fatal() << "failed to initialize OpenGL window" << Logger::endl;
    else
            Logger::debug() << "OpenGL window initialized" << Logger::endl;

    // set window title
    glfwSetWindowTitle("Project Cube - written by Florian Spechtenhauser and Christian Möllinger");

    if (bFullscreen)
    {
        // Disable the mouse cursor
        glfwDisable(GLFW_MOUSE_CURSOR);
    }

    // start up GLEW
    // set experimental flag to true, needed for correct importing of function pointers when using core-profile
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
            Logger::fatal() << "glew initialization failed" << Logger::endl;
    else
            Logger::debug() << "glew initialized" << Logger::endl;

    Logger::debug() << "Opened context with OpenGL Version " << (char *) glGetString(GL_VERSION) << Logger::endl;

    if (GLEW_VERSION_3_3)
    {
            GLint profile;

            // check if we have a core-profile
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

            // under linux with and NVIDIA optimus card, there seems to be a bug in glfw, so that no context can be created.
            // I fixed version glfw so that it an opengl-context can be created, but either it has only compatibility profile or
            // it returns a wrong value of the profile. Anyway, testing if we got a core profile returns always false on my laptop
            // false on my laptop (by christian.moellinger)

            /*
            if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
                    Logger::debug() << "got rendering context with core profile" << Logger::endl;
            else
                    Logger::fatal() << "got rendering context with compatibility profile instead of core profile" << Logger::endl;
            */
    }
    else
            Logger::fatal() << "OpenGL version 3.3 is needed but not supported" << Logger::endl;

    // Enable sRGB gamma correction for framebuffer output.
    glEnable(GL_FRAMEBUFFER_SRGB);

    //Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Enable face culling (default: backface culling)
    glEnable(GL_CULL_FACE);

    glPolygonOffset(1.1f, 4.0f);

    //set viewport
    glViewport(0,0, iWidth, iHeight);

    //clear error status
    glGetError();

    Logger::debug() << "graphic initialized" << Logger::endl;

    return bOk;
}

bool Graphics::ShutDown()
{
    bool bOk = true;

    glfwTerminate();

    return bOk;
}

void Graphics::RegisterInputListener(Graphics::IInputEventListener *pListener)
{
    m_lInputListeners.push_back(pListener);
}

void Graphics::UnRegisterInputListener(Graphics::IInputEventListener *pListener)
{
    m_lInputListeners.push_back(pListener);
}


