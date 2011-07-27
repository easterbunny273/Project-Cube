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

#include "Graphics/SceneObjects/SceneObject_AssimpImport.h"

#include "Graphics/Graphic.h"

using namespace std;

#define PI 3.14159265f

/****************************************************************
  *************************************************************** */
Graphic::Graphic()
    : m_bWindowOpenened(false),
      m_fTimeOfLastRenderCall(0),
      m_iFramesInThisSecondYet(0),
      m_iFramesPerSecond(0)
{
    //nothing to do so far, only initializer list
}

/****************************************************************
  *************************************************************** */
Graphic::~Graphic()
{
    // check if the window is closed
    assert (m_bWindowOpenened == false);
}

/****************************************************************
  *************************************************************** */
bool Graphic::InitializeOpenGL()
{
    int flags = (false) ? GLFW_FULLSCREEN : GLFW_WINDOW;	//if fullscreen is true, flags is set to GLFW_FULLSCREEN, else to GLFW_WINDOW

    glfwInit();

    // Set flags so GLFW creates the desired OpenGL context
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Activate 4x antialiasing
    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

    if (glfwOpenWindow(m_iWidth, m_iHeight, 0,0,0,0, 24, 8, flags) != GL_TRUE)
            Logger::fatal() << "failed to initialize OpenGL window" << Logger::endl;
    else
            Logger::debug() << "OpenGL window initialized" << Logger::endl;

    glfwSetWindowTitle("Free Pool (NG) written by Christian Moellinger");

    // Disable the mouse cursor
    //glfwDisable(GLFW_MOUSE_CURSOR);

    // Disable VSync
    //glfwSwaPInterval(0);

    // start up GLEW
    glewExperimental = GL_TRUE; //set experimental flag to true, needed for correct importing of function pointers when using core-profile
    if (glewInit() != GLEW_OK)
            Logger::fatal() << "glew initialization failed" << Logger::endl;
    else
            Logger::debug() << "glew initialized" << Logger::endl;

    Logger::debug() << "Opened context with OpenGL Version " << (char *) glGetString(GL_VERSION) << Logger::endl;

    if (GLEW_VERSION_3_2)
    {
            GLint profile;

            // check if we have a core-profile
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
            /*if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
                    Logger::debug() << "got rendering context with core profile" << Logger::endl;
            else
                    Logger::fatal() << "got rendering context with compatibility profile instead of core profile" << Logger::endl;
            */
    }
    else
            Logger::fatal() << "OpenGL version 3.2 is needed but not supported" << Logger::endl;

    // Enable sRGB gamma correction for framebuffer output.
    glEnable(GL_FRAMEBUFFER_SRGB);

    //Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Enable face culling (default: backface culling)
    glEnable(GL_CULL_FACE);

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
    this->m_vRenderPaths[sRenderPath] = spRoot;
}

/****************************************************************
  *************************************************************** */
void Graphic::Render()
{
    // check if any render path is set to active
    assert (m_sActiveRenderPath.empty() == false);

    //clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw the scene
    if (m_vRenderPaths.find(m_sActiveRenderPath) != m_vRenderPaths.end())
        m_vRenderPaths[m_sActiveRenderPath]->Render();
    else
        Logger::error() << "RenderPath " << m_sActiveRenderPath << " does not exist!" << Logger::endl;

    // swap back and front buffers
    glfwSwapBuffers();

    // print frames per second each second
    if (glfwGetTime() - m_fTimeOfLastRenderCall >= 1.0)
    {
        m_fTimeOfLastRenderCall = glfwGetTime();

        std::stringstream ssConversionBuffer;
        std::string sFramesPerSecond;
        ssConversionBuffer << m_iFramesInThisSecondYet;
        ssConversionBuffer >> sFramesPerSecond;

        std:string sWindowTitle = std::string("Project CUBE, frames per second: ") + sFramesPerSecond;
        glfwSetWindowTitle(sWindowTitle.data());

        m_iFramesPerSecond = m_iFramesInThisSecondYet;
        m_iFramesInThisSecondYet = 0;
    }

}

/****************************************************************
  *************************************************************** */
Graphic::Camera::Camera()
{
    m_bInitialized = false;
    m_m4ProjectionMatrix = glm::mat4();
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
    glm::vec3 v3MoveX;
    glm::vec3 v3MoveZ;

    if (m_v3MoveVector.x != 0)
    {
        //if camera should move in x-direction, we must calculate the look-at point
        //on the camera-sphere and use this vector as reference, and move normal to this vector

        v3MoveX.x = sin(((m_fRotationHorizontal+90) / 180.0) * PI);
        v3MoveX.y = 0.0;
        v3MoveX.z = cos(((m_fRotationHorizontal+90) / 180.0) * PI);

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
