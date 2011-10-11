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
#include "Graphics/RenderNode.h"
#include "Graphics/RenderNodes/RenderNode_PostEffect.h"
#include "Graphics/RenderNodes/RenderNode_RenderPass.h"
#include "Graphics/RenderNodes/RenderNode_EmptyNode.h"
#include "Graphics/RenderNodes/RenderNode_Camera.h"
#include "Graphics/RenderNodes/RenderNode_FBO.h"
#include "Graphics/RenderNodes/RenderNode_Cube.h"
#include "Graphics/RenderNodes/RenderNode_AssimpImport.h"

#include "Graphics/Graphic.h"

#include "Events.h"

#include "MainApp.h"

using namespace std;

#define PI 3.14159265f

/****************************************************************
  *************************************************************** */
Graphic::Graphic()
{


}

/****************************************************************
  *************************************************************** */
Graphic::~Graphic()
{

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
    if (m_spRenderGraphRoot)
        m_spRenderGraphRoot->Render();
    else
    {
        CreateRenderGraph();
        m_spRenderGraphRoot->Render();
    }
}

std::shared_ptr<Graphic::Scene> Graphic::Scene::Create(std::shared_ptr<Graphic::Camera> spCamera)
{
    std::shared_ptr<Graphic::Scene> spNewScene(new Graphic::Scene());
    spNewScene->m_spCamera = spCamera;

    return spNewScene;
}

void Graphic::Scene::AttachObject(std::shared_ptr<Graphic::ISceneObject> spSceneObject)
{
    m_lSceneObjects.push_back(spSceneObject);
}

void Graphic::Scene::CreateRenderGraph()
{
    m_spRenderGraphRoot = std::shared_ptr<RenderNode>(new SceneObject_Camera(m_spCamera.get()));

    for (auto iter=m_lSceneObjects.begin(); iter != m_lSceneObjects.end(); iter++)
    {
        std::shared_ptr<Graphic::ISceneObject> spSceneObject = *iter;
        std::shared_ptr<RenderNode> spRenderNode = spSceneObject->GetRenderNode();

        m_spRenderGraphRoot->AddChild(spRenderNode);
    }
}

std::shared_ptr<RenderNode> Graphic::LoadedModel::CreateRenderNode()
{
    assert (m_sFilename.size() > 0);

    std::shared_ptr<RenderNode> spRenderNode(new SceneObject_AssimpImport(m_sFilename));

    return spRenderNode;
}

std::shared_ptr<Graphic::LoadedModel> Graphic::LoadedModel::Create(std::string sFilename)
{
    std::shared_ptr<Graphic::LoadedModel> spNewNode(new LoadedModel());

    spNewNode->m_sFilename = sFilename;

    return spNewNode;
}

std::shared_ptr<Graphic::Cube> Graphic::Cube::Create()
{
    std::shared_ptr<Graphic::Cube> spNewNode(new Cube());

    return spNewNode;
}

std::shared_ptr<RenderNode> Graphic::Cube::CreateRenderNode()
{
    std::shared_ptr<RenderNode> spRenderNode(new SceneObject_Cube());

    return spRenderNode;
}

void Graphic::ISceneObject::SetTransformMatrix(glm::mat4 mNewMatrix)
{
    GetRenderNode()->SetTransformMatrix(mNewMatrix);
}

std::shared_ptr<RenderNode> Graphic::ISceneObject::GetRenderNode()
{
    if (!m_spRenderNode)
        m_spRenderNode = CreateRenderNode();

    assert(m_spRenderNode);

    return m_spRenderNode;
}
