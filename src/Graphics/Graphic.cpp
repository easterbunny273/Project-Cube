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
    : m_pSettings(NULL)
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
    // dec instances counter
    s_iInstances--;
}

/****************************************************************
  *************************************************************** */
void Graphic::Render()
{
    for (auto iter=m_mRenderLoops.begin(); iter != m_mRenderLoops.end(); iter++)
    {
        TItlRenderLoop *pRenderLoop = &(iter->second);

        pRenderLoop->spRenderTarget->ClearBuffers();
        pRenderLoop->spCamera->Move(0.0001f);
        pRenderLoop->spScene->CallRenderPath();
        pRenderLoop->spRenderTarget->SwapBuffers();
    }
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
void Graphic::Camera::AddToMoveVector(glm::vec3 vVector)
{
    m_v3MoveVector += vVector;
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

    //if (m_bActive)
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

int Graphic::AddRenderLoop(std::shared_ptr<Graphic::IRenderTarget> spRenderTarget,
                             std::shared_ptr<Graphic::Camera> spCamera,
                             std::shared_ptr<Graphic::Scene> spScene)
{
    static int iID = 0;

    TItlRenderLoop NewLoop;

    NewLoop.spRenderTarget = spRenderTarget;
    NewLoop.spCamera = spCamera;
    NewLoop.spScene = spScene;

    m_mRenderLoops[iID++] = NewLoop;
}

void Graphic::RemoveRenderLoop(int iLoopID)
{
    assert (m_mRenderLoops.find(iLoopID) != m_mRenderLoops.end());

    m_mRenderLoops.erase(iLoopID);
}

void Graphic::Scene::CallRenderPath()
{
    m_spRenderPath->Render();
}

std::shared_ptr<Graphic::Scene> Graphic::Scene::Create(std::shared_ptr<Graphic::Camera> spCamera)
{
    std::shared_ptr<SceneObject> spRootNode (new SceneObject_EmptyNode());

    std::shared_ptr<SceneObject> spCameraNode (new SceneObject_Camera(spCamera.get()));

    std::shared_ptr<SceneObject> spCubeNode (new SceneObject_Cube());
    std::shared_ptr<SceneObject> spTreppe (new SceneObject_AssimpImport("models/freepool-ng-table.3ds"));


    spRootNode->AddChild(spCameraNode);
    spCameraNode->AddChild(spTreppe);
    //spCubeNode->AddChild(spTreppe);

    spTreppe->SetTransformMatrix(glm::scale(glm::mat4(), glm::vec3(0.01f)));

    std::shared_ptr<Graphic::Scene> spNewScene(new Graphic::Scene());
    spNewScene->m_spRenderPath = spRootNode;

    return spNewScene;
}
